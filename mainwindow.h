#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    QImage imgScreen;

    // current position
    int x;
    int y;

    // current direction
    int d;

    std::vector<int> xs;
    std::vector<int> ys;
    int px;
    int py;

    void drawSnake();
    void drawPoint(int x, int y, QRgb color);
    void drawGreen(int x, int y);
    void keyPressEvent(QKeyEvent *k);

    ~MainWindow();
    
private slots:
    void newGame();
    void pause();
    void up();
    void right();
    void down();
    void left();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
