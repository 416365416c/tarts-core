#ifndef COMBATANT_H
#define COMBATANT_H

#include <QDeclarativeItem>

class Player;
class Unit : public QDeclarativeItem
{
    Q_OBJECT
    //TODO: Below (commented) Properties are planned
    //Q_PROPERTY(int facing READ facing WRITE setFacing NOTIFY facingChanged)
    //Q_PROPERTY(QDeclarativeListProperty<Ability> abilities READ abilities)
    //Ability would be a class encapsulating automatically targeted abilities, because they're damn complex blighters it seems :(
    Q_PROPERTY(int hp READ hp WRITE setHP NOTIFY hpChanged)
    Q_PROPERTY(bool sink READ isSink WRITE setSink NOTIFY sinkChanged)
    Q_PROPERTY(Player* player READ player WRITE setPlayer NOTIFY playerChanged)
    //NodeRadius is how much area it 'controls', 0 for not a node
    Q_PROPERTY(int nodeRadius READ nodeRadius WRITE setNodeRadius NOTIFY nodeRadiusChanged)

public:
    explicit Unit(QDeclarativeItem *parent = 0);
    Q_INVOKABLE Unit* getTarget(int range);

    int hp() const
    {
        return m_hp;
    }

    Player* player() const
    {
        return m_player;
    }

    bool isSink() const
    {
        return m_sink;
    }

    int nodeRadius() const
    {
        return m_nodeRadius;
    }

signals:
    void born();
    void hpChanged(int arg);

    void playerChanged(Player* arg);

    void sinkChanged(bool arg);

    void sunk(Unit* otherUnit);

    void nodeRadiusChanged(int arg);

public slots:
    void kill();//Destroy is QML one...

    void setHP(int arg)
    {
        if (m_hp != arg) {
            m_hp = arg;
            emit hpChanged(arg);
        }
    }

    void setPlayer(Player* arg)
    {
        if (m_player != arg) {
            m_player = arg;
            emit playerChanged(arg);
        }
    }

    void setSink(bool arg)
    {
        if (m_sink != arg) {
            m_sink = arg;
            emit sinkChanged(arg);
        }
    }

    void setNodeRadius(int arg)
    {
        if (m_nodeRadius != arg) {
            m_nodeRadius = arg;
            emit nodeRadiusChanged(arg);
        }
    }

private:
    friend class Game;
    int m_hp;
    Player* m_player;
    bool m_sink;
    int m_nodeRadius;
};

#endif // COMBATANT_H
