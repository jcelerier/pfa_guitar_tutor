#Projet: GridEditor

SOURCES += \
    CaseItem.cpp \
    ChordTableWidget.cpp \
    GridEditor.cpp \
    TestMain.cpp \
    EditionSelector.cpp \
    ChordTree.cpp

HEADERS += \
    CaseItem.h \
    ChordTableWidget.h \
    GridEditor.h \
    EditionSelector.h \
    ChordTree.h \

QT += xml


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../GuitarTutorAPI/release/ -lGuitarTutorAPI
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../GuitarTutorAPI/debug/ -lGuitarTutorAPI
else:symbian: LIBS += -lGuitarTutorAPI
else:unix: LIBS += -L$$PWD/../../GuitarTutorAPI/ -lGuitarTutorAPI

INCLUDEPATH += $$PWD/../../GuitarTutorAPI
DEPENDPATH += $$PWD/../../GuitarTutorAPI

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../GuitarTutorAPI/release/GuitarTutorAPI.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../GuitarTutorAPI/debug/GuitarTutorAPI.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../../GuitarTutorAPI/libGuitarTutorAPI.a
