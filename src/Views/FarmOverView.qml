import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

import Common 1.0

Item {
    id: root;

    readonly property string title: qsTr("Overview") + t.r;

    readonly property int gridWidth: Stylesheet.defaultWidgetSize;
    readonly property int gridHeight: Stylesheet.buildingImageContainerSize + 2 * Stylesheet.defaultMargin;

    property var buildingModel: QtObject {}

    GridView {
        id: buildings;

        cellWidth: root.gridWidth;
        cellHeight: root.gridHeight;

        model: buildingModel

        anchors {
            fill: parent;

            margins: Stylesheet.defaultMargin;
        }

        delegate: Item {
            id: building;

            width: root.gridWidth;
            height: root.gridHeight;

            MouseArea {
                anchors.fill: parent;
                onClicked: function () {
                    LazyFarmer.requestBuildingInfoScreen(buildings.model, index);
                }
            }

            Rectangle {
                anchors {
                    fill: parent;
                    margins: Stylesheet.smallMargin;
                }

                radius: Stylesheet.defaultRadius;

                border.color: Stylesheet.defaultBorderColor;
                color: Stylesheet.defaultBackgroundNormalColor;

                Item {
                    id: iconContainer

                    width: Stylesheet.buildingImageContainerSize;
                    height: Stylesheet.buildingImageContainerSize;

                    anchors.margins: Stylesheet.defaultMargin;

                    Image {
                        x: Stylesheet.buildingImageOffset;
                        y: Stylesheet.buildingImageOffset;

                        source: "image://resources/buildings/" + id;
                    }
                }

                ColumnLayout {
                    spacing: Stylesheet.smallSpacing;

                    anchors {
                        top: parent.top;
                        left: iconContainer.right;
                        right: parent.right;
                    }

                    Label {
                        Layout.alignment: Qt.AlignRight;
                        Layout.topMargin: Stylesheet.defaultMargin;
                        Layout.rightMargin: Stylesheet.defaultMargin;

                        font.pixelSize: Stylesheet.biggerFontSize;

                        wrapMode: Text.WrapAnywhere;
                        maximumLineCount: 1;

                        text: name
                    }

                    Label {
                        Layout.alignment: Qt.AlignRight;
                        Layout.rightMargin: Stylesheet.defaultMargin;

                        text: qsTr("Level:") + " " + level + t.r;
                    }

                    Label {
                        visible: animals > 0;

                        Layout.alignment: Qt.AlignRight;
                        Layout.rightMargin: Stylesheet.defaultMargin;

                        font.pixelSize: Stylesheet.smallerFontSize;
                        text: qsTr("Animals:") + " " + animals + t.r;
                    }

                    BusyIndicator {
                        id: workGroup

                        doneDate: doneTimestamp;
                        totalInterval: baseTimeout;

                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        Layout.leftMargin: Stylesheet.bigMargin;
                        Layout.rightMargin: Stylesheet.defaultMargin;

                        Layout.topMargin: Stylesheet.smallerMargin
                    }

                }

                ColumnLayout {
                    spacing: Stylesheet.smallFontSize;

                    anchors {
                        bottom: parent.bottom;
                        right: parent.right;

                        margins: Stylesheet.defaultMargin;
                    }

                    Label {
                        font.pixelSize: Stylesheet.smallerFontSize;

                        Layout.alignment: Qt.AlignRight;

                        text: qsTr("Farm:") + farm + t.r
                    }

                    Label {
                        font.pixelSize: Stylesheet.smallerFontSize;

                        Layout.alignment: Qt.AlignRight;

                        text: qsTr("Position:") + position + t.r
                    }

                }

            }

        }

    }

}
