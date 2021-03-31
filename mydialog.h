#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <gameengine.h>
#include <QPushButton>
#include <QPainter>
#include <QTimer>
class MyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MyDialog(QWidget *parent = nullptr);
    MyDialog(QString s1,QString s2,int w,int h);
signals:

public slots:
};

#endif // MYDIALOG_H
