import QtQuick 1.0
import TouchRTS 0.1

BuildOption{
    cost: 1
    iconSource: ""
    needsControl: false
    BasicTower{
        id: container
        hp:1000; sink: true;
        nodeRadius: 100
        onSunk: {
            otherUnit.hp -= 100;
            container.hp -= 100;
        }
    }
}
