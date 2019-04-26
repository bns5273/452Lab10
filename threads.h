#ifndef THREADS_H
#define THREADS_H
#include <QtCore>
#include <mainwindow.h>


class moveThread : public QThread {
public:
    moveThread(QMutex* m, MainWindow* w, state* s);
    // overriding the QThread's run() method
    void run();

    // variable that mutex protects
    state* s;

    MainWindow* win;

    // mutex
    QMutex* mutex;
};

class timeThread: public QThread {
public:
    timeThread(QMutex* m, MainWindow* w, state* s);
    void resetApple(MainWindow* w, int boo);
    void resetTimer();
    bool inSnake();
    void thread_handler(sigval_t sv);
    void makeTimer();
    void run();

    state* shared;
    MainWindow* win;
    QMutex* mutex;
};

class logicThread: public QThread {
public:
    logicThread(QMutex* m, MainWindow* w, state* s);

    void run();

    state* shared;
    MainWindow* win;
    QMutex* mutex;

};

#endif // THREADS_H
