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

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QMetaEnum>
#include <QtCore/QRegularExpression>
#include <QtCore/QSettings>
#include <QtCore/QFile>
#include <QtCore/QTimer>

#include <QtDebug>

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
        setLoggedIn(true);

        if (m_firstRun) {
            GameInfoExtractor extractor(m_serverDomain);
            extractor.extract(content);
        }
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

void ApiGateway::queueMessage(const QSharedPointer<ApiMessage> &message)
{
    m_messageQueue.push_back(message);

    connect(message.data(), &ApiMessage::raiseError,
            this,           &ApiGateway::handleError);

    // TODO: Handle message response
}

void ApiGateway::start()
{
    if (m_messageQueue.size() == 0)
        return;

    m_currentMessage = m_messageQueue.first();
    m_messageQueue.pop_front();

    m_currentMessage->sendMessage();
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
