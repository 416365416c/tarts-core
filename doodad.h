#ifndef DOODAD_H
#define DOODAD_H

#include <QQuickItem>

class Doodad : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool blocksPathing READ blocksPathing WRITE setBlocksPathing NOTIFY blocksPathingChanged);//TODO: Implement (when we have pathing)
    Q_PROPERTY(bool blocksLOS READ blocksLOS WRITE setBlocksLOS NOTIFY blocksLOSChanged);//TODO: Implement (when we have LOS)
    //blocksBuilding bool? So you can have 'unmazable' levels that are more TD like?
public:
    explicit Doodad(QQuickItem *parent = 0);

    bool blocksPathing() const
    {
        return m_blocksPathing;
    }

    bool blocksLOS() const
    {
        return m_blocksLOS;
    }

signals:

    void blocksPathingChanged(bool arg);

    void blocksLOSChanged(bool arg);

public slots:

    void setBlocksPathing(bool arg)
    {
        if (m_blocksPathing != arg) {
            m_blocksPathing = arg;
            emit blocksPathingChanged(arg);
        }
    }
    void setBlocksLOS(bool arg)
    {
        if (m_blocksLOS != arg) {
            m_blocksLOS = arg;
            emit blocksLOSChanged(arg);
        }
    }
private:
    bool m_blocksPathing;
    bool m_blocksLOS;
};

#endif // DOODAD_H
