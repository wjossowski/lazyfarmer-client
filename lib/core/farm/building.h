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

#include <QtCore/QVariant>

namespace Farm {

    class Building
    {
    public:
        explicit Building();

        inline int type() const { return m_type; }
        inline int farmId() const { return m_farmId; }
        inline int position() const { return m_position; }
        inline int level() const { return m_level; }
        inline int animals() const { return m_animals; }
        inline int remaining() const { return m_remaining; }

        void update(const QVariantMap &buildingInfo);

    private:
        int m_type;
        int m_farmId;
        int m_position;
        int m_level;
        int m_animals;
        int m_remaining;
    };

}

