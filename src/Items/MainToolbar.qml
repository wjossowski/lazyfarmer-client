import QtQuick 2.7
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import Common 1.0

ToolBar {
    id: root;

    property var stack;
    readonly property string title: stack.currentItem.title;

    signal menuClicked();

    RowLayout {
        anchors.fill: parent

        ToolButton {
            id: backButton;

            enabled: stack.depth > 1;

            text: enabled ? '<' : '';
            
            onClicked: {
                stack.pop();
            }

        }
        
        Item {
            id: titleText;

            Layout.fillWidth: true;

            Text {
                anchors.centerIn: parent;
                text: title;

                font {
                    pixelSize: Stylesheet.defaultFontSize;
                    capitalization: Font.AllUppercase;
                }
            }

        }
        
        ToolButton {
            id: menuButton;
            text: '\u22ee';
            
            onClicked: {
                menuClicked();
            }

        }

    }

}
