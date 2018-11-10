import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

// Login, Password, Server, Domain

Rectangle {
    id: root;

    ColumnLayout {
        anchors{
            fill: parent;
            margins: 3
        }

        spacing: 5;

        TextField {
            id: login;

            Layout.fillWidth: true;
            placeholderText: qsTr("Username") + t.r;
        }

        TextField {
            id: password;

            Layout.fillWidth: true;
            placeholderText: qsTr("Password") + t.r;

            echoMode: TextInput.Password;
        }

        TextField {
            id: domain;

            Layout.fillWidth: true;
            placeholderText: qsTr("Domain") + t.r;
        }

        TextField {
            id: server;

            Layout.fillWidth: true;
            placeholderText: qsTr("Server") + t.r;

            validator: IntValidator {
                bottom: 1
                top: 100
            }
        }

        Button {
            id: submit

            Layout.fillWidth: true;
            text: qsTr("Login") + t.r;
        }

    }
}
