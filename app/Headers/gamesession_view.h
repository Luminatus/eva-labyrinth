#ifndef GAMESESSION_VIEW_H
#define GAMESESSION_VIEW_H
#include <QWidget>
#include "ui_gamesession.h"
#include "Headers/gamesession_model.h"
#include "Headers/viewdata.h"

class GameSessionView : public QWidget
{
    Q_OBJECT
public:
    GameSessionView(QWidget* mainWindowView,GameSessionModel* gameSessionModel);
    ~GameSessionView();
    void start();
private:
    void updateWindow();

    QWidget* _mainWindowView;
    GameSessionModel* _gameSessionModel;
    Ui::GameSessionUI* ui;
    ViewData* _viewData;
protected:
    void keyPressEvent(QKeyEvent *);


public slots:
    void backToMainMenu();
    void processMove(int posX, int posY);
    void processMove(Direction);
    void processPause();
    void init();
    void updateTime(int);
    void gameWon();

};

#endif // GAMESESSION_VIEW_H
