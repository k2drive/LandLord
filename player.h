#ifndef PLAYER_H
#define PLAYER_H

#include "cards.h"
#include <QObject>

class Player : public QObject
{
    Q_OBJECT
public:
    //枚举 机器人类型
    enum Role{
        Unknow,
        Lord,
        Famer
    };
    //性别
    enum Sex{
        Man,
        Woman
    };
    //任务头像显示方向
    enum Direction{
        Left,
        Right
    };
    //玩家类型
    enum Type{
        nknow,
        Robot,
        User

    };

    explicit Player(QObject *parent = nullptr);

    //用户名字，构造对象是直接给出名字
    explicit Player(QString name, QObject *parent = nullptr);

    //名字
    void SetName(QString name);
    QString  Name();

    //玩家分数
    void SetScore(int score);
    int GetScore ();
    //人物类型
    void  SetRole (Role role);
    Role GetRole ();

    //性别
    void SetSex(Sex sex);
    Sex GetSex();

    //头像方向
    void SetDirection(Direction direction);
    Direction GetDirection();

    //玩家类型
    void SetType(Type type);
    Type GetType();

    //玩家结果
    void SetIsWin(bool iswin);
    bool GetIsWin();

    //设置当前上家和下家
    void SetPrevPlayer(Player* player);
    void SetNextPlayer(Player* player);
    Player* GetPrevPlayer();
    Player* GetNextPlayer();

    //叫地主or抢地主
    void GrabLordBet(int point);

    //存储和分发扑克牌(在发牌时候得到 发一张）
    void StoreDispatchCard(const Card& card);
    void StoreDispatchCard(Cards& cards);

    //得到所有的扑克牌
    Cards GetCards();

    //清空所有的手牌
    void ClearCards();

    //出牌
    void PlayHand(Cards& cards);

    //设置出牌的玩家和待处理的扑克牌
    void SetPendingInfo(Player* player, Cards& cards);
    Player*GetPendingPlayer();
    Cards GetPendingCards();

    //存储出牌玩家对象和打出的牌
    void  StorePendingInfo(Player* player, Cards& cards);

    //虚函数 多态
    virtual void PrepareCallLord();//准备叫地主
    virtual void PrepareCallHand();//准备出牌
    virtual void PreparePlayHand();//继续出牌
    //考虑叫地主
    virtual void ThinkCallLord();
    virtual void ThinkPlayHand();

signals:

    //通知已经下注了
    void  NotifyGrabLordBet(Player * player,int bet);
    //通知已经出牌
    void NotifyPlayHand(Player* payer, Cards &cards);
    //通知已经发牌了
    void NotifyPickCards(Player* payer, Cards &cards);


protected://改为受保护的，方便子类的使用
    QString  Myname;//名字
    int MyScore;//玩家分数
    Role MyRole;//人物类型
    Sex MySex;//性别
    Direction MyDirection;// 头像显示方向
    Type MyType;//玩家类型
    bool MyIsWin;//玩家最终结果
    Player* MyNextPlayer;//当前玩家对的前一家
    Player* MyPrevPlayer;//当前玩家的后一家
    Cards MyCards;
    Cards MyPendCards;//当前所出的牌
    Player* MyPendPlayer;//当前出牌的人

};

#endif // PLAYER_H
