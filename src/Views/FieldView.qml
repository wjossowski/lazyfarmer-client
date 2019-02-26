import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

import Common 1.0

Item {
    id: root;

    readonly property int baseSize: root.width;

    property var building;
    property var buildingData;
    property var storage;

    readonly property string title: building.name;

    ColumnLayout {
        anchors.fill: parent;

        GridLayout {
            id: field;

            property var model: buildingData.fieldModel;
            property int maxSize: Math.min(root.width, root.height);

            Layout.maximumWidth: maxSize;
            Layout.maximumHeight: maxSize;

            Layout.margins: Stylesheet.bigMargin;
            Layout.alignment: Qt.AlignCenter;

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

                    Rectangle {
                        id: wateringIndicator;

                        property int baseSize: Stylesheet.tinyIconContainerSize;

                        visible: isWatered;
                        color: "green";

                        anchors {
                            top: parent.top;
                            left: parent.left;
                        }

                        NumberAnimation {
                            target: wateringIndicator

                            properties: "height,width"

                            from: wateringIndicator.baseSize * 0.7
                            to: wateringIndicator.baseSize * 1.1

                            duration: 200
                            loops: Animation.Infinite
                            easing.type: Easing.InOutQuad
                        }

                    }

                    BusyIndicator {
                        displayText: false;
                        anchors.fill: parent;
                    }

                }

            }

        }

        RowLayout {
            id: fieldActions

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            Layout.margins: Stylesheet.defaultMargin;
            Layout.rightMargin: Stylesheet.biggerMargin;
            Layout.alignment: Qt.AlignVCenter | Qt.AlignRight;

            Button {
                text: "Edit"
            }

        }

    }

}
