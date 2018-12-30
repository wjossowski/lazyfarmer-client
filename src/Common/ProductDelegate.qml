import QtQuick 2.11
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

import Common 1.0

ProductInfoContainer {
    id: root;
    
    property int productId: -1;
    property var productsStorage: QtObject {
        readonly property int amount: 0
    }

    width: delegateStatusLabel.width + 2*Stylesheet.bigMargin;
    height: Stylesheet.tinyWidgetSize;
    
    Image {
        anchors.centerIn: parent;
        source: "image://resources/products/" + productId;
    }
    
    Label {
        id: delegateStatusLabel
        
        anchors {
            bottom: parent.bottom;
            horizontalCenter: parent.horizontalCenter;
            margins: Stylesheet.tinyMargin;
        }
        
        text: productsStorage.productName(productId) + " (" + productsStorage.amount(productId) + ")"
    }
    
}
