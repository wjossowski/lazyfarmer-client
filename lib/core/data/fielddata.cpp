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

#include "fielddata.h"

#include <QDebug>

using namespace Core;
using namespace Core::Data;

FieldData::FieldData(Player *parent)
    : BuildingData(parent)
    , m_fieldGrid(FieldGrid::Ptr::create(parent))
    , m_fieldModel(m_fieldGrid)
{

}

void FieldData::update(const QVariant &info)
{
    qDebug() << "Updating FieldsInfo" << info;

    BuildingData::update(info);
}
