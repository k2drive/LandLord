#include "player.h"
#include <QDebug>

Player::Player(QObject *parent) : QObject(parent),
    MyScore(0),
    MyRole(Role::Unknow),
    MySex(Player::Man),
    MyDirection(Player::Left),
    MyType(Type::nknow),
    MyIsWin(false),
    MyNextPlayer(nullptr),
    MyPrevPlayer(nullptr),
    MyPendPlayer(nullptr)
{
    //初始化分数和胜利
    MyScore = 0;
    MyIsWin = false;

}

//委托构造函数
Player::Player(QString name, QObject *parent) : Player(parent)
{
    Myname = name;
}

void Player::SetName(QString name)
{
    Myname = name;
}

QString Player::Name()
{
    return Myname;
}

void Player::SetScore(int score)
{
    MyScore = score;
}

int Player::GetScore()
{
    return MyScore;
}

void Player::SetRole(Player::Role role)
{
    MyRole = role;
}

Player::Role Player::GetRole()
{

    return MyRole;
}

void Player::SetSex(Player::Sex sex)
{
    MySex   = sex;
}

Player::Sex Player::GetSex()
{
    return MySex;
}

void Player::SetDirection(Player::Direction direction)
{
    MyDirection = direction;
}

Player::Direction Player::GetDirection()
{
    return MyDirection;
}

void Player::SetType(Player::Type type)
{
    MyType = type;
}

Player::Type Player::GetType()
{
    return MyType;
}

void Player::SetIsWin(bool iswin)
{
    MyIsWin = iswin;
}

bool Player::GetIsWin()
{
    return MyIsWin;
}

void Player::SetPrevPlayer(Player *player)
{
    MyPrevPlayer = player;
}

void Player::SetNextPlayer(Player *player)
{
    MyNextPlayer = player;
}

Player *Player::GetPrevPlayer()
{
    return MyPrevPlayer;
}

Player *Player::GetNextPlayer()
{
    return MyNextPlayer;
}

void Player::GrabLordBet(int point)
{
    //发射信号
    emit NotifyGrabLordBet(this, point);
}

void Player::StoreDispatchCard(const Card &card)
{
    MyCards.add(card);
    //发射信号 告诉其成为地主了
    Cards cs ;
    cs.add(card);
    emit NotifyPickCards(this, cs);//接受者是主界面
}

void Player::StoreDispatchCard(Cards &cards)
{
    MyCards.add(cards);
    //发射信号 告诉其成为地主了
    emit NotifyPickCards(this, cards);
}

Cards Player::GetCards()
{
    return MyCards;
}

void Player::ClearCards()
{
    MyCards.clear();
}

void Player::PlayHand(Cards &cards)
{
    MyCards.remove(cards);
    emit NotifyPlayHand(this, cards);//发送信号 表示 已经开始出牌 槽函数是OnPlayHand
}

void Player::SetPendingInfo(Player *player, Cards &cards)
{
    MyPendPlayer = player;
    MyPendCards = cards;
}

Player *Player::GetPendingPlayer()
{
    return MyPendPlayer;
}

Cards Player::GetPendingCards()
{
    return MyPendCards;
}

void Player::StorePendingInfo(Player *player, Cards &cards)
{
    MyPendPlayer = player;
    MyPendCards = cards;
}

void Player::PrepareCallLord()
{

}

void Player::PrepareCallHand()
{

}

void Player::PreparePlayHand()
{

}

void Player::ThinkCallLord()
{

}

void Player::ThinkPlayHand()
{

}


