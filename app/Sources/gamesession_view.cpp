#include "Headers/gamesession_view.h"
#include <QPainter>
#include <QPaintEngine>
#include <QPaintEvent>
#include <QMessageBox>
#include <QTimer>

GameSessionView::GameSessionView(QWidget* mainWindowView, GameSessionModel* gameSessionModel):
    _mainWindowView(mainWindowView),
    _gameSessionModel(gameSessionModel),
    ui(new Ui::GameSessionUI){}

void GameSessionView::start()
{
    _viewData = new ViewData(_gameSessionModel);
    _viewData->tileSize = (576 / _gameSessionModel->GetTableSize());
    if(_viewData->tileSize > 60)
        _viewData->tileSize = 60;

    ui->setupUi(this);
    ui->gameTable->setMinimumSize(_gameSessionModel->GetTableSize()*_viewData->tileSize,_gameSessionModel->GetTableSize()*_viewData->tileSize);
    int minwidth = ui->gameTable->width()+100;
    setMinimumSize(minwidth,ui->gameTable->height()+130);
    setMaximumSize(minwidth,ui->gameTable->height()+130);

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    init();
    show();
}

GameSessionView::~GameSessionView()
{
    disconnect(ui->closeButton,SIGNAL(clicked()),this,SLOT(backToMainMenu()));
    disconnect(ui->gameTable,SIGNAL(TileClicked(int,int)),this,SLOT(processMove(int,int)));

    delete _viewData;
    delete _gameSessionModel;
    delete ui;
}

void GameSessionView::init()
{
    _gameSessionModel->init();
    GameStats stats = _gameSessionModel->GetGameStats();
    ui->gameTable->setViewData(_viewData);
    ui->stepCounter->display(stats.StepCount);
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(backToMainMenu()));
    connect(ui->gameTable,SIGNAL(TileClicked(int,int)),this,SLOT(processMove(int,int)));
    connect(_gameSessionModel,SIGNAL(TimeUpdated(int)),this,SLOT(updateTime(int)));
    connect(_gameSessionModel,SIGNAL(GameWon()),this,SLOT(gameWon()));
    this->setFocus();
}



void GameSessionView::updateTime(int time)
{
    ui->timeCounter->display(time);
}


void GameSessionView::backToMainMenu()
{
    if(!_gameSessionModel->IsGameWon())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,"Kilépés","Biztosan ki akarsz lépni?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply != QMessageBox::Yes)
        {
            return;
        }
    }
    close();
    delete this;
}

void GameSessionView::updateWindow()
{
    GameStats stat = _gameSessionModel->GetGameStats();
    ui->stepCounter->display(stat.StepCount);
    updateTime(stat.GameTime);
    update();
}

void GameSessionView::processMove(int posX, int posY)
{
    if(_gameSessionModel->MakeMove(posX, posY))
    {
        updateWindow();
    }
}

void GameSessionView::gameWon()
{
        GameStats gameStats = _gameSessionModel->GetGameStats();
        QMessageBox* message = new QMessageBox();
        message->setText(QString("Nyertél! %1 másodperc alatt teljesítetted a játékot, %2 lépésből!").arg(gameStats.GameTime).arg(gameStats.StepCount));
        message->show();
        connect(message,SIGNAL(finished(int)),SLOT(backToMainMenu()));
}

void GameSessionView::processMove(Direction dir)
{
    Position pos = _gameSessionModel->GetPlayerPosition();
    switch(dir)
    {
        case Up: pos.Y--; break;
        case Down: pos.Y++; break;
        case Right: pos.X++; break;
        case Left: pos.X--; break;
    }

    this->processMove(pos.X,pos.Y);
}

void GameSessionView::processPause()
{
    if(_gameSessionModel->IsPaused())
    {
        _gameSessionModel->SetPaused(false);
    }
    else
    {
        _gameSessionModel->SetPaused(true);
    }
}

void GameSessionView::keyPressEvent(QKeyEvent * event)
{
    auto key = event->key();
    switch(key)
    {
        case Qt::Key_W:
        case Qt::Key_Up:
            processMove(Up);
            break;
        case Qt::Key_A:
        case Qt::Key_Left:
            processMove(Left);
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            processMove(Right);
            break;
        case Qt::Key_S:
        case Qt::Key_Down:
            processMove(Down);
            break;
        case Qt::Key_P:
        case Qt::Key_Escape:
            processPause();
        break;
        default: break;
    }
}
