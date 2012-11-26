HEADERS += \
    editwindow.h \
    numerisewindow.h \
    mainwindow.h \
    createwindow.h \
    player.h \
    ../GridEditor/ChordTableWidget.h \
    ../GridEditor/CaseItem.h \
    visualisationthread.h

SOURCES += \
    main.cpp \
    editwindow.cpp \
    numerisewindow.cpp \
    mainwindow.cpp \
    createwindow.cpp \
    player.cpp \
    ../GridEditor/ChordTableWidget.cpp \
    ../GridEditor/CaseItem.cpp \
    visualisationthread.cpp

    
    
LIBS += -lfmodex64-4.44.00

QT += xml
