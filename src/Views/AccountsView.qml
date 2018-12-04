import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

Item {
    id: root;

    anchors.fill: parent;

    property string title: qsTr("Accounts") + t.r;
    property var loginDialog;

    ListView {
        id: playersList;

        model: PlayerFactoryModel;

        anchors {
            top: parent.top;
            left: parent.left;
            right: parent.right
            bottom: addButton.top

            margins: 10;
            topMargin: 20;
            bottomMargin: 20;
        }

        delegate: Item {
            id: player;

            width: parent.width;
            height: 100;

            readonly property bool isReady: !!level

            ColumnLayout {
                spacing: 10;

                anchors.top: parent.top;
                anchors.topMargin: 15;

                Label {
                    text: description + "(" + level + ")";
                }

                Label {
                    text: qsTr("Job: ") + job + t.r;
                }

            }

            ColumnLayout {
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter;

                spacing: 5;

                Label {
                    Layout.fillWidth: true;
                    horizontalAlignment: "AlignHCenter"

                    text: lastError;
                    Material.foreground: Material.DeepOrange;
                }

                RowLayout {
                    Layout.alignment: Qt.AlignRight;

                    spacing: 10;

                    RoundButton {
                        id: showButton;

                        visible: player.isReady;

                        text: "\u1f4dc";

                        onClicked: {
                            LazyFarmer.showPage("Foo", PlayerFactoryModel.at(index));
                        }
                    }

                    RoundButton {
                        id: editButton;

                        visible: !player.isReady;

                        text: "\u270E";

                        onClicked: {
                            loginDialog.setUp(PlayerFactoryModel.at(index));
                            loginDialog.open();
                        }
                    }

                    RoundButton {
                        id: deleteButton;
                        Material.background: Material.DeepOrange;
                        text: "x";

                        onClicked: {
                            PlayerFactoryModel.removeAt(index);
                        }
                    }

                }

            }

            Rectangle {
                anchors {
                    bottom: parent.bottom;
                    left: parent.left;
                    right: parent.right;
                }

                color: Material.accent;

                height: 1
            }

        }

        add: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 1000 }
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
        id: addButton;

        anchors {
            bottom: parent.bottom;
            right: parent.right;
            margins: 10;
        }

        Material.background: Material.primary;

        width: 80;
        height: 80;

        text: "+";

        onClicked: {
            PlayerFactoryModel.createPlayer();
        }

    }

}
