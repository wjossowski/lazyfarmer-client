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
#include "player.h"
#include "globalgamedata.h"

using namespace Core;
using namespace Core::Data;

Field::Field(int fieldNo, Player *parent)
    : IPlayerData(parent)
    , m_fieldNo(fieldNo)
    , m_id(0)
    , m_remaining(0)
    , m_isWatered(false)
{

}

ProductDetails Field::details() const
{
    return {
        m_id,
        gameData()->productInfo(m_id).size,
        m_fieldNo
    };
}

void Field::update(const QVariant &info)
{
    const QVariantMap fieldInfo = info.toMap();

    int id = fieldInfo["Id"].toInt();
    int remaining = fieldInfo["Remaining"].toInt();
    bool isWatered = fieldInfo["IsWatered"].toBool();

    if (m_id != id
        || m_remaining != remaining
        || m_isWatered != isWatered)
    {
        m_id = id;
        m_remaining = remaining;
        m_isWatered = isWatered;

        emit fieldChanged();
    }

    IPlayerData::update(info);
}

QString Field::toString() const
{
    return QString("Field: %1 (id: %2) (fieldNo: %3), Watered: %4, Remaining: %5")
            .arg(m_name)
            .arg(m_id)
            .arg(m_fieldNo)
            .arg(m_isWatered ? "Yes" : "No")
            .arg(m_remaining);
}
