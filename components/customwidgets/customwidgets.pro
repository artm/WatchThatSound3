TEMPLATE = subdirs
SUBDIRS = lib test
CONFIG += ordered
HEADERS += SlidingStackedWidget
lib.file = lib/libcustomwidgets.pro
test.file = test/customwidgets_test.pro
