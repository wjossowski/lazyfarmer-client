import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

Item {
    id: root;

    property alias login: loginField.text;
    property alias password: passwordField.text;
    property alias domain: domainField.text;
    property alias server: serverField.text;

    signal submitClicked(string login, string password, string domain, string server);

    anchors.margins: 10;

    ColumnLayout {
        anchors{
            fill: parent;
            margins: 3
        }

        spacing: 5;

        TextField {
            id: loginField;

            Layout.fillWidth: true;
            placeholderText: qsTr("Username") + t.r;
        }

        TextField {
            id: passwordField;

            Layout.fillWidth: true;
            placeholderText: qsTr("Password") + t.r;

            echoMode: TextInput.Password;
        }

        TextField {
            id: domainField;

            Layout.fillWidth: true;
            placeholderText: qsTr("Domain") + t.r;
        }

        TextField {
            id: serverField;

            Layout.fillWidth: true;
            placeholderText: qsTr("Server") + t.r;

            validator: IntValidator {
                bottom: 1
                top: 100
            }
        }

        Button {
            id: submitButton

            Layout.fillWidth: true;
            text: qsTr("Login") + t.r;

            onClicked: {
                submitClicked(login, password, domain, server);
            }
        }

    }
}
