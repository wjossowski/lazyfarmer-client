/**
 ** This file is part of the LazyFarmer project.
 ** Copyright 2017 Wojciech Ossowski <w.j.ossowski@gmail.com>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 3 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "apigateway.h"
#include "globalgamedata.h"

#include "extractors/gameinfoextractor.h"

#include "messages/messages.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QMetaEnum>
#include <QtCore/QRegularExpression>
#include <QtCore/QSettings>
#include <QtCore/QFile>
#include <QtCore/QTimer>

#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>

#include <QtDebug>

using namespace Core;
using namespace Core::Api;
using namespace Core::Api::Messages;
using namespace Core::Extractors;

ApiGateway::ApiGateway(QObject *parent)
    : QObject(parent)
    , m_firstRun(true)
    , m_loggedIn(false)
{

}

/**
 * @brief ApiGateway::isConfigured
 * Returns true if all configuration fields are filled
 * @return Is Gateway configured
 */
bool ApiGateway::isConfigured() const
{
    return !(this->m_login.isEmpty()
             || this->m_password.isEmpty()
             || this->m_serverDomain.isEmpty()
             || this->m_serverId.isEmpty());
}

/**
 * @brief ApiGateway::setLoggedIn
 * Toggles LoggedIn status
 * @param loggedIn is user LoggedIn
 */
void ApiGateway::setLoggedIn(bool loggedIn)
{
    if (m_loggedIn != loggedIn) {
        m_loggedIn = loggedIn;

        qInfo() << (m_loggedIn ? tr("%1 successfully logged in, rid: '%2'")
                                 .arg(m_login)
                                 .arg(m_rid)
                               : tr("Logged out"));

        if (!m_loggedIn) {
            m_manager.clearAccessCache();
        }

        emit loggedInChanged(m_loggedIn);
    }
}

/**
 * @brief ApiGateway::extractRid
 * Extracts `rid` from HTML contents.
 * During the first search, whole document is loaded and constant data
 * such as `delays` and `sizes` from JS script are loaded.
 * Search depth can be stored in settings under Lookup:RidExtractorDeep.
 * this value is checked only for next calls.
 * @param reply Reply from `Login` message call.
 */
void ApiGateway::extractRid(QIODevice *reply)
{
    QSettings settings;
    settings.beginGroup("Lookup");

    const QString content = m_firstRun ? reply->readAll()
                                       : reply->read(settings.value("RidExtractorDeep", 1024).toInt());

    QRegularExpression ridRegex("var rid = '(?<rid>.*)'");
    m_rid = ridRegex.match(content).captured("rid");

    if (!m_rid.isEmpty()) {
        if (m_firstRun) {
            const auto extractor = GameInfoExtractor::baseExtractor(m_serverDomain);
            extractor->extract(content);

            queueConstantData(content);

            m_firstRun = false;
        }

        setLoggedIn(true);
    } else {
        handleError(ApiGatewayError::ErrorType::RidNotParsed);
    }
}

/**
 * @brief ApiGateway::setApiOptions
 * Sets internal login configuration
 * @param options ApiOpitons structure
 */
void ApiGateway::setApiOptions(const ApiOptions &options)
{
    m_serverId = options.serverId;
    m_serverDomain = options.serverDomain;
    m_login = options.login;
    m_password = options.password;
}

/**
 * @brief ApiGateway::queueMessage
 * Queues specified message to message queue.
 * @param message Message to be pushed
 * @param placement Describes place for pushed message
 */
void ApiGateway::queueMessage(const ApiMessage::Ptr &message, PushMessageTo placement)
{
    if (placement == PushMessageTo::Top) {
        m_messageQueue.push_front(message);
    } else {
        m_messageQueue.push_back(message);
    }

    connect(message.data(), &ApiMessage::raiseError,
            this,           &ApiGateway::handleError);
}

/**
 * @brief ApiGateway::start
 * Starts message queue runtime.
 * Handles Login/Logout and pushes those messages automatically to queue.
 */
void ApiGateway::start()
{
    if (m_messageQueue.size() == 0) {
        if (m_loggedIn) {
            queueMessage(Logout::Ptr(new Logout(this)));
        } else {
            qInfo() << "No messages left";
            m_currentMessage.reset();
            return;
        }
    } else if (!m_loggedIn) {
        queueMessage(Login::Ptr(new Login(this)), PushMessageTo::Top);
    }

    m_currentMessage = m_messageQueue.first();
    m_messageQueue.pop_front();

    sendMessage(m_currentMessage.data());
}

/**
 * @brief ApiGateway::buildStaticUrl
 * Creates static Url for specified endpoint.
 * It's used to load static data from server (images, scripts, etc).
 * Built Url depends on internal gateway configuration.
 * @param endpoint Static Endpoint Url
 * @return Static Endpoint Url
 */
QUrl ApiGateway::buildStaticUrl(const QString &endpoint)
{
    return QString("http://s%1.%2/%3")
            .arg(m_serverId)
            .arg(m_serverDomain)
            .arg(endpoint);
}

/**
 * @brief ApiGateway::buildEndpointUrl
 * Creates Url for specified endpoint.
 * Pushes specified Query data.
 * Includes Rid if necessary.
 * Built Url depends on internal gateway configuration.
 * @param endpoint Endpoint Url
 * @param data Query data
 * @param includeRid Should be `rid` included to message body
 * @return Endpoint Url
 */
QUrl ApiGateway::buildEndpointUrl(const QString &endpoint,
                                  const QList<QPair<QString, QString>> &data,
                                  bool includeRid) const
{
    QUrlQuery query;
    query.setQueryItems(data);
    if (includeRid) {
        query.addQueryItem("rid", m_rid);
    }

    const QString url = QString("http://s%1.%2/%3.php?%4")
            .arg(m_serverId)
            .arg(m_serverDomain)
            .arg(endpoint)
            .arg(query.toString());

    return QUrl(url);
}

/**
 * @brief ApiGateway::buildEndpointAjaxUrl
 * Creates Endpoint Ajax url. Calls internally `buildEndpointUrl`.
 * Built Url depends on internal gateway configuration.
 * @param endpoint Endpoint Url
 * @param data Query data
 * @param includeRid Should be `rid` included to message body
 * @return Endpoint Ajax Url
 */
QUrl ApiGateway::buildEndpointAjaxUrl(const QString &endpoint, const QList<QPair<QString, QString> > &data, bool includeRid) const
{
    return buildEndpointUrl(QString("ajax/%1").arg(endpoint), data, includeRid);
}

/**
 * @brief ApiGateway::buildHeaders
 * Fills request with `ContentTypeHeader` and `UserAgentHeader` headers.
 * @param request Request to be filled
 */
void ApiGateway::buildHeaders(QNetworkRequest &request) const
{
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0");
}

/**
 * @brief ApiGateway::recursiveRedirect
 * Calls recursively endpoint when `LocationHeader` is present.
 * @param url Url to be reached
 * @param callback Function to be called
 */
void ApiGateway::recursiveRedirect(const QString &url, const std::function<void (QIODevice *)> &callback)
{
    QNetworkRequest request(url);
    buildHeaders(request);

    auto reply = m_manager.get(request);
    connect(reply, &QNetworkReply::finished, [this, reply, callback] () {
        const auto redirectUrl = reply->header(QNetworkRequest::LocationHeader);
        if (redirectUrl.isValid()) {
            recursiveRedirect(redirectUrl.toString(), callback);
            reply->deleteLater();
        } else {
            callback(reply);
        }
    });
}

/**
 * @brief ApiGateway::sendMessage
 * Sends `messages` through gateway. Checks `message`' query type and builds
 * specified query. Handles its response and deletes it
 * automatically after handling was completed.
 * @param message Message to be sent
 */
void ApiGateway::sendMessage(ApiMessage *message)
{
    QNetworkRequest request(message->url());
#ifdef DEBUG_MODE
    qDebug() << message->url();
#endif
    buildHeaders(request);
    message->configureRequest(request);

    QNetworkReply *reply = nullptr;
    const auto messageQueryType = message->queryType();

    if (messageQueryType == QueryType::Get) {
        reply = m_manager.get(request);

    } else if (messageQueryType == QueryType::Post) {
        QUrlQuery query;
        query.setQueryItems(message->postData());

        reply = m_manager.post(request, query.toString().toLatin1());

    } else {
        // TODO: Raise error
    }

    if (reply) {
        connect(reply, &QNetworkReply::finished, [this, message, reply] () {
            message->handleResponse(reply);
        });

        connect(message, &ApiMessage::finished,
                reply,   &QNetworkReply::deleteLater);

        connect(message, &ApiMessage::finished,
                this,    &ApiGateway::start);

        message->setIsSent(true);
    }
}

/**
 * @brief ApiGateway::extractGameData
 * Places `Game Data` under specified domain to `GlobalGameData`
 */
void ApiGateway::extractGameData()
{
    GlobalGameData::registerGameData(m_serverDomain, GameInfoExtractor::globalResults(m_serverDomain));
}

/**
 * @brief ApiGateway::gameData
 * @return `Game Data` for specified domain
 */
GlobalGameData::Ptr ApiGateway::gameData() const
{
    return GlobalGameData::gameData(m_serverDomain);
}

/**
 * @brief ApiGateway::handlePlayerData
 * Re-emits `updatePlayerData` for player instance.
 * @param playerData received `Player Data`
 */
void ApiGateway::handlePlayerData(const QByteArray &playerData) const
{
    emit playerDataUpdated(playerData);
}

/**
 * @brief ApiGateway::handleError
 * Handles raised error
 * @param errorType Type of error
 * @param args Error arguments
 */
void ApiGateway::handleError(ApiGatewayError::ErrorType errorType, const QStringList &args)
{
    ApiGatewayError error(errorType);

    auto message = error.message();
    for (const auto arg : args) {
        message.arg(arg);
    }

    const QString errorMessage = tr("Error on `%1` raised. %2").arg(error.toString()).arg(message);

    emit errorRaised(errorMessage); 

    qCritical() << errorMessage;
}

/**
 * @brief ApiGateway::updateBuilding
 * Pushes `GetInfo` messages depending on BuildingType
 * @param details Building Details
 * @param type Building Type
 */
void ApiGateway::updateBuilding(const Data::BuildingDetails &details,
                                const Data::BuildingType &type)
{
    switch (type) {
    case Data::BuildingType::Farm:
        queueMessage(GetFieldInfo::Ptr(new GetFieldInfo(this, details)));
        break;
    case Data::BuildingType::AnimalProduction:
        queueMessage(GetFeedInfo::Ptr(new GetFeedInfo(this, details)));
        break;
    case Data::BuildingType::ResourceProduction:
        queueMessage(GetProductionInfo::Ptr(new GetProductionInfo(this, details)));
        break;
    default:
        break;
    }
}

/**
 * @brief ApiGateway::queueConstantData
 * Extracts file name of `Constant Data` JS file from HTML contents.
 * Queues `GetConstantData` message during first `Login`.
 * Pushes it to the top of `MessageQueue`.
 * @param content HTML content
 */
void ApiGateway::queueConstantData(const QString &content)
{
    // Create url template with version placeholder
    const QString jsConstantUrl ("js/jsconstants_%1.js");

    // Create regex to obtain version number
    QRegularExpression jscVersionRegex(QString("src=\"%1\"").arg(jsConstantUrl.arg("(?<version>.*)")));

    // Acquire version number
    const auto version = jscVersionRegex.match(content).captured("version");

    auto message = new GetConstantData(this, jsConstantUrl.arg(version));
    const auto messagePtr = GetConstantData::Ptr(message);

    // TODO: Add message to obtain library
    queueMessage(messagePtr, PushMessageTo::Top);
}

/**
 * @brief ApiGateway::handleNotLogged
 * Handles error when player is not logged in.
 * @param operation Operation to be reached.
 * @return Is Logged In status
 */
bool ApiGateway::handleNotLogged(const QString &operation)
{
    bool notLogged = !m_loggedIn;
    if (notLogged) {
        handleError(ApiGatewayError::ErrorType::NotLogged,
            { tr("Action %1 requires to be logged in.").arg(operation) });
    }

    return notLogged;
}
