#ifndef MAINWINDOWMODEL_H
#define MAINWINDOWMODEL_H

#include <QObject>
#include <QVector>
#include <QMap>
#include "Headers/gamesession_model.h"
#include "Headers/game_map.h"

class MainWindowModel : public QObject
{
    Q_OBJECT
public:
    MainWindowModel();
    ~MainWindowModel();
    void InitGame(uint n, uint light_dist);
    void LoadMaps();
    QVector<GameMap*> GetMaps();
    //bool loadGame(int,int,int,Position,Position);
private:
    GameSessionModel* _gameSessionModel;
    QVector<GameMap*> _gameMaps;
    QMap<QString,GameField> _fieldCodes;

    QMap<QString,GameField> initFieldCodes();
    bool _InitializeGameSession(uint n, uint light_dist);
    void _loadMapFromFile(QString fileName);

signals:
    void InitFailed();
    void GameReady(GameSessionModel*);

};

#endif // MAINWINDOWMODEL_H
