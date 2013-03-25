#ifndef TRACKLOADER_H
#define TRACKLOADER_H

#include <QString>
#include <QList>
#include <QtXml>
#include <QFile>

#include "LogicalTrack.h"
#include "PartTrack.h"

/**
 * @brief Cette classe permet de charger en mémoire les informations contenues dans un LogicalTrack
 * et vice versa.
 */

//Fichier source du loader/saver de pistes au format XML
class TrackLoader {

public:

    TrackLoader();
    ~TrackLoader();

	static bool convertLogicalTrackToXml(LogicalTrack* currentTrack, QString filename);
    static bool convertXmlToLogicalTrack(QString xmlFileName, LogicalTrack* currentTrack);


};

#endif // TRACKLOADER_H
