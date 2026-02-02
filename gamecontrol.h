#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include"robot.h"
#include "userplayer.h"
#include"cards.h"
#include <QObject>

struct BetRecord
{
    BetRecord()
    {
        reset();
    }
    void reset()
    {
        player = nullptr;
        bet = 0;
        times = 0;
    }
    Player* player;
    int bet;
    int times;//第几次叫地主
};

class GameControl : public QObject
{
    Q_OBJECT
public:
    //游戏状态
    enum GameStatus{
        DisPatchCard,//发牌
        CallingLord,//叫地主
        PlayingHand//出牌
    };
    //玩家状态
    enum PlayerStatus{
        ThinkingForCallLord,//考虑叫地主
        ThinkingForPlayHand,//考虑出牌
        Wining//胜利
    };

    explicit GameControl(QObject *parent = nullptr);

    //初始化玩家
    void PlayerInit();

    //得到玩家的实例对象
    Robot* GetLeftRobot();
    Robot* GetRightRobot();
    UserPlayer* GetUserPlayer();

   //因为当前玩家实时变化 所以需要set和get
    void SetCurrentPlayer(Player* player);
    Player* GetCurrentPlayer();

    //得到出牌玩家和打出的牌
    Player* GetPendPlayer();
    Cards GetPendCards();

    //初始化扑克牌
    void InitAllCards();
    //每次发一张牌
    Card TakeOneCard();
    //得到最后三张牌
    Cards GetSurplusCArds();
    //重置卡牌数据
    void RestCardData();

    //叫地主
    void StartLordCard();
    //成为地主
    void BecomeLord(Player * palyer, int bet);
    //清空所有玩家的分数
    void ClearPlyerScore();
    //得到玩家下注的最高分数
    int GetPlayerMaxBet();

    //处理叫地主槽函数
    void OnGrabBet(Player* player,int bet);

    //处理出牌的槽函数
    void OnPlayHand(Player* player,Cards  &cards);



signals:
    //告诉主窗口 当前是谁发生了变化 发生了什么变化
    void PlayerStatusChanged(Player* player,PlayerStatus status);
    void NotifyGrabLordBet(Player* player, int bet, bool flag);
    //游戏转状态变换
    void GameStatusChanged(GameStatus status);
    //通知玩家出牌
    void NotifyPlayHand(Player* player, Cards&  cards);
    //给玩家传递出牌数据
    void PendingInfo(Player* player, Cards&  cards);

private:
    Robot* MyRobotLeft = nullptr;
    Robot* MyRobotRight = nullptr;
    UserPlayer* MyUser= nullptr;
    Player* MyCurrentPlayer;//当前玩家
    int MyPassCount =0;//连续pass的次数
    Player* MyPendPlayer;//出牌玩家
    Cards MyPendCards;//所要出的牌

    Cards MyAllCards  ;
    BetRecord MyBetRecord;
    int MyCurrentBet = 0;//当前的底分

};

#endif // GAMECONTROL_H
