#include "gamepanel.h"
#include "loading.h"

#include <QPainter>
#include <QTimer>

Loading::Loading(QWidget *parent) : QWidget(parent)
{
    MyBk.load(":/images/loading.png");
    setFixedSize(MyBk.size());

    //去边框
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //背景透明
    setAttribute(Qt::WA_TranslucentBackground);

    QPixmap pixmap (":/images/progress.png");
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]()
    {
        //动态加载进度条
        MyProgress = pixmap.copy(0, 0, MyDist, pixmap.height());

        update();
        if(MyDist >= pixmap.width())
        {
            timer->stop();
            timer->deleteLater();
            GamePanel* panel = new GamePanel;
            panel->show();
            close();
        }
        MyDist += 5;
    });
    timer->start(15);
}

void Loading::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);//划到当前窗口
    p.drawPixmap(rect(), MyBk);
    p.drawPixmap(62, 417, MyProgress.width(), MyProgress.height(), MyProgress);


}
