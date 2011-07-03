#include "combatant.h"
#include "game.h"

Unit::Unit(QDeclarativeItem *parent) :
    QDeclarativeItem(parent), m_player(0)
{
}

Unit* Unit::getTarget(int range)//TODO: need to distinguish flying units?
{
    return Game::instance()->findTarget(range, this);
}

void Unit::kill()
{
    Game::instance()->cleanUp(this);
}

