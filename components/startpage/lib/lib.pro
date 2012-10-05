CONFIG += component uitools
SOURCES = StartPage.cpp
HEADERS = StartPage.hpp
wts.top.relative = ../../..
include( $$wts.top.relative/common.pri )

FORMS += \
    startpage.ui

RESOURCES += \
    startpage.qrc


