#ifndef TRACKLOADER_H
#define TRACKLOADER_H

#include <QString>
#include <QList>
#include <QtXml>
#include <QFile>
using namespace std;

#include "../GuitarTutor/Chord.h"
#include "LogicalTrack.h"
#include "PartTrack.h"

//Fichier source du loader/saver de pistes au format XML
class TrackLoader {

public:

    TrackLoader();
    ~TrackLoader();

    static bool convertLogicalTrackToXml(LogicalTrack* currentTrack);
    static bool convertXmlToLogicalTrack(QString xmlFileName, LogicalTrack* currentTrack);

    // int SaveEditorTrackInLogicalTrack(); => TODO en attente de la nouvelle vue éditeur
    // int SaveEditorTrackInXml(); => TODO en attente de la nouvelle vue éditeur

private:


};

#endif // TRACKLOADER_H
