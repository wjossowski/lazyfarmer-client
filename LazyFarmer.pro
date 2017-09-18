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
