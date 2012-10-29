CONFIG += test
SOURCES = \
    utils_test.cpp
wts.components = utils
wts.top.relative = ../../..
include( $$wts.top.relative/common.pri )



