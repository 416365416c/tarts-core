#ifndef MAP_H
#define MAP_H

#include <QDeclarativeItem>
#include <QDeclarativeListProperty>

class Player;
class Map : public QDeclarativeItem
{
    Q_OBJECT
    //For convenicence and theoretical completeness, 1 game square is 1 pixel - so maps width/height is the QDItem width/height. Same with object locations upon it.
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    //Q_PROPERTY(QDeclarativeListProperty<QObject> startingObjects READ startingObjects)//###Just children?
    Q_PROPERTY(QDeclarativeListProperty<Player> players READ players)
    //TODO: Nodes

public:
    explicit Map(QDeclarativeItem *parent = 0);

    QString title() const
    {
        return m_title;
    }

    QDeclarativeListProperty<Player> players();

signals:

    void titleChanged(QString arg);

public slots:

    void setTitle(QString arg)
    {
        if (m_title != arg) {
            m_title = arg;
            emit titleChanged(arg);
        }
    }
private:
    friend class Game;
    QString m_title;
    QList<Player*> m_players;
};

#endif // MAP_H
