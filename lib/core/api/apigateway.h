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
#include "../globalgamedata.h"

#include <QtCore/QObject>
#include <QtCore/QQueue>

#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>

#include <QtNetwork/QNetworkAccessManager>

#include <functional>

class QNetworkReply;


namespace Core {

    namespace Api {

        struct ApiOptions {
            QString serverId, serverDomain, login, password;
        };

        class ApiGateway : public QObject
        {
            Q_OBJECT

        public:
            enum class PushMessageTo {
                Top,
                Bottom
            };

            explicit ApiGateway(QObject *parent = nullptr);

            bool isConfigured() const;

            bool isLoggedIn() const { return m_loggedIn; }
            void setLoggedIn (bool loggedIn);

            const QString &rid() const { return m_rid; }
            void setRid(const QString &rid);

            void setBaseInfo(const QString &content);

            const QString &serverId() const { return m_serverId; }
            const QString &serverDomain() const { return m_serverDomain; }
            const QString &login() const { return m_login; }
            const QString &password() const { return m_password; }

            void setApiOptions(const ApiOptions &options);

            void queueMessage(const Messages::ApiMessage::Ptr &message, PushMessageTo placement = PushMessageTo::Bottom);
            void start();

            QUrl buildStaticUrl(const QString &endpoint);

            QUrl buildEndpointUrl(const QString &endpoint,
                                  const QList<QPair<QString, QString>> &data,
                                  bool includeRid = true) const;

            QUrl buildEndpointAjaxUrl(const QString &endpoint,
                                      const QList<QPair<QString, QString>> &data,
                                      bool includeRid = true) const;

            void buildHeaders(QNetworkRequest &request) const;
            void recursiveRedirect(const QUrl &url,
                                   const std::function<void (QIODevice *)> &callback);
            void sendMessage(Messages::ApiMessage *message);

            void extractGameData();
            GlobalGameData::Ptr gameData() const;

            void handlePlayerData(const QByteArray &playerData) const;
            void handleBuildingUpdate(int farm, int pos, const QVariant &data) const;

            void handleError(ApiGatewayError::ErrorType errorType, const QStringList &args = QStringList());

        public slots:
            void requestBuildingUpdate(const Data::BuildingDetails &details, const Data::BuildingType &type);

        signals:
            void buildingDataUpdated(int farm, int pos, const QVariant &data) const;
            void playerDataUpdated(const QByteArray &data) const;
            void loggedInChanged(bool changed) const;
            void errorRaised(const QString &message) const;

        private:
            void queueConstantData(const QString &content);

        private:
            bool m_loggedIn;

            QString m_serverId;
            QString m_serverDomain;
            QString m_login;
            QString m_password;

            QString m_rid;

            QQueue<Messages::ApiMessage::Ptr> m_messageQueue;
            Messages::ApiMessage::Ptr m_currentMessage;

            QNetworkAccessManager m_manager;
        };

    }

}


