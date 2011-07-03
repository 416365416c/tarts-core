#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QPoint>
#include <QColor>
#include <QDeclarativeListProperty>

class Buildable;
class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeListProperty<Buildable> buildOptions READ buildOptions)
    Q_PROPERTY(Buildable* hq READ hq WRITE setHQ NOTIFY hqChanged)
    Q_PROPERTY(QPoint startPos READ startPos WRITE setStartPos NOTIFY startPosChanged)
    Q_PROPERTY(QColor color READ setColor WRITE setColor NOTIFY colorChanged)

public:
explicit Player(QObject *parent = 0);

    QDeclarativeListProperty<Buildable> buildOptions();
    Buildable* hq() const
    {
        return m_hq;
    }

    QPoint startPos() const
    {
        return m_startPos;
    }

    QColor setColor() const
    {
        return m_color;
    }

signals:

void hqChanged(Buildable* arg);

void startPosChanged(QPoint arg);

void colorChanged(QColor arg);

public slots:

void setHQ(Buildable* arg)
{
    if (m_hq != arg) {
        m_hq = arg;
        emit hqChanged(arg);
    }
}
void setStartPos(QPoint arg)
{
    if (m_startPos != arg) {
        m_startPos = arg;
        emit startPosChanged(arg);
    }
}
void setColor(QColor arg)
{
    if (m_color != arg) {
        m_color = arg;
        emit colorChanged(arg);
    }
}
private:
    Buildable* m_hq;
    QPoint m_startPos;
    QColor m_color;
    QList<Buildable*> m_buildOptions;
};

#endif // PLAYER_H
