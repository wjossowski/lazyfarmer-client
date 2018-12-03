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

#pragma once

#include "data/common.h"

#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QSharedPointer>
#include <QtCore/QUrl>

#include <QtGui/QPixmap>
#include <QtGui/QIcon>

namespace Core {

    struct ResourceInfo
    {
        using Ptr = QSharedPointer<ResourceInfo>;

        ResourceInfo(const QString &url = QString(),
                     const QVariantList eraseAt = QVariantList(),
                     int baseSize = 0);

        QString url;
        int baseSize;

        QList<int> spritesToOmmit = QList<int>();
        QList<QPixmap> icons = QList<QPixmap>();
    };

    class ConfigReader
    {

    public:
        bool loadConfig(const QByteArray &contents);
        bool hasDownloadedResources();

        QStringList &availableDomains() { return m_availableDomains; }

        QMap<QString, ResourceInfo::Ptr> &resourceInfo() { return m_resourceInfos; }
        QUrl urlAt(const QString &key);
        QPixmap pixmapAt(const QString &key, int id);
        void storeResource(const QString &key, const QByteArray &data);

        Data::BuildingType buildingType (const int buildingId) {
            return m_buildingTypes.value(buildingId, Data::BuildingType::Unknown);
        }

        static ConfigReader &instance();

    private:
        explicit ConfigReader();

        ConfigReader(ConfigReader const&) = delete;
        ConfigReader(ConfigReader &&) = delete;
        ConfigReader& operator=(ConfigReader const&) = delete;
        ConfigReader& operator=(ConfigReader &&) = delete;

    private:
        QMap<int, Data::BuildingType> m_buildingTypes;
        QMap<QString, ResourceInfo::Ptr> m_resourceInfos;
        QStringList m_availableDomains;

    };

}

