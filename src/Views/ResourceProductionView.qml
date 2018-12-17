import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

Item {
    id: root;

    readonly property string title: qsTr("Production") + t.r;

    Rectangle {
        anchors.fill: parent;

        color: "#aa00ff";
    }

}
