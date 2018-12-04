import QtQuick 2.11

ListView {
    id: root;

    Component.onCompleted: {

    }

    delegate: Rectangle {
        height: 20;
        width: parent.width;



        color: "red";
    }
}
