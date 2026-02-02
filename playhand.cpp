#include <QDebug>
#include "playhand.h"

PlayHand::PlayHand()
{

}

PlayHand::PlayHand(Cards &cards)
{
    //分类牌 统计相同点数的个数 一张牌有多少种 2张 3张 用容器记录
    Classify(cards);
    //qDebug() << "classify ok";
    //对牌型进行判定
    JudgeCardType();
    //qDebug() << "JudgeCardType ok";
}

PlayHand::PlayHand(PlayHand::HandType type, Card::CardPoint pt, int extra)
{
    MyType = type;
    MyPt = pt;
    MyExtra = extra;

}

void PlayHand::Classify(Cards &cards)
{
    //取出传入的 将cards转换为card 放入vector<card>中
    CardList list = cards.ToCardList();

    //创建一个数组 与点数对应相同 下标是点数 存储的是该点数对应的个数
    int  CardRecore[Card::CardEnd];
    //初始化
    memset(CardRecore, 0, sizeof (int) * Card::CardEnd);

    //遍历 下标是点数 其存储的是点数对应的个数
    for (int i = 0; i < list.size(); i++)
    {
        Card c = list.at(i);
        CardRecore[c.point()]++;
    }

    //清空容器
    MyOneCard.clear();
    MyTwoCard.clear();
    MyThreeCard.clear();
    MyFourCard.clear();

    //通过CardRecord遍历 判断当前是几张牌
    for(int i = 0; i < Card:: CardEnd; i++)
    {
        if(CardRecore[i] == 1)
        {
            MyOneCard.push_back((Card::CardPoint)i);
        }

        else if (CardRecore[i] == 2)
        {
            MyTwoCard.push_back((Card::CardPoint)i);
        }
        else if(CardRecore[i] == 3)
        {
            MyThreeCard.push_back((Card::CardPoint)i);
        }
        else if (CardRecore[i] == 4)
        {
            MyFourCard.push_back((Card::CardPoint)i);
        }
    }
    /*
    qDebug() <<"OneCard is" << MyOneCard.size();
    qDebug() <<"TwoCard is"  << MyTwoCard.size();
    qDebug() <<"ThreeCard is"  << MyThreeCard.size();
    qDebug() <<"FourCard is" << MyFourCard.size();
    */
}

void PlayHand::JudgeCardType()
{
    MyType = Hand_Unknown;
    MyPt = Card::CardBegin;//等于初始值
    MyExtra = 0;

    if(IsPass())
    {
        //qDebug() <<"1 IsPass ok";
        MyType = Hand_Pass;
    }

    if(IsSingle())
    {
        //qDebug() <<"2 IsSingle ok";
        MyType = Hand_Single;
        MyPt = MyOneCard[0];
    }
    if(ISPair())
    {
        //qDebug() <<"3 ISPair ok";
        MyType = Hand_Pair;
        MyPt= MyTwoCard[0];
    }
    if(IsTripleSingle())
    {
        //qDebug() <<"5IsTripleSingle ok";
        MyType =  Hand_Triple_single;
        MyPt = MyThreeCard[0];
    }
    if(IsTriplePair())
    {
        //qDebug() <<"6 IsTriplepair ok";
        MyType =  Hand_Triple_Pair;
        MyPt = MyThreeCard[0];
    }
    if(ISTriple())
    {
        //qDebug() <<"4 ISTriple ok";
        MyType =  Hand_Triple;
        MyPt = MyThreeCard.at(0);
    }
    if(IsPlaneTwoSingle())
    {
        //qDebug() <<"8 IsPlaneTwoSingle ok";
        MyType =  Hand_Plane_Two_singe;
        //记录点数较小的
        MyPt = MyThreeCard[0];
    }
    if(IsPlaneTwoPair())
    {
        //qDebug() <<"9 IsPlaneTwoPair ok";
        MyType =  Hand_Plane_Two_Pair;
        //记录点数较小的
        MyPt = MyThreeCard[0];
    }
    if(IsPlane())
    {
        //qDebug() <<"7 IsPlane ok";
        MyType =  Hand_Plane;
        //记录点数较小的
        MyPt = MyThreeCard[0];
    }

    if(IsSeqPair())
    {
        //qDebug() <<"10 IsSeqPair ok";
        MyType =  Hand_Seq_Pair;
        //记录点数较小的
        MyPt = MyTwoCard[0];
        //记录连对的个数
        MyExtra = MyTwoCard.size();
    }
    if(IsSeqSingle())
    {
        //qDebug() <<"11 IsSeqSingle ok";
        MyType =  Hand_Seq_Single;
        //记录点数较小的
        MyPt = MyOneCard[0];
        //记录连对的个数
        MyExtra = MyOneCard.size();
    }
    if(IsBomb())
    {
        //qDebug() <<"12 IsBomb ok";
        MyType =  Hand_Bomb;
        //记录点数较小的
        MyPt = MyFourCard[0];
    }
    if(IsBombSingle())
    {
        //qDebug() <<"13 IsBombSingle ok";
        MyType =  Hand_Bomb_Single;
        //记录点数较小的
        MyPt = MyFourCard[0];
    }
    if(IsBombTwoSingle())
    {
        //qDebug() <<"14 IsBombTwoSingle ok";
        MyType =  Hand_Bomb_Two_Single;
        //记录点数较小的
        MyPt = MyFourCard[0];
    }
    if(IsBombJokers())
    {
        //qDebug() <<"15 IsBombJokers ok";
        MyType =  Hand_Bomb_Jokers;
    }
    if(IsBombJokersSingle())
    {
        //qDebug() <<"16 IsBombJokersSingle ok";
        MyType =  Hand_Bomb_Jokers_Single;
    }
    if(ISBombJokersPair())
    {
        //qDebug() <<"17 ISBombJokersPair ok";
        MyType =  Hand_Bomb_Jokers_Pair;
    }
    if(IsBombJokersTwoSingle())
    {
        //qDebug() <<"18 IsBombJokersTwoSingle ok";
        MyType =  Hand_Bomb_Jokers_Two_Single;
    }


}

bool PlayHand::IsPass()
{
    if(MyOneCard.size() == 0 && MyTwoCard.isEmpty()
            && MyThreeCard.isEmpty() && MyFourCard.isEmpty())
    {
        return true;
    }

    return false;
}

PlayHand::HandType PlayHand::GetHandType()
{
    return MyType;
}

Card::CardPoint PlayHand::GetCardPoint()
{
    return  MyPt;
}

int PlayHand::GetExtra()
{
    return MyExtra;
}

bool PlayHand::CanBeat(const PlayHand &other)
{
    //我的牌型未知
    if(MyType == Hand_Unknown)
    {
        return false;
    }
    //对方放弃出牌
    if(other.MyType == Hand_Pass)
    {
        return true;
    }
    //我是王炸
    if(MyType == Hand_Bomb_Jokers)
    {
        return true;
    }
    //我是炸弹 其他人不是
    if(MyType == Hand_Bomb && other.MyType >= Hand_Single && other.MyType <= Hand_Seq_Single)
    {
        return true;
    }
    //双方牌型一致
    if(MyType == other.MyType)
    {
        //都是连对或者是顺子
        if(MyType == Hand_Seq_Pair || MyType == Hand_Seq_Single)
        {
            return MyPt > other.MyPt && MyExtra == other.MyExtra;
        }
        else //不是连对也不是顺子
        {
            return MyPt > other.MyPt;
        }
    }
    return false;//我的牌比你的小

}

bool PlayHand::IsSingle()
{
    if(MyOneCard.size() == 1 && MyTwoCard.isEmpty()
            && MyThreeCard.isEmpty() && MyFourCard.isEmpty())
    {
        return true;
    }

    return false;
}

bool PlayHand::ISPair()
{
    if(MyTwoCard.size() == 1 && MyOneCard.isEmpty()
            && MyThreeCard.isEmpty() && MyFourCard.isEmpty())
    {
        return true;
    }

    return false;
}

bool PlayHand::ISTriple()
{
    //qDebug() << "Triple in ok";
    if(MyThreeCard.size() == 1 && MyOneCard.isEmpty()
            && MyTwoCard.isEmpty() && MyFourCard.isEmpty())
    {
        return true;
    }

    return false;
}

bool PlayHand::IsTripleSingle()
{
    //qDebug() << "IsTripleSingle in ok";
    if(MyThreeCard.size() == 1 && MyOneCard.size() == 1
            && MyTwoCard.isEmpty() && MyFourCard.isEmpty())
    {
        return true;
    }

    return false;
}

bool PlayHand::IsTriplePair()
{
    //qDebug() << "IsTriplePair in ok";
    if(MyThreeCard.size() == 1 && MyOneCard.isEmpty()
            && MyTwoCard.size() == 1 && MyFourCard.isEmpty())
    {
        return true;
    }

    return false;
}

bool PlayHand::IsPlane()
{
    if(MyThreeCard.size() == 2 && MyOneCard.isEmpty()
            && MyTwoCard.isEmpty() && MyFourCard.isEmpty())
    {
        //判断是否相邻 且不为2
        std::sort(MyThreeCard.begin(), MyThreeCard.end());
        if(MyThreeCard[1] - MyThreeCard[0] == 1 && MyThreeCard[1] < Card::Card2)
        {
            return true;
        }
    }

    return false;
}

bool PlayHand::IsPlaneTwoSingle()
{
    if(MyThreeCard.size() == 2 && MyOneCard.size() == 2
            && MyTwoCard.isEmpty() && MyFourCard.isEmpty())
    {
        //判断是否相邻 且不为2 单张不是大小王
        std::sort(MyThreeCard.begin(), MyThreeCard.end());
        std::sort(MyOneCard.begin(), MyOneCard.end());
        if(MyThreeCard[1] - MyThreeCard[0] == 1 && MyThreeCard[1] < Card::Card2
                && MyOneCard[1] != Card::CardBj && MyOneCard[0] != Card::CardSj)
        {
            return true;
        }
    }

    return false;
}

bool PlayHand::IsPlaneTwoPair()
{
    if(MyThreeCard.size() == 2 && MyOneCard.isEmpty()
            && MyTwoCard.size() == 2 && MyFourCard.isEmpty())
    {
        //判断是否相邻 且不为2
        std::sort(MyThreeCard.begin(), MyThreeCard.end());
        if(MyThreeCard[1] - MyThreeCard[0] == 1 && MyThreeCard[1] < Card::Card2)
        {
            return true;
        }
    }

    return false;
}

bool PlayHand::IsSeqPair()
{
    if(MyThreeCard.isEmpty() && MyOneCard.isEmpty()
            && MyTwoCard.size() >= 3 && MyFourCard.isEmpty())
    {
        //判断是否相邻 且不为2
        std::sort(MyTwoCard.begin(), MyTwoCard.end());
        if(MyTwoCard.last() - MyTwoCard.first() == (MyTwoCard.size() - 1)
                && MyTwoCard.last() >= Card::Card3 && MyTwoCard.last() < Card::Card2)
        {
            return true;
        }
    }

    return false;
}

bool PlayHand::IsSeqSingle()
{
    if(MyThreeCard.isEmpty() && MyOneCard.size() >= 5
            && MyTwoCard.isEmpty() && MyFourCard.isEmpty())
    {
        //判断是否相邻 且不为2
        std::sort(MyOneCard.begin(), MyOneCard.end());
        if(MyOneCard.last() - MyOneCard.first() == (MyOneCard.size() - 1)
                && MyOneCard.last() >= Card::Card3 && MyOneCard.last()< Card::Card2)
        {
            return true;
        }
    }

    return false;
}

bool PlayHand::IsBomb()
{
    if(MyThreeCard.isEmpty() && MyOneCard.isEmpty()
            && MyTwoCard.isEmpty() && MyFourCard.size() == 1)
    {
        return true;

    }

    return false;
}

bool PlayHand::IsBombSingle()
{
    if(MyThreeCard.isEmpty() && MyOneCard.size() == 1
            && MyTwoCard.isEmpty() && MyFourCard.size() == 1)
    {
        return true;

    }

    return false;
}

bool PlayHand::IsBombPair()
{
    if(MyThreeCard.isEmpty() && MyOneCard.isEmpty()
            && MyTwoCard.size() == 1 && MyFourCard.size() == 1)
    {
        return true;

    }

    return false;
}

bool PlayHand::IsBombTwoSingle()
{
    if(MyThreeCard.isEmpty() && MyOneCard.size() == 2
            && MyTwoCard.isEmpty() && MyFourCard.size() == 1)
    {
        std::sort(MyOneCard.begin(), MyOneCard.end());
        if(MyOneCard.first() != Card::CardSj && MyOneCard.last() != Card::CardBj)
        {
            return true;
        }

    }

    return false;
}

bool PlayHand::IsBombJokers()
{
    if(MyThreeCard.isEmpty() && MyOneCard.size() == 2
            && MyTwoCard.isEmpty() && MyFourCard.isEmpty())
    {
        std::sort(MyOneCard.begin(), MyOneCard.end());
        if(MyOneCard.first() == Card::CardSj && MyOneCard.last() == Card::CardBj)
        {
            return true;
        }
    }

    return false;
}

bool PlayHand::IsBombJokersSingle()
{
    if(MyThreeCard.isEmpty() && MyOneCard.size() == 3
            && MyTwoCard.isEmpty() && MyFourCard.isEmpty())
    {
        std::sort(MyOneCard.begin(), MyOneCard.end());
        if(MyOneCard[1] == Card::CardSj && MyOneCard[2]== Card::CardBj)
        {
            return true;
        }
    }

    return false;
}

bool PlayHand::ISBombJokersPair()
{
    if(MyThreeCard.isEmpty() && MyOneCard.size() == 2
            && MyTwoCard.size() == 1 && MyFourCard.isEmpty())
    {
        std::sort(MyOneCard.begin(), MyOneCard.end());
        if(MyOneCard[0] == Card::CardSj && MyOneCard[1]== Card::CardBj)
        {
            return true;
        }
    }

    return false;
}

bool PlayHand::IsBombJokersTwoSingle()
{
    if(MyThreeCard.isEmpty() && MyOneCard.size() == 4
            && MyTwoCard.isEmpty() && MyFourCard.isEmpty())
    {
        std::sort(MyOneCard.begin(), MyOneCard.end());
        if(MyOneCard[2] == Card::CardSj && MyOneCard[4]== Card::CardBj)
        {
            return true;
        }
    }

    return false;
}
