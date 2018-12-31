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

                }

                BusyIndicator {
                    id: workGroup

                    doneDate: building.doneTimestamp;
                    totalInterval: building.baseTimeout;
                    showIdleStatus: false;

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

            text: qsTr("Select output product:") + t.r;
        }

        ListView {
            id: productionSettings

            orientation: Qt.Horizontal

            Layout.margins: Stylesheet.smallMargin;
            Layout.leftMargin: Stylesheet.biggerMargin;

            Layout.fillWidth: true;
            Layout.minimumHeight: Stylesheet.tinyWidgetSize;

            spacing: Stylesheet.defaultSpacing;

            model: buildingData.inputProductsInfo;
            delegate: ProductDelegate {
                productId: modelData.Out;
                productsStorage: storage;

                isSelected: buildingData.outputProduct === productId;

                MouseArea {
                    anchors.fill: parent;
                    onClicked: function () {
                        buildingData.setOutputProductId(productId);
                    }
                }

            }

        }

        Label {
            Layout.margins: Stylesheet.smallMargin;
            Layout.leftMargin: Stylesheet.bigMargin;

            font.pixelSize: Stylesheet.bigFontSize;

            text: qsTr("Production details:") + t.r;
        }

        ProductInfoContainer {
            Layout.margins: Stylesheet.smallMargin;
            Layout.leftMargin: Stylesheet.biggerMargin;
            Layout.rightMargin: Stylesheet.biggerMargin;

            Layout.fillWidth: true;
            Layout.minimumHeight: Stylesheet.inputProductContainerSize;

            RowLayout {
                id: productionInfoContainer;

                anchors.fill: parent;

                visible: buildingData.inputProduct !== -1;

                ProductDelegate {
                    productId: buildingData.inputProduct;
                    productsStorage: storage;

                    Layout.margins: Stylesheet.tinyMargin;

                    border.color: Stylesheet.transparentColor;
                    color: Stylesheet.transparentColor;
                }

                ColumnLayout {
                    id: productionInfo;

                    property bool isOutOfResources: buildingData.productAmount > storage.amount(buildingData.inputProduct);

                    Layout.margins: Stylesheet.defaultMargin;

                    Label {
                        Layout.margins: Stylesheet.tinyMargin;
                        font.pixelSize: Stylesheet.defaultFontSize;

                        text: (function (required, owns) {
                            return qsTr("Required: %1 (owns %2)").arg(required).arg(owns)
                        }) (buildingData.productAmount, storage.amount(buildingData.inputProduct)) + t.r;
                    }

                    Label {
                        Layout.margins: Stylesheet.tinyMargin;
                        font.pixelSize: Stylesheet.defaultFontSize;

                        text: qsTr("Lasts for:") + " " + Utils.TimeUtils.timeLeftToString(buildingData.totalTime) + t.r;
                    }

                    Item {
                        // Spacing item
                        Layout.fillHeight: true;
                    }

                    Label {
                        visible: productionInfo.isOutOfResources;

                        Layout.margins: Stylesheet.tinyMargin;

                        color: Stylesheet.warningColor;

                        font.pixelSize: Stylesheet.defaultFontSize;
                        font.bold: true;

                        text: qsTr("Out of products.") + t.r;
                    }
                }

            }

            Label {
                anchors.centerIn: parent;

                visible: !productionInfoContainer.visible;

                font.pixelSize: Stylesheet.biggerFontSize;

                text: qsTr("Production not set up.") + t.r;
            }

        }

        Item {
            // Spacing item
            Layout.fillHeight: true;
        }
    }

}

