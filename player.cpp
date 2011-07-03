#include "player.h"

Player::Player(QObject *parent) :
    QObject(parent)
{
}


QDeclarativeListProperty<Buildable> Player::buildOptions()
{
    return QDeclarativeListProperty<Buildable>(this, m_buildOptions);//TODO: Memory Management
}
