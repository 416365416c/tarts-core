#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QUrl>
#include <QHash>
#include <QPoint>
#include <QVector>
#include <QDeclarativeListProperty>

class QDeclarativeEngine;
class QDeclarativeComponent;
class QDeclarativeItem;
class QGraphicsItem;
class Map;
class Player;
class Unit;
class MovingUnit;
class Doodad;
class Buildable;
class Waypoint;
class Wave
{
    public:
    Wave(Waypoint* from, Waypoint* to);
    //Note: Doesn't own ANY of these objects.
    Waypoint* goal;
    Waypoint* start;
    QList<MovingUnit*> units;
    QPoint at;
    QPoint toPoint;
    int speed;//Of slowest unit
    void addUnit(MovingUnit* u);
    void moveUnits();
};

class Game : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeListProperty<Player> players READ players NOTIFY playersChanged)//RO version, copy of map.players
    Q_PROPERTY(bool gameOver READ gameOver NOTIFY gameOverChanged)
public:
    explicit Game(QObject *parent = 0);
    static Game* instance();

    Q_INVOKABLE void registerDebugText(QObject* obj);
    Q_INVOKABLE void debugPrint(const QString &str);
    Q_INVOKABLE Player* player(int idx);//TODO: Pick one and use it everywhere!
    Q_INVOKABLE int playerIdx(Player* player);
    Q_INVOKABLE Waypoint* createWaypoint(int x, int y, int playerIdx);

    Unit* findTarget(int range, Unit*);
    void cleanUp(Unit*);
    void setEngine(QDeclarativeEngine* e)
    {
        m_engine = e;
    }

    QDeclarativeListProperty<Player> players(){
        return QDeclarativeListProperty<Player>(this, m_players);
    }

    bool gameOver() const
    {
        return m_gameOver;
    }

    bool tryMove(Unit* u, int x, int y);
    Waypoint* effectiveNext(Waypoint* way);

signals:
    void tick();//For QML convenience. Will this ever be used?
    void playersChanged(QList<Player*> arg);

    void gameOverChanged(bool arg);

public slots:
    bool build(int x, int y, Buildable*, int playerIdx=0, Waypoint* dest=0);
    void loadMap(const QUrl &url, QDeclarativeItem* parent);
    void finishLoad();
    void gameTick();
private:
    void recursiveLoadStartState(QGraphicsItem* node);
    void addUnitToWaypoint(MovingUnit* munit, Waypoint* way);
    void teleportUnitToWaypoint(MovingUnit* munit, Waypoint* way);
    static Game* m_instance;

    QObject* m_debugText;
    QDeclarativeEngine* m_engine;
    QDeclarativeComponent* m_cc;

    QDeclarativeItem* m_gameArea;
    Map* m_currentMap;
    QList<Doodad*> m_doodads;
    QList<Player*> m_players;
    QList<Wave*> m_waves;
    void calcAPSP();
    QVector<QVector<int> > m_apsp;//All pairs shortest path.//TODO: Replace with waypoint based pathing
    bool m_gameOver;
};

#endif // GAME_H
