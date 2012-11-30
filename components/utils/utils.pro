TEMPLATE = subdirs
SUBDIRS = lib test
CONFIG += ordered
HEADERS += Stub GuiTestingBase Macros DialogSpecHelper
lib.file = lib/libutils.pro
test.file = test/utils_test.pro
