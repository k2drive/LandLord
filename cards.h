#ifndef CARDS_H
#define CARDS_H
#include"card.h"
#include <QSet>
#include <QMetaType>

class Cards
{

public:
    enum SortType{
        Asc,//升序
        Desc,//降序
        Nosort//不排序
    };
    Cards();
    Cards(const Card & card);

    //添加扑克牌
    void add(const Card& card);//添加一张
    void add(const Cards& cards);//添加多张
    void add(const  QVector<Cards> &cards);

    //一次插入多个数据 （重载<<）
    Cards& operator <<(const Card& card);
    Cards& operator << (const Cards& cards);

    //删除扑克牌
    void remove(const Card& card);//删除一张
    void remove(const Cards& cards);//删除多张
    void remove(const QVector<Cards> &cards);    //删除多个多张卡牌对象

    //扑克牌的数量
    int cardCount();
    //判断当前手牌是否为空
    bool IsEmpty();
    //清空扑克牌
    void clear();

    //最大点数
    Card::CardPoint MaxPoint();
    //最小点数
    Card::CardPoint MinPoint();
    //得到手中最小的牌
    Cards GetMinCard();
    //指定点数的牌的数量
    int PointCount(Card::CardPoint point);
    //判断某张牌是否在集合中
    bool contains(const Card& card);//判断一张
    bool contains(const Cards& card);//判断多张

    //随机取一张扑克牌
    Card TakeRandCard();

    //转换函数 将MCard 转换为CardList
    CardList ToCardList(SortType types = Desc);
private:
    QSet<Card> MyCards;

};

//Q_DECLARE_METATYPE(Cards)
#endif // CARDS_H

