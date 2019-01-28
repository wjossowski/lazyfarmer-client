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

namespace Core {

    struct BuildingInfo
    {
        QString name;
    };

    struct ProductInfo
    {
        QString name;
        quint8 size;
        quint32 price;
        quint32 time;
    };

    struct ForestryInfo
    {
        QString name;
    };

    class GlobalGameData
    {
    public:
        using Ptr = QSharedPointer<GlobalGameData>;

        virtual ~GlobalGameData() = default;

        const BuildingInfo buildingInfo (const int buildingId) const {
            return m_buildingInfos.value(buildingId, { "Unknown" });
        }

        const ProductInfo productInfo (const int productId) const {
            return m_productInfos.value(productId, { "Unknown", 1, 0, UINT32_MAX });
        }
        int productSize(const int productId) {
            return productInfo(productId).size;
        }

        const ForestryInfo forestryInfo (const int forestryId) const {
            return m_forestryInfos.value(forestryId, { "Unknown" });
        }

        static void registerGameData(const QString &domain, const QVariant &data);
        static GlobalGameData::Ptr gameData(const QString &domain);

        QMap<int, BuildingInfo> buildingInfos() const { return m_buildingInfos; }
        QMap<int, ProductInfo> productInfos() const { return m_productInfos; }
        QMap<int, ForestryInfo> forestryInfos() const { return m_forestryInfos; }

    private:
        explicit GlobalGameData(const QVariant &data = QVariant());

        void createProductInfo(const QVariantMap &baseData, const QVariantMap &constantsData);
        void createBuildingInfo(const QVariantMap &baseData);
        void createForestryInfo(const QVariantMap &baseData);

    private:
        QMap<int, BuildingInfo> m_buildingInfos;
        QMap<int, ProductInfo> m_productInfos;
        QMap<int, ForestryInfo> m_forestryInfos;

        static QMap<QString, GlobalGameData::Ptr> s_gameData;

    };

}
