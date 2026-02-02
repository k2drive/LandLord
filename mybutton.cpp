#include "mybutton.h"
#include<QMouseEvent>
#include<QPainter>
#include <QObject>

MyBUtton::MyBUtton(QWidget *parent) : QPushButton(parent)
{

}

void MyBUtton::SetImage(QString normal, QString hover, QString pressed)
{
    MyNormal = normal;
    MyHover = hover;
    MyPressed = pressed;
    MyPixmap.load(MyNormal);
    update();
}

void MyBUtton::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        MyPixmap.load(MyPressed);
        update();
    }
    QPushButton::mousePressEvent(ev);
}

void MyBUtton::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        MyPixmap.load(MyNormal);
        update();
    }
    QPushButton::mouseReleaseEvent(ev);
}

void MyBUtton::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
    MyPixmap.load(MyHover);
    update();
}

void MyBUtton::leaveEvent(QEvent *ev)
{
    Q_UNUSED(ev)
    MyPixmap.load(MyNormal);
    update();
}

void MyBUtton::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)
    QPainter p(this);
    p.drawPixmap(rect(),MyPixmap);
}
