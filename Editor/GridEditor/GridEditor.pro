#Projet: GridEditor

TEMPLATE = app
TARGET =
DEPENDPATH += .


SOURCES += \
    CaseItem.cpp \
    ChordTableWidget.cpp \
    GridEditor.cpp \
    TestMain.cpp \
    EditionSelector.cpp \
    ChordTree.cpp \
    SimpleMusicPlayer.cpp \
    AudioSync.cpp \
    PartSetter.cpp \
    AudioWindow.cpp \
    TrackProperties.cpp \
    NewGridDialog.cpp \
    Waveform.cpp

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
    Waveform.h

TRANSLATIONS = GridEditor_fr.ts

QT += core \
      gui \
      xml


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../libiguitar/ -lIGuitar
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../libiguitar/ -lIGuitar
else:unix: LIBS += -L$$PWD/../../libiguitar/ -lIGuitar

INCLUDEPATH += $$PWD/../../GuitarTutorAPI $$PWD/../../GuitarTutorAPI/GuitarTutor
DEPENDPATH += $$PWD/../../libiguitar


macx: LIBS += -lfmodex
unix:!macx: LIBS += -lfmodex64-4.44.00
win32: LIBS += -L$$PWD/../../fmod/api/lib/ -lfmodex

FORMS += \
    PartSetter.ui \
    TrackProperties.ui \
    NewGridDialog.ui
