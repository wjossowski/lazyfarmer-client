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
using namespace Model;

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

        if (static_cast<PlayerRoles>(role) == PlayerRoles::PlayerObject) {
            return QVariant::fromValue(player);
        } else {
            const QByteArray roleName = roleNames() [role];
            return player->property(roleName);
        }

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
    roles.insert(static_cast<int>(PlayerRoles::LastError), "lastError");
    return roles;
}

QSharedPointer<Player> PlayerFactoryModel::create()
{
    QSharedPointer<Player> player(new Player());

    int row = m_players.count();
    beginInsertRows(QModelIndex(), row, row);

    m_players.append(player);
    connect(&*player,   &Player::dataChanged, [&] () {
        emit dataChanged(index(0), index(m_players.size()-1));
    });

    endInsertRows();

    return player;
}

void PlayerFactoryModel::removeAt(int row)
{
    if (m_players.size() <= row) {
        return;
    }

    beginRemoveRows(QModelIndex(), row, row);
    auto player = m_players.at(row);
    player->deleteLater();

    m_players.removeAt(row);
    endRemoveRows();
}
