#include "LogicalTrack.h"

/**
 * @brief LogicalTrack::LogicalTrack
 *
 * Constructeur par défaut de la classe LogicalTrack
 */
LogicalTrack::LogicalTrack() {
    m_trackName = "";
    m_artist = "";
    m_audioFileName = "";
    m_chordsPerBar = 0;
    m_beginning = 0;
    m_bar = 0;
    m_end = 0;
    m_timeSignature = 4;
}

/**
 * @brief LogicalTrack::LogicalTrack
 *
 * Constructeur obsolète initialisant de la classe LogicalTrack sans initialiser beginning, bar et mesure.
 */
LogicalTrack::LogicalTrack(QString newTrackName, QString newArtist, QString newAudoFileName, int newMesure, QList<PartTrack*> newList){
    m_trackName = newTrackName;
    m_artist = newArtist;
    m_audioFileName = newAudoFileName;
    m_chordsPerBar = newMesure;
    m_listPartTrack = newList;
}

/**
 * @brief LogicalTrack::LogicalTrack
 *
 * Constructeur permettant l'initialisation correcte de la classe LogicalTrack.
 */
LogicalTrack::LogicalTrack(QString newTrackName, QString newArtist, QString newAudoFileName, int newMesure, QList<PartTrack*> newList, int newBar, int newEnd, int newBeginning){
    //, int newTimePerMesure){
    m_trackName = newTrackName;
    m_artist = newArtist;
    m_audioFileName = newAudoFileName;
    m_chordsPerBar = newMesure;
    m_listPartTrack = newList;
    m_bar = newBar;
    m_end = newEnd;
    m_beginning = newBeginning;
    //    timePerMesure = newTimePerMesure;
}

/**
 * @brief LogicalTrack::~LogicalTrack
 *
 * Destructeur de la classe LogicalTrack
 */
LogicalTrack::~LogicalTrack() {
    for(QList<PartTrack*>::Iterator i = m_listPartTrack.begin(); i != m_listPartTrack.end() ; i++)
        delete (*i);
}

/**
 * @brief LogicalTrack::SetTrackName
 * @param newTrackName Nom de la piste
 *
 * Change le nom de la piste audio.
 */
void LogicalTrack::setTrackName(QString newTrackName) {

    m_trackName = newTrackName;

}

/**
 * @brief LogicalTrack::GetTrackName
 * @return Le nom de la piste audio.
 */
QString LogicalTrack::getTrackName() {

    return m_trackName;

}

/**
 * @brief LogicalTrack::SetArtist
 * @param newArtist Nom de l'artiste
 *
 * Change le nom de l'artiste de la piste.
 */
void LogicalTrack::setArtist(QString newArtist) {

    m_artist = newArtist;

}

/**
 * @brief LogicalTrack::GetArstist
 * @return Le nom de l'artiste.
 */
QString LogicalTrack::getArtist() {

    return m_artist;
}

/**
 * @brief LogicalTrack::SetAudioFileName
 * @param newAudioFileName Chemin du fichier audio
 *
 * Change le chemin vers le fichier audio.
 */
void LogicalTrack::setAudioFileName(QString newAudioFileName) {

    m_audioFileName = newAudioFileName;

}

/**
 * @brief LogicalTrack::SetBars
 * @param nBar Fin de la première mesure en ms
 * @param nBeginning Début de la première mesure en ms
 * @param nEnd Fin du dernier accord en ms
 *
 * Change la valeur timers nécessaires à la synchronisation audio.
 */
void LogicalTrack::setBars(int nBar, int nBeginning, int nEnd){

    m_bar = nBar;
    m_beginning = nBeginning;
    m_end = nEnd;

}

/**
 * @brief LogicalTrack::SetTimeSignature
 * @param nTS : nouvelle Signature rythmique
 *
 * Change la valeur de la signature rythmique.
 *
 */
void LogicalTrack::setTimeSignature(int nTS){

    m_timeSignature = nTS;

}

/**
 * @brief LogicalTrack::GetAudioFileName
 * @return Le nom du fichier audio.
 */
QString LogicalTrack::getAudioFileName() {

    return m_audioFileName;

}

/**
 * @brief LogicalTrack::AddPartTrackToList
 * @param newPartTrack Partie de morceau à rajouter
 *
 * Ajoute une partie à fin la track en cours.
 */
void LogicalTrack::addPartTrackToList(PartTrack* newPartTrack) {
    m_listPartTrack.append(newPartTrack);
}

/**
 * @brief LogicalTrack::getListPartTrack
 * @return Pointeur sur la liste des parties de la track en cours.
 */
QList<PartTrack*>& LogicalTrack::getPartTrackList()
{
    return m_listPartTrack;
}

/**
 * @brief LogicalTrack::getBeginning
 * @return Le temps du début du premier accord.
 */
int LogicalTrack::getBeginning(){
    return m_beginning;
}

/**
 * @brief LogicalTrack::getBar
 * @return Le temps de fin de la première mesure.
 */
int LogicalTrack::getBar(){
    return m_bar;
}

/**
 * @brief LogicalTrack::getEnd
 * @return Le temps de fin du dernier accord.
 */
int LogicalTrack::getEnd(){
    return m_end;
}

/**
 * @brief LogicalTrack::getEnd
 * @return La signature rythmique du morceau.
 */
int LogicalTrack::getTimeSignature(){
    return m_timeSignature;
}

/**
 * @brief LogicalTrack::setChordsPerBar
 * @param nb Nombre d'accords par mesure.
 *
 * Change la valeur du nombre d'accords dans une mesure. Si la valeur entrée n'est pas correcte, rep sera mis à -1.
 */
void LogicalTrack::setChordsPerBar(int nb){
    if(nb>=0){
        m_chordsPerBar = nb;
    }
    else
        m_chordsPerBar = 1;
}

/**
 * @brief LogicalTrack::getChordsPerBar
 * @return Le nombre d'accords par mesure.
 */
int LogicalTrack::getChordsPerBar() {

    return this->m_chordsPerBar;
}

/**
 * @brief LogicalTrack::setLine
 * @param line Nombre de lignes
 *
 * Change le nombre de lignes dans la track.
 */
void LogicalTrack::setLine(int line)
{
    this->m_line = line;
}

/**
 * @brief LogicalTrack::setColumn
 * @param col Nombre de colonnes
 *
 * Change le nombre de colonnes de la track.
 */
void LogicalTrack::setColumn(int col)
{
    this->m_column = col;
}

/**
 * @brief LogicalTrack::getLine
 * @return Le nombre de lignes de la track.
 */
int LogicalTrack::getLine()
{
    return m_line;
}

/**
 * @brief LogicalTrack::getColumn
 * @return Le nombre de colonnes de la ligne.
 */
int LogicalTrack::getColumn()
{
    return m_column;
}

/**
 * @brief LogicalTrack::getPartName
 * @param n Numéro de la partie
 * @return Le nom de la partie demandée.
 */
QString LogicalTrack::getPartName(int n)
{
    if(n <= m_listPartTrack.size() && n>0){
        return m_listPartTrack[n-1]->getPartName();
    }
    return m_listPartTrack[m_listPartTrack.size()-1]->getPartName();
}

/**
 * @brief LogicalTrack::setComment
 * @param comment Commentaire sur le morceau
 *
 * Modifie le commentaire du morceau.
 */
void LogicalTrack::setComment(QString comment)
{
    this->m_comment = comment;
}

/**
 * @brief LogicalTrack::getComment
 * @return Le commentaire sur le morceau.
 */
QString LogicalTrack::getComment()
{
    return m_comment;
}

/**
 * @brief LogicalTrack::getBPM
 * @return Le BPM du morceau.
 */
int LogicalTrack::getBPM()
{
    return getTimeSignature() * 60000 / (getBar() - getBeginning());
}

/**
 * @brief LogicalTrack::getFirstChord
 * @return Un pointeur vers le premier accord du morceau.
 */
TrackChord* LogicalTrack::getFirstChord()
{
    return getPartTrackList()[0]->getTrackChordsList()[0];
}
