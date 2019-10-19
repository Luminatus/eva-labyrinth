#include <QString>
#include <QtTest>

class GameSessionModelTest : public QObject
{
    Q_OBJECT

public:
    GameSessionModelTest();

private Q_SLOTS:
    void testCase1();
};

GameSessionModelTest::GameSessionModelTest()
{
}

void GameSessionModelTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(GameSessionModelTest)

#include "tst_gamesessionmodeltest.moc"
