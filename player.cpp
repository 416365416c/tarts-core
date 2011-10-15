#include "player.h"
#include "combatant.h"

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

//###Convenience and internal logic functions, move to Game?
void Player::ownUnit(Unit* unit)
{
    unit->setPlayer(this);
    m_units << unit;
    if(unit->nodeRadius())
        m_nodes << unit;
}

void Player::disownUnit(Unit* unit)
{
    unit->setPlayer(0);
    m_units.removeAll(unit);
    m_nodes.removeAll(unit);
}

