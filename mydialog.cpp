#include "mydialog.h"
#include <QFont>
#include <QDebug>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <QDialog>
#include <mydialog.h>


#define dlglabel "QLabel{ opacity:1.0 }"
#define dlgbutton "QPushButton{ background-color:#8F7A66; color:white; font-weight:bold; font-size:20px; border-radius:3px} "

MyDialog::MyDialog(QString s1,QString s2,int w,int h)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);    //   去除页面边框
    this->setFixedSize(w, h );
    this->setAutoFillBackground(true);
    this->setWindowOpacity(0.7);     //   背景和元素都设置透明效果

    QFont font;
    font.setPixelSize(55); font.setFamily("Consolas");
    font.setBold(true);//设置粗体

    QPushButton *temp = new QPushButton(this);
    temp->setText(s1);
    temp->setFixedSize(80,35);
    temp->setStyleSheet(dlgbutton);
    temp->setFont(font);
    temp->move((this->width()-temp->width())*0.5,(this->height()-temp->height())*0.7 );
    connect(temp,&QPushButton::clicked,[=](){this->close();});
    QLabel *lose = new QLabel(this);
    lose->setText(s2);
    lose->setFixedSize(300,65);
    lose->setFont(font);
    lose->setStyleSheet(dlglabel);
    lose->move((this->width()-lose->width())*0.5,(this->height()-temp->height())*0.7-70 );
}
