#Projet: GridEditor

TEMPLATE = app
TARGET = GridEditor
DESTDIR = ../app/
DEPENDPATH += .

SOURCES += \
	CaseItem.cpp \
	ChordTableWidget.cpp \
	GridEditor.cpp \
	EditionSelector.cpp \
	ChordTree.cpp \
	SimpleMusicPlayer.cpp \
	AudioSync.cpp \
	PartSetter.cpp \
	AudioWindow.cpp \
	TrackProperties.cpp \
	NewGridDialog.cpp \
	Waveform.cpp \
	EditorPanel.cpp \
	HelpWindow.cpp \
	ZoomButtons.cpp \
	TimeEdit.cpp \
	Main.cpp \
	TempoEdit.cpp \
	WaveformTimeBar.cpp \
	WaveformTimeSlider.cpp \
	CaseItemDelegate.cpp \
	StatePacket.cpp \
	UndoManager.cpp

HEADERS += \
	CaseItem.h \
	ChordTableWidget.h \
	GridEditor.h \
	EditionSelector.h \
	ChordTree.h \
	SimpleMusicPlayer.h \
	AudioSync.h \
	PartSetter.h \
	AudioWindow.h \
	TrackProperties.h \
	NewGridDialog.h \
	Waveform.h \
	EditorPanel.h \
	HelpWindow.h \
	ZoomButtons.h \
	TimeEdit.h \
	TempoEdit.h \
	WaveformTimeBar.h \
	WaveformTimeSlider.h \
	CaseItemDelegate.h \
	StatePacket.h \
	UndoManager.h

TRANSLATIONS = GridEditor_fr.ts

QT += core \
	  gui \
	  xml \
	  webkit \
	  webkitwidgets \
		  widgets
#         testlib #désactiver pour ne pas avoir la console qui s'affiche


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../libiguitar/ -lIGuitar
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../libiguitar/ -lIGuitar
else:unix: LIBS += -L$$PWD/../libiguitar/ -lIGuitar

INCLUDEPATH += $$PWD/../GuitarTutorAPI $$PWD/../GuitarTutorAPI/GuitarTutor
DEPENDPATH += $$PWD/../libiguitar


macx: LIBS += -lfmodex
unix:!macx: LIBS += -lfmodex64-4.44.00
win32: LIBS += -L$$PWD/../fmod/api/lib/ -lfmodex

FORMS += \
	PartSetter.ui \
	TrackProperties.ui \
	NewGridDialog.ui

RESOURCES += \
	EditorResources.qrc

QMAKE_CXXFLAGS = -O3
#ne sert à rien car de toute façon QtCore.dll a besoin de libstd++6.dll, etc..
#win32:QMAKE_LFLAGS = -static-libgcc -static-libstdc++ -static -lwinpthread

win32: RC_FILE = GridEditor.rc
macx:  ICON = editor.icns
