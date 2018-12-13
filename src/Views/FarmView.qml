import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

//import Common 1.0

Item {
    id: root;

    readonly property string title: qsTr("Overview") + t.r;

    function initialize (buildingModel) {
        buildings.model = buildingModel;
    }

    ListView {
        id: buildings;

        spacing: 10;

        anchors {
            fill: parent;

            margins: 20;
        }

        delegate: Rectangle {
            id: building;

            width: parent.width;
            height: 150;

            radius: 10;

            border.color: "#0c0c0c";

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

                    text: name;
                    font.pixelSize: 28;
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

            }

        }

    }

}
