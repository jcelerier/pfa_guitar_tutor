#include "Enrichment.h"

QStringList Enrichment::listOfEnrichmentString = QStringList() <<"M"<<"m"<<"+"<<"-"<<"sus2"<<"sus4"<<"b5"<<"6"<<"7"<<"9"<<"11"<<"13";

/**
 * @brief Enrichment::Enrichment
 *
 * Constructeur par défaut.
 */
Enrichment::Enrichment()
{
    initStringEquivs();
}

/**
 * @brief Enrichment::Enrichment
 * @param enrich Enrichissement
 *
 * Constructeur par recopie.
 */
Enrichment::Enrichment(const Enrichment& enrich)  : QList<e_Enrichment>(enrich)
{
    //   initStringEquivs();
    /* est-ce nécessaire ?
    while(!enrich.isEmpty())
    {
        append(enrich.takeFirst());
    }*/
}

/**
 * @brief Enrichment::Enrichment
 * @param enrich Enrichissement
 *
 * Crée un enrichissement à partir d'une chaîne de caractères.
 */
Enrichment::Enrichment(const QString &enrich)
{
    m_stringEquivs = extractEnrichmentsFromStr(enrich).getEnrichmentStringList();

}

/**
 * @brief Enrichment::Enrichment
 * @param enrichList Enrichissement
 *
 * Crée un enrichissement à partir d'une liste de chaîne de caractères.
 */
Enrichment::Enrichment(const QStringList enrichList)
{
    QString e;
    Enrichment ebis;
    //    initStringEquivs();
    foreach(e, enrichList)
        for(int i = 0; i < (ebis = extractEnrichmentsFromStr(e)).size(); i++ ) //aurait été plus propre avec un deuxième foreach
            append(ebis[i]); //attention si la liste est mal formée
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
 * @todo à approfondir. Ex: pas de C#Mmsus2sus4776b5... cf site sur le wiki
 */
bool Enrichment::isValid() const
{
    bool validity = false;
    for(int i = 0; i < size(); i++)
        validity = (0 <= at(i) && at(i) <= NUM_ENRICHMENTS);

    return validity;
}

/**
 * @brief Enrichment::isValid
 * @param enrichment Enrichissement
 * @return Vrai si est seulement si l'enrichissement est valide.
 *
 * Indique si un enrichissement provenant d'une chaîne de caractères est valide ou non.
 */
bool Enrichment::isValid(const QString& enrichment) const
{
    return extractEnrichmentsFromStr(enrichment).isValid();
}


bool Enrichment::isEmpty() const{
    return m_stringEquivs.isEmpty();
}
/**
 * @brief Enrichment::extractEnrichmentsFromStr
 * @param str Chaîne de caractères à partir de laquelle faire l'extraction
 * @return L'enrichissement extrait.
 *
 * Extrait un enrichissement à partir d'une chaîne de caractères. Attention la chaine Bm7sus2 retournera la liste
 * "m" "sus2" "7"(l'ordre a changé). En effet, la liste retourné est toujours classé dans l'ordre :
 * M,m,+,-,sus2,sus4, b5,6,7,9,11,13. Et ceux quelque soit l'ordre de la chaine initial.
 */
const Enrichment Enrichment::extractEnrichmentsFromStr(QString const &str_enr) const
{
    Enrichment e;
    QString str = str_enr;


    //Si la chaine n'a pas d'enrichissement il ne faut pas essayer d'accèder à des cases vides.
    if (str.size() < 2){
        return e;
    }
    else if(str.size() == 1 && (str.at(0) == '#' || str.at(0) == 'b')) {
        return e;
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
        }
    }
    return e;
}

/**
 * @brief Enrichment::getEnrichmentStringList
 * @return L'enrichissement sous forme de liste de chaînes de caractères.
 */
const QStringList& Enrichment::getEnrichmentStringList() const
{
    return m_stringEquivs;
}

const QStringList& Enrichment::getListOfEnrichmentStringList() const
{
	return listOfEnrichmentString;
}

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
