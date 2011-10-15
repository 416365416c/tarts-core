import QtQuick 1.0
//Moved into Laser for some reason, but could finish moving it back some day.
Rectangle { color: "blue"; radius: 2;
    id:r
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
    property Item target
    x: parent.width/2
    y: parent.height/2
    width: mag(deltaX(r.parent,target), deltaY(r.parent,target));
    height:2;
    //width:2; rotation: 2
    rotation: angle(r.parent.x-target.x, r.parent.y-target.y) + 180;
    transformOrigin: Item.TopLeft;
}
