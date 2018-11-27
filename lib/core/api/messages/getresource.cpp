/**
 ** This file is part of the LazyFarmer project.
 ** Copyright 2018 Wojciech Ossowski <zavafoj@gmail.com>.
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

#include "getresource.h"
#include "../apigateway.h"

#include "globalgamedata.h"

#include <QtCore/QMap>

using namespace Core;
using namespace Core::Api;
using namespace Core::Api::Messages;

GetResource::GetResource(ApiGateway *gateway,
                         const QString &context,
                         const QUrl &url)
    : ApiMessage (gateway, MessageType::GetResource, false)
    , m_context(context)
    , m_url(url)
{

}

void GetResource::handleResponse(QIODevice */*reply*/)
{
//    GlobalGameData::m_images.insert(m_context, QImage::)
}
