# WTS component test suite

CONFIG += test
SOURCES = timelinewidgets_test.cpp
wts.components = timelinewidgets
wts.top.relative = ../../..
include( $$wts.top.relative/common.pri )



