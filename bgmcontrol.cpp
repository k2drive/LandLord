#include "bgmcontrol.h"
#include "card.h"
#include "playhand.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QTimer>

BGMControl::BGMControl(QObject *parent) : QObject(parent)
{
    //对象实例化
    for(int i = 0; i < 5; i++)
    {
        QMediaPlayer* player = new QMediaPlayer(this);
        QMediaPlaylist* list = new QMediaPlaylist(this);
        //设置播放模式
        if(i < 2 || i == 4)
        {
            list->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);//播放一次 男生、女生和结束音乐
        }
        else if(i == 2)
        {
            list->setPlaybackMode(QMediaPlaylist::Loop);//bgm循环播放
        }
        player->setPlaylist(list);
        player->setVolume(100);//设置声音大小
        MyList.push_back(list);
        MyPlayer.push_back(player);
    }
    InitPlayList();
}

void BGMControl::InitPlayList()
{
    QStringList list;
    list << "Man" <<"Woman" << "BGM" << "Other" << "Ending";
    //读json配置文件
    QFile file(":/conf/playList.json");
    file.open(QFile::ReadOnly);
    QByteArray json = file.readAll();
    file.close();
    //解析从文件中读出的json数据
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject obj = doc.object();
    for(int i = 0; i < list.size(); i++)
    {
        QString prefix = list.at(i);
        QJsonArray array = obj.value(prefix).toArray();
        for(int j = 0; j < array.size(); j++)
        {
            MyList[i]->addMedia(QMediaContent(QUrl(array.at(j).toString())));
        }
    }
    //初始化多媒体播放列表
}

void BGMControl::StartBGM(int volume)
{
    MyList[2]->setCurrentIndex(0);
    MyPlayer[2]->setVolume(volume);
    MyPlayer[2]->play();
}

void BGMControl::StopBGM()
{
    MyPlayer[2]->stop();
}

void BGMControl::PlayerRobolordMusic(int point, BGMControl::RoleSex sex, bool isfirst)
{
    int index = sex == Man ? 0 : 1;
    if(isfirst && point > 0)
    {
        MyList[index]->setCurrentIndex(Order);
    }
    else if(point == 0)
    {
        if(isfirst)
        {
            MyList[index]->setCurrentIndex(NoOrder);
        }
        else
        {
            MyList[index]->setCurrentIndex(NoRob);
        }
    }
    else if(point == 2)
    {
        MyList[index]->setCurrentIndex(Rob1);
    }
    else if(point == 3)
    {
        MyList[index]->setCurrentIndex(Rob2);
    }
    MyPlayer[index]->play();
}

void BGMControl::PlayCardMusic(Cards cards, bool isfirst, BGMControl::RoleSex sex)
{
    //得到播放列表
    int index = sex == Man ? 0 : 1;
    QMediaPlaylist* list = MyList[index];
    Card::CardPoint pt = Card::CardPoint::CardBegin;
    //取出牌型 进行判断
    PlayHand hand(cards);
    PlayHand::HandType type = hand.GetHandType();
    if(type == PlayHand::Hand_Single || type == PlayHand::Hand_Pair || type == PlayHand::Hand_Triple)
    {
        pt = cards.TakeRandCard().point();
    }
    int number = 0;//因为 CardPoint与CardType的枚举差1 所以要减去
    switch(type)
    {
    // 单牌
        case PlayHand::Hand_Single:
        number = pt - 1;
            break;
        // 对牌
        case PlayHand::Hand_Pair:
        number = pt - 1 + 15;
            break;
        // 三张点数相同的牌
        case PlayHand::Hand_Triple:
        number = pt - 1 + 13;
            break;
        // 三带一
        case PlayHand::Hand_Triple_single:
        number = ThreeBindOne;
            break;
        // 三带二
        case PlayHand::Hand_Triple_Pair:
        number = ThreeBindPair;
            break;
        // 飞机
        case PlayHand::Hand_Plane:
        // 飞机带两个单
        case PlayHand::Hand_Plane_Two_singe:
        // 飞机带两个对儿
    case PlayHand::Hand_Plane_Two_Pair:
        number = Plane;
        break;
        // 连对
        case PlayHand::Hand_Seq_Pair:
        number = SequencePair;
            break;
        // 顺子
        case PlayHand::Hand_Seq_Single:
        number = Sequence;
            break;
        // 炸弹
        case PlayHand::Hand_Bomb:
        number = Bomb;
            break;
        // 王炸
        case PlayHand::Hand_Bomb_Jokers:
        number = JokerBomb;
            break;
        // 炸弹带一对儿
        case PlayHand::Hand_Bomb_Pair:
        // 炸弹带两单
        case PlayHand::Hand_Bomb_Two_Single:
        // 王炸带一对儿
        case PlayHand::Hand_Bomb_Jokers_Pair:
        // 王炸带两单
        case PlayHand::Hand_Bomb_Jokers_Two_Single:
        number = FourBindTwo;

        default:
            break;
    }

    //非第一个出此类型的牌
    if(!isfirst && (number >= Plane && number <= FourBindTwo))
    {
        list->setCurrentIndex(MoreBiger1 + QRandomGenerator::global()->bounded(2));
    }
    else
    {
        list->setCurrentIndex(number);
    }

    //播放音乐
    MyPlayer[index]->play();
    //对牌型进行判断 确定是炸弹飞机王炸
    if(!isfirst && (number >= Plane && number <= FourBindTwo))
    {
        list->setCurrentIndex(MoreBiger1 + QRandomGenerator::global()->bounded(2));
    }
    else
    {
        list->setCurrentIndex(number);
    }
    //播放音乐
    MyPlayer[index]->play();
    if(number == Bomb || number == JokerBomb)
    {
        PlayAssistMusic(BombVoice);
    }
    if(number == Plane)
    {
        PlayAssistMusic(PlaneVoice);
    }
}

void BGMControl::PlayLastMusic(BGMControl::CardType type, BGMControl::RoleSex sex)
{
    //找到 玩家性别、
    int index = sex == Man ? 0 : 1;
    //找到 播放列表
    QMediaPlaylist* list = MyList[index];
    if(MyPlayer[index]->state() == QMediaPlayer::StoppedState)
    {
        list->setCurrentIndex(type);
        MyPlayer[index]->play();
    }
    else
    {
        //还在播放其他音乐 需要等会
        QTimer::singleShot(1500, this, [=]()
        {
            list->setCurrentIndex(type);
            MyPlayer[index]->play();
        });
    }
}

void BGMControl::PlayPassMusic(BGMControl::RoleSex sex)
{
    //找到 玩家性别、
    int index = sex == Man ? 0 : 1;
    //找到 播放列表
    QMediaPlaylist* list = MyList[index];
    //找到要播放的音乐
    int random = QRandomGenerator::global()->bounded(4);
    list->setCurrentIndex(Pass1 + random);
    //播放音乐
    MyPlayer[index]->play();
}

void BGMControl::PlayAssistMusic(BGMControl::AssistMusic type)
{


    QMediaPlaylist::PlaybackMode mode;
    if(type == Dispatch)
    {
        //循环播放
        mode = QMediaPlaylist::CurrentItemInLoop;

    }
    else
    {
        //播放一次
        mode = QMediaPlaylist::CurrentItemOnce;
    }
    //找到 播放列表
    QMediaPlaylist* list = MyList[3];
    //找到要播放的音乐
    list->setCurrentIndex(type);
    list->setPlaybackMode(mode);
    //播放音乐
    MyPlayer[3]->play();
}

void BGMControl::StopAssistMusic()
{
    MyPlayer[3]->stop();
}

void BGMControl::PlayEndingMusic(bool iswin)
{
    if(iswin)
    {
        MyList[4]->setCurrentIndex(0);
    }
    else
    {
        MyList[4]->setCurrentIndex(1);
    }
    MyPlayer[4]->play();
}


