#include "TrackLoader.h"

TrackLoader::TrackLoader() {



}

TrackLoader::~TrackLoader() {



}

/* Fonction de chargement d'un fichier-piste au format XML
    @return NULL si le fichier spécifié n'existe pas,
    si le document XML ouvert ne peut être associé au QDomDocument,
    si le document XML n'est pas un fichier associé au projet
    @return un pointeur sur la LogicalTrack initialisée si le fichier a été correctement chargé
*/
LogicalTrack * TrackLoader::ConvertXmlToLogicalTrack(QString xmlFileName) {

    QDomDocument *dom = new QDomDocument(xmlFileName); // Structure contenant le XML préalablement chargé
    QFile xmlDoc(xmlFileName); // Ouverture du fichier XML

    if(!xmlDoc.open(QIODevice::ReadOnly)) { // Erreur à l'ouverture du .xml ?
        return NULL;
    }

    if (!dom->setContent(&xmlDoc)) { // Impossibilité de linker le fichier .xml ouvert au QDomDocument ?
        xmlDoc.close();
        return NULL;
    }

    xmlDoc.close(); // Fermeture du document xml maintenant contenu dans un QDomDocument

    // On créé la structure de donnée de la piste logique

    currentTrack = new LogicalTrack();

    QDomElement domElement = dom->documentElement();
    QDomNode node = domElement.firstChild();

    // Inscription des datas dans la structure de piste LogicalTrack

    while(!node.isNull()) {
        if(!domElement.isNull()) {

            if (domElement.tagName() == "nom") {
                currentTrack->SetTrackName(domElement.text());
            }

            else if (domElement.tagName() == "artiste") {
                currentTrack->SetArtist(domElement.text());
            }

            else if (domElement.tagName() == "fichier") {
                currentTrack->SetAudioFileName(domElement.text());
            }

            else if (domElement.tagName() == "partie") {

                if(domElement.attribute("nom",0) == 0) {
                    delete currentTrack;
                    return NULL;
                }

                PartTrack * currentPartTrack = currentTrack->AddPartTrackToList(domElement.attribute("nom",0));



                QDomNode chordNode = node.firstChild();
                QString t;
                QString nom;
                QString repetition;
                while(!chordNode.isNull()) {
                    if(!domElement.isNull()) {
                        if((t = domElement.attribute("temps", 0)) == 0 || (nom = domElement.attribute("nom", 0)) == 0) {
                            delete currentTrack;
                            return NULL;
                        }
                        else {
                            int time;
                            int rep;

                            if(time = t.toInt() == 0) {
                                delete currentTrack;
                                return NULL;
                            }

                            if((repetition = domElement.attribute("repetition",0)) == 0)
                                rep = 1;
                            else if((rep = repetition.toInt()) == 0) {
                                delete currentTrack;
                                return NULL;
                            }

                            TrackChord * c = new TrackChord(nom,time,rep);

                        }

                    }
                }
            }
        }
        node = node.nextSibling();
    }
    return currentTrack;
}
