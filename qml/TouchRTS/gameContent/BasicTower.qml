import QtQuick 1.0
import TouchRTS 0.1

Unit{
    id: container
    Component{
        id: laserComponent
        LaserProjectile{}
    }
    hp: 10;
    onHpChanged: if(hp<=0) container.kill();
    width: 20
    height: 20
    function fire(){
        var target = container.getTarget(100);
        if( target == null)
            return;
        var laser = laserComponent.createObject(container);
        laser.target = target;
    }

    Rectangle{
        id: rect
        anchors.fill: parent
        border.color: "black"
        color: "white"
    }
    Text{
        color: "white"
        anchors.centerIn: parent
        text: hp
    }

    onBorn: rect.color = container.player.color;
    Timer{
        interval: 2000
        repeat: true
        running: true
        onTriggered: {fire();}//Multiple firings might be QTBUG-18126? Doesn't happen in 2.0
    }
}
