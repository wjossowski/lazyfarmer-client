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

#include "resourceproductiondata.h"

#include <QtCore/QDebug>

using namespace Core;
using namespace Core::Data;

ResourceProductionData::ResourceProductionData(Player *parent)
    : BuildingData (parent)

    , m_outputProductId(-1)

    , m_inputProductId(-1)
    , m_productAmount(-1)
{

}

void ResourceProductionData::update(const QVariant &info)
{
    const auto resourceProductionInfo = info.toMap();

    m_inputProductInfos = resourceProductionInfo.value("ProductionInfo").toList();

    int outputProductId = resourceProductionInfo.value("CurrentProduction", -1).toInt();
    setOutputProductId(outputProductId);

    emit dataChanged();
}

void ResourceProductionData::setOutputProductId(int outputProductId)
{
    bool isTheSameProduct = m_outputProductId == outputProductId;
    bool isProductNulled = outputProductId == -1;

    if (isTheSameProduct || isProductNulled) {
        m_outputProductId = -1;
        m_inputProductId = -1;

        m_productAmount = -1;
    } else {
        m_outputProductId = outputProductId;

        QListIterator<QVariant> productsIterator(m_inputProductInfos);
        while (productsIterator.hasNext()) {
            const auto productObject = productsIterator.next().toMap();
            int storedOutputProduct = productObject.value("Out", -1).toInt();

            if (storedOutputProduct == outputProductId) {
                m_inputProductId = productObject.value("In", -1).toInt();
                m_productAmount = productObject.value("Need", -1).toInt();
                m_totalTime = productObject.value("Remaining", -1).toInt();

                break;
            }

        }
    }


    emit outputProductChanged(m_outputProductId);

}
