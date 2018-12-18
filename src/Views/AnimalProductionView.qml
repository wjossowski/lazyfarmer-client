import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

Item {
    id: root;

    readonly property string title: qsTr("Animals") + t.r;
    property var rootContext

    function initialize (buildingData) {
        root.rootContext = buildingData
    }

    Rectangle {
        anchors.fill: parent;

        Text {
            anchors.centerIn: parent;
            text: rootContext.Foo;
        }

        color: "#00ffaa";
    }

}
