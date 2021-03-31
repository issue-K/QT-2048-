#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QWidget>
#include <QGLWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>
// 手势的方向
enum GestureDirect
{
    LEFT = 0,   // 向左
    RIGHT = 1,  // 向右
    UP = 2,     // 向上
    DOWN = 3    // 向下
};

enum donghua
{
    Move = 1,
    Appear = 2
};

struct animation
{
    QPoint startpos,endpos;//开始位置,结束位置
    donghua type;//动画类型
    int dig1, dig2;//数码
    GestureDirect dirct;//动画方向
};

class gameengine : public QGLWidget
{
    Q_OBJECT
public:
    explicit gameengine(QWidget *parent = nullptr);
    int block[4][4];//方格数组
    //绘图事件
    void paintEvent(QPaintEvent *);
    //捕捉键盘
    void keyPressEvent(QKeyEvent *e);
    //初始化方格数组
    void initblock();
    //画每个方格的动画
    bool playanimation(animation&,QPainter&);
    //判断游戏是否结束
    bool checkgameover();
    //判断游戏是否胜利
    bool checkgamewin();
    //画背景
    void drawbackground(QPainter &);
    //重新开始
    void Restart_game();

    bool isanimation=false;//是否在播放动画
    QString color[2049]; //数字16进制颜色数组
    int digcount = 0;//当前有多少个数码
    int score = 0;//记录当前的分数
    QList<animation>list;//存储动画的容器
    QTimer timer;//动画定时器

signals:
    void update_score(int score,int temp);
    void over();
    void win();

public slots:
};

#endif // GAMEENGINE_H
