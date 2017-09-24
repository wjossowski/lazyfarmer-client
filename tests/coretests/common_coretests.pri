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
