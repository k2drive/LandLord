#include "robotplayhand.h"

RobotPlayHand::RobotPlayHand(Player* player,QObject *parent) : QThread(parent)
{
    MyPlayer = player;
}

void RobotPlayHand::run()
{
    msleep(2000);
    //运行出牌函数
    MyPlayer->ThinkPlayHand();
}
