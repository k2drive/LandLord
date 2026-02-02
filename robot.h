#ifndef ROBOT_H
#define ROBOT_H

#include"player.h"
#include <QObject>

class Robot : public Player
{
    Q_OBJECT
public:
    //使用基类的构造函数
    using Player::Player;
    explicit Robot(QObject* parent = nullptr);
    //虚函数重写
    void PrepareCallLord() override;
    void PreparePlayHand() override;


    //考虑叫地主
    void ThinkCallLord() override;
    void ThinkPlayHand() override;
};

#endif // ROBOT_H
