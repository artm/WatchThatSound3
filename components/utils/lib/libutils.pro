CONFIG += component uitools
SOURCES = \
    Stub.cpp \
    GuiTestingBase.cpp \
    WidgetUtils.cpp
HEADERS = \
    Stub.hpp \
    GuiTestingBase.hpp \
    Macros.hpp \
    DialogSpecHelper.hpp \
    Widgetutils.hpp
wts.top.relative = ../../..
include( $$wts.top.relative/common.pri )
