#ifndef COMBATANT_H
#define COMBATANT_H

#include <QDeclarativeItem>

class Player;
class Unit : public QDeclarativeItem
{
    Q_OBJECT
    //TODO: Rotation
    Q_PROPERTY(int hp READ hp WRITE setHP NOTIFY hpChanged)
    Q_PROPERTY(Player* player READ player WRITE setPlayer NOTIFY playerChanged)
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

signals:
    void born();
    void hpChanged(int arg);

    void playerChanged(Player* arg);

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

private:
    friend class Game;
    int m_hp;
    Player* m_player;
};

#endif // COMBATANT_H
