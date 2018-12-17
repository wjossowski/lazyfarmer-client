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

#include "configreader.h"

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QJsonDocument>
#include <QtCore/QVariantList>

#include <QDebug>

using namespace Core;
using namespace Core::Data;

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

bool ConfigReader::loadConfig(const QByteArray &contents)
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

        m_resourceInfos.insert(iterator.key(), ResourceInfo::Ptr::create(url, eraseAt, size));
    }

    m_availableDomains = configObject["available-domains"].toStringList();

    const auto buildingTypeInfo = configObject["building-config"].toMap();
    for (auto iterator = buildingTypeInfo.cbegin(); iterator != buildingTypeInfo.cend(); iterator++) {
        const auto storedBuildingType = iterator.key();
        const auto buildingTypeIds = iterator.value().toList();

        BuildingType mappedBuildingType = BuildingHelper::fromString(storedBuildingType);
        if (!BuildingHelper::isBuildingValid(mappedBuildingType)) {
            continue;
        }

        for (auto id : buildingTypeIds) {
            m_buildingTypes.insert(id.toInt(), mappedBuildingType);
        }
    }

    return true;
}

bool ConfigReader::hasDownloadedResources()
{
    if (m_resourceInfos.isEmpty())
        return false;

    auto found = std::find_if (m_resourceInfos.cbegin(), m_resourceInfos.cend(),
                               [] (const ResourceInfo::Ptr &iterator)
    {
        return iterator->icons.isEmpty();
    });

    return found == m_resourceInfos.cend();
}

QUrl ConfigReader::urlAt(const QString &key)
{
    return m_resourceInfos.value(key, ResourceInfo::Ptr::create())->url;
}

QPixmap ConfigReader::pixmapAt(const QString &key, int id)
{
    const auto icons = m_resourceInfos.value(key, ResourceInfo::Ptr::create())->icons;
    return (icons.size() >= id) ? icons.at(id -1) : QPixmap();
}

void ConfigReader::storeResource(const QString &key, const QByteArray &data)
{
    auto info = m_resourceInfos.find(key);
    if (info == m_resourceInfos.end()) {
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

ConfigReader &ConfigReader::instance()
{
    static ConfigReader reader;
    return reader;
}

ConfigReader::ConfigReader()
{
    qDebug() << "Creating config reader";
}
