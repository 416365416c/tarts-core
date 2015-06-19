#include <QtGui/QGuiApplication>
#include <qqml.h>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "game.h"
#include "player.h"
#include "map.h"
#include "doodad.h"
#include "buildable.h"
#include "combatant.h"
#include "movingunit.h"
#include "waypoint.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    const char* uri = "TouchRTS";
    qmlRegisterUncreatableType<Game>(uri, 0, 1, "Game", "Game is a C++ managed singleton.");
    qmlRegisterType<Map>(uri, 0, 1, "Map");
    qmlRegisterType<Player>(uri, 0, 1, "Player");
    qmlRegisterType<Doodad>(uri, 0, 1, "Doodad");
    qmlRegisterType<Buildable>(uri, 0, 1, "BuildOption");
    qmlRegisterType<Unit>(uri, 0, 1, "Unit");
    qmlRegisterType<MovingUnit>(uri, 0, 1, "MovingUnit");
    qmlRegisterType<Waypoint>(uri, 0, 1, "Waypoint");
    QQmlApplicationEngine engine;
    Game::instance()->setEngine(&engine);
    engine.rootContext()->setContextProperty("game", Game::instance());
    engine.load(QUrl::fromLocalFile("qml/TouchRTS/main.qml"));

    return app.exec();
}
