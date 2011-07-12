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
            "map2",
            "map3"
        ]
        delegate: Button{
            text: "Load " + modelData;
            onClicked: game.loadMap("qml/TouchRTS/" + modelData + "/" + modelData + ".qml", gameArea);
        }
    }

    Item{
        id: gameArea
        MouseArea{
	    acceptedButtons: Qt.LeftButton | Qt.RightButton
            anchors.fill: parent
            onClicked:{
		var buildOption = 0;
		if(mouse.button == Qt.RightButton)
		    buildOption = 1;
		game.build(mouse.x, mouse.y, game.players[0].buildOptions[buildOption]);
            }
        }

        x:400
        height: 400
        width: 400
    }
}
