#include "mainwindow.h"
#include <QMessageBox>
#include <QGraphicsView>
#include "ui_mainwindow.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    srand(time(NULL));

    x = 10;
    y = 5;

    scene = new QGraphicsScene();
    pixmap = new QPixmap(470, 260);
    item = new QGraphicsPixmapItem(*pixmap);

    scene->addItem(item);

    ui->graphics->setScene(scene);
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

    // temporarily using this to test removing the front square
    scene->removeItem(scene->items().at(0));
}

void MainWindow::up(){
    y--;
    drawPoint();
}

void MainWindow::down(){
    y++;
    drawPoint();
}

void MainWindow::left(){
    x--;
    drawPoint();
}

void MainWindow::right(){
    x++;
    drawPoint();
}

void MainWindow::drawPoint(){
    // each object relies on previous to instantiate
    // qpainter, qpixmap, qgraphicspixmapitem, scene, qgraphicsview


    // add some logic to see if the point is already drawn on
    // or out of bounds
    // or a goal square


    painter = new QPainter(pixmap);
    painter->fillRect(x*10, y*10, 10, 10, Qt::blue);
    painter->end();
    item = new QGraphicsPixmapItem(*pixmap);
    scene->addItem(item);

    //scene->removeItem(scene->items().at(0));
    std::cout << scene->items().length() << std::endl;
}

void MainWindow::showGoal(){
    int x1 = rand() % 48;
    int y1 = rand() % 27;

    painter = new QPainter(pixmap);
    painter->fillRect(x1*10, y1*10, 10, 10, Qt::red);
    painter->end();
    item = new QGraphicsPixmapItem(*pixmap);
    scene->addItem(item);
}
