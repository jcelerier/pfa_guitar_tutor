#include <QString>
#include <QtTest>
#include "../Track/LogicalTrack.h"
#include "../Track/TrackLoader.h"

class APITest : public QObject
{
    Q_OBJECT
    
public:
    APITest();
    
private Q_SLOTS:
    void testConvertXmlToLogical();
};

APITest::APITest()
{
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

}

QTEST_APPLESS_MAIN(APITest)

#include "APITest.moc"
