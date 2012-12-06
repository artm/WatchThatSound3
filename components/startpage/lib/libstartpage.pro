CONFIG += component
SOURCES = StartPage.cpp
HEADERS = StartPage.hpp
wts.top.relative = ../../..
wts.components += utils
include( $$wts.top.relative/common.pri )

FORMS += \
    startpage.ui \
    new_project_dialog.ui

RESOURCES += \
    startpage.qrc



