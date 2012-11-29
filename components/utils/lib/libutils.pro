CONFIG += component
SOURCES = \
    Stub.cpp \
    GuiTestingBase.cpp
HEADERS = \
    Stub.hpp \
    GuiTestingBase.hpp \
    Macros.hpp \
    DialogSpecHelper.hpp
wts.top.relative = ../../..
include( $$wts.top.relative/common.pri )
