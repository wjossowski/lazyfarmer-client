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

#include "fieldmodel.h"

using namespace Core;
using namespace Core::Data;

Model::FieldModel::FieldModel(FieldGrid::Ptr fieldGrid, QObject *parent)
    : QAbstractListModel(parent)
    , m_fieldGrid(fieldGrid)
{
    connect(&*m_fieldGrid, &Data::FieldGrid::fieldChanged, [&] (int row) {
        emit dataChanged(index(row), index(row));
    });
}

QVariant Model::FieldModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const auto field = m_fieldGrid->fieldAt(index.row());
    if (!field) {
        return QVariant();
    }

    auto dataRole = static_cast<FieldRoles>(role);
    switch (dataRole) {
    case FieldRoles::Name: return field->name();
    case FieldRoles::Id: return field->id();
    case FieldRoles::FieldNo: return field->fieldNo();
    case FieldRoles::DoneTimestamp: return field->remaining();
    case FieldRoles::BaseTimestamp: return field->remaining();
    case FieldRoles::IsMainFieldBlock: return field->isMainBlock();
    case FieldRoles::IsNotEmpty: return !field->isEmpty();
    case FieldRoles::IsWatered: return field->isWatered();
    case FieldRoles::RowSpan: {
        int size = field->size();
        return (size == 2 || size == 4) ? 2 : 1;
    }
    case FieldRoles::ColumnSpan: {
        int size = field->size();
        return (size == 4) ? 2 : 1;
    }
    }

    return QVariant();
}

bool Model::FieldModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index)
    Q_UNUSED(value)
    Q_UNUSED(role)

    return false;
}

QHash<int, QByteArray> Model::FieldModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(static_cast<int>(FieldRoles::Name),                "name");
    roles.insert(static_cast<int>(FieldRoles::Id),                  "id");
    roles.insert(static_cast<int>(FieldRoles::FieldNo),             "fieldNo");
    roles.insert(static_cast<int>(FieldRoles::DoneTimestamp),       "doneTimestamp");
    roles.insert(static_cast<int>(FieldRoles::BaseTimestamp),       "baseTimeout");
    roles.insert(static_cast<int>(FieldRoles::IsMainFieldBlock),    "isMainFieldBlock");
    roles.insert(static_cast<int>(FieldRoles::IsNotEmpty),          "isNotEmpty");
    roles.insert(static_cast<int>(FieldRoles::IsWatered),           "isWatered");
    roles.insert(static_cast<int>(FieldRoles::RowSpan),             "rowSpan");
    roles.insert(static_cast<int>(FieldRoles::ColumnSpan),          "columnSpan");
    return roles;
}
