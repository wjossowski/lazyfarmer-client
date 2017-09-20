QT       += testlib network
CONFIG   += c++14

TARGET = CoreTests
CONFIG += console
TEMPLATE = app

# Libs:
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib/core/release/ -lCoreLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib/core/debug/ -lCoreLib
else:unix: LIBS += -L$$OUT_PWD/../../lib/core/ -lCoreLib

INCLUDEPATH += $$PWD/../../lib/core $$PWD/../../lib/core/helpers
DEPENDPATH += $$PWD/../../lib/core

SOURCES += *.cpp
HEADERS += *.h

include($${PWD}/../../common.pri)
include($${PWD}/../common_test.pri)
