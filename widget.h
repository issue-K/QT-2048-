#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <gameengine.h>
#include <QPushButton>
#include <QPainter>
#include <QTimer>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void lab_show();
    gameengine *game;//游戏引擎
    QLabel *now_scorelab;//分数面板
    QLabel *high_scorelab;
    QLabel *lab;
    QPushButton *restartBtn;//重新开始游戏按钮
    QTimer *timer;

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
