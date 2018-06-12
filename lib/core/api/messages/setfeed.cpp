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

#include "setfeed.h"
#include "../apigateway.h"

using namespace Core;
using namespace Core::Api;
using namespace Core::Api::Messages;

SetFeed::SetFeed(ApiGateway *gateway,
                 const BuildingData &buindingData,
                 const ProductData &productData)
    : OneWayMessage(gateway, MessageType::SetFeed, "farm"),
      m_buildingData(buindingData),
      m_productData(productData)
{

}

const QList<QPair<QString, QString> > SetFeed::constructedMessageData() const
{
    return {
        { "mode", "inner_feed" },
        { "farm", QString::number(m_buildingData.farmId) },
        { "position", QString::number(m_buildingData.positionId) },
        { "pid", QString::number(m_productData.productId) },
        { "c", QString("%1_9|").arg(m_productData.productId) },
        { "guildjob", "0" }
    };
}
