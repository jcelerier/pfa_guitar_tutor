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
    SimpleMusicPlayer.cpp

HEADERS += \
    CaseItem.h \
    ChordTableWidget.h \
    GridEditor.h \
    EditionSelector.h \
    ChordTree.h \
    SimpleMusicPlayer.h

TRANSLATIONS = GridEditor_fr.ts

QT += core \
      gui \
      xml \
      phonon


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../GuitarTutorAPI/release/ -lIGuitar
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../GuitarTutorAPI/debug/ -lIGuitar
else:symbian: LIBS += -lIGuitar
else:unix: LIBS += -L$$PWD/../../libiguitar/ -lIGuitar

INCLUDEPATH += $$PWD/../../GuitarTutorAPI $$PWD/../../GuitarTutorAPI/GuitarTutor
DEPENDPATH += $$PWD/../../GuitarTutorAPI

#win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../GuitarTutorAPI/release/IGuitar.lib
#else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../GuitarTutorAPI/debug/IGuitar.lib
#else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../../GuitarTutorAPI/libIGuitar.a

LIBS += -lfmodex64-4.44.00
