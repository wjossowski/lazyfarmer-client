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

#include "getfeed.h"

using namespace Core;
using namespace Core::Api;
using namespace Core::Api::Messages;

GetFeed::GetFeed(ApiGateway *gateway,
                 const BuildingData &buindingData)
    : OneWayMessage(gateway, MessageType::GetFeed, "farm"),
      m_buildingData(buindingData)
{

}

const QList<QPair<QString, QString> > GetFeed::constructedMessageData() const
{
    return {
        { "mode", "inner_crop" },
        { "farm", QString::number(m_buildingData.farmId) },
        { "position", QString::number(m_buildingData.positionId) }
    };
}
