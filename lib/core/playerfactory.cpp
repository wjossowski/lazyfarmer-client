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


#include "playerfactory.h"

using namespace Core;

PlayerFactory::PlayerFactory(QObject *parent)
    : QObject(parent)
{

}

QSharedPointer<Player> PlayerFactory::create()
{
    QSharedPointer<Player> player(new Player());

    m_players.append(player);
    emit this->sizeChanged(this->size());

    return player;
}

void PlayerFactory::remove(int i)
{
    m_players.removeAt(i);
    emit this->sizeChanged(this->size());
}
