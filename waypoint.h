#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QDeclarativeItem>

class Player;
class MovingUnit;
class Waypoint : public QDeclarativeItem//Object, not Item, because it's data to be
{
    Q_OBJECT
    Q_PROPERTY(Waypoint* next READ next WRITE setNext NOTIFY nextChanged)
    Q_PROPERTY(bool attackMove READ attackMove WRITE setAttackMove NOTIFY attackMoveChanged)
    Q_PROPERTY(bool playerModifiable READ playerModifiable WRITE setPlayerModifiable NOTIFY playerModifiableChanged)
    Q_PROPERTY(int waveSize READ waveSize WRITE setWaveSize NOTIFY waveSizeChanged)
    Q_PROPERTY(Player* player READ player WRITE setPlayer NOTIFY playerChanged)

public:
    explicit Waypoint(QDeclarativeItem *parent = 0);
    Waypoint(Player* player, QDeclarativeItem *target, QDeclarativeItem *parent);

    bool attackMove() const
    {
        return m_attackMove;
    }

    Waypoint* next() const
    {
        return m_next;
    }

    bool playerModifiable() const
    {
        return m_playerModifiable;
    }

    int waveSize() const
    {
        return m_waveSize;
    }

    Player* player() const
    {
        return m_player;
    }

    QList<MovingUnit*> units;//Stored here - used by Game

signals:

    void attackMoveChanged(bool arg);

    void nextChanged(Waypoint* arg);

    void playerModifiableChanged(bool arg);

    void waveSizeChanged(int arg);

    void playerChanged(Player* arg);

public slots:

    void setAttackMove(bool arg)
    {
        if (m_attackMove != arg) {
            m_attackMove = arg;
            emit attackMoveChanged(arg);
        }
    }
    void setNext(Waypoint* arg)
    {
        if (m_next != arg) {
            m_next = arg;
            emit nextChanged(arg);
        }
    }

    void setPlayerModifiable(bool arg)
    {
        if (m_playerModifiable != arg) {
            m_playerModifiable = arg;
            emit playerModifiableChanged(arg);
        }
    }

    void setWaveSize(int arg)
    {
        if (m_waveSize != arg) {
            m_waveSize = arg;
            emit waveSizeChanged(arg);
        }
    }

    void setPlayer(Player* arg)
    {
        if (m_player != arg) {
            m_player = arg;
            emit playerChanged(arg);
        }
    }

private:
    bool m_attackMove;
    Waypoint* m_next;
    bool m_playerModifiable;
    int m_waveSize;
    Player* m_player;
};

#endif // WAYPOINT_H
