QT       += core network
CONFIG   += c++14 dll

TARGET = CoreLib
TEMPLATE = lib

INCLUDEPATH += $${PWD}/helpers

SOURCES += \
        $${PWD}/helpers/*.cpp \
        $${PWD}/fields/*.cpp \
        $${PWD}/*.cpp

HEADERS += \
        $${PWD}/helpers/*.h \
        $${PWD}/fields/*.h \
        $${PWD}/*.h

include($${PWD}/../../common.pri)
