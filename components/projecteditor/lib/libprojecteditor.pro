# WTS component:
CONFIG += component
SOURCES = \
    ProjectEditor.cpp
HEADERS = \
    ProjectEditor.hpp
wts.top.relative = ../../..
wts.components += utils
include( $$wts.top.relative/common.pri )

FORMS += \
    ProjectEditor.ui \
    storyboard.ui \
    tension.ui \
    score.ui \
    record.ui

RESOURCES += \
    ProjectEditor.qrc

