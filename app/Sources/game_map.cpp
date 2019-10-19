#include "Headers/game_map.h"
#include <QQueue>

struct GameFieldData
{
    GameField field;
    bool isSteppedOn;
    int stepCount = 0;
    GameFieldData* previous;
    GameFieldData(GameField field, GameFieldData* prev = NULL): field(field), isSteppedOn(false), previous(prev){}
};

struct StepPosition : Position
{
    Position movement;
    StepPosition(int x, int y, Position movement): Position(x,y){ this->movement = movement; }
    Position getMovedPosition() { return Position(X + movement.X, Y + movement.Y); }
};

bool GameMap::isAnalised()
{
    return _isAnalised;
}

bool GameMap::isSolvable()
{
    return isAnalised() && _isSolvable;
}

int GameMap::getMinimumSteps()
{
    return (isAnalised() && isSolvable()) ? _minimumSteps : 0;
}

GameMap* GameMap::createFromString(QString data, QString name, const QMap<QString,GameField>& code_table)
{
    data = data.trimmed();
    if(!name.length())
        return NULL;

    if(!code_table.count())
        return NULL;

    int tableSize = 0;
    QVector<QString> rows = data.split('\n').toVector();

    tableSize = rows.length();
    if(!tableSize)
        return NULL;

    //Check table dimensions
    foreach(QString line, rows)
    {
        if(line.length() != tableSize)
        {
            return NULL;
        }
    }

    QVector<QVector<GameField> >* map = new QVector<QVector<GameField> >();

    foreach(QString line, rows)
    {
        QVector<GameField> map_line;
        foreach(QChar field, line)
        {
            if(code_table.contains(field))
            {
                GameField gameField = code_table[field];
                map_line.push_back(gameField);
            }
            else
            {
                delete map;
                return NULL;
            }
        }

        map->push_back(map_line);
    }

    GameMap* gameMap = createGameMap(map,name);
    if(gameMap == NULL)
        delete map;

    return gameMap;

}

GameMap* GameMap::createGameMap(QVector<QVector<GameField> > *map, QString name)
{
    if(!name.length())
        return NULL;

    int size = map->length();

    bool isValid = true;
    bool exit_found = false;
    bool start_found = false;

    foreach(QVector<GameField> row, *map)
    {
        if(row.length() != size)
            return NULL;

        foreach(GameField field, row)
        {
            switch(field)
            {
                case Exit:
                    if(!exit_found)
                    {
                        exit_found = true;
                    }
                    else
                    {
                        isValid = false;
                    }
                    break;
                case Start:
                    if(!start_found)
                    {
                        start_found = true;
                    }
                    else
                    {
                        isValid = false;
                    }
                    break;
                default: break;
            }
            if(!isValid)
            {
                return NULL;
            }
        }
    }

    if(!start_found || !exit_found)
    {
        return NULL;
    }

    //Exit is not in the correct place
    if(map->first().last() != Exit)
    {
        return NULL;
    }

    //Start is not in the correct place
    if(map->last().first() != Start)
    {
        return NULL;
    }

    GameMap* gameMap = new GameMap(map, name);
    return gameMap;
}

void GameMap::solve()
{
    StepPosition startingPosition = StepPosition(0,getSize()-1,Position(0,0));
    QVector<Position> movementPositions = { Position(0,-1), Position(0,1), Position(1,0), Position(-1,0) };

    QVector<QVector<GameFieldData*>> dataMap;
    foreach(QVector<GameField> row, *_map)
    {
        QVector<GameFieldData*> dataRow;
        foreach(GameField field, row)
        {
            dataRow.push_back(new GameFieldData(field));
        }
        dataMap.push_back(dataRow);
    }

    bool isSolved = false;
    QQueue<StepPosition> stepList;
    stepList.push_back(startingPosition);
    while(!isSolved && stepList.length()>0)
    {
        StepPosition stepPos = stepList.first();
        stepList.pop_front();
        Position newPos = stepPos.getMovedPosition();
        if(newPos.X < 0 || newPos.X >= getSize() || newPos.Y < 0 || newPos.Y >= getSize())
        {
            continue;
        }

        GameFieldData* prevField = NULL;
        if(!(newPos == stepPos))
        {
            prevField = dataMap[stepPos.Y][stepPos.X];
        }

        GameFieldData* datafield = dataMap[newPos.Y][newPos.X];
        if(datafield->field == Exit)
        {
            isSolved = true;
            _isSolvable = true;
            _minimumSteps = prevField->stepCount;
            break;
        }
        else if(datafield->field == Wall)
        {
            continue;
        }

        else
        {
            if(!datafield->isSteppedOn)
            {
                datafield->isSteppedOn = true;
            }

            if(prevField != NULL && datafield->stepCount > 0 && datafield->stepCount <= prevField->stepCount+1)
            {
                continue;
            }
            else
            {
                if(prevField == NULL)
                {
                   datafield->stepCount=1;
                }
                else
                {
                    datafield->stepCount = prevField->stepCount + 1;
                    datafield->previous = prevField;
                }
                foreach(Position movementPos, movementPositions)
                {
                    StepPosition nextStepPos = StepPosition(newPos.X, newPos.Y, movementPos);
                    stepList.push_back(nextStepPos);
                }

            }
        }
    }

    foreach(QVector<GameFieldData*> row, dataMap)
    {
        foreach(GameFieldData* field, row)
        {
            delete field;
        }
    }

    _isAnalised = true;
}

GameField GameMap::getField(int row, int col)
{
    if(row>=0 && row<getSize() && col>=0 && col<getSize())
        return (*_map)[row][col];
    else
        return Invalid;
}

GameField GameMap::getField(Position position)
{
    return getField(position.Y, position.X);
}

QString GameMap::getName()
{
    return _name;
}

int GameMap::getSize()
{
    return _map->length();
}


