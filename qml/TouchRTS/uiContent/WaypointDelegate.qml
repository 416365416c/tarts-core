import QtQuick 1.0
import "../gameContent"

Rectangle{
    color: "goldenrod"
    width: 8
    height: 8
    x: modelData.x
    y: modelData.y
    border.color: modelData.player.color
    Line{
        color: "goldenrod"
        target: modelData.next
    }
}
