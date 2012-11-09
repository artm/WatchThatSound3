CONFIG += main_exe
SOURCES = main.cpp \
    TestWtsIntegration.cpp
wts.components = startpage utils customwidgets
wts.top.relative = ../..
include( $$wts.top.relative/common.pri )

RESOURCES += \
    WatchThatSound.qrc

HEADERS += \
    TestWtsIntegration.hpp



