#include "mainwindow.h"
#include <QApplication>
#include <csignal>
#include <iostream>
#include <unistd.h>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}


