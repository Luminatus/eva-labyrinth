#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <QVector>
#include <QMap>
#include "Headers/datastructures.h"

class GameMap{
    public:
        static GameMap* createFromString(QString data, QString name,const QMap<QString,GameField>& code_table);
        static GameMap* createGameMap(QVector<QVector<GameField> >* map, QString name);

        GameField getField(int row,int col);
        GameField getField(Position position);
        QString getName();
        int getSize();
        bool isSolvable();
        bool isAnalised();
        int getMinimumSteps();
        void solve();

    private:
        GameMap(QVector<QVector<GameField> >* map, QString name): _map(map), _name(name){}
        QVector<QVector<GameField> >* _map;
        QString _name;
        bool _isAnalised = false;
        bool _isSolvable = false;
        int _minimumSteps = 0;
};

#endif // GAME_MAP_H
