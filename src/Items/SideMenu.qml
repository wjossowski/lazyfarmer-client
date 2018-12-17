import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

Drawer {
    id: sideMenu;
    
    edge: Qt.RightEdge;

    ColumnLayout {
        anchors {
            fill: parent;
            topMargin: 10
        }

        spacing: 10

        Text {
            text: qsTr("Preferences") + t.r;

            Layout.alignment: Qt.AlignHCenter;

            font {
                pixelSize: 20;
                bold: true;
                capitalization: Font.AllUppercase;
            }
        }

        RowLayout {
            Layout.fillWidth: true;

            Layout.alignment: Qt.AlignHCenter;

            anchors.leftMargin: 20;
            anchors.rightMargin: 20;
            spacing: 20;

            Label {
                font.pixelSize: 16;
                text: qsTr("Language") + t.r;
            }

            ComboBox {
                model: t.translations;
                popup.implicitHeight: popup.contentItem.implicitHeight

                property string language: t.language
                function loadSelectedLanguage() { currentIndex = find(language); }

                onLanguageChanged: loadSelectedLanguage()
                Component.onCompleted: loadSelectedLanguage()

                onActivated: {
                    t.setLanguage(currentText)
                }

            }

        }

        // Spacer
        Item {
            Layout.fillHeight: true;
        }

        // Footer
        Text {
            text: Qt.application.name + ' v. ' + Qt.application.version
            Layout.alignment: Qt.AlignHCenter;
        }

        Text {
            text: 'by WololoPL (zavafoj@gmail.com)'
            Layout.alignment: Qt.AlignHCenter;
        }

        Item {
            height: 10;
        }

    }
    
}
