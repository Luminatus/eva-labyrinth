#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H
#include <QVector>
#include <QPair>

enum GameField{
    Start = 0,
    Path = 1,
    Wall = 2,
    Exit = 3,
    Invalid = 4
};

enum Direction{
    Up = 0,
    Right = 1,
    Down = 2,
    Left = 3
};


struct Position
{
    int X;
    int Y;
    Position(int x, int y):X(x),Y(y){}
    Position():X(0),Y(0){}
    bool operator==(Position pos){
        return X == pos.X && Y == pos.Y;
    }
    Position operator+(Position pos)
    {
        return Position(X+pos.X, Y+pos.Y);
    }
};

struct PositionMatrix
{
    int mx[2][2];
    PositionMatrix(int (&data)[2][2])
    {
        mx[0][0] = data[0][0];
        mx[0][1] = data[0][1];
        mx[1][0] = data[1][0];
        mx[1][1] = data[1][1];
    }
    PositionMatrix():mx{{0,0},{0,0}}{}
    Position transform(Position pos)
    {
        Position ret;
        ret.X = pos.X * mx[0][0] + pos.Y * mx[1][0];
        ret.Y = pos.X * mx[0][1] + pos.Y * mx[1][1];
        return ret;
    }
};

struct GameStats{
    const int StepCount;
    const int GameTime;
    const QVector<Position>* VisibleFields;
    GameStats(int stepCount, int gameTime, QVector<Position>* visibleFields) :
        StepCount(stepCount),
        GameTime(gameTime),
        VisibleFields(visibleFields)
    {}
};

#endif // DATASTRUCTURES_H
