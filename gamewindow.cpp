#include "gamewindow.h"
#include "game.h"
#include "ui_gamewindow.h"
#include <QDebug>

GameWindow::GameWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWindow)
{
    //this->setFixedSize(940,640);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background,QColor("#B1723C"));
    this->setPalette(palette);
    ui->setupUi(this);
}

void GameWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing,true);

    QPen pen=p.pen();
    pen.setColor(QColor("#8D5822"));
    pen.setWidth(7);
    p.setPen(pen);

    QBrush brush;
    brush.setColor(QColor("#EEC085"));
    brush.setStyle(Qt::SolidPattern);
    p.setBrush(brush);
    p.drawRect(20,20,600,600);

    pen.setColor(Qt::black);
    pen.setWidth(1);
    p.setPen(pen);
    for(int i=1;i<=15;i++)
    {
        p.drawLine(40,i*40,600,i*40);
        p.drawLine(i*40,40,i*40,600);
    }

    brush.setColor(Qt::black);
    p.setBrush(brush);
    p.drawEllipse(QPoint(320,320),5,5);
    p.drawEllipse(QPoint(160,160),5,5);
    p.drawEllipse(QPoint(160,480),5,5);
    p.drawEllipse(QPoint(480,160),5,5);
    p.drawEllipse(QPoint(480,480),5,5);

    for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++)
        {
            int t=game->queryBoard(i,j);
            if(t)
            {
                brush.setColor((t==1?(Qt::black):(Qt::white)));
                p.setBrush(brush);
                p.setPen(Qt::NoPen);
                p.drawEllipse(QPoint((i+1)*40,(j+1)*40),15,15);
            }
        }
    }
}

void GameWindow::mouseReleaseEvent(QMouseEvent* e)
{
    int x=(e->x()+20)/40-1,y=(e->y()+20)/40-1;
    //qDebug()<<x<<" "<<y<<endl;
    if(x<0||x>=15||y<0||y>=15)
    {
        return;
    }
    if(!game->queryBoard(x,y))
    {
        //qDebug()<<x<<" "<<y<<endl;
        game->checkAndMove(x,y);
        //qDebug()<<game->queryBoard(x,y)<<endl;
        update();
        if(game->isWin(x,y))
        {
            QMessageBox::information(this,"You win","You win!",QMessageBox::Ok);
            game->init();
            update();
        }
    }
}

GameWindow::~GameWindow()
{
    delete ui;
}
 
