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
        Component.onCompleted: {
            moveTo(0, 0);
            lineTo(100, 100);
            lineTo(0, 100);
        }
        fillMaterial: "green"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: r.visible = !r.visible
    }
}
