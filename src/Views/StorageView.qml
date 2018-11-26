import QtQuick 2.11

ListView {
    id: storage;
    model: StorageModel;

    delegate: Rectangle {
        height: 20;
        width: parent.width;

        Text {
            text: name + ' : ' + amount;
            anchors.centerIn: parent;
        }

        color: "red";
    }
}
