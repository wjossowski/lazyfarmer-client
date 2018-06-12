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

#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QSharedPointer>

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
        virtual ~GlobalGameData() = default;

        const BuildingInfo buildingInfo (const QString &buildingId) const {
            return m_buildingInfos.value(buildingId, { "Unknown Building" });
        }
        const ProductInfo productInfo (const QString &productId) const {
            return m_productInfos.value(productId, { "Unknown Product", 0, 0, UINT32_MAX });
        }
        const ForestryInfo forestryInfo (const QString &forestryId) const {
            return m_forestryInfos.value(forestryId, { "Unknown Product" });
        }

        static void registerGameData(const QString &domain, const QVariant &data);
        static QSharedPointer<GlobalGameData> gameData(const QString &domain);

        QMap<QString, BuildingInfo> buildingInfos() const { return m_buildingInfos; }
        QMap<QString, ProductInfo> productInfos() const { return m_productInfos; }
        QMap<QString, ForestryInfo> forestryInfos() const { return m_forestryInfos; }

    private:
        explicit GlobalGameData(const QVariant &data = QVariant());

        void createProductInfo(const QVariantMap &baseData, const QVariantMap &constantsData);
        void createBuildingInfo(const QVariantMap &baseData);
        void createForestryInfo(const QVariantMap &baseData);

    private:
        QMap<QString, BuildingInfo> m_buildingInfos;
        QMap<QString, ProductInfo> m_productInfos;
        QMap<QString, ForestryInfo> m_forestryInfos;

        static QMap<QString, QSharedPointer<GlobalGameData>> m_gameData;

    };

}
