#-------------------------------------------------
#
# Project created by QtCreator 2013-01-04T14:42:10
#
#-------------------------------------------------

QT       += xml testlib

QT       -= gui

TARGET = APITest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG  += debug

TEMPLATE = app


SOURCES += APITest.cpp \
    ../Track/TrackLoader.cpp \
    ../Track/LogicalTrack.cpp \
    ../Track/PartTrack.cpp \
    ../Track/TrackChord.cpp \
    ../GuitarTutor/Chord.cpp \
    ../GuitarTutor/Enrichment.cpp \
    ../GuitarTutor/Tonality.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    TestChord.h \
    ../Track/TrackLoader.h \
    ../Track/LogicalTrack.h \
    ../Track/TrackChord.h \
    ../Track/PartTrack.h \
    ../GuitarTutor/Chord.h \
    ../GuitarTutor/Enrichment.h \
    ../GuitarTutor/Tonality.h

OTHER_FILES += \
    songExample.xml
