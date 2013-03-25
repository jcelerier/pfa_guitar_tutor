TEMPLATE = lib
TARGET = IGuitar
CONFIG += staticlib
DESTDIR = ../libiguitar/
QT += xml

win32 {
CONFIG += rtti
DEPENDPATH +=  . \
			  ../portaudio/lib \
			  ../fmod/api/lib

INCLUDEPATH += . \
			   ../fmod/api/inc
}

DEPENDPATH += . \
			  chord \
			  GuitarTutor \
			  portaudio

INCLUDEPATH += . \
			   portaudio \
			   chord \
			   GuitarTutor \

# Input
HEADERS += MultiTracks.h \
		   Util.hpp \
		   MusicManager.h \
		   Track.h \
		   chord/chord_c.h \
		   chord/chordcor.h \
		   chord/ehpcp.h \
		   chord/fft.h \
		   GuitarTutor/Chord.h \
		   GuitarTutor/Enrichment.h \
		   GuitarTutor/Tonalities.h \
		   GuitarTutor/Tonality.h \
		   portaudio/pa_asio.h \
		   portaudio/pa_jack.h \
		   portaudio/pa_linux_alsa.h \
		   portaudio/pa_mac_core.h \
		   portaudio/pa_win_ds.h \
		   portaudio/pa_win_wasapi.h \
		   portaudio/pa_win_waveformat.h \
		   portaudio/pa_win_wmme.h \
		   portaudio/portaudio.h \
		   MusicPlayer.h \
		   Track/TrackLoader.h \
		   Track/PartTrack.h \
		   Track/LogicalTrack.h \
		   Track/TrackChord.h


SOURCES += MultiTracks.cpp \
		   Track.cpp \
		   Util.cpp \
		   MusicManager.cpp \
		   MusicPlayer.cpp \
		   chord/chordcor.c \
		   chord/ehpcp.c \
		   chord/fft.c \
		   chord/chord_c.c \
		   GuitarTutor/Chord.cpp \
		   GuitarTutor/Enrichment.cpp \
		   GuitarTutor/Tonalities.cpp \
		   GuitarTutor/Tonality.cpp \
		   Track/TrackLoader.cpp \
		   Track/PartTrack.cpp \
		   Track/LogicalTrack.cpp \
		   Track/TrackChord.cpp



macx: QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
macx: QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64

LIBS += -lportaudio
macx|win32: LIBS += -lfmodex
unix: LIBS += -lfmodex64-4.44.00

QMAKE_CXXFLAGS = -O3
