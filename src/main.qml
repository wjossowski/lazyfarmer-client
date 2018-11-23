import QtQuick 2.7
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "Views"

ApplicationWindow {
    visible: true

    width: 640
    height: 480

    Material.theme: Material.Green;

    title: Qt.application.name + ' v. ' + Qt.application.version

    header: ToolBar {
        RowLayout {
            anchors.fill: parent;

            spacing: 10;

            ToolButton {
                text: "<"

                onClicked: {
                    stack.pop()
                }
            }

        }
    }

    StackView {
        id: stack;

        Component.onCompleted: {
            stack.push()
        }
    }

    AccountsView {
        anchors.fill: parent;
    }

    footer: ToolBar {

    }

}
