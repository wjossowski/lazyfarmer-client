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

#include "globalgamedata.h"

#include <QtCore/QJsonDocument>

#include <QDebug>

using namespace Core;
using namespace Core::Data;

QMap<QString, GlobalGameData::Ptr> GlobalGameData::m_gameData;
QMap<int, BuildingType> GlobalGameData::m_buildingTypes;

const QVariantMap childObject (const QVariant &object, const QString &property) {
    return object.toMap().value(property).toMap();
}

void GlobalGameData::registerGameData(const QString &domain,
                                      const QVariant &data)
{
    if (m_gameData.contains(domain)) {
        return;
    }

    m_gameData.insert(domain, GlobalGameData::Ptr(new GlobalGameData(data)));
}

GlobalGameData::Ptr GlobalGameData::gameData(const QString &domain)
{
    return m_gameData.value(domain, GlobalGameData::Ptr(new GlobalGameData));
}

bool GlobalGameData::loadBuildingTypes(const QByteArray &contents)
{
    const auto json = QJsonDocument::fromJson(contents).toVariant();

    if (!json.isValid()) {
        return false;
    }

    const QVariantList buildingInfoList = json.toList();
    for (const auto &info : buildingInfoList) {
        const QVariantMap data = info.toMap();

        int id = data["id"].toInt();

        QString category = data["category"].toString();
        if (category == "Field") {
            m_buildingTypes.insert(id, BuildingType::Farm);
        } else if (category == "AnimalProduction") {
            m_buildingTypes.insert(id, BuildingType::AnimalProduction);
        } else if (category == "ResourceProduction") {
            m_buildingTypes.insert(id, BuildingType::ResourceProduction);
        }
    }

    return true;
}

GlobalGameData::GlobalGameData(const QVariant &data)
{
    const auto contents = data.toMap();
    if (contents.isEmpty()) {
        return;
    }

    const auto baseObject = childObject(contents, "base");
    const auto constantsObject = childObject(contents, "constants");
    if (baseObject.isEmpty() || constantsObject.isEmpty()) {
        return;
    }

    createProductInfo(baseObject, constantsObject);
    createBuildingInfo(baseObject);
    createForestryInfo(baseObject);
}

void GlobalGameData::createProductInfo(const QVariantMap &baseData, const QVariantMap &constantsData)
{
    const auto productNames = childObject(baseData, "products");
    const auto productPrices = childObject(baseData, "products_prices");

    const auto productTime = childObject(constantsData, "product_time");

    const auto productX = childObject(constantsData, "product_x");
    const auto productY = childObject(constantsData, "product_y");

    const auto getSize = [&] (int xSize, int ySize) {
        return ySize == 2 ? 4 : (xSize == 2 ? 2 : 1);
    };

    const auto productIds = productNames.keys();
    for (const auto &id: productIds) {
        const QString name = productNames.value(id).toString();
        if (name.isEmpty()) continue;

        quint32 price = productPrices.value(id).toString().toDouble() * 100;
        if (price == 0) continue;

        quint32 time = productTime.value(id).toString().toInt();
        if (time == 0) continue;

        quint8 size = getSize(productX.value(id).toString().toInt(),
                              productY.value(id).toString().toInt());

        m_productInfos.insert(id.toInt(), { name, size, price, time });
    }
}

void GlobalGameData::createBuildingInfo(const QVariantMap &baseData)
{
    const auto buildingNames = childObject(baseData, "buildings");

    const auto buildingIds = buildingNames.keys();
    for (const auto &id: buildingIds) {
        const QString name = buildingNames.value(id).toString();
        if (name.isEmpty()) continue;

        m_buildingInfos.insert(id.toInt(), {name});
    }
}

void GlobalGameData::createForestryInfo(const QVariantMap &baseData)
{
    const auto forestryNames = childObject(baseData, "forestry");

    const auto forestryIds = forestryNames.keys();
    for (const auto &id: forestryIds) {
        const QString name = forestryNames.value(id).toString();
        if (name.isEmpty()) continue;

        m_forestryInfos.insert(id.toInt(), {name});
    }
}
