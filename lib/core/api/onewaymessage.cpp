/**
 ** This file is part of the LazyFarmer project.
 ** Copyright 2018 Wojciech Ossowski <w.j.ossowski@gmail.com>.
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

#include "onewaymessage.h"
#include "apigateway.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

using namespace Api;
using namespace Api::Messages;

OneWayMessage::OneWayMessage(ApiGateway *gateway,
                             MessageType type,
                             const QString &endpoint)
    : ApiMessage(gateway, type),
      m_endpointUrl(endpoint)
{

}

const QUrl OneWayMessage::url() const
{
    return m_gateway->buildEndpointAjaxUrl(m_endpointUrl,
                                           this->constructedMessageData());
}

void OneWayMessage::handleResponse(QNetworkReply *reply)
{
    Q_UNUSED (reply)
    emit finished();
}
