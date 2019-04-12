#include "mainwindow.h"
#include <QApplication>
#include <csignal>
#include <iostream>
#include <unistd.h>
#include <vector>


void * movement(void* w){
    MainWindow* win = (MainWindow*) w;
    int* x = &win->x;
    int* y = &win->y;

    while(1){
        switch(win->d){
            case 0:
                *y = *y - 1;
                win->drawSnake();
                break;
            case 1:
                *x = *x - 1;
                win->drawSnake();
                break;
            case 2:
                *y = *y + 1;
                win->drawSnake();
                break;
            case 3:
                *x = *x + 1;
                win->drawSnake();
                break;
        }
        usleep(200000);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // create thread for movement
    pthread_attr_t attr;
    struct sched_param sp;
    pthread_attr_init(&attr);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedparam(&attr, &sp);
    pthread_t threadProducer0;
    pthread_create(&threadProducer0, &attr, movement, (void*) &w);

    return a.exec();
}


