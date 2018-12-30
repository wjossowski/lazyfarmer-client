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

                border.color: Stylesheet.defaultBorderColor;
                color: Stylesheet.defaultBackgroundNormalColor;

                Layout.margins: Stylesheet.bigMargin;

                Image {
                    x: Stylesheet.buildingImageOffset;
                    y: Stylesheet.buildingImageOffset;

                    source: "image://resources/buildings/" + building.id;
                }

            }

        }

        Label {
            Layout.margins: Stylesheet.smallMargin;
            Layout.leftMargin: Stylesheet.bigMargin;

            font.pixelSize: Stylesheet.bigFontSize;

            text: qsTr("Select Forage:") + t.r;
        }

        ListView {
            id: inputProductionContainer

            orientation: Qt.Horizontal

            Layout.margins: Stylesheet.smallMargin;
            Layout.leftMargin: Stylesheet.biggerMargin;

            Layout.fillWidth: true;
            Layout.minimumHeight: Stylesheet.tinyWidgetSize;

            spacing: Stylesheet.defaultSpacing;

            model: buildingData.inputProductsInfo;
            delegate: ProductDelegate {
                productId: modelData.In;
                productsStorage: storage;

                isSelected: buildingData.chosenProductId === productId;

                MouseArea {
                    anchors.fill: parent;
                    onClicked: function () {
                        root.buildingData.setChosenProductId(productId);
                    }
                }
            }

        }

        Label {
            Layout.margins: Stylesheet.smallMargin;
            Layout.leftMargin: Stylesheet.bigMargin;

            font.pixelSize: Stylesheet.bigFontSize;

            text: qsTr("Output Product:") + t.r;
        }

        ProductDelegate {
            id: outputProductContainer;

            productId: buildingData.outputProduct;
            productsStorage: storage;

            Layout.margins: Stylesheet.smallMargin;
            Layout.leftMargin: Stylesheet.biggerMargin;
        }

        Item {
            // Spacing item
            Layout.fillHeight: true;
        }
    }

}

