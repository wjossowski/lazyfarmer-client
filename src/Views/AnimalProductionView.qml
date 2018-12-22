import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

import Common 1.0

Item {
    id: root;

    property var building;
    property var buildingData;
    property var storage;

    property string title: building.name;

    Item {
        anchors.fill: parent;

        ColumnLayout {
            id: buildingInfoContainer

            Layout.preferredWidth: parent.width - iconContainer.width;

            anchors {
                top: parent.top;
                left: parent.left;

                margins: Stylesheet.defaultMargin;
                topMargin: Stylesheet.bigMargin;
            }

            spacing: Stylesheet.defaultSpacing;

            Label {
                text: building.name;

                font.pixelSize: Stylesheet.biggerFontSize;

                wrapMode: Text.WrapAnywhere;
                maximumLineCount: 1;
            }

            Item {
                Layout.fillWidth: true;

                Label {
                    anchors.left: parent.left;

                    text: qsTr("Level:") + " " + building.level + t.r;
                    font.pixelSize: Stylesheet.smallFontSize;
                }

                Label {
                    anchors.right: parent.right;

                    text: qsTr("Animals:") + " " + building.animals + t.r;
                    font.pixelSize: Stylesheet.smallFontSize;
                }
            }

        }

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
                topMargin: Stylesheet.bigMargin;
            }

            Image {
                id: buildingIcon;

                x: Stylesheet.buildingImageOffset;
                y: Stylesheet.buildingImageOffset;

                source: "image://resources/buildings/" + building.id;
            }

        }

        ListView {
            id: outputProductionContainer

            orientation: Qt.Horizontal

            anchors {
                top: iconContainer.bottom;
                bottom: parent.bottom;
                left: parent.left;
                right: parent.right;
            }

            spacing: Stylesheet.defaultSpacing;
            highlightFollowsCurrentItem: true;

            focus: true;
            model: buildingData.inputProductsInfo;
            delegate: Rectangle {
                id: inputDelegate

                property bool isSelected: buildingData.chosenProductId === modelData.In;

                anchors.margins: Stylesheet.defaultMargin;

                radius: Stylesheet.defaultRadius;

                width: 100;
                height: 50;

                border.color: "#0c0c0c";
                color: isSelected ? "#7c2c8c2c" : "#0c2c2c2c";

                Label {
                    anchors.centerIn: parent;
                    text: modelData.Name + " (" + storage.amount(modelData.In) + ")"
                }

                MouseArea {
                    anchors.fill: parent;
                    onClicked: function () {
                        root.buildingData.setChosenProductId(modelData.In);
                    }
                }

            }

        }

        ColumnLayout {
            anchors.fill: parent
        }

    }

}
