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

    ColumnLayout {
        anchors.fill: parent;

        RowLayout {
            id: topContainer;

            Layout.fillWidth: true;
            Layout.maximumHeight: iconContainer.height

            ColumnLayout {
                id: buildingInfoContainer

                Layout.fillWidth: true;
                Layout.margins: Stylesheet.bigMargin;

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

                BusyIndicator {
                    id: workGroup

                    doneDate: building.doneTimestamp;
                    totalInterval: building.baseTimeout;

                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Layout.topMargin: Stylesheet.bigMargin;
                    Layout.bottomMargin: Stylesheet.bigMargin;
                }

                Item {
                    // Spacer
                    Layout.fillHeight: true;
                }

            }

            Rectangle {
                id: iconContainer

                width: Stylesheet.buildingImageContainerSize;
                height: Stylesheet.buildingImageContainerSize;

                radius: Stylesheet.defaultRadius;

                border.color: "#0c0c0c";
                color: "#0c2c2c2c";

                Layout.margins: Stylesheet.bigMargin;

                Image {
                    x: Stylesheet.buildingImageOffset;
                    y: Stylesheet.buildingImageOffset;

                    source: "image://resources/buildings/" + building.id;
                }

            }

        }

        ListView {
            id: inputProductionContainer

            orientation: Qt.Horizontal

            Layout.margins: Stylesheet.bigMargin;
            Layout.fillWidth: true;

            spacing: Stylesheet.defaultSpacing;

            model: buildingData.inputProductsInfo;
            delegate: Rectangle {
                id: inputDelegate

                property bool isSelected: buildingData.chosenProductId === modelData.In;

                anchors.margins: Stylesheet.defaultMargin;

                radius: Stylesheet.defaultRadius;

                width: delegateStatusLabel.width + 2*Stylesheet.defaultMargin;
                height: 50;

                border.color: "#0c0c0c";
                color: isSelected ? "#7c2c8c2c" : "#0c2c2c2c";

                Label {
                    id: delegateStatusLabel
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

        Item {
            // Spacing item
            Layout.fillHeight: true;
        }
    }

}

