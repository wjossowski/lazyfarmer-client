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

#include <QtCore/QString>

namespace Core {

    namespace Data {

        static const int MAX_PLANT_COLUMNS = 12;
        static const int MAX_PLANT_ROWS = 10;

        enum class BuildingType {
            Farm,
            AnimalProduction,
            ResourceProduction,

            Unknown
        };

        struct BuildingHelper
        {
            static QString toString(BuildingType type)
            {
                switch (type) {
                case BuildingType::Farm: return "Farm";
                case BuildingType::AnimalProduction: return "AnimalProduction";
                case BuildingType::ResourceProduction: return "ResourceProduction";
                default: return "Unknown";
                }
            }
        };

        struct BuildingDetails
        {
            int farmId;
            int positionId;
        };

        struct ProductDetails
        {
            QString fieldIds () const
            {
                if (plantSize == 1) {
                    return QString::number(positionId);
                } else if (plantSize == 2) {
                    return QString("%1,%2")
                            .arg(positionId)
                            .arg(positionId+1);
                } else if (plantSize == 4) {
                    return QString("%1,%2,%3,%4")
                            .arg(positionId)
                            .arg(positionId+1)
                            .arg(positionId+MAX_PLANT_COLUMNS)
                            .arg(positionId+MAX_PLANT_COLUMNS+1);
                } else {
                    return QString();
                }
            }

            int productId;
            int plantSize;
            int positionId;
        };

        struct ProductionDetails
        {
            int productionId;
            int productionSlot;
        };

    }

}

