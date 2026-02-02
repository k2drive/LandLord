#include "countdown.h"
#include "gamecontrol.h"
#include "playhand.h"
#include <QRandomGenerator>
#include<QTimer>
#include <qdebug.h>

GameControl::GameControl(QObject *parent) : QObject(parent)
{

}

void GameControl::PlayerInit()
{
    //对象实例化
    MyRobotLeft = new Robot("机器人A",this);
    MyRobotRight = new Robot("机器人B",this);
    MyUser = new UserPlayer("我自己",this);

    //头像的显示
    MyRobotLeft->SetDirection(Player::Left);
    MyRobotRight->SetDirection(Player::Right);
    MyUser->SetDirection(Player::Right);

    //性别
    Player::Sex sex;
    //调用随机生成数函数，设置值为2，强转为Player::Sex类
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    //将性别设置给对应的玩家
    MyRobotLeft ->SetSex(sex);
    //其他玩家同理
     sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
     MyRobotRight->SetSex(sex);
     sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
     MyUser->SetSex(sex);

     //出牌顺序
     //左边机器人
     MyRobotLeft->SetPrevPlayer(MyRobotRight);
     MyRobotLeft->SetNextPlayer(MyUser);

     //右边机器人
     MyRobotRight->SetPrevPlayer(MyUser);
     MyRobotRight->SetNextPlayer(MyRobotLeft);

     //设置用户的顺序
     MyUser->SetPrevPlayer(MyRobotLeft);
     MyUser->SetNextPlayer(MyRobotRight);

     //可以使用随机生成数 随机生成当前玩家
     //当前 直接指定当前玩家
     MyCurrentPlayer = MyUser;

     //处理玩家发射出的信号
     connect(MyUser,&UserPlayer::NotifyGrabLordBet, this, & GameControl::OnGrabBet);
     connect(MyRobotLeft,&UserPlayer::NotifyGrabLordBet, this, & GameControl::OnGrabBet);
     connect(MyRobotRight,&UserPlayer::NotifyGrabLordBet, this, & GameControl::OnGrabBet);

     //传递 出牌玩家对象和出牌玩家的牌 接受者是player类的其中一个子类
      connect(this,&GameControl::PendingInfo, MyRobotLeft, & Player::StorePendingInfo);
      connect(this,&GameControl::PendingInfo, MyRobotRight, & Player::StorePendingInfo);
      connect(this,&GameControl::PendingInfo, MyUser, & Player::StorePendingInfo);

      //处理玩家出牌
      connect(MyRobotLeft, &Player::NotifyPlayHand, this, &GameControl::OnPlayHand);
      connect(MyRobotRight, &Player::NotifyPlayHand, this, &GameControl::OnPlayHand);
      connect(MyUser, &Player::NotifyPlayHand, this, &GameControl::OnPlayHand  );


}

Robot *GameControl::GetLeftRobot()
{
    return MyRobotLeft;
}

Robot *GameControl::GetRightRobot()
{
    return MyRobotRight;
}

UserPlayer *GameControl::GetUserPlayer()
{
   return MyUser;
}

void GameControl::SetCurrentPlayer(Player *player)
{
    MyCurrentPlayer = player;
}

Player *GameControl::GetCurrentPlayer()
{
    return MyCurrentPlayer;
}

Player *GameControl::GetPendPlayer()
{
    return MyPendPlayer;
}

Cards GameControl::GetPendCards()
{
    return MyPendCards;
}

void GameControl::InitAllCards()
{
    //首先清空对象
    MyAllCards.clear();

    //生成52张牌
    for(Card::CardPoint p = static_cast<Card::CardPoint>(static_cast<int>(Card::CardBegin) + 1);
        p < Card::CardSj;
        p = static_cast<Card::CardPoint>(static_cast<int>(p) + 1))
    {
        for(Card::CardSuit s = static_cast<Card::CardSuit>( static_cast<int>(Card::SuitBegin) + 1);
            s < Card::SuitEnd;
            s = static_cast<Card::CardSuit>(static_cast<int>(s) + 1)
            )
        {
            Card c(p,s);
            MyAllCards.add(c);
        }
    }
    //将大小王存到MyAllCards中
    MyAllCards.add(Card(Card::CardBj,Card::SuitBegin));
    MyAllCards.add(Card(Card::CardSj,Card::SuitBegin));
}

Card GameControl::TakeOneCard()
{
    return MyAllCards.TakeRandCard();
}

Cards GameControl::GetSurplusCArds()
{
    return MyAllCards;
}

void GameControl::RestCardData()
{
    //洗牌
    InitAllCards();
    //清空所有玩家的牌
    MyRobotLeft->ClearCards();
    MyRobotRight->ClearCards();
    MyUser->ClearCards();
    //清空当前出牌的玩家和牌
     MyPendPlayer= nullptr;
     MyPendCards.clear();

}

void GameControl::StartLordCard()
{
    //qDebug() << "[StartLordCard]entere";
    //PrepareCallHand为虚函数 要重写
    MyCurrentPlayer->PrepareCallHand();
    //发射自定义信号
    emit PlayerStatusChanged(MyCurrentPlayer, ThinkingForCallLord);//当前是 玩家考虑叫地主状态 改变玩家状态是 开始考虑叫地主
    //qDebug() << "[StartLordCard] out";
}

void GameControl::BecomeLord(Player* player, int bet)
{
    //当前玩家下注的分最高 设置为地主 将下注的分数保存下来
  MyCurrentBet = bet;
  player ->SetRole(Player::Lord);
  //通过当前玩家的上家和下家函数找到 然后通过Set Role函数设为农民
  player->GetPrevPlayer()->SetRole(Player::Famer);
  player->GetNextPlayer()->SetRole(Player::Famer);

  //设置当前玩家为地主之后 再将其设为出牌玩家
  MyCurrentPlayer = player;
  //获取最后三张牌
  player->StoreDispatchCard(MyAllCards);

  QTimer::singleShot(1000,this,[=](){
      //发射信号改变状态 给主界面发信号
      emit GameStatusChanged(PlayingHand);//信号是出牌 改变游戏状态
      emit PlayerStatusChanged(player, ThinkingForPlayHand);//发射信号 玩家考虑出牌
      //准备出牌 PrepareCallHand为虚函数 需重写
      MyCurrentPlayer->PreparePlayHand();
  });


}

void GameControl::ClearPlyerScore()
{
    //将所有玩家的分数重置为0
    MyRobotLeft->SetScore(0);
    MyRobotRight->SetScore(0);
    MyUser->SetScore(0);
}

int GameControl::GetPlayerMaxBet()
{
    return MyBetRecord.bet;
}

void GameControl::OnGrabBet(Player *player, int bet)
{

    //通知主界面玩家叫地主 更新信息提示 发射信号
    if(bet ==0 || MyBetRecord.bet >= bet)
    {
        emit NotifyGrabLordBet(player, 0,false);
    }
     else if(bet > 0 && MyBetRecord.bet == 0)
    {
        //第一个抢地主的
          emit NotifyGrabLordBet(player, bet, true);
    }
    else
    {
        //第2 3 个抢地主的
          emit NotifyGrabLordBet(player, bet,false);
    }

    //判断玩家下注是不是3分 如果是 则抢地主结束
    if (bet == 3)
    {
        //成为地主
        BecomeLord(player,bet);
        //清空数据
        MyBetRecord.reset();
        return;
    }
    //下注不够3分 对玩家的分数进行比较 分数高的是地主
    if(MyBetRecord.bet < bet)
    {
        MyBetRecord.player = player;
        MyBetRecord.bet = bet;
    }
    MyBetRecord.times++;
    //如果每个玩家都抢过地主 抢地主阶段结束
    if(MyBetRecord.times== 3)
    {
        if(MyBetRecord.bet == 0)
        {
            //发射信号回到发牌阶段
            emit GameStatusChanged(DisPatchCard);
        }
        else//有一个人抢过 该玩家即为地主
        {
             BecomeLord(MyBetRecord.player,MyBetRecord.bet);
        }
        MyBetRecord.reset();
        return;
    }
    //切换玩家，通知下一个玩家 继续抢地主
    MyCurrentPlayer = player->GetNextPlayer();
    //发送信号给主界面 告知当前状态仍为抢地主状态
   emit PlayerStatusChanged(MyCurrentPlayer,ThinkingForCallLord);
    MyCurrentPlayer->PrepareCallLord();
}

void GameControl::OnPlayHand(Player *player, Cards &cards)
{

    //将玩家的出牌信号 转发给主页面
    emit NotifyPlayHand(player, cards);//信号发送gamepanel类 即主界面 对应的槽函数是OnDisposePlayHand
    //处理pass的情况
    if(cards.IsEmpty())
    {
        MyPassCount++;
        //连续两次的pass
        if(MyPassCount >= 2)
        {
            qDebug() << "[两次pass了]";
            MyPendCards.clear();
            MyPendPlayer = player;//当前玩家获得出牌
            MyPassCount = 0;
        }
    }
    //如果不是空牌 给其他玩家发送信号，
    else if(!cards.IsEmpty())
    {
        //保存出牌玩家和打出的牌
        MyPendCards = cards;
        MyPendPlayer = player;
        MyPassCount = 0;
        //发射信号给槽函数 保存出牌人和牌
        emit PendingInfo(player, cards);//本类即接受这个信号 因为是发给每个游戏玩家 对应的槽函数是StorePendingInfo
    }
    //如果有炸弹 则底分翻倍
    //根据当前的cards判断 是否未炸弹
    PlayHand::HandType type = PlayHand(cards).GetHandType();//得到类型
    if(type == PlayHand::Hand_Bomb || type == PlayHand::Hand_Bomb_Jokers)
    {
        MyCurrentBet *= 2;
    }
    //如果玩家的牌出完了 计算本局的总分
    if(player->GetCards().IsEmpty())
    {
        //取出另外的玩家
        Player * prev = player->GetPrevPlayer();
        Player* next = player->GetNextPlayer();
        //判断当前的玩家
        if(player->GetRole() == Player::Lord)//地主
        {
            player->SetScore(player->GetScore() + 2 *MyCurrentBet);
            prev->SetScore(prev->GetScore() - MyCurrentBet);
            next->SetScore(next->GetScore() - MyCurrentBet);

            //记录状态
            player->SetIsWin(true);
            prev->SetIsWin(false);
            next->SetIsWin(false);

        }
        else //农民
        {
            player->SetIsWin(true);
            player->SetScore(player->GetScore() + MyCurrentBet);
            //判断其他玩家 谁是同一阵营
            if(prev->GetRole() == Player::Famer)
            {
                prev->SetIsWin(true);
                next->SetIsWin(false);
                prev->SetScore(prev->GetScore() + MyCurrentBet);
                next->SetScore(next->GetScore() - 2 * MyCurrentBet);
            }
            else //if(next->GetRole() == Player::Famer)
            {
                next->SetIsWin(true);
                prev->SetIsWin(false);
                next->SetScore(next->GetScore() + MyCurrentBet);
                prev->SetScore(prev->GetScore() - 2 * MyCurrentBet);
            }


        }
        emit PlayerStatusChanged(player, GameControl::Wining);//发射信号 表示游戏结束 给主界面
        return ;
    }
    //牌没有出完 切换下一个玩家
    MyCurrentPlayer = player->GetNextPlayer();
    MyCurrentPlayer->PreparePlayHand();

    emit PlayerStatusChanged(MyCurrentPlayer, GameControl::ThinkingForPlayHand);//发射信号 表示游戏继续
}

