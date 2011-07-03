#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QUrl>
#include <QHash>
#include <QDeclarativeListProperty>

class QDeclarativeEngine;
class QDeclarativeComponent;
class QDeclarativeItem;
class QGraphicsItem;
class Map;
class Player;
class Unit;
class Doodad;
class Buildable;
class Game : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeListProperty<Player> players READ players NOTIFY playersChanged)//RO version, copy of map.players
public:
    explicit Game(QObject *parent = 0);
    static Game* instance();

    Q_INVOKABLE void registerDebugText(QObject* obj);
    Q_INVOKABLE void debugPrint(const QString &str);

    Unit* findTarget(int range, Unit*);
    void cleanUp(Unit*);
    void setEngine(QDeclarativeEngine* e)
    {
        m_engine = e;
    }

    QDeclarativeListProperty<Player> players(){
        return QDeclarativeListProperty<Player>(this, m_players);
    }

signals:
    void tick();
    void playersChanged(QList<Player*> arg);

public slots:
    bool build(int x, int y, Buildable*, int playerIdx=0);
    void loadMap(const QUrl &url, QDeclarativeItem* parent);
    void finishLoad();
private:
    void recursiveLoadStartState(QGraphicsItem* node);
    static Game* m_instance;

    QObject* m_debugText;
    QDeclarativeEngine* m_engine;
    QDeclarativeComponent* m_cc;

    QDeclarativeItem* m_gameArea;
    Map* m_currentMap;
    QHash<Player*, QList<Unit*> > m_units;
    QList<Doodad*> m_doodads;
    QList<Player*> m_players;
};

#endif // GAME_H
