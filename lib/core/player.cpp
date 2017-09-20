#include "player.h"

#include <QtDebug>

Player::Player(QObject *parent)
    : QObject(parent)
{
    qDebug() << "Budujem";
}

void Player::setLevel(const quint32 &level)
{
    if (m_level != level) {
        m_level = level;
        emit levelChanged(level);
    }
}

void Player::setLevelDescription(const QString &levelDescription)
{
    if (m_levelDescription != levelDescription) {
        m_levelDescription = levelDescription;
        emit levelDescriptionChanged(levelDescription);
    }
}
