import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

Dialog {
    id: root;
    title: qsTr("Account Settings") + t.r;
    
    property var currentAccount;

    property alias login: loginField.text;
    property alias password: passwordField.text;
    property alias domain: domainField.currentText;
    property alias server: serverField.text;

    width: 350;
    
    function setUp (account) {
        currentAccount = account;

        login = currentAccount.login();
        password = "";

        var domainIndex = domainField.model.indexOf(account.domain())
        domainField.currentIndex = (domainIndex >= 0) ? domainIndex : 0;

        server = currentAccount.server();

        loginField.forceActiveFocus();
    }

    onAccepted: {
        if (!currentAccount) {
            return;
        }

        currentAccount.setApiOptions(domain, server, login, password);
    }

    standardButtons: Dialog.Ok

    contentItem: Item {
        id: contents;

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

                Keys.onReturnPressed: accept()
            }

            TextField {
                id: passwordField;

                Layout.fillWidth: true;
                placeholderText: qsTr("Password") + t.r;

                echoMode: TextInput.Password;

                Keys.onReturnPressed: accept()
            }


            TextField {
                id: serverField;

                Layout.fillWidth: true;
                placeholderText: qsTr("Server") + t.r;

                Keys.onReturnPressed: accept()

                validator: IntValidator {
                    bottom: 1
                    top: 100
                }

            }

            ComboBox {
                id: domainField;

                Layout.fillWidth: true;

                model: AvailableDomains;
                Keys.onReturnPressed: accept()
            }

        }

    }

}
