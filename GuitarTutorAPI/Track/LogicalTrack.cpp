#include "LogicalTrack.h"

/**
 * @brief LogicalTrack::LogicalTrack
 *
 * Constructeur par défaut de la classe LogicalTrack
 */
LogicalTrack::LogicalTrack() {
	trackName = "";
	artist = "";
	audioFileName = "";
	mesure = 0;
	beginning = 0;
	bar = 0;
	end = 0;
	timeSignature = 4;
}

/**
 * @brief LogicalTrack::LogicalTrack
 *
 * Constructeur obsolète initialisant de la classe LogicalTrack sans initialiser beginning, bar et mesure.
 */
LogicalTrack::LogicalTrack(QString newTrackName, QString newArtist, QString newAudoFileName, int newMesure, QList<PartTrack*> newList){
	trackName = newTrackName;
	artist = newArtist;
	audioFileName = newAudoFileName;
	mesure = newMesure;
	listPartTrack = newList;
}

/**
 * @brief LogicalTrack::LogicalTrack
 *
 * Constructeur permettant l'initialisialisation correcte de la classe LogicalTrack.
 */
LogicalTrack::LogicalTrack(QString newTrackName, QString newArtist, QString newAudoFileName, int newMesure, QList<PartTrack*> newList, int newBar, int newEnd, int newBeginning){
						   //, int newTimePerMesure){
	trackName = newTrackName;
	artist = newArtist;
	audioFileName = newAudoFileName;
	mesure = newMesure;
	listPartTrack = newList;
	bar = newBar;
	end = newEnd;
	beginning = newBeginning;
//    timePerMesure = newTimePerMesure;
}

/**
 * @brief LogicalTrack::~LogicalTrack
 *
 * Destructeur de la classe LogicalTrack
 */
LogicalTrack::~LogicalTrack() {

	for(QList<PartTrack*>::Iterator i = listPartTrack.begin(); i != listPartTrack.end() ; i++)
		delete (*i);
}

/**
 * @brief LogicalTrack::SetTrackName
 * @param NewTrackName chaîne de caractères
 *
 * Change la valeur de l'attribut TrackName de la classe LogicalTrack
 */
void LogicalTrack::setTrackName(QString newTrackName) {

	trackName = newTrackName;

}

/**
 * @brief LogicalTrack::GetTrackName
 *
 * Renvoie la valeur de l'attribut TrackName de la classe LogicalTrack
 */
QString LogicalTrack::getTrackName() {

	return trackName;

}

/**
 * @brief LogicalTrack::SetArtist
 * @param NewArtist chaîne de caractères
 *
 * Change la valeur de l'attribut Artist de la classe LogicalTrack
 */
void LogicalTrack::setArtist(QString newArtist) {

	artist = newArtist;

}

/**
 * @brief LogicalTrack::GetArstist
 *
 * Renvoie la valeur de l'attribut Artist de la classe LogicalTrack
 */
QString LogicalTrack::getArtist() {

	return artist;
}

/**
 * @brief LogicalTrack::SetAudioFileName
 * @param NewFileName chaîne de caractères
 *
 * Change la valeur de l'attribut FileName de la classe LogicalTrack
 */
void LogicalTrack::setAudioFileName(QString newAudioFileName) {

	audioFileName = newAudioFileName;

}

void LogicalTrack::setBars(int nBar, int nBeginning, int nEnd){

	bar = nBar;
	beginning = nBeginning;
	end = nEnd;

}

void LogicalTrack::setTimeSignature(int nTS){

	timeSignature = nTS;

}

/**
 * @brief LogicalTrack::GetAudioFileName
 *
 * Renvoie la valeur de l'attribut AudioFileName de la classe LogicalTrack
 */
QString LogicalTrack::getAudioFileName() {

	return audioFileName;

}

/**
 * @brief LogicalTrack::AddPartTrackToList
 * @param newPartTrack : Partie de moreceau à rajouter
 *
 * Ajoute une PartTrack à la liste des PartTrack de la LogicalTrack
 */
void LogicalTrack::addPartTrackToList(PartTrack* newPartTrack) {

	listPartTrack.append(newPartTrack);

}

/**
 * @brief LogicalTrack::getListPartTrack
 *
 * Renvoie un pointeur sur la liste des PartTrack de la LogicalTrack
 */
QList<PartTrack*>& LogicalTrack::getPartTrackList()
{
	return listPartTrack;
}

int LogicalTrack::getBeginning(){
	return beginning;
}

int LogicalTrack::getBar(){
	return bar;
}

int LogicalTrack::getEnd(){
	return end;
}

int LogicalTrack::getTimeSignature(){
	return timeSignature;
}

/**
 * @brief LogicalTrack::setRep
 *
 * Permet de changer la valeur du nombre d'accords dans une mesure. Si la valeur entrée n'est pas correcte, rep sera mis à -1.
 */
void LogicalTrack::setMesure(int nb){
	if(nb>=0){
		mesure = nb;
	}
	else
        mesure = 1;
}

int LogicalTrack::getMesure() {

	return this->mesure;
}

void LogicalTrack::setLine(int line)
{
	this->line = line;
}


void LogicalTrack::setColumn(int col)
{
	this->column = col;
}

int LogicalTrack::getLine()
{
	return line;
}


int LogicalTrack::getColumn()
{
	return column;
}

QString LogicalTrack::getPartName(int n)
{
	if(n <= listPartTrack.size() && n>0){
		return listPartTrack[n-1]->getPartName();
	}
	return listPartTrack[listPartTrack.size()-1]->getPartName();
}

void LogicalTrack::setComment(QString comment)
{
	this->comment = comment;
}

QString LogicalTrack::getComment()
{
	return comment;
}

int LogicalTrack::getBPM()
{
	return getTimeSignature() * 60000 / (getBar() - getBeginning());
}

TrackChord* LogicalTrack::getFirstChord()
{
	return getPartTrackList()[0]->getTrackChordsList()[0];
}
