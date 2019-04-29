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

void moveThread::run() {
    while(1){
        while(s->paused){
            usleep(s->speed);
        }
        mutex->lock();
        switch(s->d){
            case 0:
                s->y--;
                win->drawSnake();
                break;
            case 1:
                s->x--;
                win->drawSnake();
                break;
            case 2:
                s->y++;
                win->drawSnake();
                break;
            case 3:
                s->x++;
                win->drawSnake();
                break;
        }
        mutex->unlock();
        usleep(s->speed);
    }
}



timeThread::timeThread(QMutex* m, MainWindow* w, state* s){
    mutex = m;
    win = w;
    shared = s;
}

bool timeThread::inSnake(){
    for(int i = 0; i < shared->xs.size(); i++){
        if(shared->px == shared->xs.at(i) && shared->py == shared->ys.at(i)){
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
        if(shared->checker){
            resetApple(win,1);
            resetTimer();
        }
        mutex->unlock();

        while(shared->paused){
            usleep(shared->speed);
            if(!shared->paused)
                resetTimer();
        }
    }
}




timer_t timerid;
struct sigevent sev;
struct itimerspec trig;
bool checker = false;


void thread_handler_temp(sigval sv){
    printf("thread handler");
}

void timeThread::thread_handler(sigval_t sv) {
    QMutexLocker locker(mutex);
    shared->checker = true;
}

void timeThread::makeTimer(){
    QMutexLocker locker(mutex);
    memset(&sev, 0, sizeof(struct sigevent));
    memset(&trig, 0, sizeof(struct itimerspec));
    sev.sigev_notify = SIGEV_THREAD;

    sev.sigev_notify_function = &thread_handler_temp;
    timer_create(CLOCK_REALTIME, &sev, &timerid);
    trig.it_value.tv_sec = 10;
    timer_settime(timerid, 0, &trig, NULL);

    shared->checker = false;
}

void timeThread::resetApple(MainWindow* w, int boo){
    QMutexLocker locker(mutex);
    if(!(shared->x == shared->px && shared->y == shared->py)){
        // old pellet position will be colored over
        win->drawPoint(shared->px, shared->py, qRgb(0, 0, 0));
    }else{
        shared->score = shared->score+1;
        if(shared->score%10 == 0){
            shared->difficulty++;
            if(shared->difficulty < 3)
                shared->speed = shared->speed-50000;
        }
        win->updateDifficulty();
    }

    do{
        shared->px = rand() % 48;
        shared->py = rand() % 27;
    }while(inSnake());

    int col2 = rand()%3*10;
    int col = rand()%3*60;
    win->drawPoint(shared->px, shared->py, qRgb(col2+225, col+50, col+50));
}

void timeThread::resetTimer(){
    timer_delete(timerid);
    makeTimer();
}
