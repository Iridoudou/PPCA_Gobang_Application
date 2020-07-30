#ifndef MENU_H
#define MENU_H

#include <QDialog>
#include<QPushButton>
#include"mainwindow.h"

namespace Ui {
class menu;
}

class menu : public QDialog
{
    Q_OBJECT

public:
    explicit menu(QWidget *parent = 0);
    ~menu();
    QPushButton*start;
    QPushButton*_start;
    void init();
    void startGame();
    void _startGame();
    MainWindow *w;

private:
    Ui::menu *ui;
};

#endif // MENU_H
