#include "Headers/mainwindow_view.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindowView w;
    w.show();

    return a.exec();
}
