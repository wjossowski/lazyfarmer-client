import QtQuick 2.11

ListView {
    id: storage;
    model: BuildingModel;

    delegate: Rectangle {
        height: 20;
        width: parent.width;

        Text {
            text: name + '(level ' + level + ') @(' + farm + ', ' + position + ')' ;
            anchors.centerIn: parent;
        }

        color: "red";
    }
}
