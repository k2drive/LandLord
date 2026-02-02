#include "card.h"

Card::Card()
{

}

Card::Card(Card::CardPoint point, Card::CardSuit suit)
{
    setsuit(suit);
    setPoint(point);
}

void Card::setPoint(Card::CardPoint point)
{
    MyPoint = point;
}

void Card::setsuit(Card::CardSuit suit)
{
    MySuit = suit;
}

Card::CardPoint Card::point()const
{
    return MyPoint;
}


Card::CardSuit Card::suit() const
{
    return MySuit;
}

bool Card::operator<(const Card &other) const
{
    // 先比点数
       if (point() != other.point())
           return point() < other.point();

       // 点数相同再比花色
       return suit() < other.suit();
}

bool lessSort(const Card &c1, const Card &c2)
{
    if(c1.point() == c2.point())
    {
        return c1.suit()<c2.suit();
    }
    else
        return c1.point() < c2.point();
}

bool greaterSort(const Card &c1, const Card &c2)
{
    if(c1.point() == c2.point())
    {
        return c1.suit() > c2.suit();
    }
    else
        return c1.point() > c2.point();
}

bool operator ==(const Card &left, const Card &right)
{
    //花色和点数全都相同 则返回真 ，反之则假
    return (left.point() == right.point() && left.suit() == right.suit());
}

uint qHash(const Card &card)
{
    //为不同花色和点数计算一个独一无二的哈系数
    return card.point() * 100 + card.suit();
}
/*可以使用重载  这里使用的是 在使用++的时候使用static_cast
Card::CardPoint &operator ++(Card::CardPoint &point)
{
    int value = static_cast<int>(point)+1;
    point = static_cast<Card::CardPoint>(value);
    return point;
}

Card::CardSuit &operator ++(Card::CardSuit &suit)
{
    int value = static_cast<int>(suit)+1;
    suit = static_cast<Card::CardSuit>(value);
    return suit;
}
*/
