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

TEMPLATE = subdirs

corelib.subdir =$${PWD}/lib/core
corelib.target = CoreLib

source.file = $${PWD}/src/lazyfarmer.pro
source.target = LazyFarmer
source.depends = corelib

SUBDIRS += corelib source

CONFIG(debug, debug|release) {
    unit_tests.subdir = $${PWD}/tests
    unit_tests.target = UnitTests
    unit_tests.depends = corelib source

    SUBDIRS += unit_tests
}
