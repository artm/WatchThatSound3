CONFIG += main_exe
SOURCES = main.cpp \
    TestWtsIntegration.cpp \
    WtsShell.cpp \
    TestRunner.cpp
wts.components = startpage utils customwidgets projecteditor
wts.top.relative = ../..
include( $$wts.top.relative/common.pri )

RESOURCES += \
    WatchThatSound.qrc

HEADERS += \
    TestWtsIntegration.hpp \
    WtsShell.hpp \
    TestRunner.hpp

OTHER_FILES += application.css
