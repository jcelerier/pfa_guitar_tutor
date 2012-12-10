#ifndef TRACKLOADER_H
#define TRACKLOADER_H

#include <QString>
#include <QList>
#include <QtXml>
#include "../GuitarTutor/Chord.h"
#include "LogicalTrack.h"

//Fichier source du loader/saver de pistes au format XML
class TrackLoader {

public:

    TrackLoader();
    ~TrackLoader();

    int ConvertLogicalTrackToXml(QString xmlFileName);
    int ConvertXmlToLogicalTrack(QString xmlFileName);
    // int SaveEditorTrackInLogicalTrack(); => TODO en attente de la nouvelle vue éditeur
    // int SaveEditorTrackInXml(); => TODO en attente de la nouvelle vue éditeur

private:

    LogicalTrack* currentTrack;

};

#endif // TRACKLOADER_H
