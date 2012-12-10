#-------------------------------------------------
#
# Project created by QtCreator 2012-11-21T14:52:46
#
#-------------------------------------------------

QT       -= gui

TARGET = GuitarTutorAPI
TEMPLATE = lib
CONFIG += staticlib

DEFINES += GUITARTUTORAPI_LIBRARY

SOURCES += \
    GuitarTutorAPI.cpp \
    Chord.cpp \
    Tonality.cpp \
    Enrichment.cpp

HEADERS +=\
    GuitarTutorAPI_global.h \
    GuitarTutorAPI.h \
    Chord.h \
    Tonality.h \
    Enrichment.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEEE9CD9C
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = GuitarTutorAPI.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

unix:!symbian|win32: LIBS += -L$$PWD/IGuitar/Release/ -lIGuitar

INCLUDEPATH += $$PWD/IGuitar/Release
DEPENDPATH += $$PWD/IGuitar/Release

win32: PRE_TARGETDEPS += $$PWD/IGuitar/Release/IGuitar.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/IGuitar/Release/libIGuitar.a
