import QtQuick 1.0

Rectangle {
    id: container
    signal clicked()
    property alias text: txt.text
    width: Math.max(100, txt.width + 8);
    height: 62
    color: "white"
    border.color: "black"
    radius: 4
    Text{
        id: txt
        anchors.centerIn: parent
    }

    MouseArea{
        anchors.fill: parent
        onClicked: parent.clicked()
    }
}
