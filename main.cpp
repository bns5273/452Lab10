#include "mainwindow.h"
#include <threads.h>
#include <csignal>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <semaphore.h>
#include <QtCore>


int main(int argc, char *argv[])
{
    QMutex mutex(QMutex::Recursive);
    state shared;

    QApplication a(argc, argv);
    MainWindow w(nullptr, &shared, &mutex);
    w.show();

//    create thread for movement
    moveThread movement(&mutex, &w, &shared);
    movement.start();

//    create thread for timing
    timeThread time(&mutex, &w, &shared);
    time.start();

    return a.exec();
}
