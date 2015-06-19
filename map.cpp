#include "map.h"
#include "game.h"

Map::Map(QQuickItem *parent) :
    QQuickItem(parent)
{
}


QQmlListProperty<Player> Map::players()
{
    return QQmlListProperty<Player>(this, m_players);//TODO: Memory management
}
