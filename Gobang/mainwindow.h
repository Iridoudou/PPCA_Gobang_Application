#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPushButton>
#include<QFrame>
#include "gamemodel.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
int clickPosRow,clickPosCol;
bool selectPos;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QPushButton*exitGame;
    bool isPlayerFirst;
    Gobang*game;
    QFrame*frame;//背景板


private:
    Ui::MainWindow *ui;

    //绘制
    void paintEvent(QPaintEvent*event);
    //初始化
    void initGame();
    void endGame();

    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void chessOneByPerson();
    void chessOneByAi();
    void createButton();
};

#endif // MAINWINDOW_H
