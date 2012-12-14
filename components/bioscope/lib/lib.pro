SOURCES += \
    Bioscope.cpp \
    BioscopeDriver.cpp \
    BioscopeThread.cpp

HEADERS  += \
    Bioscope.hpp \
    BioscopeDriver.hpp \
    BioscopeThread.hpp

# ffmpeg support ...
DEFINES += WITH_FFMPEG __STDC_CONSTANT_MACROS
QMAKE_CXXFLAGS +=  -Wno-deprecated -Wno-deprecated-declarations # -Wno-attributes
QMAKE_LFLAGS = -w
# link against ffmpeg
LIBS += -lavformat -lavcodec -lavutil -lswscale
# and ffmpeg's dependencies?
#LIBS += -lm
#win32:LIBS += -lpthreadGC2 -lpsapi
#macx:LIBS += -Wl,-framework,Cocoa -lz

CONFIG += component
wts.top.relative = ../../..
include( $$wts.top.relative/common.pri )
