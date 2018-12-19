import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

import "qrc:///qml/Common/utils.js" as Utils

Item {
    id: root;

    readonly property string title: qsTr("Overview") + t.r;

    readonly property int gridWidth: 400;
    readonly property int gridHeight: 170;

    property var buildingModel: QtObject {}

    GridView {
        id: buildings;

        cellWidth: root.gridWidth;
        cellHeight: root.gridHeight;

        model: buildingModel

        anchors {
            fill: parent;

            margins: 10;
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

                    var timeLeft = Utils.calculateTimeLeft(doneTimestamp)
                    building.timeLeft = timeLeft

                    workTimeoutLabel.timeLeft = Utils.timeLeftToString(timeLeft);

                    var jobPercentage = Utils.calculateJobPercentage(timeLeft, baseTimeout);
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
                    margins: 5;
                }

                radius: 10;

                border.color: "#0c0c0c";
                color: "#0c2c2c2c";

                RowLayout {
                    spacing: 10;

                    Item {
                        anchors.margins: 10;

                        Image {
                            x: 10;
                            y: 10;
                            source: "image://resources/buildings/" + id;
                        }
                    }

                }

                ColumnLayout {
                    spacing: 5;

                    anchors {
                        top: parent.top;
                        right: parent.right;
                    }

                    Label {
                        Layout.alignment: Qt.AlignRight;
                        Layout.topMargin: 10;
                        Layout.rightMargin: 10;

                        Layout.maximumWidth: root.gridWidth - root.gridHeight;

                        font.pixelSize: 28;

                        wrapMode: Text.WrapAnywhere;
                        maximumLineCount: 1;

                        text: name
                    }

                    Label {
                        Layout.alignment: Qt.AlignRight;
                        Layout.rightMargin: 10;

                        text: qsTr("Level:") + " " + level + t.r;
                    }

                    Label {
                        visible: animals > 0;

                        Layout.alignment: Qt.AlignRight;
                        Layout.rightMargin: 10;

                        font.pixelSize: 10;
                        text: qsTr("Animals:") + " " + animals + t.r;
                    }

                    ProgressBar {
                        id: workProgressBar;

                        visible: building.isInProgress;

                        Layout.alignment: Qt.AlignRight;
                        Layout.rightMargin: 10;
                    }

                    Item {
                        id: workGroup

                        Layout.alignment: Qt.AlignRight;

                        Layout.fillHeight: true;
                        Layout.fillWidth: true;

                        Layout.rightMargin: 10;
                        Layout.topMargin: 5;

                        Label {
                            id: workTimeoutLabel;

                            property string timeLeft;

                            visible: building.isInProgress;
                            anchors {
                                verticalCenter: parent.verticalCenter;
                                left: parent.left;
                            }

                            font.pixelSize: 11;
                            text: qsTr("Ready In: ") + timeLeft;

                        }

                        Label {
                            id: workPercentIndicatorLabel;

                            property string percentage: "0.00";

                            anchors {
                                verticalCenter: parent.verticalCenter;
                                right: parent.right;
                            }

                            font.pixelSize: 11;
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
                    spacing: 5;

                    anchors {
                        bottom: parent.bottom;
                        right: parent.right;

                        margins: 10;
                    }

                    Label {
                        font.pixelSize: 10;

                        Layout.alignment: Qt.AlignRight;

                        text: qsTr("Farm:") + farm + t.r
                    }

                    Label {
                        font.pixelSize: 10;

                        Layout.alignment: Qt.AlignRight;

                        text: qsTr("Position:") + position + t.r
                    }

                }

            }

        }

    }

}
