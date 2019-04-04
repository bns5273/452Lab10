#include "mainwindow.h"
#include <QMessageBox>
#include <QGraphicsView>
#include "ui_mainwindow.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

/*
 *
 * randomly initizalizing the snake to 3 squares needs done
 *
*/


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
        imgScreen = QImage(480, 270, QImage::Format_RGB888);
        ui->screen->setPixmap(QPixmap::fromImage(imgScreen));
        ui->screen->repaint();
        srand(time(NULL));
        newGame();
}

MainWindow::~MainWindow()
{
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
    x = rand() % 48;
    y = rand() % 27;
    px = rand() % 48;
    py = rand() % 27;
    drawPoint(px, py, qRgb(255, 0, 0));

    xs.push_back(x);
    ys.push_back(y);

    drawSnake();
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
        std::cout << xs.at(0) << " " << ys.at(0) << std::endl;

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
    ui->screen->repaint();
}
