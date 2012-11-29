CONFIG += test
SOURCES = startpage_test.cpp
wts.components = startpage utils
wts.top.relative = ../../..
include( $$wts.top.relative/common.pri )
