import QtQuick 2.0
import QtQuick.PathItem 2.0

Item {
    Rectangle {
        id: r
        color: "purple"
        anchors.fill: parent
    }

    PathItem {
        width: parent.width / 2
        height: parent.height / 2
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
        onClicked: r.visible = !r.visible
    }
}
