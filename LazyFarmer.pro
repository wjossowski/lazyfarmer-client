TEMPLATE = subdirs

source.subdir = src
source.target = LazyFarmer

unit_tests.subdir = tests
unit_tests.target = LazyFarmerTests
unit_tests.depends = source

SUBDIRS = source unit_tests
