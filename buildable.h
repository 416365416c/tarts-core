#ifndef BUILDABLE_H
#define BUILDABLE_H

#include <QObject>
#include <QUrl>
class QQmlComponent;

class Buildable : public QObject
{
    Q_OBJECT
    //TODO: Build time and tech requirements
    Q_PROPERTY(int cost READ cost WRITE setCost NOTIFY costChanged)
    Q_PROPERTY(QUrl iconSource READ iconSource WRITE setIconSource NOTIFY iconSourceChanged)
    //If true (default) must be built near an owned node
    Q_PROPERTY(bool needsControl READ needsControl WRITE setNeedsControl NOTIFY needsControlChanged)
    //Delegate root object MUST be combatant? Or who cares?
    Q_CLASSINFO("DefaultProperty", "delegate")
    Q_PROPERTY(QQmlComponent* delegate READ delegate WRITE setDelegate NOTIFY delegateChanged)
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

    QQmlComponent* delegate() const
    {
        return m_delegate;
    }

    bool needsControl() const
    {
        return m_needsControl;
    }

signals:

    void costChanged(int arg);

    void iconSourceChanged(QUrl arg);

    void delegateChanged(QQmlComponent* arg);

    void needsControlChanged(bool arg);

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

void setDelegate(QQmlComponent* arg)
{
    if (m_delegate != arg) {
        m_delegate = arg;
        emit delegateChanged(arg);
    }
}

void setNeedsControl(bool arg)
{
    if (m_needsControl != arg) {
        m_needsControl = arg;
        emit needsControlChanged(arg);
    }
}

private:

int m_cost;
QUrl m_iconSource;
QQmlComponent* m_delegate;
bool m_needsControl;
};

#endif // BUILDABLE_H
