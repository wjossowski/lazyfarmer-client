/**
 ** This file is part of the LazyFarmer project.
 ** Copyright 2018 Wojciech Ossowski <w.j.ossowski@gmail.com>.
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

#include "checkcredentials.h"
#include "../apigateway.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>

const QUrl Core::Api::Messages::CheckCredentials::url() const
{
    return QUrl(QString("https://www.%1/ajax/createtoken2.php?n=%2")
                .arg(m_gateway->serverDomain())
                .arg(QDateTime::currentMSecsSinceEpoch()));
}

void Core::Api::Messages::CheckCredentials::configureRequest(QNetworkRequest &request) const
{
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1SslV3);
    request.setSslConfiguration(config);
}

const QList<QPair<QString, QString> > Core::Api::Messages::CheckCredentials::postData() const
{
    return {
        { "server", m_gateway->serverId() },
        { "username", m_gateway->login() },
        { "password", m_gateway->password() },
        { "ref", "" },
        { "retid", "" }
    };
}

void Core::Api::Messages::CheckCredentials::handleResponse(QNetworkReply *reply)
{
    const auto response = QJsonDocument::fromJson(reply->readAll());
    const auto redirectUrl = QUrl(response.array().last().toString());

    if (redirectUrl.isValid()) {
        emit finished();
    } else {
        emit raiseError(ApiGatewayError::ErrorType::InvalidCredentials);
    }
}
