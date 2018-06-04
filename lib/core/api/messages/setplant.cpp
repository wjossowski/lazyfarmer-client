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

#include "setplant.h"
#include "../apigateway.h"

using namespace Api;
using namespace Messages;

SetPlant::SetPlant(ApiGateway *gateway,
                   const BuildingData &buindingData,
                   const PlantData &plantData)
    : OneWayMessage(gateway, MessageType::SetPlant, "farm"),
      m_buildingData(buindingData),
      m_plantData(plantData)
{

}

const QList<QPair<QString, QString> > SetPlant::constructedMessageData() const
{
    return {
        { "mode", "garden_plant" },
        { "farm", QString::number(m_buildingData.farmId) },
        { "position", QString::number(m_buildingData.positionId) },
        { "pflanze[]", QString::number(m_plantData.plantId) },
        { "feld[]", QString::number(m_plantData.positionId) },
        { "felder[]", m_plantData.fieldIds() }
    };
}
