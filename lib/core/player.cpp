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

constexpr const char* KEY_GATEWAY = "GatewayOptions";

Player::Player(QObject *parent)
    : QObject(parent)
    , m_level(0)
    , m_levelPercentage(0)
    , m_money(0.0)
{
    initialize();
    initializeConnections();
}

Player::~Player()
{
    qDebug() << "Removing player" << playerDescription();
}

GlobalGameData::Ptr Player::gameData() const
{
    return m_gateway->gameData();
}

QJsonObject Player::toJson() const
{
    return {
        { KEY_GATEWAY, m_gateway->toJson()}
    };
}

void Player::fromJson(const QJsonObject &json)
{
    m_gateway->fromJson(json[KEY_GATEWAY].toObject());
}

QString Player::playerDescription() const
{
    if (!m_gateway->isConfigured()) {
        return tr("Unconfigured Account");
    } else {
        return QString("%1@s%2.%3")
                .arg(m_gateway->login())
                .arg(m_gateway->serverId())
                .arg(m_gateway->serverDomain());
    }
}

QString Player::currentJob() const
{
    return m_gateway->currentJobName();
}

void Player::setApiOptions(const ApiOptions &options)
{
    m_gateway->setApiOptions(options);
    if (m_gateway->isConfigured()) {
        m_gateway->queueMessage(GetFarmInfo::Ptr(new GetFarmInfo(&*m_gateway)));
        m_gateway->start();
    }
}

void Player::setApiOptions(const QString &domain, const QString &serverId, const QString &login, const QString &password)
{
    setApiOptions({ domain, serverId, login, password });
}

void Player::update(const QByteArray &info)
{
    PlayerInfoExtractor extractor;

    if (extractor.parseInfo(info)) {
        updateBasicInfo(extractor.basicInfo());

        m_storage->update(extractor.storageInfo());
        m_buildingList->update(extractor.farmsInfo());

        qInfo() << "Player Info Updated:";
        qInfo() << "Storage:" << m_storage->toString();
        qInfo() << "Buildings:" << m_buildingList->toString();
    }
}

void Player::updateBasicInfo(const QVariantMap &basicInfo)
{
    int level = basicInfo["Level"].toInt();
    if (m_level != level) {
        m_level = level;
        emit levelChanged(m_level);
    }

    QString levelDescription = basicInfo["LevelDescription"].toString();
    if (m_levelDescription != levelDescription) {
        m_levelDescription = levelDescription;
        emit levelDescriptionChanged(m_levelDescription);
    }

    int levelPercentage = basicInfo["LevelPercentage"].toInt();
    if (m_levelPercentage != levelPercentage) {
        m_levelPercentage = levelPercentage;
        emit levelPercentageChanged(m_levelPercentage);
    }

    double money = basicInfo["Money"].toDouble();
    if (qFuzzyCompare(m_money, money)) {
        m_money = money;
        emit moneyChanged(m_money);
    }
}

void Player::initialize()
{
    m_storage.reset(new Data::Storage(this));
    m_buildingList.reset(new Data::BuildingList(this));
    m_gateway.reset(new ApiGateway(this));
}

void Player::initializeConnections() const
{
    connect(this,         &Player::updateBuildingRequested,
            &*m_gateway,  &ApiGateway::requestBuildingUpdate);

    connect(&*m_gateway,  &ApiGateway::playerDataUpdated,
            this,         &Player::update);

    connect(&*m_gateway,  &ApiGateway::accountConfigurationChanged,
            this,         &Player::playerDescriptionChanged);

    connect(&*m_gateway,  &ApiGateway::currentJobChanged,
            this,         &Player::currentJobChanged);

    connect(&*m_gateway,          &ApiGateway::buildingDataUpdated,
            &*m_buildingList,     &BuildingList::updateBuilding);

    connect(&*m_gateway, &ApiGateway::errorRaised,
            this,        &Player::handleGatewayError);

    connect(&*m_gateway,  &ApiGateway::clearError,
            this,         &Player::handleGatewayError);

    // Signal forwarding (needed for PlayerFactory)
    connect(this, &Player::levelChanged,                this, &Player::dataChanged);
    connect(this, &Player::levelDescriptionChanged,     this, &Player::dataChanged);
    connect(this, &Player::levelPercentageChanged,      this, &Player::dataChanged);
    connect(this, &Player::moneyChanged,                this, &Player::dataChanged);
    connect(this, &Player::playerDescriptionChanged,    this, &Player::dataChanged);
    connect(this, &Player::currentJobChanged,           this, &Player::dataChanged);
    connect(this, &Player::lastErrorChanged,            this, &Player::dataChanged);
}

void Player::handleGatewayError(const QString &errorMessage)
{
    this->m_lastError = errorMessage;
    emit lastErrorChanged();
}
