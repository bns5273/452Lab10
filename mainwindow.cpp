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
            shared->d = 0;
            break;
        case Qt::Key_Left:
            shared->d = 1;
            break;
        case Qt::Key_Down:
            shared->d = 2;
            break;
        case Qt::Key_Right:
            shared->d = 3;
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

    shared = s;
    mutex = m;

//    mutex->lock();
    shared->d = 0;
    shared->imgScreen = QImage(480, 270, QImage::Format_RGB888);
//    mutex->unlock();

    // setup ui
    ui->setupUi(this);

    ui->screen->setPixmap(QPixmap::fromImage(shared->imgScreen));
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
    shared->paused = !shared->paused;
    if(shared->paused){
        ui->Pause->setText("Resume");
    }else{
        ui->Pause->setText("Pause");
    }
}

void MainWindow::up(){
    QMutexLocker locker(mutex);
    shared->d = 0;
}

void MainWindow::down(){
    QMutexLocker locker(mutex);
    shared->d = 2;
}

void MainWindow::left(){
    QMutexLocker locker(mutex);
    shared->d = 1;
}

void MainWindow::right(){
    QMutexLocker locker(mutex);
    shared->d = 3;
}

void MainWindow::drawPoint(int x0, int y0, QRgb color){
    QMutexLocker locker(mutex);
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            shared->imgScreen.setPixel(x0*10 + i, y0*10 + j, color);
        }
    }

    ui->screen->setPixmap(QPixmap::fromImage(shared->imgScreen));

    // WE NEED TO USE THE UPDATE() FUNCTION INSTEAD OF REPAINT() SO THAT
    // NEW PIXELS ARE ADDED TO A QUEUE
    // AND ONLY 1 THREAD WILL REPAINT.
    ui->screen->update();
}


void MainWindow::newGame(){
    QMutexLocker locker(mutex);
    //makeTimer();
    ui->Pause->setText("Pause");

    shared->paused = false;
    shared->imgScreen.fill(Qt::black);
    shared->score = 0;   //reset score for next game
    shared->difficulty = 0;  //reset diff
    shared->speed = 200000;  //reset speed
    shared->checker = true;
    updateDifficulty();
    //srand(time(NULL));
    shared->xs.clear();
    shared->ys.clear();
    //calc snake head
    shared->x = rand() % 44+2;
    shared->y = rand() % 23+2;



    //calc pellet and draw
    shared->px = rand() % 48;
    shared->py = rand() % 27;
    int col2 = rand()%3*10;
    int col = rand()%3*60;
    drawPoint(shared->px, shared->py, qRgb(col2+225, col+50, col+50));

    shared->xs.push_back(shared->x);
    shared->ys.push_back(shared->y);

    drawPoint(shared->x, shared->y, qRgb(244, 66, 244));
    int ran = rand()%6;
    switch(ran){
        case 0:
            if(shared->y > 24){
                shared->y++;
                drawGreen();
                shared->y++;
                drawGreen();
                shared->d = 2;
            } else{
                shared->y--;
                drawGreen();
                shared->y--;
                drawGreen();
                shared->d = 0;
            }
            break;
        case 1:
            if(shared->x < 15){
                shared->x++;
                drawGreen();
                shared->x++;
                drawGreen();
                shared->d = 3;
            } else{
                shared->x--;
                drawGreen();
                shared->x--;
                drawGreen();
                shared->d = 1;
            }
            break;
        case 2:
            shared->y++;
            drawGreen();
            if(shared->x > 15){
                shared->x--;
                drawGreen();
                shared->d = 1;
            } else{
                shared->x++;
                drawGreen();
                shared->d = 3;
            }
            break;
        case 3:
            shared->y--;
            drawGreen();
            if(shared->x > 15){
                shared->x--;
                drawGreen();
                shared->d = 1;
            } else{
                shared->x++;
                drawGreen();
                shared->d = 3;
            }
            break;
        case 4:
            shared->y--;
            drawGreen();
            if(shared->x > 15){
                shared->x--;
                drawGreen();
                shared->d = 1;
            } else{
                shared->x++;
                drawGreen();
                shared->d = 3;
            }
            break;
        default:
            shared->y++;
            drawGreen();
            if(shared->x > 15){
                shared->x--;
                drawGreen();
                shared->d = 1;
            } else{
                shared->x++;
                drawGreen();
                shared->d = 3;
            }
            break;
    }
}

void MainWindow::drawGreen(){
    QMutexLocker locker(mutex);
    shared->xs.push_back(shared->x);
    shared->ys.push_back(shared->y);
    drawPoint(shared->x, shared->y, qRgb(0, 255, 0));
}
void MainWindow::updateDifficulty(){
    QMutexLocker locker(mutex);
    QString qstr = "0000000000";
    QString qstr1 = "E";
    qstr.setNum(shared->score);
    ui->label_3->setText(qstr);

    if(shared->difficulty == 0){
        ui->label_4->setText("E");
    } else if(shared->difficulty == 1){
        ui->label_4->setText("M");
    } else{
        ui->label_4->setText("H");
    }
}

void MainWindow::drawSnake(){
    QMutexLocker locker(mutex);

    // out of bounds?
    if (shared->x > 46 || shared->x < 0 || shared->y > 26 || shared->y < 0){
//        QMessageBox::information(this, tr("You Lose!"), tr("Out of bounds!"));
        newGame();
    }

    // already taken by snake?
    for (int i = 0; i < shared->xs.size() -1; i++){
        if (shared->xs.at(i) == shared->x && shared->ys.at(i) == shared->y){
//            QMessageBox::information(this, tr("You Lose!"), tr("Stop hitting yourself!"));
            newGame();
            break;
        }
    }
    ///////////////////////////////////////////////////////////////////////
    //                       COLLISSION WITH PELLET                      //
    ///////////////////////////////////////////////////////////////////////
    // we hit the pellet
    if (shared->x == shared->px && shared->y == shared->py){
        shared->xs.push_back(shared->x);
        shared->ys.push_back(shared->y);

        // old pellet position will be colored over
        drawPoint(shared->px, shared->py, qRgb(244, 66, 244)); //colors snake body as if it ate the pellet
        //resetTimer();
        shared->checker = true;
    }


    // blank space
    else {
        drawPoint(shared->xs.at(0), shared->ys.at(0), qRgb(0, 0, 0));

        shared->xs.push_back(shared->x);
        shared->ys.push_back(shared->y);

        shared->xs.erase(shared->xs.begin());
        shared->ys.erase(shared->ys.begin());
        int x2,y2;
        x2 = shared->xs.size()-2;
        y2 = shared->ys.size()-2;

        drawPoint(shared->x, shared->y, qRgb(244, 66, 244));                   //colors snake head
        drawPoint(shared->xs.at(x2), shared->ys.at(y2), qRgb(0, 255, 0));     //colors snake body

    }
}

