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

#include "loginmessage.h"
#include "../apigateway.h"

void LoginMessage::sendMessage()
{
    if (!m_gateway->isConfigured()) {
        m_gateway->handleError(ApiGatewayError::ErrorType::NotConfigured);
        return;
    }

    QNetworkRequest request(tokenUrl());

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1SslV3);
    request.setSslConfiguration(config);

    buildHeaders(request);

    QUrlQuery credentials;
    credentials.setQueryItems({
        { "server", m_gateway->serverId() },
        { "username", m_gateway->login() },
        { "password", m_gateway->password() },
        { "ref", "" },
        { "retid", "" }
    });

    auto reply = m_manager->post(request, credentials.toString().toLocal8Bit());
    connect(reply, &QNetworkReply::finished, [this, reply] () {
        const auto data = QJsonDocument::fromJson(reply->readAll());
        if (data.isArray()) {
            recursiveRedirect(data.array().last().toString(), [this] (QNetworkReply *reply) {
                m_gateway->extractRid(reply);
            });
        } else {
            raiseError(ApiGatewayError::ErrorType::InvalidCredentials);
        }
    });
}

QUrl LoginMessage::tokenUrl() const
{
    return QUrl(QString("https://www.%1/ajax/createtoken2.php?n=%2")
                .arg(m_gateway->serverDomain())
                .arg(QDateTime::currentMSecsSinceEpoch()));
}

void LoginMessage::recursiveRedirect(const QString &url, const std::function<void (QNetworkReply *)> &callback)
{
    QNetworkRequest request(url);
    buildHeaders(request);

    auto reply = m_manager->get(request);
    connect(reply, &QNetworkReply::finished, [this, reply, callback] () {
        const auto redirectUrl = reply->header(QNetworkRequest::LocationHeader);
        if (redirectUrl.isValid()) {
            recursiveRedirect(redirectUrl.toString(), callback);
        } else {
            callback(reply);
        }
    });
}
