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

#include "../apigateway.h"

using namespace Core;
using namespace Core::Api;
using namespace Core::Api::Messages;

ApiMessage::ApiMessage(ApiGateway *gateway,
                       MessageType messageType,
                       bool isLoginRequired)
    : QObject(gateway),

      m_gateway(gateway),

      m_messageType(messageType),

      m_isLoginRequired(isLoginRequired),
      m_isSent(false)
{

}

ApiMessage::~ApiMessage() {
#if DEBUG_MODE
    qDebug() << "Destroying API Message" << MessageHelper::toString(this->m_messageType) << this;
#endif
}
