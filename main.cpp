#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include <qdeclarative.h>
#include <QDeclarativeContext>
#include "game.h"
#include "player.h"
#include "map.h"
#include "doodad.h"
#include "buildable.h"
#include "combatant.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    const char* uri = "TouchRTS";
    qmlRegisterUncreatableType<Game>(uri, 0, 1, "Game", "Game is a C++ managed singleton.");
    qmlRegisterType<Map>(uri, 0, 1, "Map");
    qmlRegisterType<Player>(uri, 0, 1, "Player");
    qmlRegisterType<Doodad>(uri, 0, 1, "Doodad");
    qmlRegisterType<Buildable>(uri, 0, 1, "Buildable");
    qmlRegisterType<Unit>(uri, 0, 1, "Combatant");
    QmlApplicationViewer viewer;
    Game::instance()->setEngine(viewer.engine());
    viewer.rootContext()->setContextProperty("game", Game::instance());
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationLockLandscape);
    viewer.setMainQmlFile(QLatin1String("qml/TouchRTS/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
