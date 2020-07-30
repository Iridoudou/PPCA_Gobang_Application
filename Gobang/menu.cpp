#include "menu.h"
#include "ui_menu.h"
#include"mainwindow.h"

menu::menu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::menu)
{
    ui->setupUi(this);
    setFixedSize(500,500);
    start=new QPushButton("电脑先手",this);
    start->setFixedSize(100,50);
    QFont font("Times New Roman",10,QFont::Bold,false);
    start->setFont(font);
    start->show();
    start->move(265,250);

    ui->setupUi(this);
    setFixedSize(500,500);
    setWindowTitle("游戏菜单");
    _start=new QPushButton("玩家先手",this);
    _start->setFixedSize(100,50);
    QFont _font("Times New Roman",10,QFont::Bold,false);
    _start->setFont(_font);
    _start->show();
    _start->move(130,250);
    init();
    //背景板
    setAutoFillBackground(true);
    QPalette p;
    p.setColor(QPalette::Background,QColor("#FAFAD2"));
    setPalette(p);
}

menu::~menu()
{
    delete ui;
}

void menu::init()
{
    connect(start,&QPushButton::clicked,this,&startGame);
    connect(_start,&QPushButton::clicked,this,&_startGame);
}

void menu::startGame()
{
    w=new MainWindow;
    w->show();
    w->isPlayerFirst=false;
    w->game->ai_side=1;
}
void menu::_startGame()
{
    w=new MainWindow;
    w->show();
    w->isPlayerFirst=true;
    w->game->ai_side=0;
}
