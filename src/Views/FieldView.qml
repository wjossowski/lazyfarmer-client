import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

import Common 1.0

Item {
    id: root;

    property var building;
    property var buildingData;
    property var storage;

    readonly property string title: building.name;

    GridLayout {
        id: field;

        property var model: buildingData.fieldModel;
        property int baseSize: 400;

        anchors {
            top: parent.top;
            horizontalCenter: parent.horizontalCenter;
            margins: Stylesheet.defaultMargin;
        }

        width:  baseSize;
        height: (field.rows/field.columns) * baseSize;

        Component.onCompleted: function () {
            field.rows = field.model.maxRows();
            field.columns = field.model.maxColumns();
        }

        Repeater {
            model: field.model;

            delegate: Rectangle {
                id: fieldMiniature
                radius: 2;

                Layout.fillWidth: true;
                Layout.fillHeight: true;

                Layout.columnSpan: [2, 4].indexOf(size) > -1 ? 2 : 1;
                Layout.rowSpan: size === 4 ? 2 : 1;

                visible: id !== -1;

                color: Stylesheet.placeholderColor;

                Image {
                    visible: id !== 0;

                    anchors.centerIn: parent
                    source: "image://resources/products/" + id;

                    scale: fieldMiniature.height / Stylesheet.smallIconContainerSize
                }
            }
        }

    }

}
