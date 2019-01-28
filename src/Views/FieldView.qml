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
                id: fieldDelegate

                visible: isMainFieldBlock;

                radius: 2;

                Layout.fillWidth: true;
                Layout.fillHeight: true;

                Layout.columnSpan: columnSpan;
                Layout.rowSpan: rowSpan;

                color: Stylesheet.placeholderColor;

                Image {
                    visible: isNotEmpty;

                    anchors.centerIn: parent
                    source: "image://resources/products/" + id;

                    scale: fieldDelegate.height / Stylesheet.smallIconContainerSize
                }

                Image {
                    visible: isWatered;

                    anchors {
                        bottom: parent.bottom;
                        left: parent.left;
                        margins: Stylesheet.tinyMargin
                    }

                    source: "image://resources/watered-indicator/1";
                }

                BusyIndicator {

                    displayText: false;

                    anchors.fill: parent;
                }

            }
        }

    }

}
