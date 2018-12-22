pragma Singleton

import QtQuick 2.11

Item {
    // Margins:
    readonly property alias tinyMargin:    __sizes.tinySize;
    readonly property alias smallMargin:   __sizes.smallSize;
    readonly property alias smallerMargin: __sizes.smallerSize;
    readonly property alias defaultMargin: __sizes.defaultSize;
    readonly property alias bigMargin:     __sizes.bigSize;

    // Spacings:
    readonly property alias smallSpacing:   __sizes.smallSize;
    readonly property alias defaultSpacing: __sizes.defaultSize;
    readonly property alias bigSpacing:     __sizes.bigSize;

    // Radiuses:
    readonly property alias defaultRadius: __sizes.defaultSize;

    // Font sizes:
    readonly property alias smallerFontSize: __sizes.smallerFontSize;
    readonly property alias smallFontSize:   __sizes.smallFontSize;
    readonly property alias defaultFontSize: __sizes.defaultFontSize;
    readonly property alias bigFontSize:     __sizes.bigFontSize;
    readonly property alias biggerFontSize:  __sizes.biggerFontSize;

    // Image offsets:
    readonly property alias buildingImageOffset:        __sizes.defaultSize;
    readonly property alias buildingImageContainerSize: __sizes.bigIconContainerSize;

    // Widget dimensions:
    readonly property alias defaultWidgetSize:   __sizes.defaultWidgetSize;
    readonly property alias accountListItemSize: __sizes.tinyWidgetSize;
    readonly property alias bigButtonRadius:     __sizes.bigButtonSize;

    // GraphicsProperties:
    readonly property int   defaultBlurRadius: 64;

    QtObject {
        id: __sizes;

        // Sizes:
        readonly property int tinySize:     3;
        readonly property int smallSize:    5;
        readonly property int smallerSize:  7;
        readonly property int defaultSize:  10;
        readonly property int bigSize:      20;

        // Font sizes:
        readonly property int smallerFontSize:  10;
        readonly property int smallFontSize:    11;
        readonly property int defaultFontSize:  16;
        readonly property int bigFontSize:      20;
        readonly property int biggerFontSize:   28;

        // Dimensions:
        readonly property int bigButtonSize:         80;
        readonly property int bigIconContainerSize: 145;
        readonly property int tinyWidgetSize:       100;
        readonly property int defaultWidgetSize:    400;

    }

    QtObject {
        id: __colors;

        // Default colors:
        readonly property color transparentLightGrey: "#0c0c0c0c";
        readonly property color transparentGrey:      "#0c2c2c2c";
        readonly property color lightGrey:            "#0c0c0c";
        readonly property color grey:                 "#2c2c2c";
    }

}
