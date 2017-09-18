QT       += core gui widgets network
CONFIG   += c++14

TARGET = LazyFarmer
TEMPLATE = app

VERSION = 0.1.0.0

# Libs:
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/core/release/ -lCoreLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/core/debug/ -lCoreLib
else:unix: LIBS += -L$$OUT_PWD/../lib/core/ -lCoreLib

INCLUDEPATH += $$PWD/../lib/core
DEPENDPATH += $$PWD/../lib/core

SOURCES += *.cpp
HEADERS += *.h
FORMS += *.ui

include($${PWD}/../common.pri)
include($${PWD}/../installs.pri)
