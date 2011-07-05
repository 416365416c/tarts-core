#include "player.h"

Player::Player(QObject *parent) :
    QObject(parent)
{
}


QDeclarativeListProperty<Buildable> Player::buildOptions()
{
    return QDeclarativeListProperty<Buildable>(this, m_buildOptions);//TODO: Memory Management
}

QDeclarativeListProperty<Unit> Player::units()
{
    return QDeclarativeListProperty<Unit>(this, m_units);
}

QDeclarativeListProperty<Waypoint> Player::waypoints()
{
    return QDeclarativeListProperty<Waypoint>(this, m_waypoints);//TODO: Memory Management?
}

