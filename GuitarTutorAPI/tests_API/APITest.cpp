#include <QString>
#include <QtTest>
#include "../../Track/LogicalTrack.h"
#include "../../Track/TrackLoader.h"

class APITest : public QObject
{
    Q_OBJECT
    
public:
    APITest();
    
private Q_SLOTS:
    void testCase1();
};

APITest::APITest()
{
}

void APITest::testCase1()
{
    LogicalTrack result = convertXmlToLogicalTrack("songExample.xml");
    QVERIFY2(result != 0, "Failure");
    cout << "la fonction de test à terminée" << endl;
}

QTEST_APPLESS_MAIN(APITest)

#include "APITest.moc"
