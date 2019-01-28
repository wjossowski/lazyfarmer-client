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

#include "fieldgrid.h"

using namespace Core;
using namespace Core::Data;

FieldGrid::FieldGrid(Core::Player *parent)
    : IPlayerData(parent)
{
    for (int i = 0; i < MAX_GRID_SIZE; i++) {
        m_fields.append(Field::Ptr::create());
    }
}

Field::Ptr FieldGrid::fieldAt(int index) const
{
    if (m_fields.count() < index) {
        return nullptr;
    } else {
        return m_fields.at(index);
    }
}

void FieldGrid::update(const QVariant &info)
{
    const auto fieldList = info.toList();
    
    IPlayerData::update(info);
}
