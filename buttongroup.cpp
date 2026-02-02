#include "buttongroup.h"
#include "ui_buttongroup.h"
#include<QVector>

ButtonGroup::ButtonGroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ButtonGroup)
{
    ui->setupUi(this);
}

ButtonGroup::~ButtonGroup()
{
    delete ui;
}

void ButtonGroup::InitButtons()
{
    //开始游戏
    ui->Start->SetImage(":/images/start-1.png",
                        ":/images/start-3.png",
                        ":/images/start-2.png");
    //出牌
    ui->PlayCard->SetImage(":/images/chupai_btn-1.png",
                           ":/images/chupai_btn-3.png",
                           ":/images/chupai_btn-2.png");

    ui->PlayCard1->SetImage(":/images/chupai_btn-1.png",
                            ":/images/chupai_btn-3.png",
                            ":/images/chupai_btn-2.png");
    //不要
    ui->Pass->SetImage(":/images/pass_btn-1.png",
                       ":/images/pass_btn-3.png",
                       ":/images/pass_btn-2.png");
    //不抢
    ui->GiveUP->SetImage(":/images/buqiang-1.png",
                         ":/images/buqiang-3.png",
                         ":/images/buqiang-2.png");
    //1 2 3 分
    ui->OneScore->SetImage(":/images/1fen-1.png",
                           ":/images/1fen-3.png",
                           ":/images/1fen-2.png");
    ui->TwoScore->SetImage(":/images/2fen-1.png",
                           ":/images/2fen-3.png",
                           ":/images/2fen-2.png");
    ui->ThreeScore->SetImage(":/images/3fen-1.png",
                             ":/images/3fen-3.png",
                             ":/images/3fen-2.png");

    //设置按钮大小
    QVector<MyBUtton*> btns;
    btns << ui->Start << ui->PlayCard << ui->PlayCard1 << ui->Pass
         << ui->GiveUP << ui->OneScore << ui->TwoScore << ui->ThreeScore;
    for(int i = 0; i < btns.size(); i++)
    {
        //设置按钮大小
        btns[i]->setFixedSize(120,80);
    }
    connect(ui->Start,&MyBUtton::clicked, this, &ButtonGroup::StartGame);
    connect(ui->PlayCard, &MyBUtton::clicked, this, &ButtonGroup::PlayHand);
    connect(ui->PlayCard1, &MyBUtton::clicked, this, &ButtonGroup::PlayHand);
    connect(ui->Pass, &MyBUtton::clicked, this, &ButtonGroup::Pass);
    connect(ui->GiveUP, &MyBUtton::clicked, this,
            [=]()
    {
       emit BetPoint(0);
    });
    connect(ui->OneScore, &MyBUtton::clicked, this,
            [=]()
    {
       emit BetPoint(1);
    });
    connect(ui->TwoScore, &MyBUtton::clicked, this,
            [=]()
    {
       emit BetPoint(2);
    });
    connect(ui->ThreeScore, &MyBUtton::clicked, this,
            [=]()
    {
       emit BetPoint(3);
    });


}

void ButtonGroup::SelectPanel(ButtonGroup::Panel type, int bet)
{
    ui->stackedWidget->setCurrentIndex(type);

    //判断当前是否为叫地主的阶段
    if(type != CallLord)
    {
        return;
    }
    else if(bet == 0) //是叫地主 将按钮 显示出来
    {
        ui->OneScore->setVisible(true);
        ui->TwoScore->setVisible(true);
        ui->ThreeScore->setVisible(true);
    }
    //显示机器人没有叫的分数
    else if(bet == 1)
    {
        ui->OneScore->setVisible(false);
        ui->TwoScore->setVisible(true);
        ui->ThreeScore->setVisible(true);
    }
    else if (bet == 2)
    {
         ui->OneScore->setVisible(false);
         ui->TwoScore->setVisible(false);
         ui->ThreeScore->setVisible(true);
    }
}
