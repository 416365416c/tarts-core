import QtQuick 1.0
import TouchRTS 0.1

BuildOption{
    cost: 1
    iconSource: "content/factoryIcon.png"
    Unit{
        id: container
        property Waypoint exit;
        BuildOption{
            id: troop
            MovingUnit{
                id: innerContainer
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
                    onTriggered: {fire();}//Multiple firings might be QTBUG-18126? Doesn't happen in 2.0
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
        }
        hp: 10;
        onHpChanged: if(hp<=0) container.kill();
        width: 20
        height: 20
        function fire(){
            game.build(0,0,troop,game.playerIdx(container.player),exit);
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

        onBorn: {
            rect.color = container.player.color;
            container.exit = game.createWaypoint(x+5, y-10, game.playerIdx(container.player));
        }
        Timer{
            interval: 6000
            triggeredOnStart: true
            repeat: true
            running: true
            onTriggered: {fire();}//Multiple firings might be QTBUG-18126? Doesn't happen in 2.0
        }
    }
}
