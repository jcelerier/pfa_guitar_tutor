#include "Configuration.h"

int Configuration::m_width = 1920;
int Configuration::m_height = 1080;

float Configuration::m_quotient = float(Configuration::m_height)/float(Configuration::originalHeight); // L'image de base est de hauteur 1080, pour mise a l'echelle

/**
 * @brief Configuration::Configuration
 */
Configuration::Configuration()
{
	m_inputIndex = 0;
	m_outputIndex = 0;
    m_pauseSetting = PAUSE_TO_LAST_CHORD;
    m_loopOnFailure = false;
    m_percentToGoToNextPart = PERCENT_TO_VALIDATE_EASY;
    m_songDirectory = "Tracks";
}

/**
 * @brief Configuration::getQuotient
 * @return Le rapport de mise à l'échelle.
 */
float Configuration::getQuotient()
{
	return Configuration::m_quotient;
}

/**
 * @brief Configuration::getWindowSize
 * @return La taille de la fenetre.
 */
QRect Configuration::getWindowSize()
{
	return QRect(0, 0, Configuration::m_width, Configuration::m_height);
}

/**
 * @brief Configuration::setWindowSize
 * @param w Largeur
 * @param h Hauteur
 *
 * Définie la taille de la fenetre.
 */
void Configuration::setWindowSize(int w, int h)
{
   Configuration::m_width = w;
   Configuration::m_height = h;
   Configuration::m_quotient = float(Configuration::m_width)/float(Configuration::originalWidth);
}

/**
 * @brief Configuration::setWindowSize
 * @param qr Taille de la fenetre
 *
 * Surcharge pour des raisons pratiques.
 */
void Configuration::setWindowSize(QRect qr)
{
   setWindowSize(qr.width(), qr.height());
}

/**
 * @brief Configuration::setSongName
 * @param songName Nom de la chanson
 *
 * Définie le nom de la chanson.
 */
void Configuration::setSongName(QString songName)
{
	m_currentSongName = songName;
}

/**
 * @brief Configuration::getSongName
 * @return Le nom de la chanson.
 */
QString Configuration::getSongName()
{
	return m_currentSongName;
}

/**
 * @brief Configuration::setAudioInput
 * @param audioInput Nom de l'entrée audio
 *
 * Définie l'entrée audio à utiliser.
 */
void Configuration::setAudioInput(QString audioInput)
{
	m_audioInput = audioInput;
}

/**
 * @brief Configuration::getAudioInput
 * @return Le nom de l'entrée audio utilisée.
 */
QString Configuration::getAudioInput()
{
	return m_audioInput;
}

/**
 * @brief Configuration::setAudioOutput
 * @param audioOutput Nom de la sortie audio
 *
 * Définie la sortie audio à utiliser.
 */
void Configuration::setAudioOutput(QString audioOutput)
{
	m_audioOutput = audioOutput;
}

/**
 * @brief Configuration::setAudioOutput
 * @return Le nom de la sortie audio utilisée.
 */
QString Configuration::getAudioOutput()
{
	return m_audioOutput;
}

/**
 * @brief Configuration::getPauseSetting
 * @return Le type de pause.
 */
int Configuration::getPauseSetting() const
{
    return m_pauseSetting;
}

/**
 * @brief Configuration::setPauseSetting
 * @param s Le type de pause
 *
 * Change le type de pause à utiliser.
 */
void Configuration::setPauseSetting(int s)
{
    m_pauseSetting = s;
}

/**
 * @brief Configuration::getLoopSetting
 * @return True si les bouclages sur les parties sont activées, false sinon.
 */
bool Configuration::getLoopSetting() const
{
    return m_loopOnFailure;
}

/**
 * @brief Configuration::setLoopSetting
 * @param b True si les bouclages sur les parties doivent etre activées, false sinon
 *
 * Change le paramèter de bouclage sur les parties.
 */
void Configuration::setLoopSetting(bool b)
{
    m_loopOnFailure = b;
}

/**
 * @brief Configuration::getDifficulty
 * @return La difficulté
 */
int Configuration::getDifficulty() const
{
    return m_percentToGoToNextPart;
}

/**
 * @brief Configuration::setDifficulty
 * @param d Difficulté
 *
 * Modifie la difficulté du jeu.
 */
void Configuration::setDifficulty(int d)
{
    m_percentToGoToNextPart = d;
}

/**
 * @brief Configuration::getSongDirectory
 * @return Le dossier de la chanson actuellement utilisée.
 */
QString Configuration::getSongDirectory() const
{
    return m_songDirectory;
}

/**
 * @brief Configuration::setSongDirectory
 * @param sd Le dossier de la chanson
 *
 * Change le dossier de la chanson.
 */
void Configuration::setSongDirectory(QString sd)
{
    m_songDirectory = sd;
}
