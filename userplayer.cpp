#include "userplayer.h"
#include "cards.h"
#include "gamecontrol.h"

UserPlayer::UserPlayer(QObject *parent):Player(parent)
{
    MyType = Player::User;
}

void UserPlayer::PrepareCallLord()
{

}

void UserPlayer::PreparePlayHand()
{
    //发射信号给主界面
    emit StartCountDown();
}
