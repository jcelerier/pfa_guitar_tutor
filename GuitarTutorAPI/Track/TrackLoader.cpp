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

    QDomElement root = dom->documentElement();

    // Inscription des datas dans la structure de piste LogicalTrack


    QString n, a, f, m;//Pour stocker les information du morceaux
    if(root.isNull()) {
        delete currentTrack;
        return NULL;
    }
        if (((n = root.attribute("nom", 0)) == 0) || ((a = root.attribute("artiste", 0)) == 0) || ((f = root.attribute("fichier", 0)) == 0) || ((m = root.attribute("casesMesure", 0)) == 0)) {
            delete currentTrack;
            return NULL;
        }

        currentTrack->SetTrackName(n);

        currentTrack->SetArtist(a);

        currentTrack->SetAudioFileName(f);

        if(m.toInt() <= 0){
            delete currentTrack;
            return NULL;
        }
        currentTrack->setMesure(m.toInt());

        QDomNode partNode = root.firstChild();

        while(!partNode.isNull()) {
            //Convertie le noeud en QDomElement pour pouvoir
            //utiliser les methodes tagName() et attribute()
            QDomElement partElement = partNode.toElement();
            //Si le noeud n'est pas une partie ou si il n'a pas d'attribut :
            if (partElement.tagName() != "partie" || (n = partElement.attribute("nom",0)) == 0) {
                delete currentTrack;
                return NULL;
            }

            PartTrack * currentPartTrack = new PartTrack(n);

            QDomNode chordNode = partElement.firstChild();
            QString t;
            QString nom;
            QString repetition;

            while(!chordNode.isNull()) {
                QDomElement chordElement = chordNode.toElement();
                //récupération des élements
                if(!chordElement.isNull()) {
                    if((nom = chordElement.attribute("nom", 0)) == 0 || (t = chordElement.attribute("temps", 0)) == 0) {
                        delete currentTrack;
                        return NULL;
                    }

                    int time;
                    if((time = t.toInt()) <= 0) {
                        delete currentTrack;
                        return NULL;
                    }

                    //L'attribut répétion peut être absent. Attention répétition ne peut pas être nul(minimum = 1)
                    int rep;
                    QString repetition;
                    if((repetition = chordElement.attribute("repetition", 0)) == 0)
                        rep = 1;
                    else if((rep = repetition.toInt()) <= 0) {
                        delete currentTrack;
                        return NULL;
                    }
                TrackChord * c = new TrackChord(nom,time,rep);
                currentPartTrack->AddChord(c);

            }//end if
            currentTrack->AddPartTrackToList(currentPartTrack);
            chordNode = chordNode.nextSibling();
        }//end while
        partNode = partNode.nextSibling();
}//end while
return currentTrack;
}
