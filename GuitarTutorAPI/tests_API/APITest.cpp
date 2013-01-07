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
    qDebug() << "test : convertXmlToLogicalTrack... ";
    LogicalTrack * piste = new LogicalTrack();
    QString xmlFileName = "songExample.xml";
    bool result = TrackLoader::convertXmlToLogicalTrack(xmlFileName, piste);
    QVERIFY2(result, "Failure : convertXmlToLogicalTrack");

    QVERIFY2(piste->GetTrackName().compare(""), "Failure : convertXmlToLogicalTrack");
    qDebug() << "ok";

}

QTEST_APPLESS_MAIN(APITest)

#include "APITest.moc"
