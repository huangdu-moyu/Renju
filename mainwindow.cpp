#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamewindow.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    this->close();
    GameWindow* GW=new GameWindow(true);
    connect(GW,SIGNAL(sendsignal()),this,SLOT(reshow()));
    GW->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    this->close();
    GameWindow* GW=new GameWindow(false);
    connect(GW,SIGNAL(sendsignal()),this,SLOT(reshow()));
    GW->show();
}

void MainWindow::reshow(){
    this->show();
}
