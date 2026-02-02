#include "animationwndow.h"
#include <QTimer>
#include <QPainter>
#include <QDebug>

AnimationWndow::AnimationWndow(QWidget *parent) : QWidget(parent)
{

}

void AnimationWndow::ShowBetSore(int bet)
{

    MyX = 0;
    if(bet == 1)
    {
        MyPixmap.load(":/images/score1.png");
    }
    else if(bet == 2)
    {
        MyPixmap.load(":/images/score2.png");
    }
    else if(bet == 3)
    {
        MyPixmap.load(":/images/score3.png");
    }


    update();//刷新

    //显示一定的时间
    // 2 秒后清空图片 用一个匿名函数 将产生的图片清除 并且更新一下
       QTimer::singleShot(2000, this, [this]() {
           MyPixmap = QPixmap();  // 关键 创建一个空的QPimap给MyPixmap 相当于清空
           update();
       });
}

void AnimationWndow::ShowSequence(AnimationWndow::Type type)
{
    MyX = 0;
    QString name = type == Pair ? ":/images/liandui.png" :":/images/shunzi.png";
    MyPixmap.load(name);
    update();

    //显示一定的时间
    // 2 秒后清空图片 用一个匿名函数 将产生的图片清除 并且更新一下
       QTimer::singleShot(2000, this, [this]() {
           MyPixmap = QPixmap();  // 关键 创建一个空的QPimap给MyPixmap 相当于清空
           update();
       });
}

void AnimationWndow::ShowJokerBomb()
{
    MyIndex = 0;
    MyX = 0;
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]
    {
       MyIndex++;
       if(MyIndex > 8)
       {
           timer->stop();
           timer->deleteLater();
           MyIndex = 8;
           hide();//隐藏
       }
       QString name =  QString(":/images/joker_bomb_%1.png").arg(MyIndex);
       MyPixmap.load(name);
       update();
    });
    timer->start(60);
}

void AnimationWndow::ShowBomb()
{
    MyIndex = 0;
    MyX = 0;
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]
    {
       MyIndex++;
       if(MyIndex > 12)
       {
           timer->stop();
           timer->deleteLater();
           MyIndex = 12;
           hide();//隐藏
       }
       QString name =  QString(":/images/bomb_%1.png").arg(MyIndex);
       MyPixmap.load(name);
       update();
    });
    timer->start(60);
}

void AnimationWndow::ShowPlane()
{
    MyX = width();
    MyPixmap.load(":/images/plane_1.png");
    setFixedHeight(MyPixmap.height());
    update();

    //计算每一份图片大小
    int step = width() / 5;//一共5章图片

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]()
    {
        static int dist = 0;//飞机移动距离
        static int timers = 0;
        dist += 5;
        if(dist >= step)
        {
            dist = 0;
            timers++;
            QString name = QString(":/images/plane_%1.png").arg(timers % 5 + 1);
            MyPixmap.load(name);
        }
        if(MyX <= -110)
        {
            timer->stop();
            timer->deleteLater();
            dist = timers = 0;
            hide();
        }
        MyX -=5;
        update();
    });
    timer->start(15);
}

void AnimationWndow::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)
    QPainter p(this);
    //当MyPixmap为空的时候 直接什么都不画
    if(!MyPixmap.isNull())
    {
        p.drawPixmap(MyX,0, MyPixmap.width(), MyPixmap.height(), MyPixmap);

    }

}
