#include "playerfactory.h"

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
