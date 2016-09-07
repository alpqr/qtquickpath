import QtQuick 2.0
import QtQuick.PathItem 2.0

Item {
    Rectangle {
        id: background
        gradient: Gradient {
            GradientStop { position: 0; color: "steelblue" }
            GradientStop { position: 1; color: "black" }
        }
        anchors.fill: parent
    }

    Text {
        font.pointSize: 20
        color: "lightGray"
        text: "QPainterPath + Qt Quick. No Canvas or QQuickPaintedItem involved."
    }

    Rectangle {
        width: 300
        height: 300
        anchors.centerIn: parent
        border.color: "black"
        border.width: 4
        color: "transparent"
        NumberAnimation on rotation {
            running: background.visible
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

            fillColor: "green"
            strokeColor: "red"
            strokeWidth: 4
            strokeStyle: PathItem.DashLine
            property real firstSpaceLength: 1
            dashPattern: [ 1, firstSpaceLength, 4, 2 ]
            NumberAnimation on firstSpaceLength { // a bit extreme example :)
                from: 1
                to: 50
                duration: 10000
                loops: Animation.Infinite
            }
        }

        // pie
        PathItem {
            anchors.right: parent.right
            anchors.top: parent.top
            width: 100
            height: 100
            strokeColor: "blue"
            fillColor: "lightGray"
            strokeWidth: 2
            function draw() {
                moveTo(50, 50)
                arcTo(20, 30, 60, 40, 60, 240)
                closeSubPath()
            }
            Component.onCompleted: draw()
            ColorAnimation on fillColor {
                from: "lightGray"
                to: "brown"
                duration: 5000
                loops: Animation.Infinite
            }
        }

        // bezier
        PathItem {
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            width: 100
            height: 100
            strokeColor: "blue"
            fillColor: "lightGray"
            strokeWidth: 2
            strokeStyle: PathItem.DashLine
            dashPattern: [ 1, 4 ]
            function draw() {
                moveTo(20, 30);
                cubicTo(80, 0, 50, 50, 80, 80);
            }
            Component.onCompleted: draw()
        }

        // star
        PathItem {
            id: star
            anchors.bottom: parent.bottom
            width: 100
            height: 100
            strokeColor: "blue"
            fillColor: fillEnabled ? "lightGray" : "transparent"
            property bool fillEnabled: true
            strokeWidth: 2
            function draw() {
                moveTo(90, 50);
                for (var i = 1; i < 5; ++i)
                    lineTo(50 + 40 * Math.cos(0.8 * i * Math.PI),
                           50 + 40 * Math.sin(0.8 * i * Math.PI))
                closeSubPath()
            }
            Component.onCompleted: draw()
        }

        Timer {
            interval: 1000
            onTriggered: star.fillRule = (star.fillRule === PathItem.OddEvenFill ? PathItem.WindingFill : PathItem.OddEvenFill)
            repeat: true
            running: true
        }
    }

    PathItem {
        id: joinTest
        x: 50
        y: 50
        width: 100
        height: 100
        strokeColor: "yellow"
        strokeWidth: 16
        function draw() {
            lineTo(50, 100)
            lineTo(0, 100)

        }
        fillColor: "transparent"
        capStyle: PathItem.RoundCap
        Component.onCompleted: draw()
    }

    Timer {
        interval: 1000
        repeat: true
        running: true
        property variant styles: [ PathItem.BevelJoin, PathItem.MiterJoin, PathItem.RoundJoin ]
        onTriggered: {
            for (var i = 0; i < styles.length; ++i)
                if (styles[i] === joinTest.joinStyle) {
                    joinTest.joinStyle = styles[(i + 1) % styles.length];
                    break;
                }
        }
    }

    // now a line with the declarative command api
    PathItem {
        anchors.bottom: parent.bottom
        anchors.margins: 50
        width: 100
        height: 100
        strokeColor: "green"
        strokeWidth: 5
        fillColor: "transparent" // just an optimization

        MoveTo {
            x: 10
            y: 10
        }
        LineTo {
            x: 90
            y: 10
            NumberAnimation on y {
                from: 10
                to: 90
                duration: 2000
                loops: Animation.Infinite
            }
        }
    }

    PathItem {
        id: ellipse
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 50
        width: 100
        height: 100
        strokeWidth: 5
        strokeColor: "yellow"
        property bool fillEnabled: false
        fillColor: fillEnabled ? "green" : "transparent"

        Ellipse {
            centerX: 50
            centerY: 50
            radiusX: 40
            radiusY: 20
        }
    }
    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: ellipse.strokeWidth = (ellipse.strokeWidth > 0 ? 0 : 5)
    }

    MouseArea {
        anchors.fill: parent
        onClicked: { background.visible = !background.visible;
            star.fillEnabled = !star.fillEnabled;
            ellipse.fillEnabled = !ellipse.fillEnabled;
        }
    }
}
