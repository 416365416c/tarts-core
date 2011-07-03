#include "map.h"
#include "game.h"

Map::Map(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
}


QDeclarativeListProperty<Player> Map::players()
{
    return QDeclarativeListProperty<Player>(this, m_players);//TODO: Memory management
}
