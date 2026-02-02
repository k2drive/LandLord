#include "endingpanel.h"

#include <QPainter>

EndingPanel::EndingPanel(bool isLord, bool isWin, QWidget *parent) : QWidget(parent)
{
    MyBk.load(":/images/gameover.png");
    setFixedSize(MyBk.size());

    //显示用户玩家的角色以及游戏状态
    MyTitle = new QLabel(this);
    if(isLord && isWin)
    {
        MyTitle->setPixmap(QPixmap(":/images/lord_win.png"));
    }
    else if(isLord && !isWin)
    {
        MyTitle->setPixmap(QPixmap(":/images/lord_fail.png"));
    }
    else if(!isLord && isWin)
    {
        MyTitle->setPixmap(QPixmap(":/images/farmer_win.png"));
    }
    else if(!isLord && !isWin)
    {
        MyTitle->setPixmap(QPixmap(":/images/farmer_fail.png"));
    }
    MyTitle->move(125,125);

    //分数面板
    MyScore = new ScorePanel(this);
    MyScore->move(75,230);
    MyScore->setFixedSize(260,160);
    MyScore->SetMyFontColor(ScorePanel::Red);
    MyScore->SetMyFontSize(11);

    //继续游戏按钮
    MyContinue = new QPushButton(this);
    MyContinue->move(84,429);
    QString style = R"(
            QPushButton{border-image:url(:/images/button_normal.png)}
            QPushButton:hover{border-image:url(:/images/button_hover.png)}
            QPushButton:pressed{border-image:url(:/images/button_pressed.png)}
                    )";//顺序不能颠倒
    MyContinue->setStyleSheet(style);
    MyContinue->setFixedSize(231,48);
    connect(MyContinue, &QPushButton::clicked, this, &EndingPanel::ContinueGame);//点击继续游戏按钮之后 再发射一个信号
}

void EndingPanel::SetPlayerScore(int left, int right, int me)
{
    MyScore->SetScores(left, right, me);
}

void EndingPanel::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)
    QPainter p(this);
    p.drawPixmap(rect(), MyBk);
}
