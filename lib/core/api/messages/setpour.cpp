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

#include "setpour.h"
#include "../apigateway.h"

using namespace Api;
using namespace Messages;

SetPour::SetPour(ApiGateway *gateway, const PlantData &plantData)
    : OneWayMessage(gateway, MessageType::SetPour, "farm"),
      m_plantData(plantData)
{

}

const QList<QPair<QString, QString>> SetPour::constructedMessageData() const
{
    return {
        { "mode", "garden_water" },
        { "farm", QString::number(m_plantData.farmId) },
        { "position", QString::number(m_plantData.positionId) },
        { "feld[]", "1" }, // TODO: Create function to get valid positions
        { "felder[]", "1,2"}
    };
}
