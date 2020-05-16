#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setMaximumHeight(498);
    w.setMinimumHeight(498);
    w.setMaximumWidth(498);
    w.setMinimumWidth(498);
    w.show();
    return a.exec();
}
