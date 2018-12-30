import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

import Common 1.0

Rectangle {
    id: root;

    property bool isSelected: false;

    anchors.margins: Stylesheet.defaultMargin;

    radius: Stylesheet.defaultRadius;

    border.color: "#0c0c0c";
    color: isSelected ? "#7c2c8c2c" : "#0c2c2c2c";

}
