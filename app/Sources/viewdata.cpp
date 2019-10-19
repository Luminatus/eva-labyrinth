#include "Headers/viewdata.h"


ViewData::ViewData(GameSessionModel *model) : _gameSessionModel(model), tableSize(model->GetTableSize()){}

GameField ViewData::GetFieldAt(int x, int y)
{
    return _gameSessionModel->GetFieldAt(x,y);
}

Position ViewData::GetPlayerPosition()
{
    return _gameSessionModel->GetPlayerPosition();
}

GameStats ViewData::GetGameStats()
{
    return _gameSessionModel->GetGameStats();
}

int ViewData::GetLightDistance() const
{
    return _gameSessionModel->GetLightDistance();
}

