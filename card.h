
#ifndef CART_H
#define CART_H

#include<QVector>

class Card
{
public:

    //花色枚举
    enum CardSuit{
        SuitBegin,//开始
        Diamond,//方块
        Club,//梅花
        Heart,//心形
        Spade,//黑桃
        SuitEnd//结束
    };

    //数字枚举
    enum CardPoint{
        CardBegin,
        Card3,
        Card4,
        Card5,
        Card6,
        Card7,
        Card8,
        Card9,
        Card10,
        CardJ,
        CardQ,
        CardK,
        CardA,
        Card2,
        CardSj,//小王 small joker
        CardBj,//大王  big joker
        CardEnd
    };

    Card();
    //带参构造函数
    Card(CardPoint point,CardSuit suit);

    //为私有成员变量提供set和get成员函数让其更改
    void setPoint(CardPoint point);
    void setsuit(CardSuit suit);
    CardPoint point() const;
    CardSuit suit() const;

    //重载<操作符
     bool operator<(const Card& other) const;
    //成员变量
private:
    CardSuit MySuit;
    CardPoint MyPoint;
};

//对象对比
bool lessSort(const Card& c1,const Card& c2);
bool greaterSort(const Card& c1,const Card& c2);

//操作符重载（==）
bool operator ==(const  Card& left, const Card& right);

//对操作符++进行重载
//直接在使用++的时候 直接使用static_cast
//Card::CardPoint & operator ++ (Card::CardPoint& point);
//Card::CardSuit& operator ++  (Card::CardSuit& suit);

//重写qHash全局函数
uint qHash (const Card& card);

//对新的有序序进行重命名
using CardList= QVector<Card>;
#endif // CART_H
