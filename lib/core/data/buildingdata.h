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

#include "iplayerdata.h"

namespace Core {

    namespace Data {

        class BuildingData : public IPlayerData
        {
            Q_OBJECT

        public:
            using Ptr = QSharedPointer<BuildingData>;

            explicit BuildingData (Player *parent = nullptr) : IPlayerData (parent) { }
            ~BuildingData() override = default;

            virtual int totalTime() const = 0;

            static BuildingData::Ptr create(Player *player, BuildingType type);

        };

    }

}
