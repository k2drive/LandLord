#include "robotgraplord.h"

//是机器人用的 可以写robot类 也可以写其父类
RobotGrapLord::RobotGrapLord(Player* player,QObject *parent) : QThread(parent)
{
    MyPlayer = player;
}

void RobotGrapLord::run()
{
    msleep(200);//指定机器人休息2s
    MyPlayer->ThinkCallLord();
}
