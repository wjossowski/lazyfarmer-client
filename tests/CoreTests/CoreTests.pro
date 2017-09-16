QT       += testlib network

TARGET = CoreTests
CONFIG += console

TEMPLATE = app

INCLUDEPATH = \
        $${PWD}/../../src \
        $${PWD}/../../src/core \
        $${PWD}/../../src/helpers

SOURCES += \
        $${PWD}/../../src/helpers/*.cpp \
        $${PWD}/../../src/core/*.cpp  \
        *.cpp

HEADERS += \
        $${PWD}/../../src/helpers/*.h \
        $${PWD}/../../src/core/*.h \
        *.h

DEFINES += "TEST_PWD=\\\"$${PWD}\\\""
DEFINES += "TEST_OUT_PWD=\\\"$${OUT_PWD}\\\""

include($${PWD}/../../common.pri)
