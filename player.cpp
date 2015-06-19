#include "player.h"
#include "combatant.h"

Player::Player(QObject *parent) :
    QObject(parent)
{
}


QQmlListProperty<Buildable> Player::buildOptions()
{
    return QQmlListProperty<Buildable>(this, m_buildOptions);//TODO: Memory Management
}

QQmlListProperty<Unit> Player::units()
{
    return QQmlListProperty<Unit>(this, m_units);
}

QQmlListProperty<Waypoint> Player::waypoints()
{
    return QQmlListProperty<Waypoint>(this, m_waypoints);//TODO: Memory Management?
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
    if(m_units.contains(unit)){
        m_units.removeAll(unit);
        m_nodes.removeAll(unit);
    }
}

