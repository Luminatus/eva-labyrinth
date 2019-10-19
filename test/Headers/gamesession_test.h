#ifndef GAMESESSION_TEST_H
#define GAMESESSION_TEST_H

#include "Headers/gamesession_model.h"
#include "Headers/datastructures.h"
#include "Headers/game_map.h"

class GameSessionModelTest : public QObject
{
    Q_OBJECT

private:
    GameSessionModel* _model;
    GameMap* map;
    bool myCondition()
    {
        return true;
    }

private slots:
    void initTestCase()
    {
        QVector<QVector<GameField> >* fields = new QVector<QVector<GameField> >(3);
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                (*fields)[i].push_back(Path);
            }
        }
        (*fields)[0][2] = Exit;
        (*fields)[2][0] = Start;
        (*fields)[2][1] = Wall;

        map = GameMap::createGameMap(fields,"test");
        _model = new GameSessionModel(map,2);
        qDebug("Called before everything else.");
    }

    void init()
    {
        _model->init();
        qDebug("Called before each test");
    }

    void testValidSteps();
    void testStepInPlace();
    void testInvalidStep();
    void testStepIntoWall();
    void testOutOfBoundsStep();
    void TestWinning();

};

#endif // GAMESESSION_TEST_H
