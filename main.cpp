#include "mainwindow.h"
#include "Definitions.h"
#include <QtCore>
#include <QApplication>
#include <iostream>
#include "GameLogic.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.startScreen();
    return a.exec();
}
