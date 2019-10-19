#include "Headers/mainwindow_model.h"
#include "Headers/datastructures.h"
#include <QTextStream>
#include <QDir>
#include <QString>
#include <QRegExp>
#include <QDebug>

MainWindowModel::MainWindowModel()
{
    _fieldCodes = initFieldCodes();
}

MainWindowModel::~MainWindowModel()
{
    if(_gameSessionModel != NULL)
    {
        delete _gameSessionModel;
    }
}

 QMap<QString, GameField> MainWindowModel::initFieldCodes()
{
     QMap<QString, GameField> map;
     map.insert("P", Path);
     map.insert("W", Wall);
     map.insert("E", Exit);
     map.insert("S", Start);

     return map;
}

void MainWindowModel::InitGame(uint n, uint light_dist)
{
    bool ret = _InitializeGameSession(n, light_dist);

    if(!ret) emit InitFailed();
    else emit GameReady(_gameSessionModel);
}

void MainWindowModel::LoadMaps()
{

    QDir directory("Maps");
    QStringList maps = directory.entryList(QStringList() << "*.map",QDir::Files);
    foreach(QString filename, maps) {
        _loadMapFromFile("Maps/"+filename);
    }
}

QVector<GameMap*> MainWindowModel::GetMaps()
{
    return _gameMaps;
}

void MainWindowModel::_loadMapFromFile(QString filename)
{
    QFileInfo fileInfo(filename);
    QString name = fileInfo.baseName();

    QFile file(filename);
    if (file.exists())
    {
        file.open(QIODevice::ReadOnly|QIODevice::Text);
        QTextStream stream(&file);
        QString map_buffer = stream.readAll();
        GameMap* map = GameMap::createFromString(map_buffer, name, _fieldCodes);
        if(map != NULL)
        {
            map->solve();
            if(map->isSolvable())
            {
                _gameMaps.push_back(map);
            }
            else
            {
                delete map;
            }
        }
    }

    return;
}

bool MainWindowModel::_InitializeGameSession(uint n, uint light_dist)
{
    if(light_dist<2)
        return false;
    if(n < _gameMaps.length())
    {
        _gameSessionModel = new GameSessionModel(_gameMaps[n], light_dist);
    }
    return true;
}

/*
bool MainWindowModel::loadGame(int tableSize, int redHP, int blueHP, Position redPos, Position bluePos)
{
    if(tableSize != Small && tableSize != Medium && tableSize != Large)
        return false;
    if(redHP < 1 || blueHP < 1 || redHP > 3 || blueHP > 3)
        return false;
    if(redPos.X <0 || redPos.X >= tableSize || redPos.Angle < 0 || redPos.Angle > 3
    || bluePos.X <0 || bluePos.X >= tableSize || bluePos.Angle < 0 || bluePos.Angle > 3)
        return false;
    _gameSessionModel = new GameSessionModel(tableSize);
    emit GameReady(_gameSessionModel);

    return true;


}
*/

