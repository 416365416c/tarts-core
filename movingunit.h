#ifndef MOVINGUNIT_H
#define MOVINGUNIT_H
#include "combatant.h"

class Waypoint;
class Wave;
class MovingUnit : public Unit
{
    Q_OBJECT
    Q_PROPERTY(Waypoint* destination READ destination WRITE setDestination NOTIFY destinationChanged)
    Q_PROPERTY(qreal speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(int stoppingRange READ stoppingRange WRITE setStoppingRange NOTIFY stoppingRangeChanged)
public:
    explicit MovingUnit(QDeclarativeItem *parent = 0);

    Waypoint* destination() const
    {
        return m_destination;
    }

    Wave* m_wave;
    qreal speed() const
    {
        return m_speed;
    }

    int stoppingRange() const
    {
        return m_stoppingRange;
    }

signals:

    void destinationChanged(Waypoint* arg);

    void speedChanged(qreal arg);

    void stoppingRangeChanged(int arg);

public slots:

    void setDestination(Waypoint* arg)
    {
        if (m_destination != arg) {
            m_destination = arg;
            emit destinationChanged(arg);
        }
    }
    void setSpeed(qreal arg)
    {
        if (m_speed != arg) {
            m_speed = arg;
            emit speedChanged(arg);
        }
    }

    void setStoppingRange(int arg)
    {
        if (m_stoppingRange != arg) {
            m_stoppingRange = arg;
            emit stoppingRangeChanged(arg);
        }
    }

private:
    Waypoint* m_destination;
    qreal m_speed;
    int m_stoppingRange;
};

#endif // MOVINGUNIT_H
