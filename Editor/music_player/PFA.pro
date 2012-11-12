HEADERS += \
    editwindow.h \
    numerisewindow.h \
    mainwindow.h \
    createwindow.h \
    player.h \
    ../chords_grid/ChordTableWidget.h \
    ../chords_grid/CaseItem.h \
    visualisationthread.h

SOURCES += \
    main.cpp \
    editwindow.cpp \
    numerisewindow.cpp \
    mainwindow.cpp \
    createwindow.cpp \
    player.cpp \
    ../chords_grid/ChordTableWidget.cpp \
    ../chords_grid/CaseItem.cpp \
    visualisationthread.cpp

    
    
LIBS += -lfmodex64-4.44.00

QT += xml
