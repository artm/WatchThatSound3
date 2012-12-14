# WTS component:
CONFIG += component
SOURCES = \
    SlidingStackedWidget.cpp \
    TristateCompoundInterface.cpp
HEADERS = \
    SlidingStackedWidget.hpp \
    TristateCompoundInterface.hpp
wts.top.relative = ../../..
include( $$wts.top.relative/common.pri )



