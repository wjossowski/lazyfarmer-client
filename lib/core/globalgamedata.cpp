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
#include <QtCore/QUrl>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QVariantList>

#include <QDebug>

using namespace Core;
using namespace Core::Data;

QMap<QString, GlobalGameData::Ptr> GlobalGameData::s_gameData;
QMap<int, BuildingType> GlobalGameData::s_buildingTypes;
QMap<QString, ResourceInfo::Ptr> GlobalGameData::s_resourceInfos;
QStringList GlobalGameData::s_availableDomains;

ResourceInfo::ResourceInfo(const QString &url,
                           const QVariantList eraseAt,
                           int baseSize)
    : url(url)
    , baseSize(baseSize)
{
    for (auto ommiter : eraseAt) {
        spritesToOmmit.append(ommiter.toInt());
    }
}


const QVariantMap childObject (const QVariant &object,
                               const QString &property)
{
    return object.toMap().value(property).toMap();
}

void GlobalGameData::registerGameData(const QString &domain,
                                      const QVariant &data)
{
    if (s_gameData.contains(domain)) {
        return;
    }

    s_gameData.insert(domain, GlobalGameData::Ptr(new GlobalGameData(data)));
}

GlobalGameData::Ptr GlobalGameData::gameData(const QString &domain)
{
    return s_gameData.value(domain, GlobalGameData::Ptr(new GlobalGameData));
}

bool GlobalGameData::loadConfig(const QByteArray &contents)
{
    const auto json = QJsonDocument::fromJson(contents).toVariant();

    if (!json.isValid()) {
        return false;
    }

    const auto configObject = json.toMap();

    const auto imageUrls = configObject["urls-images"].toMap();
    for (auto iterator = imageUrls.cbegin(); iterator != imageUrls.cend(); iterator++) {
        const QVariantMap resource = iterator.value().toMap();

        const int size = resource.value("size").toInt();
        const QString url = resource.value("url").toString();
        const QVariantList eraseAt = resource.value("erase-at").toList();

        s_resourceInfos.insert(iterator.key(), ResourceInfo::Ptr::create(url, eraseAt, size));
    }

    s_availableDomains = configObject["available-domains"].toStringList();

    const QVariantList buildingInfoList = configObject["building-config"].toList();
    for (const auto &info : buildingInfoList) {
        const QVariantMap data = info.toMap();

        int id = data["id"].toInt();

        QString category = data["category"].toString();
        if (category == "Field") {
            s_buildingTypes.insert(id, BuildingType::Farm);
        } else if (category == "AnimalProduction") {
            s_buildingTypes.insert(id, BuildingType::AnimalProduction);
        } else if (category == "ResourceProduction") {
            s_buildingTypes.insert(id, BuildingType::ResourceProduction);
        }
    }

    return true;
}

bool GlobalGameData::hasDownloadedResources()
{
    if (s_resourceInfos.isEmpty())
        return false;

    auto found = std::find_if (s_resourceInfos.cbegin(), s_resourceInfos.cend(), [] (const ResourceInfo::Ptr &iterator) {
        return iterator->icons.isEmpty();
    });

    return found == s_resourceInfos.cend();
}

QUrl GlobalGameData::urlAt(const QString &key)
{
    return s_resourceInfos.value(key, ResourceInfo::Ptr::create())->url;
}

QPixmap GlobalGameData::pixmapAt(const QString &key, int id)
{
    const auto icons = s_resourceInfos.value(key, ResourceInfo::Ptr::create())->icons;
    return (icons.size() <= id) ? icons.at(id -1) : QPixmap();
}

void GlobalGameData::storeResource(const QString &key, const QByteArray &data)
{
    auto info = s_resourceInfos.find(key);
    if (info == s_resourceInfos.end()) {
        return;
    }

    auto resource = *info;
    const auto image = QImage::fromData(data);

    int baseSize = resource->baseSize;
    int totalWidth = image.width();
    int totalHeight = image.height();

    if (totalWidth % baseSize != 0) {
        return;
    } else if (totalHeight % baseSize != 0) {
        return;
    }

    QList<QPixmap> icons;
    int index = 0; // used for filtering unnecessary pictures
    for (int h = 0; h < totalHeight; h += baseSize) {
        for (int w = 0; w < totalWidth; w += baseSize) {
            if (!resource->spritesToOmmit.contains(index)) {
                icons.append(QPixmap::fromImage(image.copy(w, h, baseSize, baseSize)));
            }

            index++;
        }
    }

#ifdef DEBUG_MODE
    for (int i = 0; i < icons.size(); i++) {
        const auto icon = icons.at(i);
        QDir temp("/tmp");
        temp.mkdir(key);
        icon.save(QString("/tmp/%1/%1_%2.png").arg(key).arg(i+1));
    }
#endif

    resource->icons = std::move(icons);

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

        quint32 price = static_cast<quint32>(productPrices.value(id).toString().toDouble() * 100);
        if (price == 0) continue;

        quint32 time = static_cast<quint32>(productTime.value(id).toString().toInt());
        if (time == 0) continue;

        int x = productX.value(id).toString().toInt();
        int y = productY.value(id).toString().toInt();

        quint8 size = static_cast<quint8>(getSize(x, y));

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
