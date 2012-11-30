# WTS component test suite

CONFIG += test
SOURCES = projecteditor_test.cpp
wts.components = projecteditor
wts.top.relative = ../../..
include( $$wts.top.relative/common.pri )



