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

#include "field.h"

using namespace Core;
using namespace Core::Data;

Field::Field(Player *parent)
    : IPlayerData(parent)
    , m_id(0)
    , m_fieldId(0)
    , m_remaining(0)
    , m_isWater(false)
{

}

Field::Field(const QVariant &info, Player *parent)
    : Field (parent)
{
    update(info);
}

void Field::update(const QVariant &info)
{
    const QVariantMap fieldInfo = info.toMap();

    int id = fieldInfo["Id"].toInt();
    int fieldId = fieldInfo["FieldId"].toInt();
    int remaining = fieldInfo["Remaining"].toInt();
    bool isWater = static_cast<bool>(fieldInfo["IsWater"].toInt());

    if (m_id != id
        || m_fieldId != fieldId
        || m_remaining != remaining
        || m_isWater != isWater)
    {
        m_id = id;
        m_fieldId = fieldId;
        m_remaining = remaining;
        m_isWater = isWater;

        emit fieldChanged();
    }
}
