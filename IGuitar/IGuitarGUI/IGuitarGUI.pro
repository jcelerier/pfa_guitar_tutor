TEMPLATE = app
TARGET = IGuitarGUI
DESTDIR = ../../app/
DEPENDPATH += .

QT += xml core gui testlib widgets

win32 {
DEPENDPATH += . \
			  ../../libsndfile/lib \
			  ../../boost/lib \
			  ../../portaudio/lib

INCLUDEPATH += . \
			   ../../libsndfile/include \
			   ../../boost/include


}

# Input
HEADERS += \
	Configuration.h \
	PlayerScene.h \
	Controler.hpp \
    PlayerChord.h \
    MyView.h \
    MenuItem.h \
    EntireSong.h \
    ButtonItem.h \
    ChordDictionary.h

SOURCES += main.cpp \
	Configuration.cpp \
	PlayerScene.cpp \
	Controler.cpp \
    EntireSong.cpp \
    MenuItem.cpp \
    MyView.cpp \
    PlayerChord.cpp \
    ButtonItem.cpp \
    ChordDictionary.cpp

FORMS += \
    ChordDictionary.ui


unix:!symbian|win32: LIBS += -L$$PWD/../../libiguitar/ -lIGuitar

INCLUDEPATH += $$PWD/../../GuitarTutorAPI
DEPENDPATH += $$PWD/../../libiguitar

#win32: PRE_TARGETDEPS += $$PWD/../../libiguitar/IGuitar.lib
#else:unix:
PRE_TARGETDEPS += $$PWD/../../libiguitar/libIGuitar.a

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
LIBS += -L$$PWD/../../boost/lib/ -lboost_thread-mgw47-mt-1_53 -lboost_system-mgw47-mt-1_53

INCLUDEPATH += $$PWD/../../boost/include
DEPENDPATH += $$PWD/../../boost/include

PRE_TARGETDEPS += $$PWD/../../boost/lib/libboost_thread-mgw47-mt-1_53.dll.a $$PWD/../../boost/lib/libboost_system-mgw47-mt-1_53.dll.a


#portaudio
LIBS += -L$$PWD/../../portaudio/lib/ -lportaudio

INCLUDEPATH += $$PWD/../../GuitarTutorAPI/portaudio
DEPENDPATH += $$PWD/../../GuitarTutorAPI/portaudio

PRE_TARGETDEPS += $$PWD/../../portaudio/lib/libportaudio.dll.a
}

TRANSLATIONS = IGuitarGUI_fr.ts

#unix: QMAKE_POST_LINK += mv IGuitarGUI ../../app/
#win32: QMAKE_POST_LINK += mv IGuitarGUI.exe ../../app/
#macx: QMAKE_POST_LINK += mv IGuitarGUI.app ../../app/

RESOURCES += \
    PlayerResources.qrc
