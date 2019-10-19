#include "Headers/tableview.h"
#include <QtMath>

TableView::TableView(QWidget* parent): QWidget(parent)
{
}


void TableView:: setViewData(ViewData* viewData)
{
   _viewData = viewData;
    int tableSize = _viewData->tableSize;
    int tileSize = _viewData->tileSize;
   setMinimumSize(tableSize*tileSize+10,tableSize*tileSize+10);
   setBaseSize(tableSize*tileSize,tableSize*tileSize);
}


void TableView::paintEvent(QPaintEvent*)
{
    int tableSize = _viewData->tableSize;
    int tileSize = _viewData->tileSize;
    Position playerPos = _viewData->GetPlayerPosition();
    const QVector<Position>* positions = _viewData->GetGameStats().VisibleFields;

    setMinimumSize(tableSize*tileSize,tableSize*tileSize);
    setBaseSize(tableSize*tileSize,tableSize*tileSize);

    QPainter painter(this);

    QPen tilePen;
    tilePen.setWidth(1);
    tilePen.setColor(QColor(30,40,40));

    painter.setPen(tilePen);

    QColor bgColor;
    bgColor.setRgb(0,0,0);

    QColor activeColor;
    activeColor.setRgb(50,60,85,100);


    painter.fillRect(0,0,tableSize*tileSize,tableSize*tileSize,bgColor);

    foreach(Position pos, *positions)
    {
        GameField value = _viewData->GetFieldAt(pos.Y,pos.X);
        float dist = qMax(qAbs(pos.X - playerPos.X), qAbs(pos.Y - playerPos.Y)) / (float)(_viewData->GetLightDistance()+1);
        int opacity = 255 - (255*dist);
        switch(value)
        {
            case Path: activeColor.setRgb(160,100,50,opacity); break;
            case Exit: activeColor.setRgb(200,190,220,opacity); break;
            case Wall: activeColor.setRgb(60,20,5,opacity); break;
            case Start: activeColor.setRgb(140,140,210,opacity); break;
            default: qDebug("Invalid position at %d - %d", pos.Y,pos.X); continue;
        }
        painter.fillRect(pos.X*tileSize, pos.Y*tileSize, tileSize,tileSize, activeColor);
    }

    painter.drawText(playerPos.X*tileSize, playerPos.Y*tileSize,tileSize,tileSize,Qt::AlignCenter,QString("P"));

    /*for (int i=0; i<tableSize; i++)
    {
        for (int j=0; j<tableSize; j++)
        {
            GameField value = _viewData->getFieldAt(i,j);
            if(value != 0)
            {
                painter.fillRect(j*tileSize,i*tileSize,tileSize,tileSize,activeColor);
                painter.drawText(j*tileSize,i*tileSize,tileSize, tileSize,Qt::AlignCenter, QString::number(value));
            }
            painter.drawRect(j*tileSize,i*tileSize,tileSize,tileSize);
        }
    }*/

}

void TableView::mousePressEvent(QMouseEvent* event)
{
    int posX = event->x() / _viewData->tileSize;
    int posY = event->y() / _viewData->tileSize;

    emit TileClicked(posX,posY);
}

TableView::~TableView()
{
}
