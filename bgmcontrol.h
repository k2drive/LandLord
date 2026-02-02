#ifndef BGMCONTROL_H
#define BGMCONTROL_H

#include "cards.h"

#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>


class BGMControl : public QObject
{
    Q_OBJECT
public:
    enum RoleSex
    {
        Man,
        Woman
    };
    enum CardType
       {
           // 单张牌
           Three,
           Foue,
           Five,
           Six,
           Seven,
           Eight,
           Nine,
           Ten,
           Jack,
           Queen,
           King,
           Ace,
           Tow,
           SmallJoker,
           BigJoker,
           // 两张牌
           Three_Double,
           Foue_Double,
           Five_Double,
           Six_Double,
           Seven_Double,
           Eight_Double,
           Nine_Double,
           Ten_Double,
           Jack_Double,
           Queen_Double,
           King_Double,
           Ace_Double ,
           Tow_Double,
           // 三张牌
           Three_Triple,
           Foue_Triple,
           Five_Triple,
           Six_Triple,
           Seven_Triple,
           Eight_Triple,
           Nine_Triple,
           Ten_Triple,
           Jack_Triple,
           Queen_Triple,
           King_Triple,
           Ace_Triple,
           Tow_Triple,
           // 其他组合
           Plane,          // 飞机
           SequencePair,   // 连对
           ThreeBindOne,   // 三带一
           ThreeBindPair,  // 三带一对
           Sequence,       // 顺子
           FourBindTwo,    // 四带二(单张)
           FourBind2Pair,  // 四带两对
           Bomb,           // 炸弹
           JokerBomb,      // 王炸
           Pass1,          // 过
           Pass2,
           Pass3,
           Pass4,
           MoreBiger1,     // 大你
           MoreBiger2,
           Biggest,        // 压死
           // 抢地主
           NoOrder,        // 不叫
           NoRob,          // 不抢
           Order,          // 叫地主
           Rob1,           // 抢地主
           Rob2,
           Last1,          // 只剩1张牌
           Last2           // 只剩2张牌
       };

       enum AssistMusic
       {
           Dispatch,   // 发牌
           SelectCard, // 选牌
           PlaneVoice, // 飞机
           BombVoice,  // 炸弹
           Alert,      // 提醒
       };

    explicit BGMControl(QObject *parent = nullptr);

    //初始化播放列表
    void InitPlayList();

    //播放背景音乐（播放和停止）
    void StartBGM(int volume);
    void StopBGM();
    //播放玩家抢地主时候音乐
    void PlayerRobolordMusic(int point, RoleSex sex, bool isfirst);//point为下注分数 sex是性别 isfirst为是否抢
    //播放出牌背景音乐 第一个出牌和后续的出牌所用的音乐不一样
    void PlayCardMusic(Cards cards, bool isfirst, RoleSex sex);//cards为打出的牌 从而找到类型 isfirst是是否出牌 sex为性别
    void PlayLastMusic(CardType type, RoleSex sex);//播放最后剩余牌的数量的音乐
    //播放不出牌音乐
    void PlayPassMusic(RoleSex sex);
    //播放辅助音乐
    void PlayAssistMusic(AssistMusic type);
    void StopAssistMusic();//停止播放辅助音乐
    //播放结束音乐
    void PlayEndingMusic(bool iswin);

signals:

private:
    //0 男生 1 女生 2bgm 3辅助音乐 4结束音乐
    QVector<QMediaPlayer*> MyPlayer;
    QVector<QMediaPlaylist*> MyList;


};

#endif // BGMCONTROL_H
