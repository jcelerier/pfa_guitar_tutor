TEMPLATE = app
TARGET = IGuitarGUI
DESTDIR = ../app/
DEPENDPATH += .

QT += xml core gui widgets webkit webkitwidgets testlib multimedia

win32 {
DEPENDPATH += . \
			  ../portaudio/lib

INCLUDEPATH += . \


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
	ChordDictionary.h \
	HelpWindow.h \
        SongManager.h \
        EntireSongBis.h \
    ConfigPanel.h \
    ScrollingItem.h

SOURCES += main.cpp \
	Configuration.cpp \
	PlayerScene.cpp \
	Controler.cpp \
        EntireSong.cpp \
	MenuItem.cpp \
	MyView.cpp \
	PlayerChord.cpp \
	ButtonItem.cpp \
	ChordDictionary.cpp \
	HelpWindow.cpp \
        SongManager.cpp \
    ConfigPanel.cpp \
    EntireSongBis.cpp \
    ScrollingItem.cpp

FORMS += \
        ChordDictionary.ui \
    ConfigPanel.ui


unix:!symbian|win32: LIBS += -L$$PWD/../libiguitar/ -lIGuitar

INCLUDEPATH += $$PWD/../GuitarTutorAPI
DEPENDPATH += $$PWD/../libiguitar

#else:unix:
PRE_TARGETDEPS += $$PWD/../libiguitar/libIGuitar.a

macx: QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
macx: QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64

unix|macx: LIBS += -lportaudio
win32:LIBS += -L$$PWD/../fmod/api/lib/ -lfmodex
unix:LIBS += -lfmodex64-4.44.00
## Partie sur les librairies :
win32 {

#portaudio
LIBS += -L$$PWD/../portaudio/lib/ -lportaudio

INCLUDEPATH += $$PWD/../GuitarTutorAPI/portaudio
DEPENDPATH += $$PWD/../GuitarTutorAPI/portaudio

PRE_TARGETDEPS += $$PWD/../portaudio/lib/libportaudio.dll.a
}

TRANSLATIONS = IGuitarGUI_fr.ts

RESOURCES +=  \
	PlayerResources.qrc
