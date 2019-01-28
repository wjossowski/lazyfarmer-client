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
#include <QtCore/QList>

constexpr const char* S_TranslationsPath = "Core/Translations";
constexpr const char* P_AvailableTranslations = "AvailableTranslations";

namespace Core {

    namespace Data {

        static const int MAX_PLANT_COLUMNS = 12;
        static const int MAX_PLANT_ROWS = 10;

        enum class BuildingType {
            Field                    = 1,
            AnimalProduction,
            ResourceProduction,

            Unknown
        };

        struct BuildingHelper
        {
            static QString toString(BuildingType type)
            {
                switch (type) {
                case BuildingType::Field: return "Field";
                case BuildingType::AnimalProduction: return "AnimalProduction";
                case BuildingType::ResourceProduction: return "ResourceProduction";
                default: return "Unknown";
                }
            }

            static bool isBuildingValid(BuildingType type)
            {
                return type != BuildingType::Unknown;
            }

            static BuildingType fromString (const QString &buildingType)
            {
                if (buildingType == "Field") {
                    return BuildingType::Field;
                } else if (buildingType == "AnimalProduction") {
                    return BuildingType::AnimalProduction;
                } else if (buildingType == "ResourceProduction") {
                    return BuildingType::ResourceProduction;
                } else {
                    return BuildingType::Unknown;
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
            static QList<int> neighbours(int pos, int size) {
                QList<int> neighbourFields;
                if (size == 2) {
                    neighbourFields << (pos + 1);
                } else if (size == 4) {
                    neighbourFields << (pos + 1)
                                    << (pos + MAX_PLANT_COLUMNS)
                                    << (pos + MAX_PLANT_COLUMNS + 1);
                }
                return neighbourFields;
            }

            QString fieldIds () const
            {
                QList<int> fields = {positionId};
                fields << neighbours(positionId, plantSize);

                QStringList fieldStrings;
                for (const auto& field : fields) {
                    fieldStrings << QString::number(field);
                }

                return fieldStrings.join(',');
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

