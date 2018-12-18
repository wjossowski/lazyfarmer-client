/**
 ** This file is part of the LazyFarmer project.
 ** Copyright 2017 Wojciech Ossowski <w.j.ossowski@gmail.com>.
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

#include "common.h"
#include "iplayerdata.h"
#include "buildingdata.h"

#include <QtCore/QVariantMap>

namespace Core {

    class Player;

    namespace Data {

        class Building : public IPlayerData
        {
            Q_OBJECT

        public:
            using Ptr = QSharedPointer<Building>;

            explicit Building(Player *parent = nullptr);
            ~Building() override = default;

            int id() const { return m_id; }
            BuildingType type() const { return m_type; }
            int farmId() const { return m_farmId; }
            int position() const { return m_position; }
            int level() const { return m_level; }
            int animals() const { return m_animals; }
            int remaining() const { return m_remaining; }
            QString name() const { return m_name; }

            bool isSetUp() const { return false; }
            bool isValid() const { return m_type != BuildingType::Unknown; }

            BuildingDetails details() const { return { m_farmId, m_position }; }

            void update(const QVariant &info) override;
            void updateBuildingData(const QVariant &info);

            QString toString() const override;

            BuildingData::Ptr buildingData() const { return m_buildingData; }

        signals:
            void fetchBuildingRequested(BuildingDetails details, BuildingType type) const;
            void buildingChanged() const;

        private:
            void initializeConnections() const;

        private:
            int m_id;
            BuildingType m_type;
            int m_farmId;
            int m_position;
            int m_level;
            int m_animals;
            int m_remaining;
            QString m_name;

            BuildingData::Ptr m_buildingData;

        };

    }

}
