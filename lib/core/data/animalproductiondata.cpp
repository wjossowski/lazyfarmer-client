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

#include "animalproductiondata.h"

#include <QtCore/QDebug>
#include <QtCore/QtMath>

using namespace Core;
using namespace Core::Data;

AnimalProductionData::AnimalProductionData(Player *parent)
    : BuildingData (parent)
    , m_outputId(-1)

    , m_totalTime(-1)
    , m_timeLeft(-1)
    , m_timeToRefeed (-1)

    , m_chosenProductId(-1)

    , m_feedAmount(0)
{

}

void AnimalProductionData::update(const QVariant &info)
{
    const auto buildingInfo = info.toMap();
    m_outputId      = buildingInfo.value("OutputProduct").toInt();
    m_totalTime     = buildingInfo.value("TotalTime").toInt();
    m_timeLeft      = buildingInfo.value("TimeLeft").toInt();
    m_timeToRefeed  = buildingInfo.value("TimeToRefeed").toInt();

    if (m_inputProductsData.isEmpty()) {
        // Fill possible product inputs
        m_chosenProductId = -1;

        // Store data for QML
        m_inputProductsInfo = buildingInfo["FeedInputInfo"].toList();

        // Store data:
        for (const auto &product : m_inputProductsInfo) {
            const auto productInfo = product.toMap();
            int productId = productInfo["In"].toInt();
            int reduction = productInfo["Remaining"].toInt();

            m_inputProductsData.insert(productId, reduction);
        }

    }

    IPlayerData::update(info);
    emit dataChanged();
}

QString AnimalProductionData::toString() const
{
    return QString ("FooBarAnimalProduction");
}

QVariant AnimalProductionData::toVariant()
{
    return QVariant::fromValue<AnimalProductionData*>(this);
}

void AnimalProductionData::setChosenProductId(int chosenProductId)
{
    bool isTheSameProduct = m_chosenProductId == chosenProductId;
    bool isProductNulled = chosenProductId == -1;

    if (isTheSameProduct || isProductNulled) {
        chosenProductId = -1;
        m_feedAmount = 0;
    } else {
        int chosenProductReducer = m_inputProductsData.value(chosenProductId, INT_MAX);
        m_feedAmount = qCeil(m_timeToRefeed / (chosenProductReducer));
    }

    m_chosenProductId = chosenProductId;
    emit chosenProductChanged(m_chosenProductId);
}
