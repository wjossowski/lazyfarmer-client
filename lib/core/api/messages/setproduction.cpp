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

#include "setproduction.h"
#include "../apigateway.h"

using namespace Api;
using namespace Messages;

SetProduction::SetProduction(ApiGateway *gateway,
                        const BuildingData &buindingData,
                        const ProductData &productData,
                        const ProductionData &productionData)
    : OneWayMessage(gateway, MessageType::SetPlant, "farm"),
      m_buildingData(buindingData),
      m_productData(productData),
      m_productionData(productionData)
{

}

const QList<QPair<QString, QString> > SetProduction::constructedMessageData() const
{
    return {
        { "mode", "setadvancedproduction" },
        { "farm", QString::number(m_buildingData.farmId) },
        { "position", QString::number(m_buildingData.positionId) },
        { "id", QString::number(m_productData.productId) },
        { "product", QString::number(m_productData.productId) },
        { "guildjob", "undefined" }
    };
}
