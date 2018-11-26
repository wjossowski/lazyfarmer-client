import QtQuick 2.7
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

ToolBar {
    id: root;

    property var stack;
    property string title;

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
                    pixelSize: 16;
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
