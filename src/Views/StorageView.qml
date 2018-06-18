import QtQuick 2.0

ListView {
    id: storage;
    model: StorageModel;

    delegate: Rectangle {
        height: 20
        width: parent.width

        Text {
            text: "Item: " + id + ", amount: " + amount
            anchors.centerIn: parent
        }

        color: Qt.red;
    }
}
