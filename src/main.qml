import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import "Views" as Views
import "Dialogs" as Dialogs

ApplicationWindow {
    visible: true

    width: 640
    height: 480

    title: Qt.application.name + ' v. ' + Qt.application.version

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: qsTr("\u25C0 %1").arg(Qt.application.name)
            }
            Item { Layout.fillWidth: true }
            Switch {
                text: qsTr("Notifications")
            }
        }
    }

    StackView {
        id: stack
        anchors.fill: parent
    }

    footer: ToolBar {

    }
}
