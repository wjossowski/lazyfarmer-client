import QtQuick 2.7
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Item {
    id: root;

    ListView {
        id: playersList;

        model: PlayerFactoryModel;

        anchors.fill: parent;
        anchors.margins: 10;

        delegate: Item {
            width: parent.width;
            height: 55;

            RowLayout {
                anchors.verticalCenter: parent.verticalCenter;

                spacing: 10

                Label {
                    text: description + "(" + level + ")";
                }

            }

            Label {
//                visible: lastError;

                anchors.centerIn: parent;

                text: lastError;

                Material.foreground: Material.DeepOrange;
            }

            RowLayout {
                anchors.verticalCenter: parent.verticalCenter;
                anchors.right: parent.right;

                Layout.alignment: Qt.AlignRight;

                spacing: 10

                Label {
                    text: job
                }

                RoundButton {
                    id: editButton
                    text: "\u270E"
                }

                RoundButton {
                    id: deleteButton
                    Material.background: Material.DeepOrange;
                    text: "x"

                    onClicked: {
                        PlayerFactoryModel.removeAt(index);
                    }
                }

            }

        }

        add: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; from: 0;    duration: 1000 }
                NumberAnimation { property: "x";       from: -100; duration: 400 }
            }
        }

        remove: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; to: 0;    duration: 400 }
                NumberAnimation { property: "x";       to: -100; duration: 400 }
            }
        }

    }

    RoundButton {
        anchors {
            bottom: parent.bottom;
            right: parent.right;
            margins: 10;
        }

        width: 80;
        height: 80;

        text: "+";

        onClicked: {
            PlayerFactoryModel.createPlayer();
        }

    }

}
