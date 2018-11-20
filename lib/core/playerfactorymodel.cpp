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


#include "playerfactorymodel.h"

using namespace Core;

PlayerFactoryModel::PlayerFactoryModel(QObject *parent)
    : QAbstractListModel (parent)
{

}

int PlayerFactoryModel::rowCount(const QModelIndex &) const
{
    return m_players.size();
}

QVariant PlayerFactoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    try {
        const auto player = m_players.at(index.row());
        const QByteArray roleName = roleNames() [role];

        return player->property(roleName);

    } catch (...) { }

    return QVariant();
}

QHash<int, QByteArray> PlayerFactoryModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(static_cast<int>(PlayerRoles::Level), "level");
    roles.insert(static_cast<int>(PlayerRoles::LevelDescription), "levelDescription");
    roles.insert(static_cast<int>(PlayerRoles::LevelPercentage), "levelPercentage");
    roles.insert(static_cast<int>(PlayerRoles::Money), "money");
    roles.insert(static_cast<int>(PlayerRoles::Description), "description");
    roles.insert(static_cast<int>(PlayerRoles::CurrentJob), "job");
    return roles;
}

QSharedPointer<Player> PlayerFactoryModel::create()
{
    QSharedPointer<Player> player(new Player());

    m_players.append(player);
    emit this->sizeChanged(this->size());

    return player;
}

void PlayerFactoryModel::remove(int i)
{
    m_players.removeAt(i);
    emit this->sizeChanged(this->size());
}
