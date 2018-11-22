import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import "Views"

ApplicationWindow {
    visible: true

    width: 640
    height: 480

    title: Qt.application.name + ' v. ' + Qt.application.version

    header: ToolBar {

    }

    ListView {
        model: PlayerFactoryModel

        delegate: Rectangle {
            width: parent.width;
            height: 60;

            ColumnLayout {
                anchors.fill: parent

                RowLayout {
                    id: info

                    anchors.left: parent.left;
                    anchors.right: parent.right;
                    anchors.top: parent.top;

                    property var parameters: [
                        description, level + ' (' +levelDescription+ ')'
                    ];

                    Repeater {
                        model: parent.parameters;

                        Text {
                            text: modelData;
                        }
                    }

                }

                Text {
                    anchors.top: info.bottom;
                    anchors.left: parent.left;
                    anchors.right: parent.right;

                    text: job
                }
            }


        }

    }

//    StackView {
//        id: stack
//        anchors.fill: parent
//    }

//    LoginView {
//        anchors.fill: parent;
//    }

//    Views.StorageView {
//        anchors {
//            top: parent.top
//            left: parent.left
//            right: parent.right
//        }

//        height: parent.height/2
//    }

//    Views.FarmView {
//        anchors {
//            bottom: parent.bottom
//            left: parent.left
//            right: parent.right
//        }

//        height: parent.height/2
//    }

    footer: ToolBar {

    }
}
