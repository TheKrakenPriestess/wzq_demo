﻿#include"mainwindow.h"
#include <QApplication>

//建模

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.getGameType(AI);
    w.show();
    return a.exec();
}
