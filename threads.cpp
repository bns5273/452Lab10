#include "threads.h"
#include "mainwindow.h"
#include <time.h>
#include <signal.h>
#include <pthread.h>


moveThread::moveThread(QMutex* m, MainWindow* w, state* s){
    this->s = s;
    win = w;
    mutex = m;
}

void moveThread::drawSnake(){
    QMutexLocker locker(mutex);

    // out of bounds?
    if (s->x > 46 || s->x < 0 || s->y > 26 || s->y < 0){
    //        QMessageBox::information(this, tr("You Lose!"), tr("Out of bounds!"));
        QMetaObject::invokeMethod(win, "newGame");
    }

    // already taken by snake?
    for (int i = 0; i < s->xs.size() -1; i++){
        if (s->xs.at(i) == s->x && s->ys.at(i) == s->y){
    //            QMessageBox::information(this, tr("You Lose!"), tr("Stop hitting yourself!"));
            QMetaObject::invokeMethod(win, "newGame");
            break;
        }
    }
    ///////////////////////////////////////////////////////////////////////
    //                       COLLISSION WITH PELLET                      //
    ///////////////////////////////////////////////////////////////////////
    // we hit the pellet
    if (s->x == s->px && s->y == s->py){
        s->xs.push_back(s->x);
        s->ys.push_back(s->y);

        // old pellet position will be colored over
        win->drawPoint(s->px, s->py, qRgb(244, 66, 244)); //colors snake body as if it ate the pellet
//        QMetaObject::invokeMethod(win, "drawPoint", Qt::DirectConnection, Q_ARG(int, s->px), Q_ARG(int, s->py), Q_ARG(QRgb, qRgb(244, 66, 244)));
        //resetTimer();
        s->checker = true;
    }


    // blank space
    else {
        win->drawPoint(s->xs.at(0), s->ys.at(0), qRgb(0, 0, 0));
//        QMetaObject::invokeMethod(win, "drawPoint", Qt::DirectConnection, Q_ARG(int, s->xs.at(0)), Q_ARG(int, s->ys.at(0)), Q_ARG(QRgb, qRgb(0, 0, 0)));

        s->xs.push_back(s->x);
        s->ys.push_back(s->y);

        s->xs.erase(s->xs.begin());
        s->ys.erase(s->ys.begin());
        int x2,y2;
        x2 = s->xs.size()-2;
        y2 = s->ys.size()-2;

        win->drawPoint(s->x, s->y, qRgb(244, 66, 244));                   //colors snake head
//        QMetaObject::invokeMethod(win, "drawPoint", Qt::DirectConnection, Q_ARG(int, s->x), Q_ARG(int, s->y), Q_ARG(QRgb, qRgb(244, 66, 244)));
        win->drawPoint(s->xs.at(x2), s->ys.at(y2), qRgb(0, 255, 0));     //colors snake body
//        QMetaObject::invokeMethod(win, "drawPoint", Qt::DirectConnection, Q_ARG(int, s->xs.at(x2)), Q_ARG(int, s->ys.at(y2)), Q_ARG(QRgb, qRgb(0, 255, 0)));

    }
}

void moveThread::run() {
    while(1){
        while(s->paused){
            usleep(s->speed);
        }
        mutex->lock();
        switch(s->d){
            case 0:
                s->y--;
                drawSnake();
                break;
            case 1:
                s->x--;
                drawSnake();
                break;
            case 2:
                s->y++;
                drawSnake();
                break;
            case 3:
                s->x++;
                drawSnake();
                break;
        }
        mutex->unlock();
        usleep(s->speed);
    }
}



timeThread::timeThread(QMutex* m, MainWindow* w, state* s){
    mutex = m;
    win = w;
    this->s = s;
}

bool timeThread::inSnake(){
    QMutexLocker locker(mutex);
    for(int i = 0; i < s->xs.size(); i++){
        if(s->px == s->xs.at(i) && s->py == s->ys.at(i)){
            printf("IN SNEK\n");
            return true;
        }
    }
    printf("NOT IN SNEK\n");
    return false;
}

void timeThread::run(){
    makeTimer();
    while(1){
        mutex->lock();
        if(s->checker){
            resetApple(win,1);
            resetTimer();
        }
        mutex->unlock();

        while(s->paused){
            usleep(s->speed);
            if(!s->paused)
                resetTimer();
        }
    }
}




timer_t timerid;
struct sigevent sev;
struct itimerspec trig;
bool checker = false;


void thread_handler_temp(sigval sv){

}

void timeThread::thread_handler(sigval_t sv) {
    QMutexLocker locker(mutex);
    s->checker = true;
}

void timeThread::makeTimer(){
    QMutexLocker locker(mutex);
    memset(&sev, 0, sizeof(struct sigevent));
    memset(&trig, 0, sizeof(struct itimerspec));
    sev.sigev_notify = SIGEV_THREAD;

    // this isn't working right...
    // cant use a member function here!
    sev.sigev_notify_function = &thread_handler_temp;
    timer_create(CLOCK_REALTIME, &sev, &timerid);
    trig.it_value.tv_sec = 10;
    timer_settime(timerid, 0, &trig, NULL);

    s->checker = false;
}

void timeThread::resetApple(MainWindow* w, int boo){
    QMutexLocker locker(mutex);
    if(!(s->x == s->px && s->y == s->py)){
        // old pellet position will be colored over
        win->drawPoint(s->px, s->py, qRgb(0, 0, 0));
//        QMetaObject::invokeMethod(win, "drawPoint", Qt::DirectConnection, Q_ARG(int, s->px), Q_ARG(int, s->py), Q_ARG(QRgb, qRgb(0, 0, 0)));
    }else{
        s->score = s->score+1;
        if(s->score%10 == 0){
            s->difficulty++;
            if(s->difficulty < 3)
                s->speed = s->speed-50000;
        }
//        win->updateDifficulty();
        QMetaObject::invokeMethod(win, "updateDifficulty");
    }

    do{
        s->px = rand() % 48;
        s->py = rand() % 27;
    }while(inSnake());

    int col2 = rand()%3*10;
    int col = rand()%3*60;
    win->drawPoint(s->px, s->py, qRgb(col2+225, col+50, col+50));
//    QMetaObject::invokeMethod(win, "drawPoint", Qt::DirectConnection, Q_ARG(int, s->px), Q_ARG(int, s->py), Q_ARG(QRgb, qRgb(col2+225, col+50, col+50)));
}

void timeThread::resetTimer(){
    timer_delete(timerid);
    makeTimer();
}
