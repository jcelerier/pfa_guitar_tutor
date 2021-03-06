#include "Chord.h"


/**
 * @brief Chord::Chord
 * @param chord Chaîne de caractères à partir de laquelle former l'accord
 *
 * Construitun accord à partir d'une chaîne de caractères.
 */
BasicChord::BasicChord(const QString& chord)
{

	m_tonality = extractTonalityFromStr(chord);
	Enrichment test_enrichment(chord);
	m_enrichment = test_enrichment; //Il existe deux algo différents pour cette fonction.
	//Je ne sais pas encore laquelle des deux est la plus efficace mais j'ai corrigé celle appelé ici.
}

BasicChord::~BasicChord()
{

}

/**
 * @brief Chord::Chord
 * @param chord Accord
 *
 * Constructeur par recopie.
 */
BasicChord::BasicChord(const BasicChord& chord)
{
	m_tonality = chord.getTonality();
	m_enrichment = chord.getEnrichment();
}

/**
 * @brief Chord::Chord
 * @param tonality Tonalité
 * @param enrichment Enrichissement
 *
 * Construit un accord à partir d'une tonalité et d'un enrichissement.
 */
BasicChord::BasicChord(const Tonality &tonality, const Enrichment &enrichment)
{
	m_tonality = Tonality(tonality);
	m_enrichment = Enrichment(enrichment);
}

/**
 * @brief Chord::isValid
 * @return Vrai si est seulement si l'accord est valide.
 *
 * Indique si un accord est valide ou non.
 */
bool BasicChord::isValid() const
{
	return m_tonality.isValid() && m_enrichment.isValid();
}

/**
 * @brief Chord::isValid
 * @param chord Accord
 * @return Vrai si est seulement si l'accord extrait depuis la chaîne de caractères est valide pour le player.
 *
 * Indique si un accord est valide ou non pour le player. C'est à dire qu'il contient simplement "M" comme enrichissement,
 * #.
 */
bool BasicChord::isValidForPlayer(const QString &chord)
{
	QRegExp r("^[A-G][#b]?m?$");
	return (r.exactMatch(chord) || chord.isEmpty());
}

/**
 * @brief Chord::isValid
 * @param chord Accord
 * @return Vrai si est seulement si l'accord extrait depuis la chaîne de caractères est valide.
 *
 * Indique si un accord est valide ou non.
 * Attention, cette fonction accepte tout les accords définis actuellement par l'éditeur(c'est à dire avec 9 types
 * d'enrichissements en plus des dièses et des bémols)
 */
bool BasicChord::isValid(const QString &chord)
{
	return BasicChord::extractTonalityFromStr(chord).isValid() && BasicChord::extractEnrichmentFromStr(chord).isValid();
}

/**
 * @brief Chord::toString
 * @return L'accord sous forme de chaîne de caractères.
 *
 * Convertie un accord en chaîne de caractères.
 */
QString BasicChord::toString()
{
	QString chord = "";
	chord += m_tonality.toString();
	chord += m_enrichment.toString();

	return chord;
}

/**
 * @brief Chord::getEnrichment
 * @return L'enrichissement de l'accord.
 *
 * Extrait l'enrichissement de l'accord.
 */
const Enrichment& BasicChord::getEnrichment() const
{
	return m_enrichment;
}

/**
 * @brief Chord::getTonality
 * @return La tonalité de l'accord.
 *
 * Extrait la tonalité de l'accord.
 */
const Tonality& BasicChord::getTonality() const
{
	return m_tonality;
}

/**
 * @brief Chord::setTonality
 * @param tone Tonalité
 *
 * Modifie la tonalité de l'accord.
 */
void BasicChord::setTonality(Tonality& tone)
{
	m_tonality = Tonality(tone);
}

/**
 * @brief Chord::setTonality
 * @param tone Tonalité
 *
 * Modifie la tonalité de l'accord à partir d'une chaîne de caractères.
 */
void BasicChord::setTonality(const QString& tone)
{
	m_tonality = Tonality(tone);
}

/**
 * @brief Chord::setEnrichment
 * @param enrichment Enrichissement
 *
 * Modifie l'enrichissement de l'accord.
 */
void BasicChord::setEnrichment(Enrichment& enrichment)
{
	m_enrichment = Enrichment(enrichment);
}

/**
 * @brief Chord::setEnrichment
 * @param enrichment Enrichissement
 *
 * Modifie l'enrichissement d'un accord à partir d'une chaîne de caractères.
 */
void BasicChord::setEnrichment(const QString& enrichment)
{
	m_enrichment = Enrichment(enrichment);
}

/**
 * @brief Chord::extractTonalityFromStr
 * @param str Chaine repésentant l'accord
 * @return La tonalité extraite.
 *
 * Extrait la tonalité d'une tonalité ou d'un accord sous forme de chaîne de caractères.
 */
Tonality BasicChord::extractTonalityFromStr(const QString str)
{
	Tonality tone;
	if(str.size() == 0){//Si on lui passe une chaine vide, elle renvoie nul
		qCritical("L'accord reçu est vide");
		return tone;
	}
	else if (str.size() == 1){//On ne doit pas accéder à la case 1 si str est de taille 1
		tone = Tonality(str.left(1));
	}
	if(str.size() > 1){
		if(str.at(1) == 'b' || str.at(1) == '#')//On prend la sous-chaine de 2 ou 1 caractères correspondant à une tonalité valide
			tone = Tonality(str.left(2));
		else
			tone = Tonality(str.left(1));
	}

	return tone;
}

/**
 * @brief Chord::extractEnrichmentFromStr
 * @param str_enr : Accord
 * @return L'enrichissement extrait.
 *
 * Extrait l'enrichissement d'un accord sous forme de chaîne de caractères.
 */
const Enrichment& BasicChord::extractEnrichmentFromStr(QString const str_enr)
{
	Enrichment e(str_enr);
	Enrichment &e2 = e;
	return e2;
}

/**
 * @brief Chord::convertChordToStringList
 * @param chord : Chaine de caractère représentant un accord.
 * @return Une liste d'accord.
 *
 * Convertit une chaine représentant un accord en un liste selon une logique que je ne m'explique pas encore.
 */
QStringList BasicChord::convertChordToStringList(const QString chord)
{
	QStringList result;
	if(chord == "C" || chord == "D" || chord == "F" || chord == "G" || chord == "A")
		result << chord;
	else if(chord == "C#")
		result << "C#" << "Db";
	else if(chord == "D#")
		result << "D#" << "Eb";
	else if(chord == "E")
		result << "E" << "Fb";
	else if(chord == "F#")
		result << "F#" << "Gb";
	else if(chord == "G#")
		result << "G#" << "Ab";
	else if(chord == "A#")
		result << "A#" << "Bb";
	else if(chord == "B")
		result << "B" << "Cb";
	else if(chord == "Cm" || chord == "Dm" || chord == "Fm" || chord == "Gm" || chord == "Am")
		result << chord;
	else if(chord == "C#m")
		result << "C#m" << "Dbm";
	else if(chord == "D#m")
		result << "D#m" << "Ebm";
	else if(chord == "Em")
		result << "Em" << "Fbm";
	else if(chord == "F#m")
		result << "F#m" << "Gbm";
	else if(chord == "G#m")
		result << "G#m" << "Abm";
	else if(chord == "A#m")
		result << "A#m" << "Bbm";
	else if(chord == "Bm")
		result << "Bm" << "Cbm";
	else
		result << "";
	return result;
}
