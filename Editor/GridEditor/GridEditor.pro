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
    PartSetter.cpp

HEADERS += \
    CaseItem.h \
    ChordTableWidget.h \
    GridEditor.h \
    EditionSelector.h \
    ChordTree.h \
    SimpleMusicPlayer.h \
    AudioSync.h \
    PartSetter.h

TRANSLATIONS = GridEditor_fr.ts

QT += core \
      gui \
      xml


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../GuitarTutorAPI/release/ -lIGuitar
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../GuitarTutorAPI/debug/ -lIGuitar
else:unix: LIBS += -L$$PWD/../../libiguitar/ -lIGuitar

INCLUDEPATH += $$PWD/../../GuitarTutorAPI $$PWD/../../GuitarTutorAPI/GuitarTutor
DEPENDPATH += $$PWD/../../libiguitar


macx: LIBS += -lfmodex
win32|unix: LIBS += -lfmodex64-4.44.00

FORMS += \
    PartSetter.ui
