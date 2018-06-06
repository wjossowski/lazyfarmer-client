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
#include "helpers/gameinfoextractor.h"
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

using namespace Api;
using namespace Api::Messages;
using namespace Helpers;

ApiGateway::ApiGateway(QObject *parent)
    : QObject(parent)
    , m_firstRun(true)
    , m_loggedIn(false)
{

}

bool ApiGateway::isConfigured() const
{
    return !(this->m_login.isEmpty()
             || this->m_password.isEmpty()
             || this->m_serverDomain.isEmpty()
             || this->m_serverId.isEmpty());
}

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

void ApiGateway::extractRid(QNetworkReply *reply)
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

            // Create url template with version placeholder
            const QString jsConstantUrl ("js/jsconstants_%1.js");

            // Create regex to obtain version number
            QRegularExpression jscVersionRegex(QString("src=\"%1\"")
                                               .arg(jsConstantUrl.arg("(?<version>.*)")));

            // Acquire version number
            const auto version = jscVersionRegex.match(content).captured("version");

            // TODO: Add message to obtain library
            queueMessage(QSharedPointer<GetConstantData>(new GetConstantData(this, jsConstantUrl.arg(version))),
                         true /*push to top*/);

            m_firstRun = false;
        }

        setLoggedIn(true);
    } else {
        handleError(ApiGatewayError::ErrorType::RidNotParsed);
    }
}

void ApiGateway::setApiOptions(const ApiOptions &options)
{
    m_serverId = options.serverId;
    m_serverDomain = options.serverDomain;
    m_login = options.login;
    m_password = options.password;
}

void ApiGateway::queueMessage(const QSharedPointer<ApiMessage> &message, bool pushToTop)
{
    if (pushToTop) {
        m_messageQueue.push_front(message);
    } else {
        m_messageQueue.push_back(message);
    }

    connect(message.data(), &ApiMessage::raiseError,
            this,           &ApiGateway::handleError);
}

void ApiGateway::start()
{
    if (m_messageQueue.size() == 0){
        qDebug() << "No messages eleft";
        m_currentMessage.reset();
        return;
    }

    m_currentMessage = m_messageQueue.first();
    m_messageQueue.pop_front();

    sendMessage(m_currentMessage.data());
}

QUrl ApiGateway::buildStaticUrl(const QString &endpoint)
{
    return QString("http://s%1.%2/%3")
            .arg(m_serverId)
            .arg(m_serverDomain)
            .arg(endpoint);
}

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

QUrl ApiGateway::buildEndpointAjaxUrl(const QString &endpoint, const QList<QPair<QString, QString> > &data, bool includeRid) const
{
    return buildEndpointUrl(QString("ajax/%1").arg(endpoint), data, includeRid);
}

void ApiGateway::buildHeaders(QNetworkRequest &request) const
{
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0");
}

void ApiGateway::recursiveRedirect(const QString &url, const std::function<void (QNetworkReply *)> &callback)
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

void ApiGateway::handleError(ApiGatewayError::ErrorType errorType, const QStringList &args)
{
    ApiGatewayError error(errorType);

    auto message = error.message();
    if (!args.isEmpty()) {
        for (const auto arg : args) {
            message.arg(arg);
        }
    }

    const QString errorMessage = tr("Error on `%1` raised. %2").arg(error.toString()).arg(message);

    emit errorRaised(errorMessage); 

    qCritical() << errorMessage;
}

bool ApiGateway::handleNotLogged(const QString &operation)
{
    bool notLogged = !m_loggedIn;
    if (notLogged) {
        handleError(ApiGatewayError::ErrorType::NotLogged,
            { tr("Action %1 requires to be logged in.").arg(operation) });
    }

    return notLogged;
}
