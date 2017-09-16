QT       += core gui widgets network xml
CONFIG   += c++14

TARGET = LazyFarmer
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
VERSION = 0.1.0.0

win32 {
    QMAKE_TARGET_COMPANY = $${COMPANY_NAME}
}

INCLUDEPATH += \
        $${PWD}/core \
        $${PWD}/widgets

SOURCES += \
        $${PWD}/core/*.cpp \
        $${PWD}/widgets/*.cpp \
        main.cpp

HEADERS += \
        $${PWD}/core/*.h \
        $${PWD}/widgets/*.h

FORMS += \
        $${PWD}/widgets/*.ui

CONFIG(debug, debug|release) {
    INCLUDEPATH += $${PWD}/helpers

    SOURCES += \
              $${PWD}/helpers/*.cpp
    HEADERS += \
              $${PWD}/helpers/*.h

}

include($${PWD}/../common.pri)
include($${PWD}/installs.pri)
