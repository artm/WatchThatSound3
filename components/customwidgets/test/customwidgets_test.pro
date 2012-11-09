# WTS component test suite

CONFIG += test
SOURCES = customwidgets_test.cpp
wts.components = customwidgets
wts.top.relative = ../../..
include( $$wts.top.relative/common.pri )



