QT       += testlib network

TARGET = CoreTests
CONFIG += console

TEMPLATE = app

SOURCES += *.cpp
HEADERS += *.h

DEFINES += "TEST_PWD=\\\"$${PWD}\\\""
