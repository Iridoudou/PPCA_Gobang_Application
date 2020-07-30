#include "MainWindow.h"
#include"menu.h"
#include"dialog.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    menu m;
    m.show();



    //w.setWindowTitle("五子棋");

    return a.exec();
}
