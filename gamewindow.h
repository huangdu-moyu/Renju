#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QTimerEvent>
#include <QPainter>
#include <QMessageBox>
#include <QTime>
#include <QLineEdit>
#include "gamestate.h"

namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT

private slots:

    void on_timeEdit_userTimeChanged(const QTime &time);

    void on_restart_clicked();

    void on_Undo_clicked();
public:
    explicit GameWindow(bool ai,QWidget *parent = nullptr);
    ~GameWindow();

protected:
    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent *);
    void mouseReleaseEvent(QMouseEvent* e);

private:
    bool ai;
    Ui::GameWindow *ui;
    GameState* game=new GameState;
    QTime timeSet;
        QLineEdit lineEditGet;
        int setSecond;
        int const_second;
        int timerId;
        int total_second;
        bool hasSetTime;
        bool hasStart;
        bool isblack;
};

#endif // GAMEWINDOW_H
