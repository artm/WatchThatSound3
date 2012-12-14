SOURCES += \
    main.cpp \
    BioscopeTestSuite.cpp \
    PlayerShell.cpp

HEADERS  += \
    PlayerShell.hpp \
    BioscopeTestSuite.hpp

FORMS += \
    player.ui

RESOURCES += \
    player.qrc

CONFIG += test uitools
wts.components = bioscope
wts.top.relative = ../../..
include( $$wts.top.relative/common.pri )
