#include "player.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

#include <QtDebug>

Player::Player(QObject *parent)
    : QObject(parent)
    , m_level(0)
    , m_levelPercentage(0)
{

}

void Player::setBasicInfo(const QVariantMap &basicInfo)
{
    m_level = basicInfo["Level"].toInt();
    m_levelDescription = basicInfo["LevelDescription"].toString();
    m_levelPercentage = basicInfo["LevelPercentage"].toInt();
}
