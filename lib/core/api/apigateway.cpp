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

#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QMetaEnum>
#include <QtCore/QRegularExpression>
#include <QtCore/QSettings>
#include <QtCore/QFile>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QtDebug>

ApiGateway::ApiGateway(QObject *parent)
    : QObject(parent)
    , m_firstRun(true)
    , m_loggedIn(false)
    , m_configured(false)
{

}

void ApiGateway::setOptions(const QVariantMap &options)
{
    m_options = {
        options["login"].toString(),
        options["password"].toString(),
        options["domain"].toString(),
        options["server"].toString()
    };

    m_configured = true;
}

void ApiGateway::login()
{
    if (!m_configured) {
        raiseError(ApiGatewayError::NotConfigured);
        return;
    }

    QNetworkRequest request(tokenUrl());

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1SslV3);
    request.setSslConfiguration(config);

    buildHeaders(request);

    QUrlQuery credentials;
    credentials.setQueryItems({
        { "server", m_options.server },
        { "username", m_options.login },
        { "password", m_options.password },
        { "ref", "" }, { "retid", "" }
    });

    auto reply = m_manager.post(request, credentials.toString().toLocal8Bit());
    connect(reply, &QNetworkReply::finished, [this, reply] () {
        const auto data = QJsonDocument::fromJson(reply->readAll());
        if (data.isArray()) {
            recursiveRedirect(data.array().last().toString(), [this] (QNetworkReply *reply) {
                if (extractRid(reply)) {
                    setLoggedIn(true);
                    getFarmInfo();
                } else {
                    raiseError(ApiGatewayError::RidNotParsed);
                }
            });
        } else {
            raiseError(ApiGatewayError::InvalidCredentials);
        }
    });
}

void ApiGateway::logout()
{
    QNetworkRequest request(endpointUrl("main", {
        { "page", "logout" },
        { "logoutbutton", "1" }
    }, false));
    buildHeaders(request);

    auto reply = m_manager.get(request);
    connect(reply, &QNetworkReply::finished, [this, reply] () {
        setLoggedIn(false);
        m_manager.clearAccessCache();
    });
}

void ApiGateway::getFarmInfo()
{
    if (handleNotLogged(FUNCTION_NAME))
        return;

    QNetworkRequest request(endpointAjaxUrl("farm", {
        { "mode", "getfarms" }
    }));

    auto reply = m_manager.get(request);
    connect(reply, &QNetworkReply::finished, [this, reply] {
        QFile f("D:\\chuj.json");
        if (f.open(QFile::WriteOnly))
            f.write(reply->readAll());
    });
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
        } else {
            callback(reply);
        }
    });
}

void ApiGateway::buildHeaders(QNetworkRequest &request) const
{
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0");
}

bool ApiGateway::extractRid(QNetworkReply *reply)
{
    QSettings settings;
    settings.beginGroup("Lookup");

    QString content = m_firstRun ? reply->readAll()
                                 : reply->read(settings.value("RidExtractorDeep", 1024).toInt());

    if (content.isEmpty())
        return false;

    QRegularExpression ridRegex("var rid = '(?<rid>.*)'");
    m_rid = ridRegex.match(content).captured("rid");

    if (m_firstRun) {
        GameInfoExtractor extractor(m_options.domain);
        extractor.extract(content);
    }

    return !m_rid.isEmpty();
}

QUrl ApiGateway::tokenUrl() const
{
    return QUrl(QString("https://www.%1/ajax/createtoken2.php?n=%2")
                .arg(m_options.domain)
                .arg(QDateTime::currentMSecsSinceEpoch()));
}

QUrl ApiGateway::endpointUrl(const QString &endpoint,
                                 const QList<QPair<QString, QString> > &data,
                                 bool includeRid) const
{
    QUrlQuery query;
    query.setQueryItems(data);
    if (includeRid) {
        query.addQueryItem("rid", m_rid);
    }

    const QString url = QString("http://s%1.%2/%3.php?%4")
            .arg(m_options.server)
            .arg(m_options.domain)
            .arg(endpoint)
            .arg(query.toString());

    return QUrl(url);
}

QUrl ApiGateway::endpointAjaxUrl(const QString &endpoint, const QList<QPair<QString, QString> > &data, bool includeRid) const
{
    return endpointUrl(QString("ajax/%1").arg(endpoint), data, includeRid);
}

void ApiGateway::setLoggedIn(bool loggedIn)
{
    if (m_loggedIn != loggedIn) {
        m_loggedIn = loggedIn;

        qInfo() << (m_loggedIn ? tr("%1 successfully logged in, rid: '%2'").arg(m_options.login).arg(m_rid)
                               : tr("Logged out"));

        emit loggedInChanged(m_loggedIn);
    }
}

bool ApiGateway::handleNotLogged(const QString &operation)
{
    bool notLogged = !m_loggedIn;
    if (notLogged) {
        raiseError(ApiGatewayError::NotLogged, { tr("Action %1 requires to be logged in.").arg(operation) });
    }

    return notLogged;
}

void ApiGateway::raiseError(ApiGatewayError::Type errorType, const QStringList &args)
{
    ApiGatewayError error(errorType);

    auto message = error.message();
    if (!args.isEmpty()) {
        for (const auto arg : args) {
            message.arg(arg);
        }
    }

    const QString errorMessage = tr("Error `%1` raised. %2").arg(error.toString()).arg(message);

    emit errorRaised(errorMessage);
    qCritical() << errorMessage;
}
