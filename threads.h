#ifndef THREADS_H
#define THREADS_H
#include <QtCore>
#include <mainwindow.h>


extern volatile bool checker;

class moveThread : public QThread {
public:
    moveThread(QMutex* m, MainWindow* w, state* s);
    void run();

private:
    state* s;
    MainWindow* win;
    QMutex* mutex;
    void drawSnake();
};

class logicThread: public QThread {
public:
    logicThread(QMutex* m, MainWindow* w, state* s);
    void run();

private:
    state* s;
    MainWindow* win;
    QMutex* mutex;

    void resetApple(MainWindow* w, int boo);
    void resetTimer();
    bool inSnake();
    void makeTimer();
};

#endif // THREADS_H
