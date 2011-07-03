import QtQuick 1.0
import TouchRTS 0.1
import "../gameContent" as GenericGame

Map {
    players:[
        Player{
            id: redPlayer
            hq: GenericGame.BasicHQ{}
            buildOptions: Buildable{GenericGame.BasicWall{}}
            color: "red"
            startPos: "150,40"
        }
        ,
        Player{
            id: bluePlayer
            hq: GenericGame.BasicHQ{}
            buildOptions: GenericGame.BasicBuildable{}
            color: "blue"
            startPos: "150,260"
        }
    ]

    width: 320
    height: 320
    Image{
        source: "cheeseTile.jpg"
        fillMode: Image.Tile;
    }
    Doodad{//Tree
        x: 60
        y: 60
        width: 40
        height: 40
        blocksPathing: true
        blocksLOS:  true
        Rectangle{
            color: "green"
            anchors.fill:  parent
            radius: 4
        }
    }
    Doodad{//Also Tree
        x: 240
        y: 240
        width: 40
        height: 40
        blocksPathing: true
        blocksLOS:  true
        Rectangle{
            color: "green"
            anchors.fill:  parent
            radius: 4
        }
    }
}
