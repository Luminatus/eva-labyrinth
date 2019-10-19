#ifndef GAMESESSIONMODEL_H
#define GAMESESSIONMODEL_H
#include <QObject>
#include <QVector>
#include <QTimer>
#include "Headers/datastructures.h"
#include "Headers/game_map.h"


class GameSessionModel : public QObject
{
    Q_OBJECT
public:
    GameSessionModel(GameMap* map, uint light_dist);
    ~GameSessionModel();
    void init();
    int GetTableSize();
    uint GetLightDistance();
    bool IsGameWon();
    bool IsPaused();
    void SetPaused(bool);
    GameStats GetGameStats();
    Position GetPlayerPosition();
    GameField GetFieldAt(int,int);
    GameField GetFieldAt(Position);
    bool MakeMove(int x,int y);
private:
    bool isValidMove(int x,int y);
    void endGame();
    void setPaused(bool);
    QVector<Position>* getVisiblePositions();

    const int _lightDistance;
    GameMap* _gameMap;
    QTimer* _timer;
    bool _gameInitialized = false;
    bool _isPaused = true;
    int _gameTime;
    int _remaining;
    int _stepCount;
    Position _playerPosition;
    QVector<Position>* _visiblePositions = NULL;

signals:
    void TimeUpdated(int);
    void GameWon();
private slots:
    void updateTime();
    void restartTime();
};

#endif // GAMESESSIONMODEL_H
