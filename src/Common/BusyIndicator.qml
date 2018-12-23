import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

import Common 1.0

ColumnLayout {
    id: root;
    
    property var doneDate: null;
    property int totalInterval: NaN;
    
    property double percentDone: 0.0
    property int timeLeft: 0;

    property bool isInProgress: Number.isFinite(root.totalInterval) && root.timeLeft >= 0;
    
    Timer {
        id: workTimer;
        
        interval: 1000
        repeat: true;

        running: root.isInProgress

        function updateProgress () {
            if (!root.isInProgress) {
                return;
            }

            root.timeLeft = Utils.TimeUtils.calculateTimeLeft(root.doneDate)
            root.percentDone = Utils.TimeUtils.calculateJobPercentage(root.timeLeft, root.totalInterval);
        }
        
        onTriggered: updateProgress();
        Component.onCompleted: updateProgress();
        
    }
    
    ProgressBar {
        id: workProgressBar;
        
        visible: root.isInProgress;
        value: root.percentDone;
        
        Layout.fillWidth: true;
    }
    
    Item {
        id: workInfoGroup
        
        Layout.topMargin: Stylesheet.smallMargin;
        Layout.fillWidth: true;
        
        Label {
            id: workTimeoutLabel;
            
            visible: root.isInProgress
            anchors {
                verticalCenter: parent.verticalCenter;
                left: parent.left;
            }
            
            font.pixelSize: Stylesheet.smallFontSize;
            text: qsTr("Ready In: ") + Utils.TimeUtils.timeLeftToString(root.timeLeft) + t.r;
            
        }
        
        Label {
            id: workPercentIndicatorLabel;
            
            anchors {
                verticalCenter: parent.verticalCenter;
                right: parent.right;
            }
            
            font.pixelSize: Stylesheet.smallFontSize;
            text: (function (percent) {
                if (root.isInProgress) {
                    return qsTr("Done at: ") + " " + (100 * percent).toFixed(2) + "%";
                } else {
                    return qsTr("Idle")
                }
            }) (root.percentDone, t.r)
            
        }
        
    }
    
}
