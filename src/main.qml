import QtQuick 2.7
import QtQuick.Controls 2.0

import "Views" 1.0 as Views
import "Dialogs" 1.0 as Dialogs

ApplicationWindow {
    visible: true
    width: 640
    height: 480

    Views.AccountsView {
        anchors.centerIn: parent
    }

}
