#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QPushButton>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    QPushButton*start;
    void init();
    void startGame();
};

#endif // DIALOG_H
