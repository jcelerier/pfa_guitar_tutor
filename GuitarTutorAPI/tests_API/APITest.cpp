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
    QVERIFY2(result, "Failure");
    qDebug() << "la fonction de test à terminée" << endl;


}

QTEST_APPLESS_MAIN(APITest)

#include "APITest.moc"
