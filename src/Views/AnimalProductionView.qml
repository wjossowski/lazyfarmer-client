import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

import "qrc:///qml/Common/utils.js" as Utils

Item {
    id: root;

    property var building: QtObject {}
    property var animalProduction: QtObject {}

    property string title: building.name;

    Item {
        anchors.fill: parent;

        Rectangle {
            width: 140;
            height: 140;

            radius: 10;

            border.color: "#0c0c0c";
            color: "#0c2c2c2c";

            anchors {
                top: parent.top;
                right: parent.right;
                margins: 5;
            }

            Image {
                id: buildingIcon;

                x: 7;
                y: 7;

                source: "image://resources/buildings/" + building.id;
            }

        }

        ColumnLayout {
            anchors.fill: parent
        }

    }

}
