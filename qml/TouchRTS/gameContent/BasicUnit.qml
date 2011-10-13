import QtQuick 1.0
import TouchRTS 0.1

MovingUnit{
    id: innerContainer
    property bool canShoot: true
    Component{
        id: laserComponent
        LaserProjectile{width: 10; height: 10}
    }
    speed: 1
    hp: 1
    onHpChanged: if(hp<=0) innerContainer.kill();
    function fire(){
        var target = innerContainer.getTarget(40);
        if( target == null)
            return;
        var laser = laserComponent.createObject(innerContainer);
        laser.target = target;
    }
    Timer{
        interval: 2000
        repeat: true
        running: true
        onTriggered: {if(uberParent.canShoot) fire();}//Multiple firings might be QTBUG-18126? Doesn't happen in 2.0
    }
    width: 10
    height: 10
    Rectangle{
        id: rect2
        anchors.fill: parent
        radius: 4
        border.color: "black"
        color: "white"
    }
    onBorn: rect2.color = innerContainer.player.color
}
