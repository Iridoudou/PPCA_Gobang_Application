#include "dialog.h"
#include "ui_dialog.h"
#include"mainwindow.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setFixedSize(500,500);
    start=new QPushButton("开始游戏",this);
    start->setFixedSize(100,50);
    QFont font("Times New Roman",20,QFont::Bold,false);
    start->setFont(font);
    start->show();
    start->move(250,400);
}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::init()
{
    connect(start,&QPushButton::clicked,this,&startGame);
}

void Dialog::startGame()
{
    MainWindow w;
    w.show();
}
