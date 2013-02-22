TEMPLATE = app
TARGET =
DEPENDPATH += .
QT += xml core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32 {
DEPENDPATH += . \
              ../../libsndfile/lib \
              ../../boost/lib \
              ../../portaudio/lib

INCLUDEPATH += . \
               ../../libsndfile/include \
               ../../boost/include

QMAKE_CXXFLAGS += -fexceptions
CONFIG += rtti
}

# Input
HEADERS += RenderArea.hpp \
    audioconfiguration.h \
    SongSelection.hpp \
    Configuration.h \
    PlayerScene.h \
    entiresong.h \
    myView.h \
    menuitem.h \
    buttonitem.h \
    playerchord.h \
    Controler.hpp

SOURCES += main.cpp RenderArea.cpp \
    audioconfiguration.cpp \
    SongSelection.cpp \
    Configuration.cpp \
    PlayerScene.cpp \
    entiresong.cpp \
    myView.cpp \
    menuitem.cpp \
    buttonitem.cpp \
    playerchord.cpp \
    Controler.cpp

FORMS += \
    audioconfiguration.ui \
    SongSelection.ui


unix:!symbian|win32: LIBS += -L$$PWD/../../libiguitar/ -lIGuitar

INCLUDEPATH += $$PWD/../../GuitarTutorAPI
DEPENDPATH += $$PWD/../../libiguitar

win32: PRE_TARGETDEPS += $$PWD/../../libiguitar/IGuitar.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../libiguitar/libIGuitar.a

macx: QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
macx: QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64

unix|macx: LIBS += -lsndfile -lportaudio -lboost_thread-mt
macx: LIBS += -lboost_system-mt

## Partie sur les librairies :
win32 {
#libsndfile
unix:!symbian|win32: LIBS += -L$$PWD/../../libsndfile/lib -lsndfile

INCLUDEPATH += $$PWD/../../libsndfile/include
DEPENDPATH += $$PWD/../../libsndfile/lib

PRE_TARGETDEPS += $$PWD/../../libsndfile/lib/libsndfile.a



#boost::thread
LIBS += -L$$PWD/../../boost/lib/ -lboost_thread_win32

INCLUDEPATH += $$PWD/../../boost/include
DEPENDPATH += $$PWD/../../boost/include

PRE_TARGETDEPS += $$PWD/../../boost/lib/libboost_thread_win32.dll.a


#portaudio
LIBS += -L$$PWD/../../portaudio/lib/ -lportaudio

INCLUDEPATH += $$PWD/../../GuitarTutorAPI/portaudio
DEPENDPATH += $$PWD/../../GuitarTutorAPI/portaudio

PRE_TARGETDEPS += $$PWD/../../portaudio/lib/libportaudio.a
}

unix: QMAKE_POST_LINK += mv IGuitarGUI ../../app/
win32: QMAKE_POST_LINK += mv IGuitarGUI.exe ../../app/
macx: QMAKE_POST_LINK += mv IGuitarGUI.app ../../app/

OTHER_FILES += \
    ressources/transport.png \
    ressources/testmask.png \
    ressources/menu-btn.png \
    ressources/bgwide.png \
    ressources/background.png \
    ../../app/transport.png \
    ../../app/noalbum.png \
    ../../app/minibarretemps.png \
    ../../app/menu.png \
    ../../app/maskplaying.png \
    ../../app/chordbar.png \
    ../../app/btn_bg.png \
    ../../app/bgwide.png \
    ../../app/barretemps.png \
    ../../app/album.png
