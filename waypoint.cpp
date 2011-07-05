#include "waypoint.h"

Waypoint::Waypoint(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
  , m_next(0)
  , m_attackMove(true)
  , m_waveSize(0)
  , m_playerModifiable(false)
  , m_player(0)
{
}

Waypoint::Waypoint(Player* player, QDeclarativeItem *target, QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
  , m_next(0)
  , m_attackMove(true)
  , m_waveSize(0)
  , m_playerModifiable(false)
  , m_player(player)
{
    setX(target->x() + target->width()/2);
    setY(target->y() + target->height()/2);
}

