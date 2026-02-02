#ifndef PLAYHAND_H
#define PLAYHAND_H

#include "card.h"
#include "cards.h"



class PlayHand
{
public:
    //出牌的组合和方式
    enum HandType
    {
        Hand_Unknown, //未知 非法牌
        Hand_Pass, //过

        Hand_Single, //单张
        Hand_Pair, //对子

        Hand_Triple, // 三个
        Hand_Triple_single, //三带一
        Hand_Triple_Pair, //三带二

        Hand_Plane, //飞机
        Hand_Plane_Two_singe, //飞机带单张
        Hand_Plane_Two_Pair,  //飞机带对子

        Hand_Seq_Pair, //连对
        Hand_Seq_Single,  //顺子

        Hand_Bomb,  //炸弹
        Hand_Bomb_Single,  //四代一
        Hand_Bomb_Pair,  //四带一对
        Hand_Bomb_Two_Single,  //四带二

        Hand_Bomb_Jokers, //王炸
        Hand_Bomb_Jokers_Single, //王炸带一
        Hand_Bomb_Jokers_Pair, //王炸带对子
        Hand_Bomb_Jokers_Two_Single //王炸带二

    };
    PlayHand();

    //传递给类一组牌 通过类分析出牌型 点数 以及相关的附属信息(比如顺子 ：记录牌的数量)
    PlayHand (Cards & cards);
    PlayHand (HandType type , Card::CardPoint pt, int  extra);

    //得到牌的属性
    HandType GetHandType();
    Card::CardPoint GetCardPoint();
    int GetExtra();

    //比较自己的牌型和其他人牌型的大小
    bool CanBeat(const PlayHand& other);


private:

    //分类牌 统计相同点数的个数 一张牌有多少种 2张 3张 用容器记录
    void Classify(Cards & cards);
    //对牌型进行判定
    void  JudgeCardType();

    //通过函数判读当前是什么类型的牌
    bool IsPass();  //放弃出牌
    bool IsSingle();  //单张
    bool ISPair(); //对子
    bool ISTriple(); // 三个相同的
    bool IsTripleSingle(); // 三带一
    bool IsTriplePair(); //三带一对
    bool IsPlane(); // 飞机
    bool IsPlaneTwoSingle();  //飞机带两单
    bool IsPlaneTwoPair();  //飞机带两对
    bool IsSeqPair();  //连对
    bool IsSeqSingle();  //顺子
    bool IsBomb();  //炸弹
    bool IsBombSingle();  //四带一
    bool IsBombPair();  // 四带一对
    bool IsBombTwoSingle();  //四带二
    bool IsBombJokers();  //王炸
    bool IsBombJokersSingle();  //王炸带单张
    bool ISBombJokersPair(); // 王炸带一对
    bool IsBombJokersTwoSingle();  // 王炸带两单



private:
    HandType MyType;
    Card::CardPoint  MyPt;
    int MyExtra;
    QVector<Card::CardPoint> MyOneCard; //存储单张点数的扑克牌
    QVector<Card::CardPoint> MyTwoCard; //存储两张点数的扑克牌
    QVector<Card::CardPoint> MyThreeCard; //存储三张点数的扑克牌
    QVector<Card::CardPoint> MyFourCard; //存储四张点数的扑克牌 例如村的3则说明有炸弹是3
};

#endif // PLAYHAND_H
