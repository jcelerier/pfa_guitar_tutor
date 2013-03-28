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
 * Constructeur permettant l'initialisialisation correcte de la classe LogicalTrack.
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
 * @param NewTrackName chaîne de caractères
 *
 * Change la valeur de l'attribut TrackName de la classe LogicalTrack
 */
void LogicalTrack::setTrackName(QString newTrackName) {

    m_trackName = newTrackName;

}

/**
 * @brief LogicalTrack::GetTrackName
 *
 * Renvoie la valeur de l'attribut TrackName de la classe LogicalTrack
 */
QString LogicalTrack::getTrackName() {

    return m_trackName;

}

/**
 * @brief LogicalTrack::SetArtist
 * @param NewArtist chaîne de caractères
 *
 * Change la valeur de l'attribut Artist de la classe LogicalTrack
 */
void LogicalTrack::setArtist(QString newArtist) {

    m_artist = newArtist;

}

/**
 * @brief LogicalTrack::GetArstist
 *
 * Renvoie la valeur de l'attribut Artist de la classe LogicalTrack
 */
QString LogicalTrack::getArtist() {

    return m_artist;
}

/**
 * @brief LogicalTrack::SetAudioFileName
 * @param NewFileName chaîne de caractères
 *
 * Change la valeur de l'attribut FileName de la classe LogicalTrack
 */
void LogicalTrack::setAudioFileName(QString newAudioFileName) {

    m_audioFileName = newAudioFileName;

}

/**
 * @brief LogicalTrack::SetBars
 * @param nBar : nouvelle valeur pour bar
 * @param nBeginning : nouvelle valeur pour beginning
 * @param nEnd : nouvelle valeur pour end
 *
 * Change la valeur des attributs bar, beginning et end qui représentent des temps spéciaux pour
 * synchorniser la lecture du morceau audio avec les accords.
 */
void LogicalTrack::setBars(int nBar, int nBeginning, int nEnd){

    m_bar = nBar;
    m_beginning = nBeginning;
    m_end = nEnd;

}

/**
 * @brief LogicalTrack::SetTimeSignature
 * @param
 *
 * Change la valeur de timeSignature
 */
void LogicalTrack::setTimeSignature(int nTS){

    m_timeSignature = nTS;

}

/**
 * @brief LogicalTrack::GetAudioFileName
 *
 * Renvoie la valeur de l'attribut AudioFileName de la classe LogicalTrack
 */
QString LogicalTrack::getAudioFileName() {

    return m_audioFileName;

}

/**
 * @brief LogicalTrack::AddPartTrackToList
 * @param newPartTrack : Partie de moreceau à rajouter
 *
 * Ajoute une PartTrack à la liste des PartTrack de la LogicalTrack
 */
void LogicalTrack::addPartTrackToList(PartTrack* newPartTrack) {

    m_listPartTrack.append(newPartTrack);

}

/**
 * @brief LogicalTrack::getListPartTrack
 *
 * Renvoie un pointeur sur la liste des PartTrack de la LogicalTrack
 */
QList<PartTrack*>& LogicalTrack::getPartTrackList()
{
    return m_listPartTrack;
}

/**
 * @brief LogicalTrack::getBeginning
 *
 * Renvoie le temps correspondant à beginning.
 */
int LogicalTrack::getBeginning(){
    return m_beginning;
}

/**
 * @brief LogicalTrack::getBar
 *
 * Renvoie le temps correspondant à bar.
 */
int LogicalTrack::getBar(){
    return m_bar;
}

/**
 * @brief LogicalTrack::getEnd
 *
 * Renvoie le temps correspondant à end.
 */
int LogicalTrack::getEnd(){
    return m_end;
}

/**
 * @brief LogicalTrack::getEnd
 *
 * Renvoie le temps correspondant à la TimeSignature.
 */
int LogicalTrack::getTimeSignature(){
    return m_timeSignature;
}

/**
 * @brief LogicalTrack::setRep
 *
 * Permet de changer la valeur du nombre d'accords dans une mesure. Si la valeur entrée n'est pas correcte, rep sera mis à -1.
 */
void LogicalTrack::setMesure(int nb){
    if(nb>=0){
        m_chordsPerBar = nb;
    }
    else
        m_chordsPerBar = 1;
}

int LogicalTrack::getChordsPerBar() {

    return this->m_chordsPerBar;
}

void LogicalTrack::setLine(int line)
{
    this->m_line = line;
}


void LogicalTrack::setColumn(int col)
{
    this->m_column = col;
}

int LogicalTrack::getLine()
{
    return m_line;
}


int LogicalTrack::getColumn()
{
    return m_column;
}

QString LogicalTrack::getPartName(int n)
{
    if(n <= m_listPartTrack.size() && n>0){
        return m_listPartTrack[n-1]->getPartName();
    }
    return m_listPartTrack[m_listPartTrack.size()-1]->getPartName();
}

void LogicalTrack::setComment(QString comment)
{
    this->m_comment = comment;
}

QString LogicalTrack::getComment()
{
    return m_comment;
}

int LogicalTrack::getBPM()
{
    return getTimeSignature() * 60000 / (getBar() - getBeginning());
}

TrackChord* LogicalTrack::getFirstChord()
{
    return getPartTrackList()[0]->getTrackChordsList()[0];
}
