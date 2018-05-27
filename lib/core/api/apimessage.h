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
#include <QtCore/QSharedPointer>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include <QtNetwork/QNetworkRequest>

class ApiGateway;
class QNetworkAccessManager;

enum class MessageType {
    MessageLogin,
    MessageLogout,
    MessageGetFarmInfo,
    MessageUnknown
};

class ApiMessage : public QObject
{
    Q_OBJECT

public:
    explicit ApiMessage(ApiGateway *gateway,
                        MessageType messageType = MessageType::MessageUnknown,
                        bool isLoginRequired = true);

    virtual ~ApiMessage();

    template <typename T>
    static QSharedPointer <ApiMessage> create (ApiGateway *gateway) { return QSharedPointer<T> (new T(gateway)); }

    bool isSent() const { return m_isSent; }
    void setIsSent(bool isSent) { m_isSent = isSent; }

    bool isResponseReceived() const { return m_isResponseReceived; }
    void setIsResponseReceived(bool isResponseReceived) { m_isResponseReceived = isResponseReceived; }

public slots:
    virtual void sendMessage() = 0;

signals:
    void raiseError(ApiGatewayError::ErrorType errorType, const QStringList &args = QStringList());
    void finished();

protected:
    void buildHeaders(QNetworkRequest &request) const;
    QUrl buildEndpointUrl(const QString &endpoint,
                     const QList<QPair<QString, QString>> &data,
                     bool includeRid = true) const;
    QUrl buildEndpointAjaxUrl(const QString &endpoint,
                         const QList<QPair<QString, QString>> &data,
                         bool includeRid = true) const;
    bool handleNotLogged(const QString &operation);

protected:
    MessageType m_messageType;
    ApiGateway *m_gateway;
    QNetworkAccessManager *m_manager;

    bool m_isLoginRequired;
    bool m_isSent;
    bool m_isResponseReceived;
};
