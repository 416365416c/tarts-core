import QtQuick 1.0
import "uiContent"

Rectangle {
    id: root
    width: 800; height: 423//N900 Size if not maximized
    Text{
        id: deb
        Component.onCompleted: game.registerDebugText(deb);
    }

    ListView{
        y: 40
        height: 400
        model: [
            "map0",
            "map1",
            "map2"
        ]
        delegate: Button{
            text: "Load " + modelData;
            onClicked: game.loadMap("qml/TouchRTS/" + modelData + "/" + modelData + ".qml", gameArea);
        }
    }

    Item{
        id: gameArea
        MouseArea{
            anchors.fill: parent
            onClicked: game.build(mouse.x, mouse.y, game.players[0].buildOptions[0]);
        }

        x:400
        height: 400
        width: 400
    }
}
