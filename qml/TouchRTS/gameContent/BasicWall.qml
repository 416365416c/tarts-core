import QtQuick 2.1
import TouchRTS 0.1

Unit{
    id: container
    hp: 10;
    onHpChanged: if(hp<=0) container.kill();
    width: 20
    height: 20
    Rectangle{
        id: rect
        anchors.fill: parent
        border.color: "white"
        color: "white"
    }
    Text{
        color: "white"
        anchors.centerIn: parent
        text: hp
    }

    onBorn: rect.color = container.player.color;
}
