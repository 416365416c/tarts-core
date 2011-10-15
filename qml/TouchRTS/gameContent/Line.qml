import QtQuick 1.0
//Moved into Laser for some reason, but could finish moving it back some day.
Rectangle { color: "blue"; radius: 2;
    x: parent.width/2
    y: parent.height/2
    width: mag(deltaX(p.parent,target), deltaY(p.parent,target));
    id:r
    height:2;
    //width:2; rotation: 2
    rotation: angle(p.parent.x-target.x, p.parent.y-target.y) + 180;
    transformOrigin: Item.TopLeft;
    property bool runOnce: true
    opacity: 0
    SequentialAnimation on opacity{ running: true;
        NumberAnimation{ from:0; to:1; duration: 250; easing.type: Easing.OutQuad}
        ScriptAction{ script: p.target.hp = p.target.hp - 1;}
        NumberAnimation{ from:1; to:0; duration: 250; easing.type: Easing.OutQuad}
    }
}
