######################################################################
# Automatically generated by qmake (2.01a) mer. dc. 5 18:46:09 2012
######################################################################

TEMPLATE = lib
TARGET = IGuitar
CONFIG += staticlib
QT += xml

win32 {
CONFIG += rtti
DEPENDPATH +=  . \
              ../libsndfile/lib \
              ../boost/lib \
              ../portaudio/lib

INCLUDEPATH += . \
               ../libsndfile/include \
               ../boost/include

}

DEPENDPATH += . \
              chord \
              GuitarTutor \
              IScoreLight \
              portaudio \
              IScoreLight/ECOMachine \
              IScoreLight/Editor \
              IScoreLight/EnginesPrivate \
              IScoreLight/Includes \
              IScoreLight/StoryLine \
              IScoreLight/ECOMachine/PetriNet \
              IScoreLight/ECOMachine/Process \
              IScoreLight/Editor/CSP
INCLUDEPATH += . \
               portaudio \
               chord \
               IScoreLight/Includes \
               GuitarTutor \
               IScoreLight/EnginesPrivate \
               IScoreLight/ECOMachine \
               IScoreLight/ECOMachine/PetriNet \
               IScoreLight/ECOMachine/Process \
               IScoreLight/StoryLine \
               IScoreLight/Editor/CSP \
               IScoreLight/Editor

# Input
HEADERS += MultiTracks.h \
           MusicManager.h \
           ScoreManager.h \
           Track.h \
           chord/chord.h \
           chord/chordcor.h \
           chord/ehpcp.h \
           chord/fft.h \
           GuitarTutor/Chord.h \
           GuitarTutor/Enrichment.h \
           GuitarTutor/GuitarTutorAPI.h \
           GuitarTutor/GuitarTutorAPI_global.h \
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
           IScoreLight/ECOMachine/ChangeTempo.hpp \
           IScoreLight/ECOMachine/ControlPointInformations.hpp \
           IScoreLight/ECOMachine/ECOMachine.hpp \
           IScoreLight/ECOMachine/TriggerPointInformations.hpp \
           IScoreLight/Editor/Editor.hpp \
           IScoreLight/EnginesPrivate/EnginesPrivate.hpp \
           IScoreLight/Includes/CSPTypes.hpp \
           IScoreLight/Includes/Engines.hpp \
           IScoreLight/StoryLine/StoryLine.hpp \
           IScoreLight/ECOMachine/PetriNet/Arc.hpp \
           IScoreLight/ECOMachine/PetriNet/ArithmeticException.hpp \
           IScoreLight/ECOMachine/PetriNet/ExtendedInt.hpp \
           IScoreLight/ECOMachine/PetriNet/IllegalArgumentException.hpp \
           IScoreLight/ECOMachine/PetriNet/IncoherentStateException.hpp \
           IScoreLight/ECOMachine/PetriNet/OutOfBoundException.hpp \
           IScoreLight/ECOMachine/PetriNet/PetriNet.hpp \
           IScoreLight/ECOMachine/PetriNet/PetriNetItem.hpp \
           IScoreLight/ECOMachine/PetriNet/PetriNetNode.hpp \
           IScoreLight/ECOMachine/PetriNet/Place.hpp \
           IScoreLight/ECOMachine/PetriNet/PriorityTransitionAction.hpp \
           IScoreLight/ECOMachine/PetriNet/ThreadSafeList.hpp \
           IScoreLight/ECOMachine/PetriNet/Token.hpp \
           IScoreLight/ECOMachine/PetriNet/Transition.hpp \
           IScoreLight/ECOMachine/PetriNet/TransitionAction.hpp \
           IScoreLight/ECOMachine/PetriNet/TransitionBitArray.hpp \
           IScoreLight/ECOMachine/Process/MultiTypeCurve.hpp \
           IScoreLight/ECOMachine/Process/NetworkMessageCurves.hpp \
           IScoreLight/ECOMachine/Process/Process.hpp \
           IScoreLight/ECOMachine/Process/ProcessInclude.hpp \
           IScoreLight/ECOMachine/Process/ProcessStep.hpp \
           IScoreLight/ECOMachine/Process/SendNetworkMessageProcess.hpp \
           IScoreLight/ECOMachine/Process/StringParser.hpp \
           IScoreLight/ECOMachine/Process/StringTypeFunctions.hpp \
           IScoreLight/Editor/CSP/AllenRelation.hpp \
           IScoreLight/Editor/CSP/AntPostRelation.hpp \
           IScoreLight/Editor/CSP/BinaryRelation.hpp \
           IScoreLight/Editor/CSP/BinaryTemporalRelation.hpp \
           IScoreLight/Editor/CSP/ConstrainedBox.hpp \
           IScoreLight/Editor/CSP/ConstrainedTemporalEntity.hpp \
           IScoreLight/Editor/CSP/ControlPoint.hpp \
           IScoreLight/Editor/CSP/CSP.hpp \
           IScoreLight/Editor/CSP/CSPConstrainedVariable.hpp \
           IScoreLight/Editor/CSP/CSPLinearConstraint.hpp \
           IScoreLight/Editor/CSP/IllegalArgumentException.hpp \
           IScoreLight/Editor/CSP/InvalidFileFormatException.hpp \
           IScoreLight/Editor/CSP/OutOfBoundException.hpp \
           IScoreLight/Editor/CSP/TriggerPoint.hpp \
           MusicPlayer.h \
    Track/TrackLoader.h \
    Track/PartTrack.h \
    Track/LogicalTrack.h \
    Track/TrackChord.h
SOURCES += MultiTracks.cpp \
           MusicManager.cpp \
           ScoreManager.cpp \
           Track.cpp \
           chord/chord.c \
           chord/chordcor.c \
           chord/ehpcp.c \
           chord/fft.c \
           GuitarTutor/Chord.cpp \
           GuitarTutor/Enrichment.cpp \
           GuitarTutor/GuitarTutorAPI.cpp \
           GuitarTutor/Tonalities.cpp \
           GuitarTutor/Tonality.cpp \
           IScoreLight/Engines.cpp \
           IScoreLight/ECOMachine/ChangeTempo.cpp \
           IScoreLight/ECOMachine/ControlPointInformations.cpp \
           IScoreLight/ECOMachine/ECOMachine.cpp \
           IScoreLight/ECOMachine/TriggerPointInformations.cpp \
           IScoreLight/Editor/Editor.cpp \
           IScoreLight/EnginesPrivate/EnginesPrivate.cpp \
           IScoreLight/StoryLine/StoryLine.cpp \
           IScoreLight/ECOMachine/PetriNet/Arc.cpp \
           IScoreLight/ECOMachine/PetriNet/ExtendedInt.cpp \
           IScoreLight/ECOMachine/PetriNet/PetriNet.cpp \
           IScoreLight/ECOMachine/PetriNet/PetriNetItem.cpp \
           IScoreLight/ECOMachine/PetriNet/PetriNetNode.cpp \
           IScoreLight/ECOMachine/PetriNet/Place.cpp \
           IScoreLight/ECOMachine/PetriNet/PriorityTransitionAction.cpp \
           IScoreLight/ECOMachine/PetriNet/ThreadSafeList.cpp \
           IScoreLight/ECOMachine/PetriNet/Token.cpp \
           IScoreLight/ECOMachine/PetriNet/Transition.cpp \
           IScoreLight/ECOMachine/PetriNet/TransitionAction.cpp \
           IScoreLight/ECOMachine/PetriNet/TransitionBitArray.cpp \
           IScoreLight/ECOMachine/Process/MultiTypeCurve.cpp \
           IScoreLight/ECOMachine/Process/NetworkMessageCurves.cpp \
           IScoreLight/ECOMachine/Process/Process.cpp \
           IScoreLight/ECOMachine/Process/SendNetworkMessageProcess.cpp \
           IScoreLight/ECOMachine/Process/StringParser.cpp \
           IScoreLight/ECOMachine/Process/StringTypeFunctions.cpp \
           IScoreLight/Editor/CSP/AllenRelation.cpp \
           IScoreLight/Editor/CSP/AntPostRelation.cpp \
           IScoreLight/Editor/CSP/BinaryRelation.cpp \
           IScoreLight/Editor/CSP/BinaryTemporalRelation.cpp \
           IScoreLight/Editor/CSP/ConstrainedBox.cpp \
           IScoreLight/Editor/CSP/ConstrainedTemporalEntity.cpp \
           IScoreLight/Editor/CSP/ControlPoint.cpp \
           IScoreLight/Editor/CSP/CSP.cpp \
           IScoreLight/Editor/CSP/CSPConstrainedVariable.cpp \
           IScoreLight/Editor/CSP/CSPLinearConstraint.cpp \
           IScoreLight/Editor/CSP/TriggerPoint.cpp \
           MusicPlayer.cpp \
    Track/TrackLoader.cpp \
    Track/PartTrack.cpp \
    Track/LogicalTrack.cpp

macx: QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
macx: QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64

LIBS += -lsndfile -lboost -lportaudio
macx: LIBS += -lfmodex
win32|unix: LIBS += -lfmodex64-4.44.00

QMAKE_POST_LINK += cp libIGuitar.a ../libiguitar/

