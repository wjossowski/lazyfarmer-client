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

#pragma once

#include "apigatewayerror.h"

#include <QtCore/QObject>

#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <functional>

class ApiGateway : public QObject
{
    Q_OBJECT

public:
    explicit ApiGateway(QObject *parent = nullptr);

    const QMap<QString, QString> &options () { return m_options; }
    void setOptions(const QMap<QString, QString> &options) { m_options = options; }
    inline bool isConfigured() const { return !m_options.isEmpty(); }

    inline bool isLoggedIn() const { return m_loggedIn; }
    void setLoggedIn (bool loggedIn);

    const QString &rid() const { return m_rid; }
    void extractRid(QNetworkReply *reply);

    void raiseError(ApiGatewayError::Type errorType, const QStringList &args = QStringList());

    QNetworkAccessManager *accessManager() { return &m_manager; }

signals:
    void loggedInChanged(bool changed) const;
    void errorRaised(const QString &message) const;

private:
    bool handleNotLogged(const QString &operation);

private:
    bool m_firstRun;
    bool m_loggedIn;

    QMap<QString, QString> m_options;

    QString m_rid;

    QNetworkAccessManager m_manager;
};
