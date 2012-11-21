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
    Tonalities.cpp \
    GuitarTutorAPI.cpp \
    Tuning.cpp

HEADERS +=\
        GuitarTutorAPI_global.h \
    Tonalities.h \
    GuitarTutorAPI.h \
    Tuning.h

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
