import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

import Common 1.0

Drawer {
    id: sideMenu;
    
    edge: Qt.RightEdge;

    ColumnLayout {
        anchors {
            fill: parent;
            topMargin: Stylesheet.defaultMargin;
        }

        spacing: Stylesheet.defaultSpacing;

        Text {
            text: qsTr("Preferences") + t.r;

            Layout.alignment: Qt.AlignHCenter;

            font {
                pixelSize: Stylesheet.bigFontSize;
                bold: true;
                capitalization: Font.AllUppercase;
            }
        }

        RowLayout {
            Layout.fillWidth: true;

            Layout.alignment: Qt.AlignHCenter;

            Layout.leftMargin: Stylesheet.bigMargin;
            Layout.rightMargin: Stylesheet.bigMargin;
            spacing: Stylesheet.bigSpacing;

            Label {
                font.pixelSize: Stylesheet.defaultFontSize;
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
            height: Stylesheet.defaultMargin;
        }

    }
    
}
