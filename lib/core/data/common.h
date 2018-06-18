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

        static const unsigned int MAX_PLANT_COLUMNS = 12;
        static const unsigned int MAX_PLANT_ROWS = 10;

        struct Building
        {
            unsigned int farmId;
            unsigned int positionId;
        };

        struct Product
        {
            explicit Product (unsigned int product_id = 1,
                              unsigned int plant_size = 1,
                              unsigned int position_id = 1)
                : productId(product_id), plantSize(plant_size), positionId(position_id) { }

            QString fieldIds () const {
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
                            .arg(positionId+MAX_PLANT_COLUMNS+1);;
                } else {
                    return QString();
                }
            }

            unsigned int productId;
            unsigned int plantSize;
            unsigned int positionId;
        };

        struct Production
        {
            explicit Production(unsigned int production_id = 1,
                                unsigned int production_slot = 1)
                : productionId(production_id), productionSlot(production_slot) { }

            unsigned int productionId;
            unsigned int productionSlot;

        };

    }

}

