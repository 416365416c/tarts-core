import QtQuick 1.0

Item {
        property Item target: p
        width: 20; height: 20; id: p
        function deltaX(a,b){//a,b are items
            return (a.x+(a.width/2)) - (b.x+(b.width/2));
        }
        function deltaY(a,b){//a,b are items
            return (a.y+(a.height/2)) - (b.y+(b.height/2));
        }
        function mag(xArg,yArg){
            return Math.sqrt(xArg*xArg + yArg*yArg);
        }
        function angle(xArg,yArg){
            var ret = (Math.atan2(yArg,xArg) * 180)/Math.PI;
            if(ret < 0)
                ret = (ret+360);
            return ret;
        }
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
        Timer {interval: 500;running:true; repeat: false;  onTriggered: p.destroy();}
}
