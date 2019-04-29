#ifndef MAINWINDOW_H
#define MAINWINDOW_H


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
#include <QMutex>
#include <QSharedMemory>

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <vector>

typedef struct {
    bool checker;
    QImage imgScreen;

    // current position
    int x;
    int y;

    // current direction
    int d;

    // snake
    std::vector<int> xs;
    std::vector<int> ys;

    // pellet
    int px;
    int py;

    //Current score
    int score = 0;

    //difficulty
    int difficulty = 0;

    //speed
    int speed = 200000;

    //pause
    bool paused = false;
} state;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    state* s;
    QMutex* mutex;

    explicit MainWindow(QWidget *parent = nullptr, state* s = nullptr, QMutex* m = nullptr);

    Q_SLOT void drawPoint(int x, int y, QRgb color);

    ////////////////////////
    //      Functions     //
    ////////////////////////
    void drawSnake();
    void keyPressEvent(QKeyEvent *k);

    ~MainWindow();
    
private slots:
    void pause();
    void up();
    void right();
    void down();
    void left();
    void newGame();
    void drawGreen();
    void updateDifficulty();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
