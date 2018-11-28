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

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QRegularExpression>
#include <QtCore/QSettings>

using namespace Core;
using namespace Core::Api;
using namespace Core::Api::Messages;

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
    const auto response = QJsonDocument::fromJson(reply->readAll());
    const auto redirectUrl = QUrl(response.array().last().toString());

    if (redirectUrl.isValid()) {
        m_gateway->recursiveRedirect(redirectUrl, [this] (QIODevice *reply) {
            this->extractRid(reply);
        });
    } else {
        emit raiseError(ApiGatewayError::ErrorType::InvalidCredentials);
    }
}

void Login::extractRid(QIODevice *reply)
{
    QSettings settings;
    settings.beginGroup("Lookup");

    bool firstRun = m_gateway->rid().isEmpty();
    const QString content = firstRun ? reply->readAll()
                                     : reply->read(settings.value("RidExtractorDeep", 1024).toInt());

    QRegularExpression ridRegex("var rid = '(?<rid>.*)'");
    const QString rid = ridRegex.match(content).captured("rid");

    m_gateway->setRid(rid);

    if (firstRun) {
        m_gateway->setBaseInfo(content);
    }

    if (!GlobalGameData::hasDownloadedResources()) {
        m_gateway->queueConfigResources();
    }

    emit finished();
}
