#include "cards.h"
#include<QSet>
#include"card.h"
#include<QRandomGenerator>

Cards::Cards()
{

}

Cards::Cards(const Card &card)
{
    add(card);
}

void Cards::add(const Card& card)
{
    MyCards.insert(card);
}

void Cards::add(const Cards& cards)
{
    MyCards.unite(cards.MyCards);
}

void Cards::add(const QVector<Cards> &cards)
{
    for(int i = 0; i < cards.size(); i++)
    {
        add(cards.at(i));
    }
}

Cards &Cards::operator <<(const Card &card)
{
    Cards::add(card);
    return *this;
}

Cards &Cards::operator <<(const Cards &cards)
{
    Cards::add(cards);
    return *this;
}



void Cards::remove(const Card &card)
{
    MyCards.remove(card);
}

void Cards::remove(const Cards &cards)
{
    MyCards.subtract(cards.MyCards);
}

void Cards::remove(const QVector<Cards> &cards)
{
    for (int i = 0; i < cards.size();i++)
    {
        remove(cards.at(i));
    }
}

int Cards::cardCount()
{
    return MyCards.size();
}

bool Cards::IsEmpty()
{
    return MyCards.isEmpty();
}

void Cards::clear()
{
    MyCards.clear();
}

Card::CardPoint Cards::MaxPoint()
{
    Card::CardPoint Max = Card::CardBegin;
    if(!MyCards.isEmpty())
    {
        for(auto it = MyCards.begin();it != MyCards.end();it++)
        {
            if(it->point() > Max)
            {
                Max = it->point();
            }
        }

    }
    return Max;
}

 Card::CardPoint Cards::MinPoint()
{
    Card::CardPoint Min = Card::CardEnd;
    if(!MyCards.isEmpty())
    {
        for (const Card& it:MyCards)
        {
            if(it.point()< Min)
            {
                Min = it.point();
            }
        }
    }
    return Min;
 }

 Cards Cards::GetMinCard()
 {
     if(MyCards.isEmpty())
     {
         //空则返回 空
         return Cards();
     }
    Card::CardPoint point = MinPoint();//得到最小点数
    Cards card;
    //在手牌中找到最小点数对应的任意一张牌
    auto it = MyCards.begin();
    //空循环 查找到要找的牌
    for(; it != MyCards.end(); it++)
    {
        if(it->point() == point)
        {
            //通过迭代器找到卡牌
            card = *it;
            //去除要出的卡牌
            MyCards.erase(it);
            //返回的card则为删除的某张牌后的对象
            return card;
        }
    }
    //没找到最小的牌 返回空
    return Cards();
 }

int Cards::PointCount(Card::CardPoint point)
{
    int count = 0;//指定点数的牌的数量的数量的计数；
    for(auto it = MyCards.begin(); it != MyCards.end(); it++)
    {
        if(it->point() == point)
        {
            count++;
        }
    }
    return count;
}

bool Cards::contains(const Card &card)
{
    return MyCards.contains(card);
}

bool Cards::contains(const Cards &cards)
{
    return MyCards.contains(cards.MyCards);
}

Card Cards::TakeRandCard()
{
    //生成一个随机数
    int num = QRandomGenerator::global()->bounded(MyCards.size());
    //只读迭代器
    QSet<Card>::const_iterator it = MyCards.constBegin();
    //空循环 查找到要找的牌
    for(int i =0; i < num ;i++,it++);
    //通过迭代器找到卡牌
    Card card = *it;
    //去除要删除的卡牌
    MyCards.erase(it);
    //返回的card则为删除的某张牌后的对象
    return card;
}

CardList Cards::ToCardList(Cards::SortType type)
{
    CardList list;
    for(const Card& it : MyCards)
    {
        list << it;
    }
    if(type == Asc)//升序排列
    {
        std::sort(list.begin(),list.end(),lessSort);
    }
    else if (type == Desc)
    {
        std::sort(list.begin(),list.end(),greaterSort);
    }

    return list;
}



