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
#include "messages/apimessage.h"

#include <QtCore/QObject>
#include <QtCore/QQueue>

#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>

#include <QtNetwork/QNetworkAccessManager>

#include <functional>

class QNetworkReply;

namespace Api {

    struct ApiOptions {
        QString serverId, serverDomain, login, password;
    };

    class ApiGateway : public QObject
    {
        Q_OBJECT

    public:
        explicit ApiGateway(QObject *parent = nullptr);

        bool isConfigured() const;

        bool isLoggedIn() const { return m_loggedIn; }
        void setLoggedIn (bool loggedIn);

        const QString &rid() const { return m_rid; }
        void extractRid(QNetworkReply *reply);

        const QString &serverId() const { return m_serverId; }
        const QString &serverDomain() const { return m_serverDomain; }
        const QString &login() const { return m_login; }
        const QString &password() const { return m_password; }

        void setApiOptions(const ApiOptions &options);

        void queueMessage(const QSharedPointer<Messages::ApiMessage> &message, bool pushToTop = false);
        void start();

        QUrl buildStaticUrl(const QString &endpoint);

        QUrl buildEndpointUrl(const QString &endpoint,
                              const QList<QPair<QString, QString>> &data,
                              bool includeRid = true) const;

        QUrl buildEndpointAjaxUrl(const QString &endpoint,
                                  const QList<QPair<QString, QString>> &data,
                                  bool includeRid = true) const;

        void buildHeaders(QNetworkRequest &request) const;
        void recursiveRedirect(const QString &url,
                               const std::function<void (QNetworkReply *)> &callback);
        void sendMessage(Messages::ApiMessage *message);

        QNetworkAccessManager *accessManager() { return &m_manager; }

        void extractGameData();
        void handleError(ApiGatewayError::ErrorType errorType, const QStringList &args = QStringList());

    signals:
        void loggedInChanged(bool changed) const;
        void errorRaised(const QString &message) const;

    private:
        void queueConstantData(const QString &content);
        bool handleNotLogged(const QString &operation);

    private:
        bool m_firstRun;
        bool m_loggedIn;

        QString m_serverId;
        QString m_serverDomain;
        QString m_login;
        QString m_password;

        QString m_rid;

        QQueue<QSharedPointer<Messages::ApiMessage>> m_messageQueue;
        QSharedPointer<Messages::ApiMessage> m_currentMessage;
        QNetworkAccessManager m_manager;
    };

}

