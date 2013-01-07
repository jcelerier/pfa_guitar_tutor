#include "TrackLoader.h"

TrackLoader::TrackLoader() {



}

TrackLoader::~TrackLoader() {



}

/* Fonction de chargement d'un fichier-piste au format XML
    @return false si le fichier spécifié n'existe pas,
    si le document XML ouvert ne peut être associé au QDomDocument,
    si le document XML n'est pas un fichier associé au projet
    @return un pointeur sur la LogicalTrack initialisée si le fichier a été correctement chargé
*/
bool TrackLoader::convertXmlToLogicalTrack(QString xmlFileName, LogicalTrack* currentTrack) {

    QDomDocument *dom = new QDomDocument(xmlFileName); // Structure contenant le XML préalablement chargé
    QFile xmlDoc(xmlFileName); // Ouverture du fichier XML

    if(!xmlDoc.open(QIODevice::ReadOnly)) { // Erreur à l'ouverture du .xml ?
        qCritical("Erreur ouverture XML");
        return false;
    }

    if (!dom->setContent(&xmlDoc)) { // Impossibilité de linker le fichier .xml ouvert au QDomDocument ?
        xmlDoc.close();
        qCritical("erreur linkage .xml au QDomDocument");
        return false;
    }

    xmlDoc.close(); // Fermeture du document xml maintenant contenu dans un QDomDocument

    // On créé la structure de donnée de la piste logique
    currentTrack = new LogicalTrack();

    QDomElement root = dom->documentElement();

    // Inscription des datas dans la structure de piste LogicalTrack

    QString n, a, f, m;//Pour stocker les information du morceaux : n = nom, a = artiste, f = fichier, m = nbr mesures
    if(root.isNull()) { //Si le l'arborescence xml est vide
        delete currentTrack;
        qCritical("Pas d'information xml");
        return false;
    }

    //Si des informations sur le morceau sont absentes.
    if (((n = root.attribute("nom", 0)) == 0) || ((a = root.attribute("artiste", 0)) == 0) || ((f = root.attribute("fichier", 0)) == 0) || ((m = root.attribute("casesMesure", 0)) == 0)) {
        delete currentTrack;
        qCritical("Informations sur le morceau incorectes");
        return false;
    }

    currentTrack->SetTrackName(n);

    currentTrack->SetArtist(a);

    currentTrack->SetAudioFileName(f);

    if(m.toInt() <= 0){ // nbr de mesures incorrect(negatif ou nul)
        delete currentTrack;
        qCritical("Nbr de mesures incorrect");
        return false;
    }
    currentTrack->setMesure(m.toInt());

    QDomNode partNode = root.firstChild();

    while(!partNode.isNull()) { //Chargement des infos sur les parties de morceau
        //Convertie le noeud en QDomElement pour pouvoir utiliser les methodes tagName() et attribute()
        QDomElement partElement = partNode.toElement();

        //Si le noeud n'est pas une partie ou si il n'a pas d'attribut nom :
        if (partElement.tagName() != "partie" || (n = partElement.attribute("nom",0)) == 0) {
            delete currentTrack;
            qCritical("Le fils de la racine n'est pas une 'partie' ou il n'a pas d'attribut nom");
            return false;
        }

        PartTrack * currentPartTrack = new PartTrack(n);

        QDomNode chordNode = partElement.firstChild();
        QString t1;
        QString name;
        QString repetition;
        int rep, t2;

        while(!chordNode.isNull()) { //Chargement des infos sur les accords du morceau
            QDomElement chordElement = chordNode.toElement();

            //Vérification de la présence et de la validité des attributs de l'accord (nom, temps, nbr de répétition)
            if((name = chordElement.attribute("nom", 0)) == 0 || (t1 = chordElement.attribute("temps", 0)) == 0) {
                delete currentTrack;
                qCritical("Le nom ou le temps sont absent");
                return false;
            }

            if((t2 = t1.toInt()) <= 0) { //Si le temps rentré est illogique(<= 0)
                delete currentTrack;
                qCritical("Le temps rentré n'est pas correct");
                return false;
            }

            //L'attribut répétion peut être absent. Attention répétition ne peut pas être nul(minimum = 1)
            repetition = chordElement.attribute("repetition", 0);
            if(repetition == 0){
                rep = 1;
            }
            else if((rep = repetition.toInt()) <= 0) {
                delete currentTrack;
                qCritical("La répétition est incorecte");
                return false;
            }
            TrackChord * c = new TrackChord(name, t2, rep);
            currentPartTrack->AddChord(c);

            currentTrack->AddPartTrackToList(currentPartTrack);
            chordNode = chordNode.nextSibling();
        }//end while
        partNode = partNode.nextSibling();
    }//end while
    return true;
}
