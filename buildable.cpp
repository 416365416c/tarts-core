
#include "buildable.h"

Buildable::Buildable(QObject *parent) :
    QObject(parent), m_needsControl(true)
{
}
