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
#include "api/messages/messages.h"
#include "extractors/playerinfoextractor.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

#include <QtDebug>

using namespace Core;
using namespace Core::Data;
using namespace Core::Extractors;
using namespace Core::Api;
using namespace Core::Api::Messages;

Player::Player(QObject *parent)
    : QObject(parent)
    , m_level(0)
    , m_levelPercentage(0)
    , m_money(0.0)
{
    initialize();
    initializeConnections();
}

GlobalGameData::Ptr Player::gameData() const
{
    return m_gateway.gameData();
}

void Player::update(const QByteArray &info)
{
    PlayerInfoExtractor extractor;

    if (extractor.parseInfo(info)) {
        updateBasicInfo(extractor.basicInfo());

        m_storage->update(extractor.storageInfo());
        m_buildingList->update(extractor.farmsInfo());

        qInfo() << "Player Info Updated:";
        qInfo() << m_storage->toString();
        qInfo() << m_buildingList->toString();
    }
}

void Player::updateBuilding(int farm, int position, Data::BuildingType type)
{
    const Data::BuildingDetails details { farm, position };

    switch (type) {
    case BuildingType::Farm:
        m_gateway.queueMessage(GetFieldInfo::Ptr(new GetFieldInfo(&m_gateway, details)));
        break;
    case BuildingType::AnimalProduction:
        m_gateway.queueMessage(GetFeedInfo::Ptr(new GetFeedInfo(&m_gateway, details)));
        break;
    case BuildingType::ResourceProduction:
        m_gateway.queueMessage(GetProductionInfo::Ptr(new GetProductionInfo(&m_gateway, details)));
        break;
    default:
        break;
    }
}

void Player::updateBasicInfo(const QVariantMap &basicInfo)
{
    m_level = basicInfo["Level"].toInt();
    m_levelDescription = basicInfo["LevelDescription"].toString();
    m_levelPercentage = basicInfo["LevelPercentage"].toInt();

    m_money = basicInfo["Money"].toDouble();
}

void Player::initialize()
{
    m_storage.reset(new Data::Storage(this));
    m_buildingList.reset(new Data::BuildingList(this));
}

void Player::initializeConnections() const
{
    connect(&m_gateway, &ApiGateway::updatePlayerData,
            this,       &Player::update);
}
