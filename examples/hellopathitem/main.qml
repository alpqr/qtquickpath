import QtQuick 2.0
import QtQuick.PathItem 2.0

Item {
    Rectangle {
        id: r
        color: "purple"
        anchors.fill: parent
    }

    Rectangle {
        width: 300
        height: 300
        anchors.centerIn: parent
        border.color: "black"
        border.width: 4
        color: "transparent"
        NumberAnimation on rotation {
            from: 0
            to: 360
            duration: 5000
            loops: Animation.Infinite
        }

        PathItem {
            id: path
            anchors.fill: parent
            property real ex: 100
            property real ey: 100
            onExChanged: regen()
            onEyChanged: regen()
            function regen() {
                clear();
                lineTo(ex, ey);
                lineTo(0, ey);
                closeSubPath();
                addEllipseWithCenter(150, 150, 50, 50)
                update()
            }
            SequentialAnimation on ex {
                NumberAnimation {
                    from: 100
                    to: 300
                    duration: 2000
                    easing.type: Easing.OutBounce
                }
                NumberAnimation {
                    from: 300
                    to: 100
                    duration: 2000
                    easing.type: Easing.InBounce
                }
                PauseAnimation { duration: 2000 }
                loops: Animation.Infinite
            }
            SequentialAnimation on ey {
                NumberAnimation {
                    from: 100
                    to: 300
                    duration: 2000
                    easing.type: Easing.OutBounce
                }
                NumberAnimation {
                    from: 300
                    to: 100
                    duration: 2000
                    easing.type: Easing.InBounce
                }
                PauseAnimation { duration: 2000 }
                loops: Animation.Infinite
            }
            Component.onCompleted: regen()

            fillMaterial: "green"
            strokeMaterial: "white"
            strokeWidth: 4
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: { path.ex = path.ex + 1; r.visible = !r.visible }
    }
}
