#include "Headers/gamesession_test.h"
#include <QtTest>

void GameSessionModelTest::testValidSteps()
{
    //starting from (0,2)
    Position playerPosition = _model->GetPlayerPosition();
    bool res;
    qDebug("Initial position (%d,%d)", playerPosition.X, playerPosition.Y);

    res = _model->MakeMove(0,1);
    playerPosition = _model->GetPlayerPosition();
    QVERIFY(
                res &&
                playerPosition.X == 0 &&
                playerPosition.Y == 1
    );

    res = _model->MakeMove(1,1);
    playerPosition = _model->GetPlayerPosition();
    QVERIFY(
                res &&
                playerPosition.X == 1 &&
                playerPosition.Y == 1
    );

    res = _model->MakeMove(0,1);
    playerPosition = _model->GetPlayerPosition();
    QVERIFY(
                res &&
                playerPosition.X == 0 &&
                playerPosition.Y == 1
    );

    res = _model->MakeMove(0,2);
    playerPosition = _model->GetPlayerPosition();
    QVERIFY(
                res &&
                playerPosition.X == 0 &&
                playerPosition.Y == 2
    );
}

void GameSessionModelTest::testOutOfBoundsStep()
{
    bool res = _model->MakeMove(-1,2);
    Position playerPosition = _model->GetPlayerPosition();
    QVERIFY(!res && playerPosition.X == 0 && playerPosition.Y == 2);
}


void GameSessionModelTest::testInvalidStep()
{
    bool res = _model->MakeMove(0,0);
    Position playerPosition = _model->GetPlayerPosition();
    QVERIFY(!res && playerPosition.X == 0 && playerPosition.Y == 2);
}

void GameSessionModelTest::testStepInPlace()
{
    bool res = _model->MakeMove(0,2);
    Position playerPosition = _model->GetPlayerPosition();
    QVERIFY(!res && playerPosition.X == 0 && playerPosition.Y == 2);
}

void GameSessionModelTest::testStepIntoWall()
{
    bool res = _model->MakeMove(1,2);
    Position playerPosition = _model->GetPlayerPosition();
    QVERIFY(!res && playerPosition.X == 0 && playerPosition.Y == 2);
}

void GameSessionModelTest::TestWinning()
{
    QVERIFY(!_model->IsGameWon());
    _model->MakeMove(0,1);
    QVERIFY(!_model->IsGameWon());
    _model->MakeMove(0,0);
    QVERIFY(!_model->IsGameWon());
    _model->MakeMove(1,0);
    QVERIFY(!_model->IsGameWon());
    _model->MakeMove(2,0);

    QVERIFY(_model->IsGameWon());
}


QTEST_APPLESS_MAIN(GameSessionModelTest)

