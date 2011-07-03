#include "game.h"
#include "map.h"
#include "buildable.h"
#include "combatant.h"
#include "doodad.h"
#include "player.h"
#include <QDeclarativeComponent>
#include <QDeclarativeItem>
Game* Game::m_instance = 0;
Game* Game::instance(){
    if(!m_instance)
        m_instance = new Game();
    return m_instance;
}


bool dimensionalCheck(int aP, int aL, int bP, int bL)
{
    int a1 = aP;
    int a2 = aP + aL;
    int b1 = bP;
    int b2 = bP + bL;
    if(b1 >= a1 && b1 <= a2)
        return true;
    if(b2 >= a1 && b2 <= a2)
        return true;
    if(b1 <= a1 && b2 >= a2)
        return true;
    return false;
}

bool collide(Unit* a, Unit* b)
{
    return dimensionalCheck(a->x(), a->width(), b->x(), b->width())
            && dimensionalCheck(a->y(), a->height(), b->y(), b->height());
}

//TODO: range from edges, not center?
bool withinRange(Unit* a, int r, Unit* b)
{
    QPoint aP(a->x() + a->width()/2, a->y() + a->height()/2);
    QPoint bP(b->x() + b->width()/2, b->y() + b->height()/2);
    return (bP - aP).manhattanLength() <= r;
}

void testRangeFunctions()
{
    Unit a,b;
    a.setWidth(2);
    a.setHeight(2);
    b.setWidth(2);
    b.setHeight(2);
    qDebug() << "Overlapping (t)" << withinRange(&a,0,&b);
    qDebug() << "Colliding (t)" << collide(&a,&b);
    a.setX(1);
    qDebug() << "To the left, well within(t)" << withinRange(&a,10,&b);
    qDebug() << "Colliding (t)" << collide(&a,&b);
    a.setX(10);
    qDebug() << "To the left, just within(t)" << withinRange(&a,10,&b);
    qDebug() << "Colliding (f)" << collide(&a,&b);
    a.setX(11);
    qDebug() << "To the left, just without(f)" << withinRange(&a,10,&b);
    qDebug() << "Colliding (f)" << collide(&a,&b);
    a.setX(310);
    qDebug() << "To the left, way off(f)" << withinRange(&a,10,&b);
}

Game::Game(QObject *parent) :
    QObject(parent), m_debugText(0), m_engine(0), m_currentMap(0), m_cc(0)
{
    testRangeFunctions();
}

void Game::loadMap(const QUrl &url, QDeclarativeItem* parent)
{
    if(!m_engine)
        return;
    qDebug() << "loading " << url;
    if(!m_cc)
        m_cc = new QDeclarativeComponent(m_engine,  this);
    m_cc->loadUrl(url);
    m_gameArea = parent;
    if(m_cc->isLoading())
        connect(m_cc, SIGNAL(statusChanged(QDeclarativeComponent::Status)),
                this, SLOT(finishLoad()));
    else
        finishLoad();
}

void Game::finishLoad(){
    if(m_cc->isError()){
        qDebug() << "Error loading: " << m_cc->errors();
        return;
    }
    m_currentMap = qobject_cast<Map*>(m_cc->create(qmlContext(m_gameArea)));
    if(!m_currentMap){
        qDebug() << "Error loading map.";
        return;
    }
    m_currentMap->setParentItem(m_gameArea);
    m_players = m_currentMap->m_players;//Yes, we share references.
    playersChanged(m_players);
    recursiveLoadStartState(m_currentMap);

    for(int i=0; i<m_players.size(); i++)
        build(m_players[i]->startPos().x(), m_players[i]->startPos().y(), m_players[i]->hq(), i);

    debugPrint("Map Loaded: " + QString::number(m_currentMap->width()) + "," + QString::number(m_currentMap->height()));
}

void Game::recursiveLoadStartState(QGraphicsItem *node)
{
    foreach(QGraphicsItem* child, node->childItems()){
        Unit* startingUnit = qobject_cast<Unit*>(child->toGraphicsObject());
        if(startingUnit){
            m_units[startingUnit->player()] << startingUnit;
        }
        Doodad* doodad = qobject_cast<Doodad*>(child->toGraphicsObject());
        if(doodad){
            m_doodads << doodad;
        }
        recursiveLoadStartState(child);
    }
}


Unit* Game::findTarget(int range, Unit* attacker)
{
    //TODO: Optimize! And pick nearest - not random in case of ties
    foreach(Player* player, m_players){
        if(attacker->player() == player)
            continue;
        foreach(Unit* other, m_units[player]){
            if(withinRange(attacker, range, other))
                return other;
        }
    }
    return 0;
}

void Game::cleanUp(Unit* unit)
{
    foreach(Player* player, m_players)
        m_units[player].removeAll(unit);
    unit->deleteLater();
}

bool Game::build(int x, int y, Buildable* b, int playerIdx)
{
    QDeclarativeComponent* cc = b->delegate();
    QObject* obj = cc->create(qmlContext(m_currentMap));
    Unit* unit = qobject_cast<Unit*>(obj);
    if(!unit){
        qDebug() << "Error in build - not a game object";
    }else{
        unit->setX(x);
        unit->setY(y);
        foreach(Player* player, m_players){
            foreach(Unit* other, m_units[player]){
                if(collide(unit, other)){
                    delete unit;
                    return false;
                }
            }
        }

        unit->setParentItem(m_currentMap);
        unit->setPlayer(m_players[playerIdx]);
        m_units[m_players[playerIdx]] << unit;
        unit->born();
    }
    return true;
}


void Game::registerDebugText(QObject* obj)
{
    m_debugText = obj;
}

void Game::debugPrint(const QString &str)
{
    //TODO: Make it a scrollable area and just append or something
    if(m_debugText)
        m_debugText->setProperty("text", str);
}
