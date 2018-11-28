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

#include "buildingmodel.h"

#include <QtCore/QDebug>

using namespace Core;
using namespace Model;

BuildingModel::BuildingModel(const Data::BuildingList::Ptr &buildings, QObject *parent)
    : QAbstractListModel (parent)
    , m_buildings(buildings)
{
    connect(&*buildings,    &Data::BuildingList::buildingListChanged,
            this,           &BuildingModel::reload);
}

int BuildingModel::rowCount(const QModelIndex &) const
{
    return m_buildings->size();
}

QVariant BuildingModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const auto building = m_buildings->buildingAt(index.row());
    if (!building->isValid()) {
        return QVariant();
    }

    auto dataRole = static_cast<BuildingRoles>(role);
    switch (dataRole) {
    case BuildingRoles::Name: return building->name();
    case BuildingRoles::Icon: return GlobalGameData::pixmapAt("buildings", building->id());
    case BuildingRoles::Id: return building->id();
    case BuildingRoles::Type: return static_cast<int>(building->type());
    case BuildingRoles::FarmId: return building->farmId();
    case BuildingRoles::Position: return building->position();
    case BuildingRoles::Level: return building->level();
    case BuildingRoles::Animals: return building->animals();
    case BuildingRoles::Remaining: return building->remaining();
    }

    return QVariant();
}

QHash<int, QByteArray> BuildingModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(static_cast<int>(BuildingRoles::Name), "name");
    roles.insert(static_cast<int>(BuildingRoles::Icon), "icon");
    roles.insert(static_cast<int>(BuildingRoles::Id), "id");
    roles.insert(static_cast<int>(BuildingRoles::Type), "type");
    roles.insert(static_cast<int>(BuildingRoles::FarmId), "farm");
    roles.insert(static_cast<int>(BuildingRoles::Position), "position");
    roles.insert(static_cast<int>(BuildingRoles::Level), "level");
    roles.insert(static_cast<int>(BuildingRoles::Animals), "animals");
    roles.insert(static_cast<int>(BuildingRoles::Remaining), "remaining");
    return roles;
}

void BuildingModel::reload()
{
    qDebug() << "Reloading Building Model" << m_buildings->size();

    beginResetModel();
    endResetModel();
}
