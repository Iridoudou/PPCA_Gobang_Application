#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "gamemodel.h"
#include<QPainter>
#include<QMouseEvent>
#include<QMessageBox>
#include<QPalette>
#include<math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMouseTracking(true);
    //设置窗口大小
    setFixedSize(MARGIN*2+BLOCK_SIZE*(BOARD_GRAD_SIZE-1)+BUTTON_WIDTH,MARGIN*2+BLOCK_SIZE*(BOARD_GRAD_SIZE-1));
    setWindowTitle("五子棋游戏");
    //初始化
    initGame();
    //退出按钮
    exitGame=new QPushButton("退出游戏",this);
    exitGame->setFixedSize(100,50);
    QFont font("Times New Roman",10,QFont::Bold,false);
    exitGame->setFont(font);
    exitGame->show();
    exitGame->move(this->width()-4*MARGIN,this->height()-5*MARGIN);
    connect(exitGame,&QPushButton::clicked,this,&endGame);
    //背景板
    setAutoFillBackground(true);
    QPalette p;
    p.setColor(QPalette::Background,QColor("#FFE4B5"));
    setPalette(p);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::paintEvent(QPaintEvent*event)
{
    QPainter painter(this);
    exitGame->show();
    //绘制棋盘
    painter.setRenderHint(QPainter::Antialiasing,true);//抗锯齿
    for(register int i=0;i<BOARD_GRAD_SIZE;++i)
    {
        painter.drawLine(MARGIN+BLOCK_SIZE*i,MARGIN,MARGIN+BLOCK_SIZE*i,size().height()-MARGIN);//画竖线
        painter.drawLine(MARGIN,MARGIN+BLOCK_SIZE*i,size().width()-MARGIN-BUTTON_WIDTH,MARGIN+BLOCK_SIZE*i);//画横线
    }
    //绘制选中点
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    //绘制落子标记（防止鼠标出框越界）
    if(clickPosRow>0&&clickPosRow<BOARD_GRAD_SIZE&&
       clickPosCol>0&&clickPosCol<BOARD_GRAD_SIZE&&
            game->isEmpty(clickPosRow-1,clickPosCol-1))
    {
        if(game->turn%2!=0)
            brush.setColor(Qt::black);
        else
            brush.setColor(Qt::white);
        painter.setBrush(brush);
        painter.drawRect(MARGIN+BLOCK_SIZE*clickPosCol-MARKK_SIZE/2,MARGIN+BLOCK_SIZE*clickPosRow-MARKK_SIZE/2,6,6);
    }
    //绘制棋子
    for(register int i=0;i<BOARD_GRAD_SIZE-2;++i)
        for(register int j=0;j<BOARD_GRAD_SIZE-2;++j)
        {
            if(game->board[i][j]==0)
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(MARGIN+BLOCK_SIZE*(j+1)-CHESS_RADIUS,MARGIN+BLOCK_SIZE*(i+1)-CHESS_RADIUS,2*CHESS_RADIUS,2*CHESS_RADIUS);
            }
            else if(game->board[i][j]==1)
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(MARGIN+BLOCK_SIZE*(j+1)-CHESS_RADIUS,MARGIN+BLOCK_SIZE*(i+1)-CHESS_RADIUS,2*CHESS_RADIUS,2*CHESS_RADIUS);
            }
        }
    for(register int i=0;i<BOARD_GRAD_SIZE-2;++i)
        for(register int j=0;j<BOARD_GRAD_SIZE-2;++j)
        {
            if(game->judge({i,j},game->ai_side))
            {
                QMessageBox::StandardButton btnValue=QMessageBox::information(this,"五子棋游戏","电脑胜利 即将开始下一局比赛");
                if(btnValue==QMessageBox::Ok)
                    game->init();
            }
            else if(game->judge({i,j},1-game->ai_side))
            {
                QMessageBox::StandardButton btnValue=QMessageBox::information(this,"五子棋游戏","玩家胜利 即将开始下一局比赛");
                if(btnValue==QMessageBox::Ok)
                    game->init();
            }
        }
    if(game->flipFlag)
    {
        QMessageBox::StandardButton btnValue=QMessageBox::information(this,"","电脑换手");
        game->flipFlag=false;
        if(btnValue==QMessageBox::Ok)
        {
            return;
        }
    }



}
void MainWindow::initGame()
{
    game=new Gobang;
    game->init();
}
void MainWindow::endGame()
{
    exit(0);
}

void MainWindow::mouseMoveEvent(QMouseEvent*event)
{
    int x=event->x();
    int y=event->y();
    if(x>=MARGIN+BLOCK_SIZE/2&&
            x<size().width()-BUTTON_WIDTH-MARGIN-BLOCK_SIZE/2&&
            y>=MARGIN+BLOCK_SIZE/2&&
            y<size().height()-MARGIN-BLOCK_SIZE/2)
    {
        int col=(x-MARGIN)/BLOCK_SIZE;
        int row=(y-MARGIN)/BLOCK_SIZE;

        int leftTopPosX=MARGIN+BLOCK_SIZE*col;
        int leftTopPosY=MARGIN+BLOCK_SIZE*row;

        clickPosRow=-1;
        clickPosCol=-1;

        int len=0;

        selectPos=false;
//与四个坐标的距离
        len=sqrt((x-leftTopPosX)*(x-leftTopPosX)+(y-leftTopPosY)*(y-leftTopPosY));
        if(len<POS_OFFSET)
        {
            clickPosRow=row;
            clickPosCol=col;
            if(game->isEmpty(clickPosRow-1,clickPosCol-1))
                selectPos=true;
        }

        len=sqrt((x-leftTopPosX-BLOCK_SIZE)*(x-leftTopPosX-BLOCK_SIZE)+(y-leftTopPosY)*(y-leftTopPosY));
        if(len<POS_OFFSET)
        {
            clickPosRow=row;
            clickPosCol=col+1;
            if(game->isEmpty(clickPosRow-1,clickPosCol-1))
                selectPos=true;
        }

        len=sqrt((x-leftTopPosX)*(x-leftTopPosX)+(y-leftTopPosY-BLOCK_SIZE)*(y-leftTopPosY-BLOCK_SIZE));
        if(len<POS_OFFSET)
        {
            clickPosRow=row+1;
            clickPosCol=col;
            if(game->isEmpty(clickPosRow-1,clickPosCol-1))
                selectPos=true;
        }

        len=sqrt((x-leftTopPosX-BLOCK_SIZE)*(x-leftTopPosX-BLOCK_SIZE)+(y-leftTopPosY-BLOCK_SIZE)*(y-leftTopPosY-BLOCK_SIZE));
        if(len<POS_OFFSET)
        {
            clickPosRow=row+1;
            clickPosCol=col+1;
            if(game->isEmpty(clickPosRow-1,clickPosCol-1))
                selectPos=true;
        }
    }
    update();
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(selectPos==false)
        return;
    else
        selectPos=false;
    //人下棋
    if(isPlayerFirst)
    {
        chessOneByPerson();
    }
    //ai下棋
    chessOneByAi();
    isPlayerFirst=true;


}
void MainWindow::chessOneByPerson()
{
    if(clickPosRow!=-1&&clickPosCol!=-1&&game->isEmpty(clickPosRow-1,clickPosCol-1))
    {
        game->place(clickPosRow-1,clickPosCol-1,1-game->ai_side);
        game->turn++;
        repaint();
        //update();
    }
}
void MainWindow::chessOneByAi()
{
    std::pair<int,int> aiAction;
    if(isPlayerFirst)
        aiAction=game->action(std::make_pair(clickPosRow-1,clickPosCol-1));
    else
        aiAction=game->action(std::make_pair(-1,-1));
    game->place(aiAction.first,aiAction.second,game->ai_side);
    repaint();
    //update();
}
/*
void MainWindow::createButton()
{
    QPushButton *refreshBtn = new QPushButton(tr("刷新"));
    QPushButton *addBtn = new QPushButton(tr("添加"));
    QPushButton *editBtn = new QPushButton(tr("编辑"));
    QPushButton *deleteBtn = new QPushButton(tr("删除"));

    QWidget *cornerWidget = new QWidget;

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(refreshBtn);
    hlayout->addWidget(addBtn);
    hlayout->addWidget(editBtn);
    hlayout->addWidget(deleteBtn);

    cornerWidget->setLayout(hlayout);
    ui->tabWidget->setCornerWidget(cornerWidget,Qt::TopRightCorner);
    ui->tabWidget->setStyleSheet("QTabBar::tab{min-width:30ex; min-height:15ex}");
}*/
