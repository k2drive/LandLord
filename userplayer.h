#ifndef USERPLAYER_H
#define USERPLAYER_H

#include"player.h"
#include <QObject>

class UserPlayer : public Player
{
    Q_OBJECT
public:
    //使用基类的构造函数
    using Player::Player;
    explicit UserPlayer(QObject* parent = nullptr);
    //虚函数重写
    void PrepareCallLord() override;
    void PreparePlayHand() override;

signals:
    void StartCountDown();

};

#endif // USERPLAYER_H
