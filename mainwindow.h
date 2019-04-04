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

    // snake array
    struct point{
        int x, y;
    };

    std::vector<int> xs;
    std::vector<int> ys;
    std::vector<int> zs;

    void drawSnake();
    void drawPoint(int x, int y, QRgb color);

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
