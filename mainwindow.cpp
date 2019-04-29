#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>
#include <QtCore>
#include <string>


void MainWindow::keyPressEvent(QKeyEvent *k){
    QMutexLocker locker(mutex);
    int c = k->key();
    switch(c){
        case Qt::Key_Up:
            s->d = 0;
            break;
        case Qt::Key_Left:
            s->d = 1;
            break;
        case Qt::Key_Down:
            s->d = 2;
            break;
        case Qt::Key_Right:
            s->d = 3;
            break;
    }
}

MainWindow::MainWindow(QWidget *parent, state* s, QMutex* m) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // locks the mutex for the duration of the function run-time
    // NOT THE DURATION OF THE MAINWINDOW INSTANCE!
    QMutexLocker locker(mutex);

    // WITHOUT THIS THE ARROW KEYS ARE NOT CAUGHT BY keypressevent()
    QWidget::grabKeyboard();

    this->s = s;
    mutex = m;

    s->d = 0;
    s->imgScreen = QImage(480, 270, QImage::Format_RGB888);

    // setup ui
    ui->setupUi(this);

    ui->screen->setPixmap(QPixmap::fromImage(s->imgScreen));
    ui->screen->repaint();
    srand(time(NULL));
    newGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::pause(){
    //QMessageBox MessageBox;
    //QMessageBox::information(this, tr("Game Paused"), tr("Click "PAUSE" button again to unpause"));
    s->paused = !s->paused;
    if(s->paused){
        ui->Pause->setText("Resume");
    }else{
        ui->Pause->setText("Pause");
    }
}

void MainWindow::up(){
    QMutexLocker locker(mutex);
    s->d = 0;
}

void MainWindow::down(){
    QMutexLocker locker(mutex);
    s->d = 2;
}

void MainWindow::left(){
    QMutexLocker locker(mutex);
    s->d = 1;
}

void MainWindow::right(){
    QMutexLocker locker(mutex);
    s->d = 3;
}

void MainWindow::drawGreen(){
    QMutexLocker locker(mutex);
    s->xs.push_back(s->x);
    s->ys.push_back(s->y);
    drawPoint(s->x, s->y, qRgb(0, 255, 0));
//    QMetaObject::invokeMethod(win, "drawPoint", Qt::DirectConnection, Q_ARG(int, s->x), Q_ARG(int, s->y), Q_ARG(QRgb, qRgb(0, 255, 0)));
}

void MainWindow::newGame(){
    QMutexLocker locker(mutex);
    //makeTimer();
    ui->Pause->setText("Pause");


    s->paused = false;
    s->imgScreen.fill(Qt::black);
    s->score = 0;   //reset score for next game
    s->difficulty = 0;  //reset diff
    s->speed = 200000;  //reset speed
    s->checker = true;
    updateDifficulty();
    //srand(time(NULL));
    s->xs.clear();
    s->ys.clear();
    //calc snake head
    s->x = rand() % 44+2;
    s->y = rand() % 23+2;



    //calc pellet and draw
    s->px = rand() % 48;
    s->py = rand() % 27;
    int col2 = rand()%3*10;
    int col = rand()%3*60;

    drawPoint(s->px, s->py, qRgb(col2+225, col+50, col+50));
//    QMetaObject::invokeMethod(win, "drawPoint", Qt::DirectConnection, Q_ARG(int, s->px), Q_ARG(int, s->py), Q_ARG(QRgb, qRgb(col2+225, col+50, col+50)));

    s->xs.push_back(s->x);
    s->ys.push_back(s->y);

    drawPoint(s->x, s->y, qRgb(244, 66, 244));
//    QMetaObject::invokeMethod(win, "drawPoint", Qt::DirectConnection, Q_ARG(int, s->x), Q_ARG(int, s->y), Q_ARG(QRgb, qRgb(244, 66, 244)));
    int ran = rand()%6;
    switch(ran){
        case 0:
            if(s->y > 24){
                s->y++;
                drawGreen();
                s->y++;
                drawGreen();
                s->d = 2;
            } else{
                s->y--;
                drawGreen();
                s->y--;
                drawGreen();
                s->d = 0;
            }
            break;
        case 1:
            if(s->x < 15){
                s->x++;
                drawGreen();
                s->x++;
                drawGreen();
                s->d = 3;
            } else{
                s->x--;
                drawGreen();
                s->x--;
                drawGreen();
                s->d = 1;
            }
            break;
        case 2:
            s->y++;
            drawGreen();
            if(s->x > 15){
                s->x--;
                drawGreen();
                s->d = 1;
            } else{
                s->x++;
                drawGreen();
                s->d = 3;
            }
            break;
        case 3:
            s->y--;
            drawGreen();
            if(s->x > 15){
                s->x--;
                drawGreen();
                s->d = 1;
            } else{
                s->x++;
                drawGreen();
                s->d = 3;
            }
            break;
        case 4:
            s->y--;
            drawGreen();
            if(s->x > 15){
                s->x--;
                drawGreen();
                s->d = 1;
            } else{
                s->x++;
                drawGreen();
                s->d = 3;
            }
            break;
        default:
            s->y++;
            drawGreen();
            if(s->x > 15){
                s->x--;
                drawGreen();
                s->d = 1;
            } else{
                s->x++;
                drawGreen();
                s->d = 3;
            }
            break;
    }
}

void MainWindow::drawPoint(int x0, int y0, QRgb color){
    QMutexLocker locker(mutex);
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            s->imgScreen.setPixel(x0*10 + i, y0*10 + j, color);
        }
    }

    ui->screen->setPixmap(QPixmap::fromImage(s->imgScreen));

    // WE NEED TO USE THE UPDATE() FUNCTION INSTEAD OF REPAINT() SO THAT
    // NEW PIXELS ARE ADDED TO A QUEUE
    // AND ONLY 1 THREAD WILL REPAINT.
    ui->screen->update();
}

void MainWindow::updateDifficulty(){
    QMutexLocker locker(mutex);
    QString qstr = "0000000000";
    QString qstr1 = "E";
    qstr.setNum(s->score);
    ui->label_3->setText(qstr);

    if(s->difficulty == 0){
        ui->label_4->setText("E");
    } else if(s->difficulty == 1){
        ui->label_4->setText("M");
    } else{
        ui->label_4->setText("H");
    }
}
