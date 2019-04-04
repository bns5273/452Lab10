#include "mainwindow.h"
#include <QMessageBox>
#include <QGraphicsView>
#include "ui_mainwindow.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

/*
 * we need separate vectors for the snake and the pellet. was a bad idea!
 * extending the snake needs implemented
 * randomly initizalizing the snake to 3 squares needs done
 * newGame needs done better
 *
*/


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    imgScreen = QImage(480, 270, QImage::Format_RGB888);
    imgScreen.fill(Qt::black);
    ui->screen->setPixmap(QPixmap::fromImage(imgScreen));
    ui->screen->repaint();


    srand(time(NULL));

    x = rand() % 48;
    y = rand() % 27;

    // pellet
    xs.push_back(rand() % 48);
    ys.push_back(rand() % 27);
    zs.push_back(1);

    xs.push_back(x);
    ys.push_back(y);
    zs.push_back(0);

    drawSnake();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::newGame(){
    x = 10;
    y = 5;
}

void MainWindow::pause(){
    // no need to implement yet

}

void MainWindow::up(){
    y--;
    drawSnake();
}

void MainWindow::down(){
    y++;
    drawSnake();
}

void MainWindow::left(){
    x--;
    drawSnake();
}

void MainWindow::right(){
    x++;
    drawSnake();
}


void MainWindow::drawSnake(){

    // add some logic to see if the point is already drawn on
    // or out of bounds
    // or a goal square

    for (int i = 0; i < xs.size() -1; i++){
        if (xs.at(i) == x && ys.at(i) == y){
            // snake!
            if (zs.at(i) == 0){
                QMessageBox::information(this, tr("hit snake"), tr("You Lose!"));
            }

            // pellet
            else if (zs.at(i) == 1){
                zs.at(i) = 0;           // food pixel becomes snake
                xs.push_back(x);
                ys.push_back(y);
                zs.push_back(0); // 0 for snake, 1 for pellet,
                std::cout << "PELLET" << std::endl;
            }
        }
        // blank space
        else {
            drawPoint(xs.at(0), ys.at(0), qRgb(0, 0, 0));
            std::cout << xs.at(0) << " " << ys.at(0) << std::endl;

            xs.push_back(x);
            ys.push_back(y);
            zs.push_back(0);

            xs.erase(xs.begin());
            ys.erase(ys.begin());
            zs.erase(zs.begin());
        }
    }


    // draw snake + pellet
    for (int k = 0; k < xs.size(); k++){
        if (zs.at(k) == 0){
            drawPoint(xs.at(k), ys.at(k), qRgb(0, 255, 0));
        }
        else{
            drawPoint(xs.at(k), ys.at(k), qRgb(255, 0, 0));
        }
    }
}

void MainWindow::drawPoint(int x0, int y0, QRgb color){
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            imgScreen.setPixel(x0*10 + i, y0*10 + j, color);
        }
    }

    ui->screen->setPixmap(QPixmap::fromImage(imgScreen));
    ui->screen->repaint();
}
