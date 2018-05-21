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
#include "messages.h"

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QMetaEnum>
#include <QtCore/QRegularExpression>
#include <QtCore/QSettings>
#include <QtCore/QFile>

#include <QtDebug>

ApiGateway::ApiGateway(QObject *parent)
    : QObject(parent)
    , m_firstRun(true)
    , m_loggedIn(false)
{

}

void ApiGateway::setLoggedIn(bool loggedIn)
{
    if (m_loggedIn != loggedIn) {
        m_loggedIn = loggedIn;

        qInfo() << (m_loggedIn ? tr("%1 successfully logged in, rid: '%2'").arg(m_options["login"]).arg(m_rid)
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

    QString content = m_firstRun ? reply->readAll()
                                 : reply->read(settings.value("RidExtractorDeep", 1024).toInt());

    QRegularExpression ridRegex("var rid = '(?<rid>.*)'");
    m_rid = ridRegex.match(content).captured("rid");

    if (!m_rid.isEmpty()) {
        setLoggedIn(true);

        if (m_firstRun) {
            GameInfoExtractor extractor(m_options["domain"]);
            extractor.extract(content);
        }
    } else {
        raiseError(ApiGatewayError::RidNotParsed);
    }
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

bool ApiGateway::handleNotLogged(const QString &operation)
{
    bool notLogged = !m_loggedIn;
    if (notLogged) {
        raiseError(ApiGatewayError::NotLogged,
                   { tr("Action %1 requires to be logged in.").arg(operation) });
    }

    return notLogged;
}
