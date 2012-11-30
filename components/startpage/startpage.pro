TEMPLATE = subdirs
SUBDIRS = lib test
CONFIG += ordered
HEADERS += StartPage
lib.file = lib/libstartpage.pro
test.file = test/startpage_test.pro
