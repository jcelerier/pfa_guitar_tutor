#include "Enrichment.h"

QStringList Enrichment::listOfEnrichmentString = QStringList() <<""<<"m" /*<<"+"<<"-"<<"sus2"<<"sus4"<<"b5"<<"6"<<"7"<<"9"<<"11"<<"13"*/;

/**
 * @brief Enrichment::Enrichment
 *
 * Constructeur par défaut.
 */
Enrichment::Enrichment()
{
	initStringEquivs();
	error = false;
}

/**
 * @brief Enrichment::Enrichment
 * @param enrich Enrichissement
 *
 * Constructeur par recopie.
 */
Enrichment::Enrichment(const Enrichment& enrich)  : QList<e_Enrichment>(enrich)
{

}

/**
 * @brief Enrichment::Enrichment
 * @param enrich Enrichissement
 *
 * Crée un enrichissement à partir d'une chaîne de caractères.
 */
Enrichment::Enrichment(const QString &enrich)
{
	Enrichment ebis;
	error = !extractEnrichmentsFromStr(enrich, ebis);
	m_stringEquivs = ebis.getEnrichmentStringList();

}


/**
 * @brief Enrichment::initStringEquivs
 *
 * Initialisation de la variable m_stringEquivs
 */
void Enrichment::initStringEquivs()
{}

/**
 * @brief Enrichment::isValid
 * @return Vrai si et seulement si l'enrichissement est valide.
 *
 * Indique si un enrichissement est valide.
 */
bool Enrichment::isValid() const
{
	return !error;
}

/**
 * @brief Enrichment::isValid
 * @param enrichment Enrichissement
 * @return Vrai si est seulement si l'enrichissement est valide.
 *
 * Indique si un enrichissement provenant d'une chaîne de caractères est valide ou non.
 */
bool Enrichment::isValid(const QString& enrichment)
{
	Enrichment ebis;
	return Enrichment::extractEnrichmentsFromStr(enrichment, ebis);
}

/**
 * @brief Enrichment::isEmpty
 * @return True si l'enrichissement est vide, false sinon.
 */
bool Enrichment::isEmpty() const{
	return m_stringEquivs.isEmpty();
}

/**
 * @param str_enr : Chaîne de caractères à partir de laquelle faire l'extraction
 * @param e : Un enrichissement qui va etre modifier par effet de bord. Attention à ce qu'il soit vide.
 * @return Un booléen disant si la fonction a réussi.
 *
 * Extrait un enrichissement à partir d'une chaîne de caractères. Attention la chaine Bm7sus2 retournera la liste
 * "m" "sus2" "7"(l'ordre a changé). En effet, la liste retourné est toujours classée dans l'ordre :
 * M,m,+,-,sus2,sus4, b5,6,7,9,11,13. Et ce, quel que soit l'ordre de la chaine initiale.
 */
bool Enrichment::extractEnrichmentsFromStr(QString const &str_enr, Enrichment &e)
{
	QString str = str_enr;

	//Si la chaine n'a pas d'enrichissement il ne faut pas essayer d'accèder à des cases vides.
	if (str.size() < 2){
		return true;
	}
	else if(str.size() == 1 && (str.at(0) == '#' || str.at(0) == 'b')) {
		return true;
	}

	//On enlève les caractères faisant partie de la tonalité
	str.remove(0,1);
	if(str.at(0) == '#' || str.at(0) == 'b'){
		str.remove(0,1);
	}

	//On rajoute les enrichissement trouvés les un après les autres
	for(int i = 0; i < NUM_ENRICHMENTS; i++){
		if(str.contains(listOfEnrichmentString.at(i))){
			e.addEnrichment(listOfEnrichmentString.at(i));
			str.remove(listOfEnrichmentString.at(i));
		}
	}

	if(QString::compare(str, "") == 0){
		return true;
	}
	else
		return false;

}

/**
 * @brief Enrichment::getEnrichmentStringList
 * @return L'enrichissement sous forme de liste de chaînes de caractères.
 *
 * Permet de convertir un Enrichment en une liste chaine de caractères
 * correspondant à chacun des enrichissements.
 */
const QStringList& Enrichment::getEnrichmentStringList() const
{
	return m_stringEquivs;
}

/**
 * @brief Enrichment::getListOfEnrichmentStringList
 * @return La liste des enrichissements possibles dans l'ancienne version.
 *
 * Permet de récupérer la liste de toutes les valeurs d'enrichissements possibles.
 */
const QStringList& Enrichment::getListOfEnrichmentStringList() const
{
	return listOfEnrichmentString;
}

/**
 * @brief Enrichment::addEnrichment
 *
 * Un accord peut avoir plusieurs enrichissements. Un seul objet Enrichment permet de tous
 * les représenter. Cette fonction permet d'ajouter un enrichissement dans la liste
 * correspondant à un accord.
 */
void Enrichment::addEnrichment(const QString &newEnrich){
	m_stringEquivs.append(newEnrich);
}

/**
 * @brief Enrichment::toString
 * @return L'enrichissement en chaîne de caractères.
 *
 * Converti un enrichissement en chaîne de caractères.
 */
QString Enrichment::toString()
{
	QString str = "";
	if(!(m_stringEquivs.isEmpty())){
		for(int i = 0; i < m_stringEquivs.size(); i++){
			str += m_stringEquivs.at(i);
		}
	}
	return str;
}
