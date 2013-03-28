#include "TrackLoader.h"


TrackLoader::TrackLoader() {

}

bool TrackLoader::convertLogicalTrackToXml(LogicalTrack* currentTrack, QString fname) {
   // QString fname = currentTrack->getTrackName().append(".xml");
	QFile file(fname);
	QTextStream out;
	QDomDocument doc;
	QDomElement root = doc.createElement("morceau");
	QDomElement newPart;
	QDomElement newChord;

	QList<TrackChord*> chordsList;
	QList<TrackChord*>::iterator iChord;
	//Initialisation de la racine morceau
	doc.appendChild(root);
	root.setAttribute("nom", currentTrack->getTrackName());
	root.setAttribute("artiste", currentTrack->getArtist());
    root.setAttribute("chordMesure", currentTrack->getChordsPerBar());
	root.setAttribute("fichier", currentTrack->getAudioFileName());
	root.setAttribute("line", currentTrack->getLine());
	root.setAttribute("column", currentTrack->getColumn());

	root.setAttribute("bar", currentTrack->getBar());
	root.setAttribute("beginning", currentTrack->getBeginning());
	root.setAttribute("end", currentTrack->getEnd());
	root.setAttribute("timeSignature", currentTrack->getTimeSignature());
	root.setAttribute("comment", currentTrack->getComment());

	//ajout des parties
	QList<PartTrack*> partList = currentTrack->getPartTrackList();

	QList<PartTrack*>::const_iterator iPart;

	for(iPart = partList.begin(); iPart < partList.end(); ++iPart)
	{
		newPart = doc.createElement("partie");
		newPart.setAttribute("nom", (*iPart)->getPartName());

		//ajout des accords
		chordsList = (*iPart)->getTrackChordsList();
		for(iChord = chordsList.begin(); iChord < chordsList.end(); ++iChord)
		{
			newChord = doc.createElement("accord");
			newChord.setAttribute("temps", (*iChord)->getBeginningInMs());
			newChord.setAttribute("repetition", (*iChord)->getRepetition());
			newChord.setAttribute("nom", (*iChord)->getChord());

			newPart.appendChild(newChord);
		}//fin ajout accord

		root.appendChild(newPart);
	}//fin ajout partie


	//écriture dans le fichier
	if(!file.open(QIODevice::WriteOnly)){//ouverture
		qCritical("Impossible d'ouvrir le fichier %s", qPrintable(fname));
		return false;
	}
	out.setDevice(&file);//mise en relation avec le flux "out"

	//ajout de l'en-tête xml
	QDomNode node = doc.createProcessingInstruction("xml", "version=\"1.0\"");
	doc.insertBefore(node, doc.firstChild());

	doc.save(out, 2);
	return true;
}


/**
 * Fonction de chargement d'un fichier-piste au format XML
 *@params xmlFileName : Chemin vers le fichier xml à parser
 *@params currentTrack : Pointeur vers un LogicalTrack. Les données de celui-ci seront
 *remplacé par les données du XML.
 *@return un booleen true si la fonction s'est correctement executé et false sinon.
 *return false si le fichier spécifié n'existe pas,
 *si le document XML ouvert ne peut être associé au QDomDocument,
 *si le document XML n'est pas un fichier associé au projet
*/
bool TrackLoader::convertXmlToLogicalTrack(QString xmlFileName, LogicalTrack* currentTrack) {

	TrackChord* previousChord = 0;
	TrackChord * currentChord = 0;
	PartTrack* previousPart = 0;

	QDomDocument *dom = new QDomDocument(xmlFileName); // Structure contenant le XML préalablement chargé
	QFile xmlDoc(xmlFileName); // Ouverture du fichier XML

	if(!xmlDoc.open(QIODevice::ReadOnly)) { // Erreur à l'ouverture du .xml ?
		qCritical("API.concertLogicalToXML : Erreur ouverture XML");
		return false;
	}

	if (!dom->setContent(&xmlDoc)) { // Impossibilité de linker le fichier .xml ouvert au QDomDocument ?
		xmlDoc.close();
		qCritical("API.concertLogicalToXML : Erreur linkage .xml au QDomDocument");
		return false;
	}

	xmlDoc.close(); // Fermeture du document xml maintenant contenu dans un QDomDocument

	QDomElement root = dom->documentElement();

	// Inscription des datas dans la structure de piste LogicalTrack
	QString n, a, f, m, line, column, bar, beginning, end, tmp, com;//Pour stocker les information du morceaux : n = nom, a = artiste, f = fichier, m = nbr mesures

	if(root.isNull()) { //Si le l'arborescence xml est vide
		delete currentTrack;
		qCritical("API.concertLogicalToXML : Pas d'information xml");
		return false;
	}

	//Chargement sans vérification des noeuds.
	n = root.attribute("nom", 0);
	a = root.attribute("artiste", 0);
	f = root.attribute("fichier", 0);
	line = root.attribute("line", 0);
	column = root.attribute("column", 0);
	m = root.attribute("chordMesure", 0);
	bar = root.attribute("bar", 0);
	beginning = root.attribute("beginning", 0);
	end = root.attribute("end", 0);
	tmp = root.attribute("timeSignature", 0);
	com = root.attribute("comment", 0);

	currentTrack->setTrackName(n);
	currentTrack->setArtist(a);
	currentTrack->setAudioFileName(f);
	currentTrack->setComment(com);
	currentTrack->setLine(line.toInt());
	currentTrack->setColumn(column.toInt());
	currentTrack->setBars(bar.toInt(), beginning.toInt(), end.toInt());
	currentTrack->setTimeSignature(tmp.toInt());

	//    //Chargement du morceau avec vérification des noeuds.
	//    if (((n = root.attribute("nom", 0)) == 0) ||
	//        ((a = root.attribute("artiste", 0)) == 0) ||
	//        ((f = root.attribute("fichier", 0)) == 0) ||
	//        ((line = root.attribute("line", 0)) == 0) ||
	//        ((column = root.attribute("column", 0)) == 0) ||
	//        ((m = root.attribute("casesMesure", 0)) == 0))
	//    {
	//        delete currentTrack;
	//        qCritical("Informations sur le morceau incompletes.");
	//        return false;
	//    }

	if(m.toInt() <= 0){ // nbr de mesures incorrect(negatif ou nul)
		delete currentTrack;
		qCritical("API.concertLogicalToXML : Nbr de mesures incorrect");
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
			qCritical("API.concertLogicalToXML : Le fils de la racine n'est pas une 'partie' ou il n'a pas d'attribut nom");
			return false;
		}

		PartTrack * currentPartTrack = new PartTrack(n);
		currentPartTrack->setPrevious(previousPart);
		if(previousPart != 0) previousPart->setNext(currentPartTrack);
		previousPart = currentPartTrack;

		QDomNode chordNode = partElement.firstChild();
		QString t1;
		QString name;
		QString repetition;
		int rep, t2;

		while(!chordNode.isNull())
		{ //Chargement des infos sur les accords du morceau
			QDomElement chordElement = chordNode.toElement();

			//Vérification de la présence et de la validité des attributs de l'accord (nom, temps, nbr de répétition)
			if((name = chordElement.attribute("nom", 0)) == 0 || (t1 = chordElement.attribute("temps", 0)) == 0) {
				delete currentTrack;
				qCritical("API.concertLogicalToXML : Le nom ou le temps sont absent");
				return false;
			}

			if((t2 = t1.toInt()) < 0) { //Si le temps rentré est illogique(<= 0)
				delete currentTrack;
				qCritical("API.concertLogicalToXML : L'un des temps rentré est négatif...");
				return false;
			}

			if((t2 = t1.toInt()) < 0) { //Si le temps rentré est illogique(<= 0)
				delete currentTrack;
				qCritical("API.concertLogicalToXML : Les temps ne sont pas rentrés dans l'ordre croissant.");
				return false;
			}

			//L'attribut répétion peut être absent. Attention répétition ne peut pas être nul(minimum = 1)
			repetition = chordElement.attribute("repetition", 0);

			//repetition c'est un qstring, donc avant qu'il soit égal à 0...
			if(repetition == 0)
			{
				rep = 1;
			}
			else if((rep = repetition.toInt()) < 0) {
				delete currentTrack;
				qCritical("API.concertLogicalToXML : La répétition est négative");
				return false;
			}
			else if((rep = repetition.toInt()) == 0) {
				rep = 1;
				qCritical("API.concertLogicalToXML : La répétition est nulle");
			}

			currentChord = new TrackChord(name, t2, rep, previousChord, 0, currentPartTrack );
			if(previousChord != 0) previousChord->setNext(currentChord);
			previousChord = currentChord;
			currentPartTrack->AddChord(currentChord);

			chordNode = chordNode.nextSibling();
		  }//end while

		currentTrack->addPartTrackToList(currentPartTrack);
		partNode = partNode.nextSibling();

	}//end while

	currentChord->setNext(0);
	return true;
}
