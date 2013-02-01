# WTS component:
CONFIG += component
SOURCES = TimeLineWidget.cpp \
    TimeLineController.cpp \
    EditController.cpp \
    WtsAudio.cpp \
    Synced.cpp \
    SoundBuffer.cpp \
    SharpLine.cpp \
    Project.cpp \
    BufferItem.cpp \
    Rainbow.cpp \
    ScoreSymbol.cpp \
    ScoreEditor.cpp \
    StoryBoard.cpp \
    TimeLineItem.cpp \
    CurveEditor.cpp \
    SequencerTimeLine.cpp \
    WaveformWidget.cpp \
    SampleNameValidator.cpp
HEADERS = TimeLineWidget.hpp \
    TimeLineController.hpp \
    EditController.hpp \
    WtsAudio.hpp \
    Synced.hpp \
    SoundBuffer.hpp \
    SharpLine.hpp \
    Project.hpp \
    BufferItem.hpp \
    Rainbow.hpp \
    ScoreSymbol.hpp \
    ScoreEditor.hpp \
    ThumbsSpread.hpp \
    Common.hpp \
    StoryBoard.hpp \
    TimeLineItem.hpp \
    CurveEditor.hpp \
    SequencerTimeLine.hpp \
    WaveformWidget.hpp \
    SampleNameValidator.hpp
wts.components += bioscope
wts.top.relative = ../../..
include( $$wts.top.relative/common.pri )

LIBS += -lportaudio -lsndfile
