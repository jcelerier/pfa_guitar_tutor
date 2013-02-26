#Projet: GridEditor

TEMPLATE = app
TARGET =
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
    Util.cpp \
    ZoomButtons.cpp \
    TimeEdit.cpp \
    Main.cpp \
    TempoEdit.cpp \
    WaveformTimeBar.cpp \
    WaveformTimeSlider.cpp

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
    Util.hpp \
    ZoomButtons.h \
    TimeEdit.h \
    TempoEdit.h \
    WaveformTimeBar.h \
    WaveformTimeSlider.h

TRANSLATIONS = GridEditor_fr.ts

QT += core \
      gui \
      xml \
      webkit


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


unix:QMAKE_POST_LINK += mv GridEditor ../../app/
macx:QMAKE_POST_LINK += mv GridEditor.app ../../app/
win32:QMAKE_POST_LINK += mv GridEditor.exe ../../app/
