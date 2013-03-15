#include <QString>
#include <QtTest>
#include "../Track/LogicalTrack.h"
#include "../Track/TrackLoader.h"
#include "../GuitarTutor/Enrichment.h"

class APITest : public QObject
{
    Q_OBJECT
    
public:
    APITest();
    
private Q_SLOTS:
    void testExtractEnrichmentsFromStr();
    void testExtractTonalityFromStr();
    void testConvertXmlToLogical();
    void testConvertLogicalTrackToXml();
    void testIsValid();
    void testIsValidForPlayer();

private:
    LogicalTrack* initLogicalTrack();

};

APITest::APITest()
{
}

//LogicalTrack* APITest::initLogicalTrack(){

//    return new LogicalTrack;
//}

void APITest::testIsValid(){
    QString chord1 = "A#";
    QString chord2 = "B";
    QString chord3 = "CM";
    QString chord4 = "jfhtf-";
    QString chord5 = "Csus4";
    QString chord6 = "Ebb5";

    QVERIFY2(BasicChord::isValid(chord1), "Chord1 not valid");
    QVERIFY2(BasicChord::isValid(chord2), "Chord2 not valid");
    QVERIFY2(BasicChord::isValid(chord3), "Chord3 not valid");
    QVERIFY2(!BasicChord::isValid(chord4), "Chord4 must not be valid");
    QVERIFY2(BasicChord::isValid(chord5), "Chord5 not valid");
    QVERIFY2(BasicChord::isValid(chord6), "Chord6 not valid");
}

void APITest::testIsValidForPlayer(){
    QString chord1 = "A#";
    QString chord2 = "B";
    QString chord3 = "Cm";
    QString chord4 = "jfhtf-";

    QVERIFY2(BasicChord::isValidForPlayer(chord1), "Chord1 not valid");
    QVERIFY2(BasicChord::isValidForPlayer(chord2), "Chord2 not valid");
    QVERIFY2(BasicChord::isValidForPlayer(chord3), "Chord3 not valid");
    QVERIFY2(!BasicChord::isValidForPlayer(chord4), "Chord4 must not be valid");
}

void APITest::testExtractEnrichmentsFromStr(){

    Enrichment e1("A");
    QVERIFY2(e1.getEnrichmentStringList().isEmpty(), "Empty enrichment is not empty");

    Enrichment e2("B#");
    Enrichment e3("Bb");
    QVERIFY2(e2.getEnrichmentStringList().isEmpty(), "Empty enrichment is not empty");
    QVERIFY2(e3.getEnrichmentStringList().isEmpty(), "Empty enrichment is not empty");


    Enrichment e4("C#m");
    QVERIFY2(!(e4.isEmpty()), "e4 : The result is empty");
    QVERIFY2(!e4.getEnrichmentStringList().at(0).compare("m"), "e4 : Chord not corresponding");

    Enrichment e5("C#M");
    QVERIFY2(!(e5.getEnrichmentStringList().isEmpty()), "e5 : The result is empty");
    QVERIFY2(!e5.getEnrichmentStringList().at(0).compare("M"), "e5 : Chord not corresponding");

    Enrichment e6("C#sus2");
    QVERIFY2(!(e6.getEnrichmentStringList().isEmpty()), "e6 : The result is empty");
    QVERIFY2(!e6.getEnrichmentStringList().at(0).compare("sus2"), "e6 : Chord not corresponding");

    Enrichment e7("Ebm7sus2");
    QVERIFY2(!(e7.getEnrichmentStringList().isEmpty()), "e7 : The result is empty");
    QVERIFY2(!e7.getEnrichmentStringList().at(0).compare("m"), "e7 : Enrichment 1 not corresponding");
    QVERIFY2(!e7.getEnrichmentStringList().at(1).compare("sus2"), "e7 : Enrichment 2 not corresponding");
    QVERIFY2(!e7.getEnrichmentStringList().at(2).compare("7"), "e7 : Enrichment 3 not corresponding");
}

void APITest::testExtractTonalityFromStr(){
    Tonality e1("A");
    QVERIFY2(!e1.toString().compare("A"), "e1 : tonality not corresponding");

    Tonality e2("B#");
    Tonality e3("Bb");
    QVERIFY2(e2.getBaseNote() == B, "e2 : tonality not corresponding");
    QVERIFY2(e3.getBaseNote() == B, "e3 : tonality not corresponding");
    QVERIFY2(e2.getAlteration() == SHARP, "e2 : tonality not corresponding");
    QVERIFY2(e3.getAlteration() == FLAT, "e3 : tonality not corresponding");

    Tonality e4("C#m");
    QVERIFY2(e4.getBaseNote() == C, "e4 : base note not corresponding");
    QVERIFY2(e4.getAlteration() == SHARP, "e4 : Chord not corresponding");

}

void APITest::testConvertXmlToLogical()
{
    LogicalTrack * piste = new LogicalTrack();
    QString xmlFileName = "songExample.xml";
    bool result = TrackLoader::convertXmlToLogicalTrack(xmlFileName, piste);

    QVERIFY2(result, "Failure : convertXmlToLogicalTrack");
    QVERIFY2((piste->getTrackName().compare("NomDuMorceau") == 0 ), "Failure : convertXmlToLogicalTrack");
    QVERIFY2((piste->getArtist().compare("Artiste") == 0), "Failure : convertXmlToLogicalTrack");
    QVERIFY2((piste->getAudioFileName().compare("Beatles.mp3") == 0), "Failure : convertXmlToLogicalTrack");
    QVERIFY2((piste->getMesure()) == 2, "Failure : convertXmlToLogicalTrack");

    QList<PartTrack*> listeParties = piste->getPartTrackList();

    TrackLoader::convertLogicalTrackToXml(piste, "reverse.xml");

}

void APITest::testConvertLogicalTrackToXml(){

    QList<TrackChord*> chords1;
    QList<TrackChord*> chords2;
    QList<TrackChord*> chords3;

    QList<PartTrack*> partTracks;

    chords1.append(new TrackChord("C#m", 1, 1));
    chords1.append(new TrackChord("A#", 1.64525, 1));


    chords2.append(new TrackChord("Db", 1, 2));
    chords2.append(new TrackChord("Db", 1, 2));
    chords2.append(new TrackChord("Db", 1.321, 2));


    chords3.append(new TrackChord("E#m", 1.2, 1));
    chords3.append(new TrackChord("B#M", 0.8, 1));
    chords3.append(new TrackChord("C#", 2.55, 3));


    partTracks.append(new PartTrack("couplet1", chords1));
    partTracks.append(new PartTrack("refrain", chords2));
    partTracks.append(new PartTrack("couplet2", chords3));

    LogicalTrack* piste = new LogicalTrack("numb", "Linkin Park", "numb.xml", 2, partTracks, 10, 10, 10);

    bool result = TrackLoader::convertLogicalTrackToXml(piste, "lolilol.xml");

    QVERIFY2(result, "function failure");




}

QTEST_APPLESS_MAIN(APITest)

#include "APITest.moc"
