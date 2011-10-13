import QtQuick 1.0
import TouchRTS 0.1

BuildOption{
    cost: 1
    iconSource: ""
    BasicTower{
        id: container
        hp:1000; sink: true;
        onSunk: {
            otherUnit.hp -= 100;
            container.hp -= 100;
        }
    }
}
