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

    ListView{
        id: builder0
        y: 40; x: 120
        width: 64
        height: 400
        model: game.players[0].buildOptions
        delegate: Image{
            source: iconSource
            MouseArea{
                anchors.fill: parent
                onClicked: builder0.currentIndex = index;
            }
            Rectangle{
                anchors.fill: parent
                border.color: ListView.isCurrentItem?game.players[0].color:"black"
                border.width: 8
                z: -1
            }
        }
    }
    ListView{
        y: 40; x: 200
        width: 64
        height: 400
        id: builder1
        model: game.players[1].buildOptions
        delegate: Image{
            source: iconSource
            MouseArea{
                anchors.fill: parent
                onClicked: builder1.currentIndex = index;
            }
            Rectangle{
                anchors.fill: parent
                border.color: ListView.isCurrentItem?game.players[1].color:"black"
                border.width: 8
                z: -1
            }
        }
    }

    Item{
        id: gameArea
        MouseArea{
	    acceptedButtons: Qt.LeftButton | Qt.RightButton
            anchors.fill: parent
            onClicked:{
		var player = 0;
		if(mouse.button == Qt.RightButton)
		    player = 1;
                var idx = builder0.currentIndex;
                if(player == 1)
                    idx = builder1.currentIndex;
                var resBool = game.build(mouse.x, mouse.y, game.players[player].buildOptions[idx],player);
                console.log("Building " + player + ", " + idx + " happened? " + resBool);
            }
        }

        x:400
        height: 400
        width: 400
    }
}
