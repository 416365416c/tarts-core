#include "game.h"
#include "map.h"
#include "buildable.h"
#include "combatant.h"
#include "doodad.h"
#include "player.h"
#include "waypoint.h"
#include "movingunit.h"
#include <QDeclarativeComponent>
#include <QDeclarativeItem>
#include <QTimer>
#include <QRect>
#include <QPoint>
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
    m_players.clear();
    //TODO: CLEAN UP PREVIOUS MAP Properly (esp units)!
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
    qDebug() << "Loading Map" << m_currentMap << m_players;
    playersChanged(m_players);

    m_gameOver = false;
    //HQs
    for(int i=0; i<m_players.size(); i++)
        build(m_players[i]->startPos().x(), m_players[i]->startPos().y(), m_players[i]->hq(), i);
    //Other starting units
    recursiveLoadStartState(m_currentMap);
    //Sentinel Waypoints
    m_players[0]->m_waypoints << new Waypoint(m_players[0], m_players[1]->m_units[0], m_currentMap);
    m_players[1]->m_waypoints << new Waypoint(m_players[1], m_players[0]->m_units[0], m_currentMap);
    debugPrint("Map Loaded: " + QString::number(m_currentMap->width()) + "," + QString::number(m_currentMap->height()));
    QTimer::singleShot(16, this, SLOT(gameTick()));
}

void Game::recursiveLoadStartState(QGraphicsItem *node)
{
    foreach(QGraphicsItem* child, node->childItems()){
        Unit* startingUnit = qobject_cast<Unit*>(child->toGraphicsObject());
        if(startingUnit){
            startingUnit->player()->m_units << startingUnit;
            startingUnit->born();
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
    //TODO: Implement better Targeting Architecture
    //TODO: Optimize! And pick nearest - not random in case of ties
    foreach(Player* player, m_players){
        if(attacker->player() == player)
            continue;
        foreach(Unit* other, player->m_units){
            if(withinRange(attacker, range, other))
                return other;
        }
    }
    return 0;
}

void Game::cleanUp(Unit* unit)
{
    unit->m_player->m_units.removeAll(unit);
    MovingUnit* munit = qobject_cast<MovingUnit*>(unit);
    if(munit && munit->m_wave)
        munit->m_wave->units.removeAll(munit);
    unit->deleteLater();
}

bool Game::build(int x, int y, Buildable* b, int playerIdx, Waypoint* dest)
{
    if(m_gameOver)
        return false;
    QDeclarativeComponent* cc = b->delegate();
    QObject* obj = cc->create(qmlContext(m_currentMap));
    Unit* unit = qobject_cast<Unit*>(obj);
    if(!unit){
        qDebug() << "Error in build - not a game object";
    }else{
        unit->setX(x);
        unit->setY(y);
        foreach(Player* player, m_players){
            foreach(Unit* other, player->m_units){
                if(collide(unit, other)){
                    delete unit;
                    return false;
                }
            }
        }

        unit->setParentItem(m_currentMap);
        unit->setPlayer(m_players[playerIdx]);
        m_players[playerIdx]->m_units << unit;
        if(!qobject_cast<MovingUnit*>(unit))//stationary
            calcAPSP();
        else{
            qobject_cast<MovingUnit*>(unit)->setDestination(dest);
        }
        unit->born();
    }
    return true;
}

bool unitAtWaypoint(MovingUnit* munit)//TODO: Much more complex logic once waves bunch up
{
    if(!munit->destination())
        return false;
    return munit->x() == munit->destination()->x() && munit->y() == munit->destination()->y();
}

Waypoint* Game::effectiveNext(Waypoint* way)
{
    if(!way)//HACK
        return m_players[0]->m_waypoints[0];
    if(way->next())
        return way->next();
    if(way->player())
        return way->player()->m_waypoints[0];
    return 0;
}

void Game::addUnitToWaypoint(MovingUnit* munit, Waypoint* way)
{
    way->units << munit;
    if(way->units.count() >= way->waveSize()){
        m_waves << new Wave(effectiveNext(way), effectiveNext(way));//XXX GODDAMN TERRIBLE HACK
        foreach(MovingUnit* u, way->units)
            m_waves.back()->addUnit(u);
        way->units.clear();
    }
}

void Game::teleportUnitToWaypoint(MovingUnit* munit, Waypoint* way)
{
    munit->setX(way->x());
    munit->setY(way->y());
    addUnitToWaypoint(munit, way);
}


Waypoint* Game::createWaypoint(int x, int y, int playerIdx)
{
    Waypoint* ret = new Waypoint(m_currentMap);
    ret->setPlayer(m_players[playerIdx]);
    m_players[playerIdx]->m_waypoints << ret;
    ret->setX(x);
    ret->setY(y);
    ret->setNext(m_players[playerIdx]->m_waypoints[0]);//XXX HACK
    return ret;
}

Player* Game::player(int idx)
{
    return m_players[idx];
}

void Game::gameTick()
{
    //Move units
    foreach(Player* player, m_players){
        foreach(Unit* unit, player->m_units){
            MovingUnit* munit = qobject_cast<MovingUnit*>(unit);
            if(!munit)//TODO: Perhaps a second list would be faster?
                continue;
            if(unitAtWaypoint(munit) && !munit->m_wave)
                addUnitToWaypoint(munit, munit->destination());
            else if(!munit->m_wave)
                teleportUnitToWaypoint(munit, munit->destination());//TODO: Avoid segfaults on destination
        }
    }
    foreach(Wave* wave, m_waves){
        wave->moveUnits();
        if(wave->units.isEmpty()){
            m_waves.removeAll(wave);
            delete wave;
        }
    }

    //Fire units? Or they do that themselves?


    //2P simple vic condition
    foreach(Player* player, m_players)//TODO: Isn't victory condition merely the destruction of the HQ?
        if(player->m_units.isEmpty())
            m_gameOver = true;

    if(!m_gameOver)
        QTimer::singleShot(16, this, SLOT(gameTick()));
}

struct GraphHelperFns{
    static int width;
    static int height;
    static int v(int x,int y){
        return x + y*width;
    }
    static int v(const QPoint &p){
        return v(p.x(), p.y());
    }

    static QList<QPoint> neighbours(int x,int y){
        QList<QPoint> ret;
        QRect r(0,0,width-1,height-1);
        for(int i=-1; i<=1; i++){
            for(int j=-1; j<=1; j++){
                if(i==0 && j== 0)
                    continue;
                if(r.contains(x+i, y+j, false))
                    ret << QPoint(x+i, y+j);
            }
        }
        return ret;
    }
};

void Game::calcAPSP()
{
    /*
      MAJOR TODO:
        Need path for AMove, which doesn't count buildings
        Need path for Move, which does
        Need them calc'd stored on a per Waypoint basis
        And don't build Vs for all damn grid squares!
        Also need a simple (pathToLocal) function for getting back to the trail (or giving up) or going around small obstacles
      */
    return;
    int v = m_currentMap->width()*m_currentMap->height();
    m_apsp.resize(v);
    for(int i=0; i<v; i++){
        m_apsp[i].resize(v);
        m_apsp[i].fill(INT_MAX);
    }

    GraphHelperFns::width = m_currentMap->width();
    GraphHelperFns::height = m_currentMap->height();

    //Init grid
    for(int x=0; x<m_currentMap->width(); x++){
        for(int y=0; y<m_currentMap->height(); y++){
            int curV = GraphHelperFns::v(x,y);
            m_apsp[curV][curV] = 0;
            foreach(const QPoint &p, GraphHelperFns::neighbours(x,y))
                m_apsp[GraphHelperFns::v(p)][curV] = 1;
        }
    }

    //Block out buildings
    foreach(Player* player, m_players){
        foreach(Unit* unit, player->m_units){
            if(qobject_cast<MovingUnit*>(unit))
                continue;
            for(int x=unit->x(); x<unit->x()+unit->width(); x++)
                for(int y=unit->y(); y<unit->y()+unit->height(); y++)
                    foreach(const QPoint &p, GraphHelperFns::neighbours(x,y))
                        m_apsp[GraphHelperFns::v(p)][GraphHelperFns::v(x,y)] = INT_MAX;
        }
    }

    //FW APSP
    for(int i=0; i<v; i++)
        for(int j=0; j<v; j++)
            for(int k=0; k<v; k++)
                if(m_apsp[j][i] + m_apsp[i][k] < m_apsp[j][k])
                    m_apsp[j][k] = m_apsp[j][i] + m_apsp[i][k];
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

Wave::Wave(Waypoint* to, Waypoint* from):goal(to),start(from)
{
    at = QPoint(from->x(), from->y());
    toPoint = QPoint(to->x(), to->y());
    speed = INT_MAX;
}

void Wave::addUnit(MovingUnit *u)
{
    if(u->speed() < speed)
        speed = u->speed();
    units << u;
    u->m_wave = this;
}

bool Game::tryMove(Unit* u, int x, int y)
{
    //TODO: Constantly partially updated blocking map will be WAY faster
    QRect newRect(u->x() + x, u->y() + y, u->width(), u->height());
    foreach(Player* player, m_players){
        foreach(Unit* u2, player->m_units){
            if(u==u2)
                continue;
            QRect otherRect(u2->x(), u2->y(), u2->width(), u2->height());
            if(newRect.intersects(otherRect))
                return false;
        }
    }
    if(x)
        u->setX(u->x() + x);
    if(y)
        u->setY(u->y() + y);
    return true;
}

void Wave::moveUnits()
{
    if(speed == INT_MAX || units.isEmpty())
        return;
    int targetX = goal->x();
    int targetY = goal->y();
    foreach(MovingUnit* u, units){
        for(int i=0; i<speed; i++){
            int xDiff = targetX - u->x();
            int yDiff = targetY - u->y();
            if(xDiff == 0 && yDiff == 0){
                u->m_wave = 0;//Release it, it's there (this should be done en masse...)
                i = speed;
                continue;
            }
            if(qAbs(xDiff) > qAbs(yDiff)){
                if(xDiff < 0)
                    xDiff = -1;
                else
                    xDiff = 1;
                if(!Game::instance()->tryMove(u, xDiff, 0))
                    if(!Game::instance()->tryMove(u,0,1))
                        if(!Game::instance()->tryMove(u,0,-1))
                            Game::instance()->tryMove(u,xDiff*-1,0);//TODO: Also wait for others to move around before moving back into your old spot
            }else{
                if(yDiff < 0)
                    yDiff = -1;
                else
                    yDiff = 1;
                if(!Game::instance()->tryMove(u, 0, yDiff))
                    if(!Game::instance()->tryMove(u,1,0))
                        if(!Game::instance()->tryMove(u,-1,0))
                            Game::instance()->tryMove(u,0,yDiff*-1);
            }
        }
    }
    for(int i=0; i<units.count(); i++){
        MovingUnit* u = units[i];
        if(u->m_wave != this)
            units.removeAt(i--);
        //TODO: Better clean up when out of units than in gameTick?
    }
}
