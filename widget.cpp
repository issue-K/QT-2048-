#include "widget.h"
#include "ui_widget.h"
#include <QFont>
#include <QDebug>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <QDialog>
#include <mydialog.h>

#define labelstyle "QLabel{ border:5px solid #F0FFF0; color:#FAF8EF; background-color:#BBADA0; border-radius:10px };"
#define Btnstyle "QPushButton{ border:5px solid #8F7A66; color:#F9F6F2; background-color:#8F7A66; border-radius:10px }"
#define dlglabel "QLabel{ opacity:1.0 }"
#define dlgbutton "QPushButton{ background-color:#8F7A66; color:white; font-weight:bold; font-size:20px; border-radius:3px} "


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    QFont font;
    font.setPixelSize(25); font.setFamily("Consolas");
    font.setBold(true);//设置粗体

    static QLabel *lab = new QLabel(this);
    lab->setGeometry(QRect(50,40,50,20));
    lab->setFont(font);
    int y = 40; double opac = 1;
    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,[=]()mutable{
        if( y>35 )  y--,opac -= 0.025;
        else    y-=3,opac -= 0.075;
        if( y<0 )
        {
            y = 40, opac = 1;
            timer->stop();
            lab->hide();
        }
        QString sty = QString("QLabel{ color:rgba(0,0,0,%1); }").arg(opac);
        lab->move(50,y);
        lab->setStyleSheet(sty);
    });

//    QGraphicsOpacityEffect *m_pGraphicsOpacityEffect = new QGraphicsOpacityEffect(lab);
//    m_pGraphicsOpacityEffect->setOpacity(1);
//    lab->setGraphicsEffect(m_pGraphicsOpacityEffect);

//    QPropertyAnimation *animation1 = new QPropertyAnimation(lab, "geometry");
//    animation1->setDuration(500);
//    animation1->setStartValue(QRect(lab->x(), lab->y(), lab->width(), lab->height()));
//    animation1->setEndValue(QRect(lab->x(), lab->y()-40, lab->width(), lab->height()));
//    QPropertyAnimation *animation2 = new QPropertyAnimation(lab, "windowOpacity");
//    animation2->setDuration(500);
//    animation2->setStartValue(1);
//    animation2->setEndValue(0);


    ui->setupUi(this);
    resize(400,600);
    game = new gameengine(this);
    game->setGeometry(2,200,400,400);//设置矩形的大小和位置

    now_scorelab = new QLabel(QString("score:\n 0"),this);
    now_scorelab->setGeometry(20,30,180,60);
    now_scorelab->setStyleSheet(labelstyle);
    now_scorelab->setFont(font);

    high_scorelab = new QLabel(QString("high score:\n 99999"),this);
    high_scorelab->setGeometry(215,30,180,60);
    high_scorelab->setStyleSheet(labelstyle);
    high_scorelab->setFont(font);

    restartBtn = new QPushButton(QString("Restart!!"),this);
    restartBtn->setGeometry(100,120,200,70);
    restartBtn->setStyleSheet(Btnstyle);
    restartBtn->setFont(font);

    connect(game,&gameengine::update_score,[=](int score,int temp){
        QString s = QString("score:\n %1").arg(score);
        now_scorelab->setText(s);
        lab->raise(); lab->show();
        lab->setText(QString("+%1").arg(temp));
        timer->start(20);
    });

    connect(restartBtn,&QPushButton::clicked,[=](){
        game->Restart_game();
        now_scorelab->setText(QString("score:\n %1").arg(0));
    });

    connect(game,&gameengine::over,[=]()mutable{
        game->update();
        MyDialog *now = new MyDialog("重试","Game Over!",this->width(),this->height());
        now_scorelab->setText(QString("score:\n %1").arg(0));
        now->move(this->x()+15,this->y()+40);
        now->exec();
        game->Restart_game();
    });

    connect(game,&gameengine::win,[=](){
        qDebug() << "游戏胜利";
    });
}

void Widget::lab_show()
{

}

Widget::~Widget()
{
    delete ui;
}
