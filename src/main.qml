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

//    StackView {
//        id: stack
//        anchors.fill: parent
//    }

    LoginView {
        anchors.fill: parent;
    }

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
