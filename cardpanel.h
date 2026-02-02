#ifndef CARDPANEL_H
#define CARDPANEL_H

#include <QWidget>
#include "card.h"
#include"player.h"

class CardPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CardPanel(QWidget *parent = nullptr);

    //设置图片函数
    void SetImage(QPixmap &front,QPixmap &back);//front正面图片 back是背面图片
    QPixmap Image();//返回前正图片

    void setFrontSide(bool flag);//扑克牌显示哪一面
    bool IsFrontSide();//获取函数 判断当前是正面还是背面

    void SetSelected(bool flag);//记录窗口是否被选中了1为选中0为未选中
    bool IsSelected();//判断当前窗口是否为选中状态

    //扑克牌的花色和点数
    void SetCard(Card & card);
    Card Card();

    //扑克牌的所有者
    void  SetOwner(Player *player);
    Player* Owner();

    //模拟扑克牌的点击事件
    void Clicked();


protected:
    //事件处理函数
    void  paintEvent(QPaintEvent *ev);//是正面
    void mousePressEvent(QMouseEvent *ev) ;

signals:
    void CardSelected(Qt::MouseButton button);


private:

    QPixmap m_front;
    QPixmap MyBack;
    bool MyIsFront;//判断是否为正面图片
    bool IsSelect;//当前窗口是否被选中
    class Card MyCard;//当前扑克牌的花色和点数
    Player *MyOwner;//当前扑克牌的所有者
};



#endif // CARDPANEL_H
