#ifndef BUILDABLE_H
#define BUILDABLE_H

#include <QObject>
#include <QUrl>
class QDeclarativeComponent;

class Buildable : public QObject
{
    Q_OBJECT
    //TODO: Build time and tech requirements
    Q_PROPERTY(int cost READ cost WRITE setCost NOTIFY costChanged)
    Q_PROPERTY(QUrl iconSource READ iconSource WRITE setIconSource NOTIFY iconSourceChanged)
    //Delegate root object MUST be combatant? Or who cares?
    Q_CLASSINFO("DefaultProperty", "delegate")
    Q_PROPERTY(QDeclarativeComponent* delegate READ delegate WRITE setDelegate NOTIFY delegateChanged)
public:
    explicit Buildable(QObject *parent = 0);

    int cost() const
    {
        return m_cost;
    }

    QUrl iconSource() const
    {
        return m_iconSource;
    }

    QDeclarativeComponent* delegate() const
    {
        return m_delegate;
    }

signals:

    void costChanged(int arg);

    void iconSourceChanged(QUrl arg);

    void delegateChanged(QDeclarativeComponent* arg);

public slots:

void setCost(int arg)
{
    if (m_cost != arg) {
        m_cost = arg;
        emit costChanged(arg);
    }
}

void setIconSource(QUrl arg)
{
    if (m_iconSource != arg) {
        m_iconSource = arg;
        emit iconSourceChanged(arg);
    }
}

void setDelegate(QDeclarativeComponent* arg)
{
    if (m_delegate != arg) {
        m_delegate = arg;
        emit delegateChanged(arg);
    }
}

private:

int m_cost;
QUrl m_iconSource;
QDeclarativeComponent* m_delegate;
};

#endif // BUILDABLE_H
