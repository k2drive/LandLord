#include "countdown.h"

#include <QPainter>

CountDown::CountDown(QWidget *parent) : QWidget(parent)
{
    setFixedSize(70, 70);
    MyTimer = new QTimer(this);
    connect(MyTimer, &QTimer::timeout, this, [=]
    {
        MyCount --;
        if(MyCount < 10 && MyCount > 0)
        {
            MyClock.load(":/images/clock.png");
            //得到最后的数字
            MyNumber = QPixmap(":/images/number.png")
                    .copy(MyCount*(30+10), 0, 30, 42)
                    .scaled(20, 30);//scaled将图片缩放 这个比列刚好可以放到闹钟里
            if(MyCount == 5)
            {
                emit NotMuchTime();
            }
        }
        else if(MyCount <= 0)
        {
            MyClock = QPixmap();
            MyNumber = QPixmap();
            //通知主界面当前用户时间用完了
            MyTimer->stop();
            emit timeout();//发射给主窗口 让其自动出一张最小的牌
        }

        update();
    });
}

void CountDown::ShowCountDown()
{
    MyCount = 15;
    MyTimer->start(1000);//1s间隔 发射自带的timeout信号
}

void CountDown::StopCountDown()
{
    MyTimer->stop();
    MyClock = QPixmap();
    MyNumber = QPixmap();
    update();
}

void CountDown::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawPixmap(rect(), MyClock);
    p.drawPixmap(24, 24, MyNumber.width(), MyNumber.height(), MyNumber);


}
