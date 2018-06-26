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

#include "building.h"
#include "player.h"
#include "globalgamedata.h"

using namespace Core;
using namespace Core::Data;

Building::Building(Player *parent)
    : IPlayerData(parent)
    , m_type(0)
    , m_farmId(0)
    , m_position(0)
    , m_level(0)
    , m_animals(0)
    , m_remaining(0)
{

}

void Building::update(const QVariant &info)
{
    const QVariantMap buildingInfo = info.toMap();

    int type = buildingInfo["Type"].toInt();
    int farmId = buildingInfo["FarmId"].toInt();
    int position = buildingInfo["Position"].toInt();
    int level = buildingInfo["Level"].toInt();
    int animals = buildingInfo["Animals"].toInt();
    int remaining = buildingInfo["Remaining"].toInt();

    if (m_type != type
        || m_farmId != farmId
        || m_position != position
        || m_level != level
        || m_animals != animals
        || m_remaining != remaining)
    {
        m_type = type;
        m_farmId = farmId;
        m_position = position;
        m_level = level;
        m_animals = animals;
        m_remaining = remaining;

        m_name = m_owner->gameData()->buildingInfo(m_type).name;

        emit buildingChanged();
    }

}

QString Building::toString() const
{
    return QString("Building: %1 (id:%2) (%3, %4) Level: %5 Remaining: %6")
            .arg(m_name)
            .arg(m_type)
            .arg(m_farmId)
            .arg(m_position)
            .arg(m_level)
            .arg(m_remaining);
}
