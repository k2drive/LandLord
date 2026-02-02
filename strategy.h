#ifndef STRATEGY_H
#define STRATEGY_H

#include"player.h"
#include"playhand.h"

class Strategy
{
public:
    Strategy(Player* player, const Cards& cards);

    //指定出牌的策略
    Cards MakeStrategy();
    //第一个出牌
    Cards FistPlay();
    //得到比指定牌型大的牌
    Cards GetGreaterCards(PlayHand type);
    //找到大于指定的牌时， 判断是否出牌or过 true则出牌 false则过
    bool WhetherToBeat(Cards& cards);//出牌算法

    //找出指定数量的 相同点数的 牌 找出count张 点数为point的牌
    Cards FindSamePointCards(Card::CardPoint point, int count);
    //找出所有点数 数量为count的牌-> 得到一个多张扑克牌数组 比如count为4 则是要找到所有的炸弹
    QVector<Cards> FindCardsByCount(int count);
    //根据点数范围找牌
    Cards GetRangeCards(Card::CardPoint begin, Card::CardPoint end);
    //按照牌型找牌， 并且指定要找的牌是否要大过指定的牌型 beat为真则表示是要大于 反之则不管 给最小
    QVector<Cards> FindCardType(PlayHand hand, bool beat);

    //从指定的Cards对象中挑出满足条件的顺子
    void PickSeqSngles(QVector<QVector<Cards>> & AllSeqRecord,const  QVector<Cards> &SeqSingle, const Cards& cards);
    //最优的顺子的集合的筛选函数
    QVector<Cards> PickOptimalSeqSngles();

    //调试函数
    void setDebugForceCards(const Cards &cards);
     void clearDebugForce();

private:
    using function = Cards(Strategy::*)(Card::CardPoint point);//函数指针
    struct  CardInfo
    {
        Card::CardPoint begin;
        Card::CardPoint end;
        int extra;//顺子or连对的数量
        bool beat;
        int number;//指定点数的牌的数量
        int base;//最基础的顺子or连对的数量
        function getseq;
    };
    //以point进行搜索 找number张
    QVector<Cards> GetCards(Card::CardPoint point , int number);
    //找三带一or二的第二步
    QVector<Cards> GetTripleSingleOrPair(Card::CardPoint begin, PlayHand::HandType type);
    //飞机普通的
    QVector<Cards> GetPlane(Card::CardPoint begin);
    //找飞机带一or二
    QVector<Cards> GetPlaneTwoSingleOrTwoPair(Card::CardPoint begin, PlayHand::HandType type);
    QVector<Cards> GetSeqPairOrSeqSingle(CardInfo & info);
    Cards GetBaseSeqPair(Card::CardPoint point);
    Cards GetBaseSeqSingle(Card::CardPoint point);
    QVector<Cards> GetBomb(Card::CardPoint begin);

private:

    Player* MyPlayer;//保存已出牌的玩家
    Cards MyCards;//保存已出的牌
    bool MyDebugForce = false;
    Cards MyForcecards;

};

#endif // STRATEGY_H
