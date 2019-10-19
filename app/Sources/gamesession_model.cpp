#include "Headers/gamesession_model.h"
#include <QLinkedList>

struct LightParams
{
    int Start;
    int Threshold;
    int Remainder;
    int Step;
    bool IsPositiveDirection;
};

struct LightMap
{
    public:
        bool get(int,int);
        void set(int,int,bool);
        LightMap(int);
    private:
        QVector<QVector<bool> > _map;
        int _distance;
};

LightMap::LightMap(int distance): _distance(distance)
{
    int lightMapSize = _distance*2 + 1;
    _map.resize(lightMapSize);
    for(int i=0; i< _map.length(); i++)
    {
        _map[i].resize(lightMapSize);
        _map[i].fill(false);
    }

    _map[_distance][_distance] = true; //player position
}

bool LightMap::get(int row,int col)
{
    row += _distance;
    col += _distance;

    return _map[row][col];
}

void LightMap::set(int row, int col, bool val)
{
    row += _distance;
    col += _distance;

    _map[row][col] = val;

}


GameSessionModel::GameSessionModel(GameMap* map, uint light_dist): _lightDistance(light_dist)
{
    _gameMap = map;
}

void GameSessionModel::init()
{
    if(_gameInitialized)
    {
        endGame();
    }
    _playerPosition = Position(0,_gameMap->getSize()-1);
    _stepCount = 0;
    _gameTime = 0;
    _timer = new QTimer(this);
    _timer->setInterval(1000);

    connect(_timer,SIGNAL(timeout()),this,SLOT(updateTime()));

    _isPaused = false;
    _timer->start();
    _gameInitialized = true;
}

GameSessionModel::~GameSessionModel()
{
    delete _visiblePositions;
    delete _timer;
}

void GameSessionModel::endGame()
{
    if(_timer != NULL)
    {
        disconnect(_timer,SIGNAL(timeout()),this,SLOT(updateTime()));
        delete _timer;
        _timer = NULL;
    }
    if(_visiblePositions != NULL)
    {
        delete _visiblePositions;
        _visiblePositions = NULL;
    }
}

int GameSessionModel::GetTableSize()
{
    return _gameMap->getSize();
}

uint GameSessionModel::GetLightDistance()
{
    return _lightDistance;
}

bool GameSessionModel::IsGameWon()
{
    return _gameMap->getField(_playerPosition) == Exit;
}

GameField GameSessionModel::GetFieldAt(int row, int col)
{
    return _gameMap->getField(row,col);
}

GameField GameSessionModel::GetFieldAt(Position position)
{
    return _gameMap->getField(position);
}

Position GameSessionModel::GetPlayerPosition()
{
    return _playerPosition;
}

bool GameSessionModel::isValidMove(int x, int y)
{
     GameField field = _gameMap->getField(y,x);
     int x_diff = _playerPosition.X - x;
     int y_diff = _playerPosition.Y - y;
     return (
        (field == Path || field == Exit || field == Start) &&
        (x_diff == 0 || y_diff == 0) &&
        (abs(x_diff + y_diff) == 1)
     );
}

bool GameSessionModel::IsPaused()
{
    return _isPaused;
}

void GameSessionModel::SetPaused(bool val)
{
    if(IsGameWon())
        return;

    setPaused(val);
}

void GameSessionModel::setPaused(bool val)
{
    if(!_gameInitialized)
        return;

    bool isPaused = IsPaused();
    if(isPaused != val)
    {
        if(val)
        {
            _remaining =_timer->remainingTime();
            _timer->stop();
            qDebug("Remaining time is %d",_remaining);
            _isPaused = true;
        }
        else
        {
            int offset = _remaining;
            _remaining = 0;
            _isPaused = false;

            if(offset > 0)
            {
                _timer->singleShot(offset,this,SLOT(restartTime()));
            }
            else
            {
                _timer->start();
            }

            //_timer->start();
        }
    }
}

void GameSessionModel::restartTime()
{
    if(!IsPaused())
    {
        updateTime();
        _timer->start();
    }
}

bool GameSessionModel::MakeMove(int x, int y)
{
    if(!IsGameWon() && !IsPaused() && isValidMove(x,y))
    {        
        _playerPosition.X=x; _playerPosition.Y = y;
        _stepCount++;        
        if(_visiblePositions != NULL)
        {
            delete _visiblePositions;
            _visiblePositions = NULL;
        }

        if(IsGameWon())
        {
            _timer->stop();
            _isPaused = true;
            emit GameWon();
        }

        return true;
    }
    else
    {
        return false;
    }
}

/*
void GameSessionModel::GameStep()
{
    if(_activePlayer!=None) return;
    if(_currentCommand==5){ endRound(); return; }
    Command redCommand = _RedPlayer->Memory[_currentCommand];
    Command blueCommand = _BluePlayer->Memory[_currentCommand];
    if(redCommand.Type==blueCommand.Type)
        executeParallelCommands();
    else if(redCommand.Type>blueCommand.Type)
    {
        executeCommand(_RedPlayer);
        executeCommand(_BluePlayer);
    }
    else
    {
        executeCommand(_BluePlayer);
        executeCommand(_RedPlayer);
    }
    _currentCommand++;
    emit MakeStep(redCommand,blueCommand);
}
*/


GameStats GameSessionModel::GetGameStats()
{
    auto visiblePositions =  getVisiblePositions();
    return GameStats(_stepCount, _gameTime, visiblePositions);
}

QVector<Position>* GameSessionModel::getVisiblePositions()
{
    if(_visiblePositions != NULL)
    {
        return _visiblePositions;
    }
    else
    {
        _visiblePositions = new QVector<Position>();

        LightMap lightMap(_lightDistance);
        QVector<PositionMatrix> positionTransformations;

        int mx_params[4][2][2] = { { {1,0},{0,1} },{ {0,1},{1,0} },{ {-1,0},{0,-1} },{ {0,-1},{-1,0} } };
        for(int i=0; i< 4; i++)
        {
            positionTransformations.push_back(PositionMatrix(mx_params[i]));
        }

         /*= {
            PositionMatrix({{1,0},{0,1}}),
            PositionMatrix({{0,1},{1,0}}),
            PositionMatrix({{-1,0},{0,-1}}),
            PositionMatrix({{0,-1},{-1,0}})
        };*/

        foreach(PositionMatrix trans, positionTransformations)
        {
            QLinkedList<QPair<int,QLinkedList<LightParams> > > processQueue;

            QLinkedList<LightParams> lightParams;
            QLinkedList<LightParams> left;
            QLinkedList<LightParams> right;
            for(int i=1; i<=_lightDistance; i++)
            {
                for(int j=0; j<2; j++)
                {
                    LightParams param;
                    param.Remainder=0;
                    param.Start=1;
                    param.Threshold=_lightDistance;
                    param.Step = i+1;
                    param.IsPositiveDirection = (bool)(j%2);
                    lightParams.push_back(param);
                }

            }

            bool blocked = false;
            for(int i=1; i<=_lightDistance && !blocked; i++)
            {
                QMutableLinkedListIterator<LightParams> it(lightParams);


                Position offset = trans.transform(Position(0,i));
                lightMap.set(offset.Y,offset.X,true);
                GameField field = _gameMap->getField(_playerPosition+offset);
                if(field == Wall)
                {
                    blocked = true;
                }

                while(it.hasNext())
                {
                    LightParams param = it.next();
                    param.Remainder+= param.Step;

                    if(blocked)
                    {
                        it.remove();
                    }
                    else
                    {
                        if(param.Remainder >= param.Threshold)
                        {
                            param.Remainder %= param.Threshold;
                            param.Start=i;

                            if(param.IsPositiveDirection)
                                right.push_back(param);
                            else
                                left.push_back(param);
                            //lightParams.erase(it);
                            it.remove();
                        }
                        it.setValue(param);
                    }
                }
            }

            processQueue.append(QPair<int,QLinkedList<LightParams> >(-1,left));
            processQueue.append(QPair<int,QLinkedList<LightParams> >(1,right));

            while(!processQueue.empty())
            {
                auto queueElem = processQueue.first();
                processQueue.pop_front();
                int col_offset = queueElem.first;
                QLinkedList<LightParams> params = queueElem.second;
                QLinkedList<LightParams> started;
                QLinkedList<LightParams> finished;
                int index = abs(col_offset);

                while(!(params.empty() && started.empty()) && index<=_lightDistance)
                {
                    bool local_blocked = false;
                    QMutableLinkedListIterator<LightParams> it(params);
                    while(it.hasNext())
                    {
                        LightParams p = it.next();
                        if(index >= p.Start)
                        {
                            started.append(p);
                            it.remove();
                        }
                    }

                    if(!started.empty())
                    {
                        Position offset = trans.transform(Position(col_offset,index));
                        GameField field = _gameMap->getField(_playerPosition+offset);
                        if(field != Invalid)
                        {
                            lightMap.set(offset.Y,offset.X,true);
                        }
                        else
                        {
                            qDebug("invalid field at %d-%d",col_offset,index);
                        }

                        if(field == Wall || field == Invalid)
                        {
                            local_blocked = true;
                        }
                    }

                    it = QMutableLinkedListIterator<LightParams>(started);
                    while(it.hasNext())
                    {
                        LightParams p = it.next();
                        if(true || p.Start != index) //not added in this iteration
                        {
                            if(local_blocked)
                            {
                                it.remove();
                            }
                            else
                            {
                                if(p.Start != index)
                                    p.Remainder+=p.Step;
                                if(p.Remainder >= p.Threshold)
                                {
                                    p.Remainder %= p.Threshold;
                                    p.Start = index;
                                    finished.append(p);
                                    it.remove();
                                    //started.erase(it);
                                }
                                it.setValue(p);
                            }
                        }

                    }

                    index++;
                }

                if(!finished.empty())
                {
                    int next_col = finished.first().IsPositiveDirection ? col_offset+1 : col_offset-1;
                    processQueue.append(QPair<int,QLinkedList<LightParams> >(next_col,finished));
                }

            }
        }

        for(int i=-_lightDistance; i<=_lightDistance; i++)
        {
            for(int j=-_lightDistance; j<=_lightDistance; j++)
            {
                if(lightMap.get(i,j))
                {
                    _visiblePositions->push_back(Position(_playerPosition.X+j,_playerPosition.Y+i));
                }
            }
        }

        //temporarily return all positions
        /*for(int i=0; i<_gameMap->getSize(); i++)
        {
            for(int j=0; j<_gameMap->getSize(); j++)
            {
                Position pos(j,i);
                _visiblePositions->push_back(pos);
            }
        }*/

        return _visiblePositions;
    }

}

void GameSessionModel::updateTime()
{
    _gameTime++;
    emit TimeUpdated(_gameTime);
}

