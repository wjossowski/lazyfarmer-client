import QtQuick 2.7
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "Views"

ApplicationWindow {
    id: root;
    visible: true

    width: 640
    height: 480

    Material.theme: Material.Light;
    Material.primary: Material.color(Material.Green);
    Material.accent: Material.color(Material.LightGreen);

    title: Qt.application.name + ' v. ' + Qt.application.version

    AccountsView {
        anchors.fill: parent;
    }

    Text {
        anchors.bottom: parent.bottom;
        anchors.left: parent.left;
        anchors.margins: 10;

        text: root.width + ' x ' + root.height;
    }

}
