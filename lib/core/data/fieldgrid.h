/**
 ** This file is part of the LazyFarmer project.
 ** Copyright 2019 Wojciech Ossowski <w.j.ossowski@gmail.com>.
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

#include "field.h"

namespace Core {

    namespace Data {

        class FieldGrid : public IPlayerData
        {
            Q_OBJECT

        public:
            static constexpr int MAX_GRID_COLUMNS = 12;
            static constexpr int MAX_GRID_ROWS = 10;
            static constexpr int MAX_GRID_SIZE = MAX_GRID_COLUMNS * MAX_GRID_ROWS;

            using Ptr = QSharedPointer<FieldGrid>;

            explicit FieldGrid (Player *parent = nullptr);
            ~FieldGrid() override = default;

            Field::Ptr fieldAt(int index) const;

            void update(const QVariant &info) override;

        signals:
            void fieldChanged(int index) const;

        private:
            QList<Field::Ptr> m_fields;

        };

    }

}
