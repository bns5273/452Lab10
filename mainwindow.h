#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsPixmapItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    QGraphicsScene *scene;
    QPainter *painter;
    QPixmap *pixmap;
    QGraphicsPixmapItem *item;

    int x;
    int y;
    void drawPoint();
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
