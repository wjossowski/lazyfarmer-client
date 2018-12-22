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

            property int timeLeft: 0;
            property bool isInProgress: !Number.isNaN(doneTimestamp) && timeLeft >= 0;

            Timer {
                id: updateTimer

                interval: 1000
                running: building.isInProgress
                repeat: true;

                onTriggered: function () {
                    if (!building.isInProgress) {
                        return;
                    }

                    var timeLeft = Utils.TimeUtils.calculateTimeLeft(doneTimestamp)
                    building.timeLeft = timeLeft

                    workTimeoutLabel.timeLeft = Utils.TimeUtils.timeLeftToString(timeLeft);

                    var jobPercentage = Utils.TimeUtils.calculateJobPercentage(timeLeft, baseTimeout);
                    workProgressBar.value = jobPercentage;
                    workPercentIndicatorLabel.percentage = (100*jobPercentage).toFixed(2);

                }
            }

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

                border.color: "#0c0c0c";
                color: "#0c2c2c2c";

                RowLayout {
                    spacing: Stylesheet.defaultSpacing;

                    Item {
                        anchors.margins: Stylesheet.defaultMargin;

                        Image {
                            x: Stylesheet.buildingImageOffset;
                            y: Stylesheet.buildingImageOffset;
                            source: "image://resources/buildings/" + id;
                        }
                    }

                }

                ColumnLayout {
                    spacing: Stylesheet.smallSpacing;

                    anchors {
                        top: parent.top;
                        right: parent.right;
                    }

                    Label {
                        Layout.alignment: Qt.AlignRight;
                        Layout.topMargin: Stylesheet.defaultMargin;
                        Layout.rightMargin: Stylesheet.defaultMargin;

                        Layout.maximumWidth: root.gridWidth - root.gridHeight;

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

                    ProgressBar {
                        id: workProgressBar;

                        visible: building.isInProgress;

                        Layout.alignment: Qt.AlignRight;
                        Layout.rightMargin: Stylesheet.defaultMargin;
                    }

                    Item {
                        id: workGroup

                        Layout.alignment: Qt.AlignRight;

                        Layout.fillHeight: true;
                        Layout.fillWidth: true;

                        Layout.rightMargin: Stylesheet.defaultMargin;
                        Layout.topMargin: Stylesheet.smallMargin;

                        Label {
                            id: workTimeoutLabel;

                            property string timeLeft;

                            visible: building.isInProgress;
                            anchors {
                                verticalCenter: parent.verticalCenter;
                                left: parent.left;
                            }

                            font.pixelSize: Stylesheet.smallFontSize;
                            text: qsTr("Ready In: ") + timeLeft;

                        }

                        Label {
                            id: workPercentIndicatorLabel;

                            property string percentage: "0.00";

                            anchors {
                                verticalCenter: parent.verticalCenter;
                                right: parent.right;
                            }

                            font.pixelSize: Stylesheet.smallFontSize;
                            text: (function () {
                                if (building.isInProgress) {
                                    return qsTr("Done at: ") + " " + percentage + "%";
                                } else {
                                    return qsTr("Idle")
                                }
                            })(percentage, t.r)

                        }

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
