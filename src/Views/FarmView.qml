import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

Item {
    id: root;

    readonly property string title: qsTr("Overview") + t.r;

    function initialize (buildingModel) {
        buildings.model = buildingModel;
    }

    ListView {
        id: buildings;

        anchors {
            fill: parent;

            margins: 20;
        }

        delegate: Item {
            id: building;

            width: parent.width;
            height: 150;

            RowLayout {
                spacing: 10;

                Image {
                    source: "image://resources/buildings/" + id;
                }

                Label {
                    text: name + "(" + level + ")";
                }
            }

        }

    }

}
