#include "movingunit.h"
#include "waypoint.h"

MovingUnit::MovingUnit(QDeclarativeItem *parent) :
    Unit(parent), m_wave(0), m_destination(0)
{
}
