#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QMessageBox>
#include "gamestate.h"

namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

protected:
    void paintEvent(QPaintEvent*);
    void mouseReleaseEvent(QMouseEvent* e);

private:
    Ui::GameWindow *ui;
    GameState* game=new GameState;
};

#endif // GAMEWINDOW_H
