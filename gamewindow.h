#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QMessageBox>
#include "game.h"

namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

protected:
    void paintEvent(QPaintEvent*);
    void mouseReleaseEvent(QMouseEvent* e);

private:
    Ui::GameWindow *ui;
    Game* game=new Game;
};

#endif // GAMEWINDOW_H
