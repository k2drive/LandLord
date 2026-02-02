#include "cardpanel.h"
#include <QMouseEvent>
#include <QPainter>
#include "card.h"

CardPanel::CardPanel(QWidget *parent) : QWidget(parent)
{
    MyIsFront = true;//默认是正面图片
    MyOwner = nullptr;
    IsSelect = true;

}

void CardPanel::SetImage(QPixmap &front, QPixmap &back)
{
   m_front = front;
    MyBack = back;

    setFixedSize(m_front.size());//获取尺寸信息
    //设置好了之后重新绘制窗口(更新)
    update();

}

QPixmap CardPanel::Image()
{
    return m_front;
}

void CardPanel::setFrontSide(bool flag)
{
    MyIsFront = flag;
}

bool CardPanel::IsFrontSide()
{
    return MyIsFront;
}

void CardPanel::SetSelected(bool flag)
{
    IsSelect = flag;
}

bool CardPanel::IsSelected()
{
    return IsSelect;
}

void CardPanel::SetCard(class Card &card)
{
    MyCard = card;
}

Card CardPanel::Card()
{
    return MyCard;
}

void CardPanel::SetOwner(Player *player)
{
    MyOwner = player;
}

Player *CardPanel::Owner()
{
    return MyOwner;
}

void CardPanel::Clicked()
{
    //右键也是发射这个信号 模拟左键的
    emit CardSelected(Qt::LeftButton);//接收者是游戏的主窗口
}

void CardPanel::paintEvent(QPaintEvent* ev)
{
    Q_UNUSED(ev)
    QPainter p(this);

    if(MyIsFront){
        //是正面开始绘制正面图片
        p.drawPixmap(rect(),m_front);//tect()的到对应窗口尺寸信息
    }
    else{
          p.drawPixmap(rect(),MyBack);
    }
}

void CardPanel::mousePressEvent(QMouseEvent *ev)
{
    //发射鼠标点击事件信号产生效果是持续的
    emit CardSelected(ev->button());//接收者是游戏的主窗口
}


