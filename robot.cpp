#include <qdebug.h>
#include "robot.h"
#include "strategy.h"
#include "robotgraplord.h"
#include "robotplayhand.h"
#include"playhand.h"

Robot::Robot(QObject *parent):Player(parent)
{
    MyType = Player::Robot;
}

void Robot::PrepareCallLord()
{
    //先创建子线程 参数是当前玩家对象
    RobotGrapLord * SubThread = new RobotGrapLord(this);
    //线程执行完成之后 会发送一个人finish信号 对这个信号处理 然后结束线程的执行
    connect(SubThread, &RobotGrapLord::finished, this, [=]()
    {
        //qDebug() << "RobotGrapLord 子线程析构 " << "Robot name :" << this->Name();
        SubThread->deleteLater();
    });
    SubThread->start();//启动子线程

}

void Robot::PreparePlayHand()
{
    //创建子线程对象
    RobotPlayHand * SubThread = new RobotPlayHand(this);
    //线程执行完成之后 会发送一个人finish信号 对这个信号处理 然后结束线程的执行
    connect(SubThread, &RobotGrapLord::finished, this, [=]()
    {
        //qDebug() << "RobotPlayHand 子线程析构 " << "Robot name :" << this->Name();
        SubThread->deleteLater();
    });
    //通过线程对象启动线程
    SubThread->start();
}

void Robot::ThinkCallLord()
{
    //基于牌型给权重 算
    //大小王 6，顺子or炸弹 5， 三张4，2的权重 3，对子 1
    int weigth = 0;//权重
    Strategy st(this, MyCards);

    //得到大小王的数量
    weigth += st.GetRangeCards(Card::CardSj, Card::CardBj).cardCount() * 6;

    //计算顺子
    //qDebug()<<"PickOptimalSeqSngles is bet";
    QVector<Cards> optSeq = st.PickOptimalSeqSngles();
    weigth += optSeq.size() * 5;

    //计算炸弹
    QVector<Cards> bombs = st.FindCardsByCount(4);
    weigth += bombs.size() * 5;

    //计算2的数量
    weigth += MyCards.PointCount(Card::Card2) * 3;

    //计算三张
    //提出炸弹和顺子和2对其的影响
    Cards tem = MyCards;
    tem.remove(optSeq);
    tem.remove(bombs);
    Cards card2 = st.GetRangeCards(Card::Card2, Card::Card2);
    tem.remove(card2);
    QVector<Cards> triples = Strategy(this, tem).FindCardsByCount(3);
    weigth += triples.size() * 4;

    //计算对子
    //消除上述的影响
    tem.remove(triples);
    QVector<Cards> pairs = Strategy(this,tem).FindCardsByCount(2);
    weigth += pairs.size() * 1;

    //根据权重下分
    if(weigth >= 22)
    {
        GrabLordBet(3);
    }
    else if (weigth < 22 && weigth >= 18)
    {
        GrabLordBet(2);
    }
    else if(weigth < 18 && weigth >= 10)
    {
        GrabLordBet(1);
    }
    else
    {
        GrabLordBet(0);
    }

}

void Robot::ThinkPlayHand()
{

    Strategy st(this, MyCards);
    Cards  cs = st.MakeStrategy();//指定策略得到若干张牌
    //class::PlayHand hd(cs);
    //qDebug() << "cs cards type is " << hd.GetHandType();
    PlayHand(cs);



}
