# WTS component test suite

CONFIG += test
SOURCES = %ComponentName:l%_test.cpp
wts.components = %ComponentName:l%
wts.top.relative = ../../..
include( $$wts.top.relative/common.pri )


