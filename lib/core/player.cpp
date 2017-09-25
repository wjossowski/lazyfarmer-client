/**
 ** This file is part of the LazyFarmer project.
 ** Copyright 2017 Wojciech Ossowski <w.j.ossowski@gmail.com>.
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

#include "player.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

#include <QtDebug>

Player::Player(QObject *parent)
    : QObject(parent)
    , m_level(0)
    , m_levelPercentage(0)
    , m_money(0.0)
    , m_storage(new Storage)
    , m_gateway(new ApiGateway)
{
    initialize();
    initializeConnections();
}

void Player::update(const QVariantMap &basicInfo)
{
    m_level = basicInfo["Level"].toInt();
    m_levelDescription = basicInfo["LevelDescription"].toString();
    m_levelPercentage = basicInfo["LevelPercentage"].toInt();
    m_money = basicInfo["Money"].toDouble();
}

void Player::initialize()
{

}

void Player::initializeConnections() const
{

}
