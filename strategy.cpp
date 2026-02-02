#include "strategy.h"
#include <functional>
#include<QMap>
#include<QDebug>

Strategy::Strategy(Player *player, const Cards &cards)
{
    MyPlayer = player;
    MyCards = cards;
}

Cards Strategy::MakeStrategy()
{
    //得到出牌玩家对象和打出的牌
    Player* PendPlayer = MyPlayer->GetPendingPlayer();
    Cards PendCards = MyPlayer->GetPendingCards();
    //判断上次出牌的玩家是不是自己
    if(PendPlayer == MyPlayer || PendPlayer == nullptr)//上次玩家是我自己（自己出了牌都没有人要 转了一圈又到自己） 或者游戏才开始
    {
        //自己 没有限制
        //直接出牌
        return FistPlay();

    }
    else
    {
        //已出牌玩家不是自己 需要找出比已出牌玩家点数大的牌
        PlayHand type(PendCards);
        //qDebug() << "MyPlayer cards type is" << type.GetHandType()<< "MyPlayer count is " << PendCards.cardCount() ;
        Cards BeatCards = GetGreaterCards(type);


        //找到了点数大的牌需要考虑是否出牌
        bool ShouldBeat = WhetherToBeat(BeatCards);

        if(ShouldBeat)
        {
            //出牌
            return BeatCards;
        }
        else
        {
            //不出牌
            return Cards();
        }
    }

    return Cards();

}


/*
Cards Strategy::FistPlay()
{

    //判断玩家手中是否只剩单一的牌型
    qDebug() << "enter FistPlay ";
    PlayHand hand(MyCards);
    //是单一的牌型
    if(hand.GetHandType() != PlayHand::Hand_Unknown)
    {
        return MyCards;
    }
    //不是单一的牌型
    //判断玩家手里是否有顺子
    QVector<Cards> OPtimalSeq = PickOptimalSeqSngles();
    if(!OPtimalSeq.isEmpty())
    {
        //得到单牌的数量
        int BaseNUmber = FindCardsByCount(1).size();
        //把得到顺子的集合从玩家的手中删除
        Cards save = MyCards;
        save.remove(OPtimalSeq);
        //最后搜素 删除后顺子后的单牌数量
        int LastNum = Strategy(MyPlayer, save).FindCardsByCount(1).size();
        if(LastNum < BaseNUmber)//删除顺子后的单牌小于未删除的单牌数量
        {
            return OPtimalSeq[0];
        }
    }

    bool HasPlane, HasTriple, HasPair;
    HasPair = HasTriple = HasPlane = false;
    Cards backup = MyCards;//备份

    //有没有炸弹
    QVector<Cards> BombArray = FindCardType(PlayHand(PlayHand::Hand_Bomb, Card::CardBegin, 0),false);
    backup.remove(BombArray);
    //有没有飞机
    QVector<Cards> PlaneArray = Strategy(MyPlayer,backup).FindCardType(PlayHand(PlayHand::Hand_Plane, Card::CardBegin, 0),false);
    if(!PlaneArray.isEmpty())
    {
        HasPlane = true;
        backup.remove(PlaneArray);
    }
    //有没有三个
    QVector<Cards> SeqTripleArray = Strategy(MyPlayer,backup).FindCardType(PlayHand(PlayHand::Hand_Triple, Card::CardBegin, 0),false);
    if(!SeqTripleArray.isEmpty())
    {
        HasTriple = true;
        backup.remove(SeqTripleArray);
    }
    //有咩有连对
    QVector<Cards> SeqPairArray = Strategy(MyPlayer,backup).FindCardType(PlayHand(PlayHand::Hand_Seq_Pair, Card::CardBegin, 0),false);
    if(!SeqPairArray.isEmpty())
    {
        HasPair = true;
        backup.remove(SeqPairArray);
    }

    if(HasPair)//有连对
    {
        //返回最长的连对
        Cards MaxPair;
        for(int i = 0; i < SeqPairArray.size(); i++)
        {
            if(SeqPairArray[i].cardCount() > MaxPair.cardCount())
            {
                MaxPair = SeqPairArray[i];
            }
        }
        return MaxPair;
    }

    if(HasPlane)//有飞机 看能凑出什么样子的飞机
    {
        //飞机带两对子
        bool TwoPairFond = false;//记录有没有两个对子
        QVector<Cards> PairArray;
        for(Card::CardPoint point = Card::Card3; point <= Card::Card10; point = (Card::CardPoint)(point + 1))
        {
            Cards pair = Strategy(MyPlayer, backup).FindSamePointCards(point ,2);//搜索对子
            PairArray.push_back(pair);
            if(PairArray.size() == 2)
            {
                TwoPairFond = true;
                break;
            }
        }
        if(TwoPairFond)//有飞机的两个对子
        {
            Cards temp = PlaneArray[0];
            temp.add(PairArray);
            return temp;
        }
        //飞机带两张单牌
        else
        {
            bool TwoSingleFond = false;//记录有没有两个单牌
            QVector<Cards> SingleArray;
            for(Card::CardPoint point = Card::Card3; point <= Card::Card10; point = (Card::CardPoint)(point + 1))
            {
                if(backup.PointCount(point) == 1)// 当前点数有单牌
                {
                    Cards single = Strategy(MyPlayer, backup).FindSamePointCards(point ,1);//搜索单张
                    SingleArray.push_back(single);
                    if(SingleArray.size() == 2)
                    {
                        TwoSingleFond = true;
                        break;
                    }
                }

            }
            if(TwoSingleFond)//有飞机的两个单牌
            {
                Cards temp = PlaneArray[0];
                temp.add(SingleArray);
                return temp;
            }
            else
            {
                //飞机
                return PlaneArray[0];

            }
        }
    }

    if(HasTriple)//有三张 看能不能凑出三带一or二
    {
        //将搜索每个点数  每个点数进行判定
        if(PlayHand(SeqTripleArray[0]).GetCardPoint() < Card::CardA)//小于A的1个or2个 则出
        {
            //每一个点数对应的数量有多少 遍历
            for(Card::CardPoint point = Card::Card3; point < Card::CardA; point = (Card::CardPoint)(point + 1))
            {
                //得到点数对应的牌的数量
                int  PointCount = backup.PointCount(point);
                //看是一个还是一对
                if(PointCount == 1)
                {
                    //得到牌的数量
                    Cards Single = Strategy(MyPlayer, backup).FindSamePointCards(point, 1);
                    Cards temp = SeqTripleArray[0];
                    temp.add(Single);
                    return temp;
                }
                else if (PointCount == 2)
                {

                    //得到牌的数量
                    Cards Pair = Strategy(MyPlayer, backup).FindSamePointCards(point, 2);
                    Cards temp = SeqTripleArray[0];
                    temp.add(Pair);
                    return temp;
                }

            }
        }

        //没有对子也没有单张
        return SeqTripleArray[0];
    }

    //出单排or对子
    //对下一个玩家的手牌进行判断
    Player* nextplayer = MyPlayer->GetNextPlayer();
    //判断当前玩家与下一个玩家是不是同一个阵营
    if(nextplayer->getCards().cardCount() == 1 && MyPlayer->GetRole() !=nextplayer->GetRole())//不是同一个阵营
    {
        //从大到小遍历
        for(Card::CardPoint point = Card::CardPoint(Card::CardEnd - 1);
            point >= Card::Card3; point = (Card::CardPoint)(point - 1))
        {
            //当前点数在玩家手牌中的数量有多少
            int pointcount = backup.PointCount(point);
            if(pointcount == 1)
            {
                //要重新定义Strategy 才能获得正确的值
                Cards single = Strategy(MyPlayer,backup).FindSamePointCards(point ,1);
                return single;
            }
            else if(pointcount == 2)
            {
                Cards pair = Strategy(MyPlayer,backup).FindSamePointCards(point,2);
                return pair;
            }
        }
    }
    else //同一个阵营
    {
        //从小到大遍历
        for(Card::CardPoint point = Card::CardPoint(Card::Card3);
            point < Card::CardEnd; point = (Card::CardPoint)(point + 1))
        {
            //当前点数在玩家手牌中的数量有多少
            int pointcount = backup.PointCount(point);
            if(pointcount == 1)
            {
                //要重新定义Strategy 才能获得正确的值
                Cards single = Strategy(MyPlayer,backup).FindSamePointCards(point ,1);
                return single;
            }
            else if(pointcount == 2)
            {
                Cards pair = Strategy(MyPlayer,backup).FindSamePointCards(point,2);
                return pair;
            }
        }
    }

    return Cards();
}

*/
Cards Strategy::FistPlay()
{
    //qDebug() << "[FistPlay] enter";

    // ================== 决策 0：是否一次出完 ==================
    {
        PlayHand hand(MyCards);
        if (hand.GetHandType() != PlayHand::Hand_Unknown)
        {
            qDebug() << "[FistPlay] one hand finish";
            return MyCards;
        }
    }

    // ================== 决策 1：顺子是否能优化手牌结构  该部分所调用的函数正确==================
    {
        QVector<Cards> seqs = PickOptimalSeqSngles();


        if (!seqs.isEmpty())
        {
            int baseSingles = FindCardsByCount(1).size();

            Cards remain = MyCards;
            remain.remove(seqs);

            int afterSingles = Strategy(MyPlayer, remain).FindCardsByCount(1).size();

            if (afterSingles < baseSingles)
            {
                qDebug() << "[FistPlay] choose optimal seq";
                return seqs.first();   // 安全
            }
        }
    }

    // ================== 决策 2：拆牌分析（不越界） ==================
    Cards backup = MyCards;

    QVector<Cards> bombs =FindCardType(PlayHand(PlayHand::Hand_Bomb, Card::CardBegin, 0), false);

    if (!bombs.isEmpty())
    {
        backup.remove(bombs);
    }

    QVector<Cards> planes =Strategy(MyPlayer, backup).FindCardType(PlayHand(PlayHand::Hand_Plane, Card::CardBegin, 0), false);
    if (!planes.isEmpty())
    {
        backup.remove(planes);
    }

    QVector<Cards> triples =Strategy(MyPlayer, backup).FindCardType(PlayHand(PlayHand::Hand_Triple, Card::CardBegin, 0), false);
    if (!triples.isEmpty())
    {
        backup.remove(triples);
    }

    QVector<Cards> seqPairs =Strategy(MyPlayer, backup).FindCardType(PlayHand(PlayHand::Hand_Seq_Pair, Card::CardBegin, 0), false);

    // ================== 决策 3：优先出最长连对 ==================
    if (!seqPairs.isEmpty())
    {
        Cards best;
        for ( Cards &c : seqPairs)
        {
            if (c.cardCount() > best.cardCount())
                best = c;
        }
        qDebug() << "[FistPlay] choose seq pair";
        return best;
    }

    // ================== 决策 4：飞机（带牌） ==================
    if (!planes.isEmpty())
    {
        Cards plane = planes.first();
        Cards remain = MyCards;
        remain.remove(plane);

        QVector<Cards> pairs;
        QVector<Cards> singles;

        for (int p = Card::Card3; p < Card::CardA; ++p)
        {
            Card::CardPoint pt = static_cast<Card::CardPoint>(p);
            int cnt = remain.PointCount(pt);

            if (cnt >= 2)
                pairs << Strategy(MyPlayer, remain).FindSamePointCards(pt, 2);
            else if (cnt == 1)
                singles << Strategy(MyPlayer, remain).FindSamePointCards(pt, 1);
        }

        if (pairs.size() >= 2)
        {
            plane.add(pairs[0]);
            plane.add(pairs[1]);
            qDebug() << "[FistPlay] plane + 2 pairs";
            return plane;
        }

        if (singles.size() >= 2)
        {
            plane.add(singles[0]);
            plane.add(singles[1]);
            qDebug() << "[FistPlay] plane + 2 singles";
            return plane;
        }

        qDebug() << "[FistPlay] pure plane";
        return plane;
    }

    // ================== 决策 5：三带（安全） ==================
    if (!triples.isEmpty())
    {
        Cards triple = triples.first();
        Card::CardPoint tp = PlayHand(triple).GetCardPoint();
        if (tp < Card::CardA)
        {
            Cards remain = MyCards;
            remain.remove(triple);
            for (int p = Card::Card3; p <= Card::CardA; ++p)
            {
                Card::CardPoint pt = static_cast<Card::CardPoint>(p);
                int cnt = remain.PointCount(pt);

                if (cnt == 1)
                {
                    triple.add(Strategy(MyPlayer, remain).FindSamePointCards(pt, 1));
                    qDebug() << "[FistPlay] triple + single";
                    return triple;
                }
                if (cnt == 2)
                {
                    triple.add(Strategy(MyPlayer, remain).FindSamePointCards(pt, 2));
                    qDebug() << "[FistPlay] triple + pair";
                    return triple;
                }
            }
        }

        qDebug() << "[FistPlay] pure triple";
        return triple;
    }

    // ================== 决策 6：单 / 对（考虑阵营） ==================
    Player *next = MyPlayer->GetNextPlayer();
    bool enemyDanger =next && next->GetCards().cardCount() == 1 && MyPlayer->GetRole() != next->GetRole();

    if (enemyDanger)
    {
        // 从大到小
        for (int p = Card::CardEnd - 1; p >= Card::Card3; --p)
        {
            Card::CardPoint pt = static_cast<Card::CardPoint>(p);
            int cnt = backup.PointCount(pt);

            if (cnt == 1)
                return Strategy(MyPlayer, backup).FindSamePointCards(pt, 1);
            if (cnt == 2)
                return Strategy(MyPlayer, backup).FindSamePointCards(pt, 2);
        }
    }
    else
    {
        // 从小到大
        for (int p = Card::Card3; p < Card::CardEnd; ++p)
        {
            Card::CardPoint pt = static_cast<Card::CardPoint>(p);
            int cnt = backup.PointCount(pt);

            if (cnt == 1)
                return Strategy(MyPlayer, backup).FindSamePointCards(pt, 1);
            if (cnt == 2)
                return Strategy(MyPlayer, backup).FindSamePointCards(pt, 2);
        }
    }

    qDebug() << "[FistPlay] no card";
    return Cards();
}


Cards Strategy::GetGreaterCards(PlayHand type)
{
    //type是已出牌玩家的出牌类型
    //出牌玩家和当前玩家是不是同一阵营
    Player* PendPlayer = MyPlayer->GetPendingPlayer();//得到当前出牌玩家

    if(PendPlayer != nullptr && PendPlayer->GetRole() != MyPlayer->GetRole() && PendPlayer->GetCards().cardCount() <= 3)//不同阵营 且当前出牌玩家的牌数量很少了
    {
        qDebug() << "PendPlayer card is <= 3";

        //先找炸弹
        QVector<Cards> bombs = FindCardsByCount(4);
        for(int i = 0; i < bombs.size(); i++)
        {
            if(PlayHand(bombs[i]).CanBeat(type))
            {
                return bombs[i];
            }
        }


        //搜所当前玩家有没有王炸
        Cards sj = FindSamePointCards(Card::CardSj, 1);
        Cards bj = FindSamePointCards(Card::CardBj, 1);

        if(!sj.IsEmpty() && !bj.IsEmpty())//有大王炸 则返回
        {
            Cards jokers;
            jokers << sj << bj;

            return jokers;
        }
    }
    //当前玩家和下一个玩家是不是同一阵营
    Player* NextPlayer = MyPlayer->GetNextPlayer();//得到当前的出牌玩家
    //将玩家手中的顺子剔除出去
    Cards remain = MyCards;//备份
    remain.remove(Strategy(MyPlayer, remain).PickOptimalSeqSngles());//得到删除顺子的牌

    //匿名函数 通过bind可调用对象绑定器 进行绑定 在传参 传递的参数个数 在匿名函数中写入
    auto beatcard =  std::bind([=](const Cards & cards){
        //找到对应牌型的牌 但是比下一个玩家大
        QVector<Cards> BeatCardsArray = Strategy(MyPlayer, cards).FindCardType(type, true);
        //判断是否为空
        if(!BeatCardsArray.isEmpty())//当前玩家找到对应的类型且大于下家的牌
        {
            //不同阵营 且下家牌少于2
            if(MyPlayer->GetRole() != NextPlayer->GetRole() && NextPlayer->GetCards().cardCount() <= 2)
            {

                return BeatCardsArray.back();
            }
            else
            {
                //不急着压 先出最小的
                qDebug()<< "3";
                return BeatCardsArray.front();
            }
        }

        return Cards();
    },std::placeholders::_1);//_1得到可调用参数为1个

    Cards cs;
    if(!(cs = beatcard(remain)).IsEmpty())
    {

        return cs;
    }

    else//剔除顺子之后 没有大于下家的牌 则将顺子加回
    {
        if(!(cs = beatcard(MyCards)).IsEmpty())
        {

            return cs;
        }
    }

    return Cards();
}

/*
Cards Strategy::GetGreaterCards( PlayHand type)
{
    // 1 防御性检查 非空指针
    Player* pendPlayer = MyPlayer->GetPendingPlayer();
    if (!pendPlayer)
    {
        qDebug() << "[GetGreaterCards] no pending player";
        return Cards();
    }

    Player* nextPlayer = MyPlayer->GetNextPlayer();
    if (!nextPlayer)
    {
        qDebug() << "[GetGreaterCards] no next player";
        return Cards();
    }

    qDebug() << "[GetGreaterCards]" << "type =" << type.GetHandType();

    // 2️ 不同阵营 + 对方快没牌了 → 先考虑炸弹
    if (pendPlayer->GetRole() != MyPlayer->GetRole()
            && pendPlayer->getCards().cardCount() <= 3)
    {
        QVector<Cards> bombs = FindCardsByCount(4);
        for ( Cards& bomb : bombs)
        {
            if (PlayHand(bomb).CanBeat(type))
            {
                return bomb;
            }
        }

        // 王炸
        Cards sj = FindSamePointCards(Card::CardSj, 1);
        Cards bj = FindSamePointCards(Card::CardBj, 1);
        if (!sj.IsEmpty() && !bj.IsEmpty())
        {
            Cards jokers;
            jokers << sj << bj;
            return jokers;
        }
    }

    // 3️ 先尝试「剔除顺子后的牌」
    Cards remain = MyCards;
    QVector<Cards> seqs = Strategy(MyPlayer, remain).PickOptimalSeqSngles();
    remain.remove(seqs);

    auto tryBeat = [&](const Cards& source) -> Cards//返回值是cards 匿名函数
    {
        QVector<Cards> arr = Strategy(MyPlayer, source).FindCardType(type, true);

        if (arr.isEmpty())
            return Cards();

        // 敌对阵营且对方快没牌 → 用最大的压
        if (MyPlayer->GetRole() != nextPlayer->GetRole()
                && nextPlayer->getCards().cardCount() <= 2)
        {
            return arr.last();
        }

        // 否则用最小的
        return arr.first();
    };

    // 4️ 两轮尝试
    Cards res = tryBeat(remain);
    if (!res.IsEmpty())
        return res;

    res = tryBeat(MyCards);
    if (!res.IsEmpty())
        return res;

    return Cards();
}

*/


bool Strategy::WhetherToBeat(Cards &cards)
{
    //没有大过对方的牌 false
    if(cards.IsEmpty())
    {
        return false;
    }

    //得到当前出牌玩家的对象
    Player* pendPlayer = MyPlayer->GetPendingPlayer();
    //判断当前玩家和要出牌玩家是不是同一阵营
    if(MyPlayer->GetRole() == pendPlayer->GetRole())
    {
        //手里的牌所剩无几， 且是一个完整的牌型
        Cards left = MyCards;//备份
        left.remove(cards);//现在left是打出后的牌
        //对牌型进行判断
        if(PlayHand(left).GetHandType() != PlayHand::Hand_Unknown)//不是未知的牌型 则可以出牌
        {
            return true;
        }
        //

        Card::CardPoint BasePoint = PlayHand(cards).GetCardPoint(); //获取最小点数
        //如果手里的牌点数中最小的是 2大小王 则不出牌 不拿大牌压同阵营的
        if(BasePoint == Card::Card2 || BasePoint == Card::CardSj || BasePoint == Card::CardBj)
        {
            return false;
        }
    }
    else //不同阵营
    {
        PlayHand MyHand(cards);
        //如果是三个2带一对or单张 则不出牌
        if(MyHand.GetHandType() == PlayHand::Hand_Triple_Pair
                || MyHand.GetHandType() == PlayHand::Hand_Triple_single
                || MyHand.GetCardPoint() == Card::Card2)
        {
            return false;
        }
        //如果cards是对2 并且玩家的手里牌数量大于等于10 && 自己牌数量大于等于5 则暂时放弃出牌
        if(MyHand.GetHandType() == PlayHand::Hand_Pair && MyHand.GetCardPoint() == Card::Card2
                && pendPlayer->GetCards().cardCount() >= 10 && MyPlayer->GetCards().cardCount() >=5)
        {
            return false;
        }
    }
    return true;
}

Cards Strategy::FindSamePointCards(Card::CardPoint point, int count)
{

    if(count < 1 || count > 4)
    {
        return Cards();
    }
    //大小王
    if(point == Card::CardSj || point == Card::CardBj)
    {
        if(count > 1)
        {
            return Cards();
        }

        Card card;
        card.setPoint(point);
        card.setsuit(Card::SuitBegin);
        if(MyCards.contains(card))//是否真正存在
        {
            Cards cards;
            cards.add(card);
            return cards;
        }
        //没找到 返回空对象
        return Cards();
    }

    //不是大小王
    int FindCount = 0;
    Cards FindCards;
    for(int suit = Card::SuitBegin + 1; suit < Card::SuitEnd; suit++)
    {
        //基于传递花色和单数构造对象
        Card card;
        card.setPoint(point);
        card.setsuit((Card::CardSuit)suit);

        //判断下card是不是在MyCards里面
        if(MyCards.contains(card))//是否真正存在
        {
            FindCount++;
            FindCards.add(card);
            if(FindCount == count)//找到了count个点数为point的牌
            {
                return FindCards;
            }

        }
    }

    //上述都没有满足 返回空
    return Cards();
}

QVector<Cards> Strategy::FindCardsByCount(int count)
{
    //qDebug() << "FindCardsByCount enter";
    //非法操作
    if(count < 1 || count > 4)
    {
        return QVector<Cards>();
    }

    QVector<Cards> CardsArry;
    for(Card::CardPoint point = Card::Card3; point < Card::CardEnd; point = (Card::CardPoint)(point + 1))
    {
        if(MyCards.PointCount(point) == count)
        {
            Cards cs;
            cs << FindSamePointCards(point, count);
            CardsArry<<cs;
        }
    }

    return CardsArry;

}

Cards Strategy::GetRangeCards(Card::CardPoint begin, Card::CardPoint end)
{
    Cards RangeCards;
    //基于这个范围遍历
    for(Card::CardPoint point = begin; point < end; point = (Card::CardPoint)(point + 1))
    {
        //得到对应点数对应的牌的数量
        int count = MyCards.PointCount(point) ;
        Cards cs = FindSamePointCards(point, count);
        RangeCards << cs;
    }

    return RangeCards;

}

QVector<Cards> Strategy::FindCardType(PlayHand hand, bool beat)
{
    qDebug() << "[FindCardType] enter";

    //读出拓展信息在Playehand中
    PlayHand::HandType type = hand.GetHandType();
    Card::CardPoint point = hand.GetCardPoint();
    int extra = hand.GetExtra();

    //确定开始点数
    //beat为true是 则比当前点数大一就可以 反之则不管
    Card::CardPoint BeginPoint = beat ? Card::CardPoint(point + 1) : Card::Card3;
     qDebug() << "[FindCardType] hand type is :" << type << "BenginPoint is" << BeginPoint;

    //进行类型的判断
    switch(type)
    {
    case PlayHand::Hand_Single:
        return GetCards(BeginPoint, 1);
    case PlayHand::Hand_Pair:
        return GetCards(BeginPoint, 2);
    case PlayHand::Hand_Triple:
        return GetCards(BeginPoint, 3);
        //三带一or二 都需要进行三张牌的搜索 在找对应的一张or两张
    case PlayHand::Hand_Triple_single:
        return GetTripleSingleOrPair(BeginPoint, PlayHand::Hand_Single);
    case PlayHand::Hand_Triple_Pair:
        return GetTripleSingleOrPair(BeginPoint, PlayHand::Hand_Pair);
        //飞机一类
    case PlayHand::Hand_Plane:
        return GetPlane(BeginPoint);
    case PlayHand::Hand_Plane_Two_singe:
        return GetPlaneTwoSingleOrTwoPair(BeginPoint, PlayHand::Hand_Single);
    case PlayHand::Hand_Plane_Two_Pair:
        return GetPlaneTwoSingleOrTwoPair(BeginPoint, PlayHand::Hand_Pair);

    case PlayHand::Hand_Seq_Pair:
    {
        //结构体初始化
        CardInfo info;
        info.begin = BeginPoint;
        info.end = Card::CardQ;
        info.number = 2;
        info.base = 3;
        info.extra = extra;
        info.beat = beat;
        info.getseq = &Strategy::GetBaseSeqPair;
        return GetSeqPairOrSeqSingle(info);
    }
    case PlayHand::Hand_Seq_Single:
    {
        //结构体初始化
        CardInfo info;
        info.begin = BeginPoint;
        info.end = Card::Card10;
        info.number = 1;
        info.base = 5;
        info.extra = extra;
        info.beat = beat;
        info.getseq = &Strategy::GetBaseSeqSingle;
        return GetSeqPairOrSeqSingle(info);
    }
    case PlayHand::Hand_Bomb:
        return GetBomb(BeginPoint);
    default:
        return QVector<Cards>();
    }
}

void Strategy::PickSeqSngles(QVector<QVector<Cards>> & AllSeqRecord, const QVector<Cards> &SeqSingle,const  Cards &cards)
{
    //得到所有顺子的组合
    QVector<Cards> AllSeq =  Strategy(MyPlayer,cards).FindCardType(PlayHand(PlayHand::Hand_Seq_Single, Card::Card::CardBegin, 0),false);
    //判空
    if(AllSeq.isEmpty())
    {
        //递归函数 结束递归 将满足条件的顺子传递给调用者
        AllSeqRecord << SeqSingle;
    }
    else//对顺子进行筛选
    {
        Cards SaveCards = cards;//移除之前 做备份
        //遍历得到所有的顺子
        for(int i = 0; i < AllSeq.size(); i++)
        {
            //将顺子取出
            Cards aScheme = AllSeq.at(i);
            //将顺子从用户手中删除
            Cards temp = SaveCards;//备份
            temp.remove(aScheme);
            //将顺子保存下来
            QVector<Cards> SeqArray = SeqSingle;
            SeqArray << aScheme;

            //递归检测还有没有其他顺子
            //SeqArray存贮一轮for循环中 多轮递归的所有可用顺子
            //AllSeqRecord 存储多轮for循环中 多轮递归得到的所有可用顺子
            PickSeqSngles(AllSeqRecord, SeqArray, temp);
        }
    }
}

QVector<Cards> Strategy::PickOptimalSeqSngles()
{
    //qDebug()<< "PickOptimalSeqSngles enter";
    QVector<QVector<Cards>> SeqRecord;
    QVector<Cards> SeqSingles;
    //先删除炸弹和三个 再2找顺子
    Cards save = MyCards;
    save.remove(FindCardsByCount(4));
    save.remove(FindCardsByCount(3));
    PickSeqSngles(SeqRecord, SeqSingles, save);
    if(SeqRecord.isEmpty())//一个数据也没有
    {
        return QVector<Cards>();
    }
    //遍历容器
    QMap<int, int> SeqMark;//第2个int是 mark 第1个int是SeqRecod

    for(int i = 0; i < SeqRecord.size(); i++)
    {
        Cards BackUpCards = MyCards;
        QVector<Cards> SeqArray = SeqRecord[i];
        BackUpCards.remove(SeqArray);

        //判断剩下的单排数量 数量越少 顺子的组合越合理
        QVector<Cards> SingleArray = Strategy(MyPlayer, BackUpCards).FindCardsByCount(1);

        CardList cardlist;
        for (int j = 0; j < SingleArray.size(); j++)
        {
            cardlist << SingleArray[j].ToCardList();
        }

        //找点数相对较大的顺子
        int mark = 0;
        for(int j = 0; j < cardlist.size(); j++)
        {
            mark += cardlist[j].point() + 15;//15为基准值
        }
        SeqMark.insert(i, mark);//mark有可能重复 所以将mark作为value
    }

    //遍历map
    int value =  0;
    int ComMark = 1000;
    for(auto it = SeqMark.constBegin(); it != SeqMark.constEnd(); it++)
    {
        if(it.value() < ComMark)
        {
            ComMark = it.value();
            value = it.key();
        }
    }
    qDebug()<<"[PickOptimalSeqSngles] SeqRecord  out size is " << SeqRecord.size();

    return SeqRecord[value];

}




void Strategy::setDebugForceCards(const Cards &cards)
{
    MyForcecards = cards;
    MyDebugForce = true;
}

void Strategy::clearDebugForce()
{
    MyDebugForce = false;
    MyForcecards.clear();
}


QVector<Cards> Strategy::GetCards(Card::CardPoint point, int number)
{
    QVector<Cards> FindCardsArry;

    //根据点数进行遍历
    for(Card::CardPoint pt = point; pt < Card::CardEnd; pt = (Card::CardPoint)(pt + 1))
    {
        //搜索过程尽量不拆分别的牌型
        if(MyCards.PointCount(pt) == number)
        {
            Cards cs = FindSamePointCards(pt, number);
            FindCardsArry << cs;
        }

    }

    return FindCardsArry;
}


QVector<Cards> Strategy::GetTripleSingleOrPair(Card::CardPoint begin, PlayHand::HandType type)
{

    //找到点数相同的三张牌
    QVector<Cards> FindCardArry = GetCards(begin, 3);
    if(!FindCardArry.isEmpty())
    {
        //将找到的牌从用户手中删除
        //先复制在删除
        Cards RemainCards = MyCards;
        RemainCards.remove(FindCardArry);
        //搜索牌型
        //搜索单牌or成对的牌
        //递归调用
        Strategy st(MyPlayer, RemainCards);
        QVector<Cards> CardsArry = st.FindCardType(PlayHand(type, Card::CardBegin, 0), false);
        if(!CardsArry.isEmpty())
        {
            //将找的牌和三张点数形同的牌进行组合
            //遍历
            for(int i = 0; i < FindCardArry.size(); i++)
            {
                FindCardArry[i].add(CardsArry.at(i));
            }

        }
        else
        {
            FindCardArry.clear();
        }
    }


    //将最终结果返回给函数
    return FindCardArry;
}



QVector<Cards> Strategy::GetPlane(Card::CardPoint begin)
{
    QVector<Cards> FindCardArry;
    for(Card::CardPoint point = begin; point <= Card::CardK; point = (Card::CardPoint)(point + 1))
    {
        //根据当前点数和数量进行搜索
        Cards PrevCards = FindSamePointCards(point, 3);
        Cards NextCards = FindSamePointCards((Card::CardPoint)(point + 1), 3);
        //当前后都不为空 则是飞机
        if(!PrevCards.IsEmpty() && !NextCards.IsEmpty())
        {
            Cards tem;
            tem << PrevCards << NextCards;
            FindCardArry << tem;
        }
    }

    return FindCardArry;
}

QVector<Cards> Strategy::GetPlaneTwoSingleOrTwoPair(Card::CardPoint begin, PlayHand::HandType type)
{
    //找飞机
    QVector<Cards> FindCardArry = GetPlane(begin);
    if(!FindCardArry.isEmpty())
    {
        //将找到的牌从用户手中删除
        //先复制在删除
        Cards RemainCards = MyCards;
        RemainCards.remove(FindCardArry);
        //搜索牌型
        //搜索单牌or成对的牌
        //递归调用
        Strategy st(MyPlayer, RemainCards);
        QVector<Cards> CardsArry = st.FindCardType(PlayHand(type, Card::CardBegin, 0), false);
        if(CardsArry.size() >= 2)
        {
            //遍历 将其找到的添加到飞机的组合中
            for(int i = 0; i < FindCardArry.size(); i++)
            {
                Cards tem;
                tem << CardsArry[0] << CardsArry[1];
                FindCardArry[i].add(tem);
            }
        }
        else
        {
            FindCardArry.clear();
        }
    }


    //将最终结果返回给函数
    return FindCardArry;

}

QVector<Cards> Strategy::GetSeqPairOrSeqSingle(CardInfo & info)
{
    QVector<Cards> FindCardsArray;
    //判断当前要找的连对or顺子是否要大于指定的
    if(info.beat)
    {
        //最少三个 最大A
        for(Card::CardPoint point = info.begin; point <= info.end; point = (Card::CardPoint)(point+ 1))
        {
            //表示能够找到对应点数的牌
            bool found = true;
            //找到进行存储
            Cards SeqCards;
            for(int i = 0; i < info.extra; i++)
            {
                //基于点数和数量进行搜素 根据指定点数和个数进行找
                Cards cards = FindSamePointCards((Card::CardPoint)(point+ i), info.number);
                if(cards.IsEmpty() || (point + info.extra >= Card::Card2))
                {
                    //没找到 清空
                    found = false;
                    SeqCards.clear();
                    break;
                }
                else
                {
                    SeqCards << cards;
                }
            }
            if(found)
            {
                FindCardsArray << SeqCards;
                return FindCardsArray;
            }
        }
    }
    else
    {
        for(Card::CardPoint point = info.begin; point <= info.end; point = (Card::CardPoint)(point+ 1))
        {

            Cards BaseSeq = (this ->*info.getseq)(point);
            //判断是否为空
            if(BaseSeq.IsEmpty())
            {
                continue;//空则继续
            }
            //存储到容器中
            FindCardsArray << BaseSeq;

            //基础的
            int followed = info.base;
            //存储后续找到的
            Cards AlreadyFollowCards;
            while(true)
            {
                //确定开始点数
                Card::CardPoint FollowPoint = Card::CardPoint (point + followed);
                //判断是否超出了上限
                if(FollowPoint >= Card::Card2)
                {
                    break;
                }
                Cards followedCards = FindSamePointCards(FollowPoint, info.number);
                if(followedCards.IsEmpty())
                {
                    break;
                }
                else
                {
                    AlreadyFollowCards << followedCards;
                    Cards NewSeq = BaseSeq;
                    NewSeq << AlreadyFollowCards;
                    FindCardsArray << NewSeq;
                    followed++;
                }
            }
        }
    }

    return FindCardsArray;
}

Cards Strategy::GetBaseSeqPair(Card::CardPoint point)
{

    //找到三个连续的对子
    Cards cards0 = FindSamePointCards(point, 2);
    Cards cards1 = FindSamePointCards((Card::CardPoint)(point + 1), 2);
    Cards cards2 = FindSamePointCards((Card::CardPoint)(point + 2), 2);
    Cards baseseq;
    if(!cards0.IsEmpty() && !cards1.IsEmpty() && !cards2.IsEmpty())
    {
        baseseq << cards0 << cards1 << cards2;
    }
    return baseseq;
}

Cards Strategy::GetBaseSeqSingle(Card::CardPoint point)
{
    //找到5个连续的顺子
    Cards cards0 = FindSamePointCards(point, 1);
    Cards cards1 = FindSamePointCards((Card::CardPoint)(point + 1), 1);
    Cards cards2 = FindSamePointCards((Card::CardPoint)(point + 2), 1);
    Cards cards3 = FindSamePointCards((Card::CardPoint)(point + 3), 1);
    Cards cards4 = FindSamePointCards((Card::CardPoint)(point + 4), 1);
    Cards baseseq;
    if(!cards0.IsEmpty() && !cards1.IsEmpty() && !cards2.IsEmpty() && !cards3.IsEmpty() && !cards4.IsEmpty())
    {
        baseseq << cards0 << cards1 << cards2 << cards3 << cards4;
    }
    return baseseq;
}

QVector<Cards> Strategy::GetBomb(Card::CardPoint begin)
{
    QVector<Cards> FindCardsArray;
    for(Card::CardPoint point = begin; point < Card::CardEnd; point = (Card::CardPoint)(point + 1))
    {
        Cards cs = FindSamePointCards(point, 4);
        if(!cs.IsEmpty())
        {
            FindCardsArray << cs;
        }
    }

    return FindCardsArray;
}
