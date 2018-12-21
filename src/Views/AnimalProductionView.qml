import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

import Common 1.0

Item {
    id: root;

    property var building: QtObject {}
    property var animalProduction: QtObject {}

    property string title: building.name;

    Item {
        anchors.fill: parent;

        Rectangle {
            id: iconContainer

            width: Stylesheet.buildingImageContainerSize;
            height: Stylesheet.buildingImageContainerSize;

            radius: Stylesheet.defaultRadius;

            border.color: "#0c0c0c";
            color: "#0c2c2c2c";

            anchors {
                top: parent.top;
                right: parent.right;
                margins: Stylesheet.defaultMargin;
            }

            Image {
                id: buildingIcon;

                x: Stylesheet.buildingImageOffset;
                y: Stylesheet.buildingImageOffset;

                source: "image://resources/buildings/" + building.id;
            }

        }

        ColumnLayout {
            anchors.fill: parent
        }

    }

}
