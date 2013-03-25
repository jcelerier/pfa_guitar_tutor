TEMPLATE = app
TARGET = IGuitarGUI
DESTDIR = ../app/
DEPENDPATH += .

QT += xml core gui widgets webkit webkitwidgets multimedia

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
unix:!macx:LIBS += -lfmodex64-4.44.00
macx: LIBS += -lfmodex
## Partie sur les librairies :
win32 {

#portaudio
LIBS += -L$$PWD/../portaudio/lib/ -lportaudio.dll

INCLUDEPATH += $$PWD/../GuitarTutorAPI/portaudio
DEPENDPATH += $$PWD/../GuitarTutorAPI/portaudio

PRE_TARGETDEPS += $$PWD/../portaudio/lib/libportaudio.dll.a

#ne sert à rien car de toute façon QtCore.dll a besoin de libstd++6.dll, etc..
#QMAKE_LFLAGS = -static-libgcc -static-libstdc++ -static -lwinpthread
}

TRANSLATIONS = IGuitarGUI_fr.ts

RESOURCES +=  \
	PlayerResources.qrc

QMAKE_CXXFLAGS = -O3

win32: RC_FILE = IGuitarGui.ico
