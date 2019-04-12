#include "mainwindow.h"
#include <QMessageBox>
#include <QGraphicsView>
#include "ui_mainwindow.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>
#include <QKeyEvent>


void MainWindow::keyPressEvent(QKeyEvent *k){
    int c = k->key();
    switch(c){
        case Qt::Key_Up:
            d = 0;
            break;
        case Qt::Key_Left:
            d = 1;
            break;
        case Qt::Key_Down:
            d = 2;
            break;
        case Qt::Key_Right:
            d = 3;
            break;
    }
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // TODO
    // need to fix default direction. I think byrnes measured the longest side of the
    // snake and set the direction perpendicular to that axis
    d = 0;

    // WITHOUT THIS THE ARROW KEYS ARE NOT CAUGHT BY keypressevent()
    QWidget::grabKeyboard();

    // setup ui
    ui->setupUi(this);
    imgScreen = QImage(480, 270, QImage::Format_RGB888);
    ui->screen->setPixmap(QPixmap::fromImage(imgScreen));
    ui->screen->repaint();
    srand(time(NULL));
    newGame();
}

MainWindow::~MainWindow()
{
    pthread_exit(NULL);

    delete ui;
}

void MainWindow::newGame(){


//    imgScreen = QImage(480, 270, QImage::Format_RGB888);
    imgScreen.fill(Qt::black);
//    ui->screen->setPixmap(QPixmap::fromImage(imgScreen));
//    ui->screen->repaint();


    //srand(time(NULL));
    xs.clear();
    ys.clear();
    //calc snake head
    x = rand() % 44+2;
    y = rand() % 23+2;

    //calc pellet and draw
    px = rand() % 48;
    py = rand() % 27;
    drawPoint(px, py, qRgb(255, 0, 0));

    xs.push_back(x);
    ys.push_back(y);

    drawPoint(x, y, qRgb(0, 255, 0));
    int ran = rand()%6;
    //std::cout << ran << "\n";
    switch(ran){
        case 0:
        if(y > 24){
            y++;
            drawGreen(x,y);
            y++;
            drawGreen(x,y);
        } else{
            y--;
            drawGreen(x,y);
            y--;
            drawGreen(x,y);
        }
        break;
        case 1:
        if(x < 15){
            x++;
            drawGreen(x,y);
            x++;
            drawGreen(x,y);
        } else{
            x--;
            drawGreen(x,y);
            x--;
            drawGreen(x,y);
        }
        break;
        case 2:
        y++;
        drawGreen(x,y);
        if(x > 15){
            x--;
            drawGreen(x,y);
        } else{
            x++;
            drawGreen(x,y);
        }
        break;
        case 3:
            y--;
            drawGreen(x,y);
            if(x > 15){
                x--;
                drawGreen(x,y);
            } else{
                x++;
                drawGreen(x,y);
            }
        break;
        case 4:
            y--;
            drawGreen(x,y);
            if(x > 15){
                x--;
                drawGreen(x,y);
            } else{
                x++;
                drawGreen(x,y);
            }
        break;
        default:
            y++;
            drawGreen(x,y);
            if(x > 15){
                x--;
                drawGreen(x,y);
            } else{
                x++;
                drawGreen(x,y);
            }
        break;
    }
}

void MainWindow::drawGreen(int x, int y){
    xs.push_back(x);
    ys.push_back(y);
    drawPoint(x, y, qRgb(0, 255, 0));
}

void MainWindow::pause(){
    // TODO
}

void MainWindow::up(){
    d = 0;
}

void MainWindow::down(){
    d = 2;
}

void MainWindow::left(){
    d = 1;
}

void MainWindow::right(){
    d = 3;
}


void MainWindow::drawSnake(){
    std::cout << d << std::endl;

    // out of bounds?
    if (x > 46 || x < 0 || y > 26 || y < 0){
        QMessageBox::information(this, tr("You Lose!"), tr("Out of bounds!"));
        newGame();
    }

    // already taken by snake?
    for (int i = 0; i < xs.size() -1; i++){
        if (xs.at(i) == x && ys.at(i) == y){
            QMessageBox::information(this, tr("You Lose!"), tr("Stop hitting yourself!"));
            newGame();
        }
    }

    // pellet?
    if (x == px && y == py){
        xs.push_back(x);
        ys.push_back(y);

        // old pellet position will be colored over
        drawPoint(px, py, qRgb(0, 255, 0));
        px = rand() % 48;
        py = rand() % 27;
        drawPoint(px, py, qRgb(255, 0, 0));

        std::cout << "PELLET" << std::endl;
    }

    // blank space
    else {
        drawPoint(xs.at(0), ys.at(0), qRgb(0, 0, 0));

        xs.push_back(x);
        ys.push_back(y);

        xs.erase(xs.begin());
        ys.erase(ys.begin());

        drawPoint(x, y, qRgb(0, 255, 0));
    }
}

void MainWindow::drawPoint(int x0, int y0, QRgb color){
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            imgScreen.setPixel(x0*10 + i, y0*10 + j, color);
        }
    }

    ui->screen->setPixmap(QPixmap::fromImage(imgScreen));

    // WE NEED TO USE THE UPDATE() FUNCTION INSTEAD OF REPAINT() SO THAT
    // NEW PIXELS ARE ADDED TO A QUEUE
    // AND ONLY THE MAIN THREAD WILL REPAINT. THIS MEANS THERE IS NO NEED
    // FOR MANAGING A SHARED RESOURCE
    ui->screen->update();
}
