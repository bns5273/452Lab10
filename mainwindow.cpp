#include "mainwindow.h"
#include <QMessageBox>
#include <QGraphicsView>
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    painter = new QPainter(pixmap);
    painter->fillRect(x*10, y*10, 10, 10, Qt::blue);
    painter->end();
    item = new QGraphicsPixmapItem(*pixmap);
    scene->addItem(item);
}
