
#include "gamewindow.h"
#include "gamestate.h"
#include "ui_gamewindow.h"
#include "solver.h"
#include <QDebug>
#include <QString>
#include <QTimerEvent>
GameWindow::GameWindow(bool ai,QWidget *parent) : ai(ai),QWidget(parent),
                                          ui(new Ui::GameWindow)
{
    //this->setFixedSize(940,640);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor("#B1723C"));
    this->setPalette(palette);
    ui->setupUi(this);
    timeSet = ui->timeEdit->time();
    ui->lineEdit_black->setText(timeSet.toString());
    ui->lineEdit_white->setText(timeSet.toString());
    total_second = 0;
    hasSetTime = false;
    hasStart = false;
    isblack = true;
    const_second=30;
}

void GameWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    QPen pen = p.pen();
    pen.setColor(QColor("#8D5822"));
    pen.setWidth(7);
    p.setPen(pen);

    QBrush brush;
    brush.setColor(QColor("#EEC085"));
    brush.setStyle(Qt::SolidPattern);
    p.setBrush(brush);
    p.drawRect(20, 20, 600, 600);

    pen.setColor(Qt::black);
    pen.setWidth(1);
    p.setPen(pen);
    for (int i = 1; i <= 15; i++)
    {
        p.drawLine(40, i * 40, 600, i * 40);
        p.drawLine(i * 40, 40, i * 40, 600);
    }

    brush.setColor(Qt::black);
    p.setBrush(brush);
    p.drawEllipse(QPoint(320, 320), 5, 5);
    p.drawEllipse(QPoint(160, 160), 5, 5);
    p.drawEllipse(QPoint(160, 480), 5, 5);
    p.drawEllipse(QPoint(480, 160), 5, 5);
    p.drawEllipse(QPoint(480, 480), 5, 5);

    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            int t = game->queryBoard(i, j);
            if (t != -1)
            {
                brush.setColor((t == 1 ? (Qt::black) : (Qt::white)));
                p.setBrush(brush);
                p.setPen(Qt::NoPen);
                p.drawEllipse(QPoint((i + 1) * 40, (j + 1) * 40), 15, 15);
            }
        }
    }
}

void GameWindow::mouseReleaseEvent(QMouseEvent *e)
{
    static Solver s;
    int x = (e->x() + 20) / 40 - 1, y = (e->y() + 20) / 40 - 1;
    //qDebug()<<x<<" "<<y<<endl;
    if (x < 0 || x >= 15 || y < 0 || y >= 15)
    {
        return;
    }
    if (game->queryBoard(x, y) == -1)
    {
        game->checkAndMove(x, y);
        repaint();
        if (game->isWin(x, y))
        {
            QMessageBox::information(this, "You win", QString(game->queryPlayer()?"白":"黑")+"胜!", QMessageBox::Ok);
            game->clear();
            isblack=1;
            total_second=0;
            update();
        }
        else
        {

            killTimer(timerId); //关闭计时器

            //重置显示屏
            setSecond = const_second;   //储存设置的时间

            timeSet.setHMS(setSecond / 3600, (setSecond - setSecond / 3600 * 3600) / 60, setSecond - setSecond / 60 * 60);
            if (total_second == 0)
            {
                ui->lineEdit_total->setText(timeSet.toString());
            }
            ui->lineEdit_black->setText(timeSet.toString());
            ui->lineEdit_white->setText(timeSet.toString());

            setSecond = const_second;   //储存设置的时间
            timerId = startTimer(1000); //计时开始,每秒调用一次timerEvent函数，并获得这个定时器的Id

            isblack = !isblack;
        }
        if(ai)
        {
            s.set(game);
            //qDebug()<<s->st->queryBoard(x,y);
            //qDebug()<<GameState::zobristBoard[255][0]<<endl;
            int u = s.getNextMove();
            //qDebug()<<"nextMove:"<<(u-17)/16<<" "<<(u-17)%16;
            game->checkAndMove(u);
            if (game->isWin(u))
            {
                QMessageBox::information(this, "You lose", "You lose!", QMessageBox::Ok);
                game->clear();
                isblack=1;
                total_second=0;
            }
            else
            {

                killTimer(timerId); //关闭计时器

                //重置显示屏
                setSecond = const_second;   //储存设置的时间

                timeSet.setHMS(setSecond / 3600, (setSecond - setSecond / 3600 * 3600) / 60, setSecond - setSecond / 60 * 60);
                if (total_second == 0)
                {
                    ui->lineEdit_total->setText(timeSet.toString());
                }
                ui->lineEdit_black->setText(timeSet.toString());
                ui->lineEdit_white->setText(timeSet.toString());

                timerId = startTimer(1000); //计时开始,每秒调用一次timerEvent函数，并获得这个定时器的Id

                isblack = !isblack;
            }
            update();
        }
    }
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::on_timeEdit_userTimeChanged(const QTime &time) //设置时间
{
    ui->lineEdit_black->setText(timeSet.toString());
    ui->lineEdit_white->setText(timeSet.toString());
    const_second = time.hour() * 3600 + time.minute() * 60 + time.second();
    setSecond = const_second; //储存设置的时间

    hasSetTime = true;
}

void GameWindow::on_Undo_clicked() //暂停按钮的实现
{
    if(ai)
    {
        if(game->moves.size()>=2)
        {
            game->remove(*(game->moves.rbegin()));
            game->remove(*(game->moves.rbegin()));
        }
    }
    else
    {
        if(game->moves.size()>=1)
        {
            game->remove(*(game->moves.rbegin()));
        }
    }
    update();
}

void GameWindow::timerEvent(QTimerEvent *)
{


        setSecond -= 1;

        //设置显示屏时间
        timeSet.setHMS(setSecond / 3600, (setSecond - setSecond / 3600 * 3600) / 60, setSecond - setSecond / 60 * 60);
        if (isblack)
        {
            ui->lineEdit_black->setText(timeSet.toString());
        }
        else
            ui->lineEdit_white->setText(timeSet.toString());

        total_second += 1;
        timeSet.setHMS(total_second / 3600, (total_second - total_second / 3600 * 3600) / 60, total_second - total_second / 60 * 60);
        ui->lineEdit_total->setText(timeSet.toString());

        if (setSecond == 0)
        {
            QMessageBox::information(this, "提示", "超时！");
            killTimer(timerId); //时间到,关闭计时器
            isblack=1;
            total_second=0;
            game->clear();
            update();
        }
}

void GameWindow::on_restart_clicked()
{
    game->clear();
    isblack=1;
    total_second=0;
    update();
}
