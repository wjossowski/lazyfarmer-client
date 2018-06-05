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

#include "login.h"
#include "../apigateway.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>

using namespace Api;
using namespace Messages;

const QUrl Login::url() const
{
    return QUrl(QString("https://www.%1/ajax/createtoken2.php?n=%2")
                .arg(m_gateway->serverDomain())
                .arg(QDateTime::currentMSecsSinceEpoch()));
}

void Login::configureRequest(QNetworkRequest &request) const
{
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1SslV3);
    request.setSslConfiguration(config);
}

const QList<QPair<QString, QString> > Login::postData() const
{
    return {
        { "server", m_gateway->serverId() },
        { "username", m_gateway->login() },
        { "password", m_gateway->password() },
        { "ref", "" },
        { "retid", "" }
    };
}

void Login::handleResponse(QNetworkReply *reply)
{
    const auto data = QJsonDocument::fromJson(reply->readAll());
    if (data.isArray()) {
        m_gateway->recursiveRedirect(data.array().last().toString(), [this] (QNetworkReply *reply) {
            m_gateway->extractRid(reply);
            emit finished();
        });
    } else {
        emit raiseError(ApiGatewayError::ErrorType::InvalidCredentials);
    }
}
