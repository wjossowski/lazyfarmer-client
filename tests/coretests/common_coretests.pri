# Qt modules used within this test module
QT += network

# Libs:
win32:CONFIG(release, debug|release): LIBS += -L$${OUT_PWD}/../../../lib/core/release/ -lCoreLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$${OUT_PWD}/../../../lib/core/debug/ -lCoreLib
else:unix: LIBS += -L$${OUT_PWD}/../../../lib/core/ -lCoreLib

INCLUDEPATH += $${PWD}/../../lib
DEPENDPATH += $${PWD}/../../lib/core

# Base:
include($${PWD}/../common_test.pri)
