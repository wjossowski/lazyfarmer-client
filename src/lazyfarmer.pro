# This file is part of the LazyFarmer project.
# Copyright 2017 Wojciech Ossowski <w.j.ossowski@gmail.com>.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

QT       += core gui widgets network qml quick quickcontrols2
CONFIG   += c++14 rtti

TARGET = LazyFarmer
TEMPLATE = app

VERSION = 0.1.0

# Libs:
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/core/release/ -lCoreLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/core/debug/ -lCoreLib
else:unix: LIBS += -L$$OUT_PWD/../lib/core/ -lCoreLib

INCLUDEPATH += $$PWD/../lib

SOURCES += \
    $$PWD/providers/*.cpp \
    $$PWD/*.cpp

HEADERS += \
    $$PWD/providers/*.h \
    $$PWD/*.h

include($${PWD}/../common.pri)
include($${PWD}/../installs.pri)

DISTFILES += \
    Dialogs/*.qml \
    Views/*.qml \
    Items/*.qml \
    Common/* \
    *.qml

RESOURCES += resources.qrc

QML_IMPORT_PATH += \
    $$PWD/.

OTHER_FILES += \
    qtquickcontrols2.conf

