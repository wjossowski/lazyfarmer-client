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

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>

#include <functional>

class ApiGateway : public QObject
{
    Q_OBJECT

public:
    static constexpr auto ErrorsName = "Errors";

    explicit ApiGateway(QObject *parent = nullptr);
    void setOptions(const QVariantMap &options);

    void login();
    void logout();

    void getFarmInfo();

    inline bool isLoggedIn() const { return m_loggedIn; }
    inline bool isConfigured() const { return m_configured; }

signals:
    void loggedInChanged(bool changed);
    void errorRaised(const QString &message);

private:
    void recursiveRedirect(const QString &url, const std::function<void (QNetworkReply *)> &callback);
    void buildHeaders(QNetworkRequest &request) const;
    bool extractRid(QNetworkReply *reply);

    QUrl tokenUrl() const;
    QUrl endpointUrl(const QString &endpoint, const QList<QPair<QString, QString>> &data, bool includeRid = true) const;
    QUrl endpointAjaxUrl(const QString &endpoint, const QList<QPair<QString, QString>> &data, bool includeRid = true) const;

    void setLoggedIn (bool loggedIn);
    bool handleNotLogged(const QString &operation);

    void raiseError(ApiGatewayError::Type errorType, const QStringList &args = QStringList());

private:
    bool m_firstRun;
    bool m_loggedIn;
    bool m_configured;

    struct {
        QString login;
        QString password;
        QString domain;
        QString server;
    } m_options;

    QString m_rid;

    QNetworkAccessManager m_manager;
};
