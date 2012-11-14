######################################################################
# Automatically generated by qmake (2.01a) mer. avr. 20 15:08:44 2011
######################################################################

CONFIG -= release
CONFIG += debug
TEMPLATE = app
TARGET = 
DEPENDPATH += . 
INCLUDEPATH += . ../IGuitar/src
QMAKE_LIBDIR += ../IGuitar/Release
QT += opengl
LIBS +=  -lsndfile -lportaudio -lIGuitar -lGL -lglut -lGLU

# Input
HEADERS += MainWindow.hpp OpenGLWidget.h RenderArea.hpp ./chordfft/chord_fft.h ./chordfft/chordcor_fft.h ./chordfft/ehpcp_fft.h ./chordfft/fft.h ./chordfft/filter_fft.h ./chordfft/median_fft.h
SOURCES += main.cpp MainWindow.cpp OpenGLWidget.cpp RenderArea.cpp ./chordfft/chord_fft.c ./chordfft/chordcor_fft.c ./chordfft/ehpcp_fft.c ./chordfft/fft.c ./chordfft/filter_fft.c ./chordfft/median_fft.c
