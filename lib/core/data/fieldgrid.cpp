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

#include <QtCore/QDebug>

using namespace Core;
using namespace Core::Data;

FieldGrid::FieldGrid(Core::Player *parent)
    : IPlayerData(parent)
{
    for (int fieldId = 0; fieldId < MAX_GRID_SIZE; fieldId++) {
        auto field = Field::Ptr::create(fieldId, parent);
        connect(&*field, &Field::fieldChanged, [&] () {
            emit fieldChanged(fieldId);
        });

        m_fields.append(field);
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
    const auto fieldInfos = info.toMap()["FieldsInfo"].toList();

    for (const auto &fieldInfoRecord : fieldInfos) {
        const auto recordObject = fieldInfoRecord.toMap();
        int fieldNo = recordObject["FieldId"].toInt() - 1;

        auto field = fieldAt(fieldNo);
        if (field) {
            qDebug() << "Updating field" << fieldNo;
            field->update(fieldInfoRecord);
        }
    }
    
    IPlayerData::update(info);
}
