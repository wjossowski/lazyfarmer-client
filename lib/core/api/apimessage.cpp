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
#include "apigateway.h"

#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>

ApiMessage::ApiMessage(ApiGateway *gateway, MessageType messageType, bool isLoginRequired)
    : QObject(gateway),

      m_messageType(messageType),

      m_gateway(gateway),
      m_manager(gateway->accessManager()),

      m_isLoginRequired(isLoginRequired),
      m_isSent(false),
      m_isResponseReceived(false)
{

}

ApiMessage::~ApiMessage() {
#if DEBUG_MODE
    qDebug() << "Destroying API Message" << this->metaObject()->className() << this;
#endif
}

void ApiMessage::buildHeaders(QNetworkRequest &request) const
{
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0");
}

QUrl ApiMessage::buildEndpointUrl(const QString &endpoint,
                                  const QList<QPair<QString, QString> > &data,
                                  bool includeRid) const
{
    QUrlQuery query;
    query.setQueryItems(data);
    if (includeRid) {
        query.addQueryItem("rid", m_gateway->rid());
    }

    const QString url = QString("http://s%1.%2/%3.php?%4")
            .arg(m_gateway->serverId())
            .arg(m_gateway->serverDomain())
            .arg(endpoint)
            .arg(query.toString());

#if DEBUG_MODE
    qDebug() << "Building Query:" << url << query.toString();
#endif

    return QUrl(url);
}

QUrl ApiMessage::buildEndpointAjaxUrl(const QString &endpoint,
                                      const QList<QPair<QString, QString> > &data,
                                      bool includeRid) const
{
    return buildEndpointUrl(QString("ajax/%1").arg(endpoint), data, includeRid);
}

bool ApiMessage::handleNotLogged(const QString &operation)
{
    bool notLogged = !m_gateway->isLoggedIn();
    if (notLogged) {
        raiseError(ApiGatewayError::ErrorType::NotLogged,
        { tr("Action %1 requires to be logged in.").arg(operation) });
    }

    return notLogged;
}
