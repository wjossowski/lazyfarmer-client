# Common Qt module config for unit tests
QT += testlib
QT -= gui

# Common qmake config
CONFIG += c++14
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

# Common defines
DEFINES += "TEST_PWD=\\\"$${PWD}\\\""
DEFINES += "TEST_OUT_PWD=\\\"$${OUT_PWD}\\\""
