#include<QTimer>
#include <QLabel>
#include <QMap>

#include "scorepanel.h"
#include "ui_gamepanel.h"
#include"gamecontrol.h"
#include"cardpanel.h"
#include "animationwndow.h"
#include "countdown.h"
#include "bgmcontrol.h"



class GamePanel : public QMainWindow  // 确保继承 QMainWindow
{
    Q_OBJECT
public:
    explicit GamePanel(QWidget *parent = nullptr);
    ~GamePanel();

    enum AnimationType{
        ShunZi,
        LianDui,
        Plane,
        JokerBomb,
        Bomb,
        Bet
    };

    //初始化游戏控制类信息
    void GameControlInit();
    //更新分数面板的分数
    void UpdatePlayerScore();
    //切割并存储扑克牌图片
    void InitCardMap();
    //裁剪图片
    void CropImage(QPixmap & pix, int x, int y, Card & c);
    //初始化游戏按钮
    void InitBUttonsGroup();
    //初始化玩家在窗口的上下文环境
    void InitPlayerConnectext();
    //初始化游戏场景
    void InitGameScene();
    //处理游戏状态
    void GameStatusPrecess(GameControl::GameStatus status);
    //发牌
    void StartDispathCard();
    //移动扑克牌
    void CardMoveStep(Player* player, int curpos);
    //处理分发的扑克牌
    void DisPosCard(Player* player, const Cards& cards);
    //更新扑克牌在窗口的显示
    void UpdatePlayerCards(Player* player);
    //加载玩家头像
    QPixmap LoadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role);

    //槽函数
    //定时器的处理动作
    void OnDispatchChard();
    //处理玩家状态的变化
    void OnPlayerStatusChangd(Player* player, GameControl::PlayerStatus status);
    //处理玩家状态的变化
    void OnGrabLordBet(Player* player ,int bet, bool flag);
    //处理玩家出牌
    void OnDisposePlayHand(Player* player,  Cards &cards);
    //处理玩家选牌
    void OnCardSelected(Qt::MouseButton button);
    //处理用户出牌
    void OnUserPlayHand();
    //处理玩家放弃出牌
    void OnUserPass();

    //显示特效动画
    void ShowAnimation(AnimationType type, int bet = 0);
    //隐藏 玩家上一轮打出的牌
    void HidePlayerDropCards(Player * player);
    //显示玩家的最终得分
    void ShowEndingScorePanel();
    //初始化闹钟倒计时
    void InitCountDown();



protected:
   virtual void paintEvent(QPaintEvent* ev) ;
    void mouseMoveEvent(QMouseEvent* ev);

private:
    enum CardAlign{
        Horizontal,//水平对其
        Vertical//垂直对其
    };
    struct PlayerContext
    {
        //出票扑克篇显示的区域
        QRect CardRect;
        //出牌区域
        QRect PlayHanRect;
        //扑克牌对其方式
        CardAlign Align;
        //扑克牌显示方式正or背面
        bool IsFrontSide;
        //游戏过程的提示信息
        QLabel* Info;
        //玩家的头像
        QLabel * RoleImg;
        //玩家刚打出的牌
        Cards LastCards;
    };

    Ui::GamePanel *ui;   // UI 指针
    QPixmap MyBkIMage;
    GameControl* MyGameControl;
    QVector<Player*> MyPlayerLIst;
    QMap<Card, CardPanel*> MyCardMap;
    QSize MyCardSize;//便于其他类的使用
    QPixmap MyCardBackImage;
    QMap<Player*, PlayerContext> MyContextMap;
    CardPanel* MyBaseCard;
    CardPanel* MyMoveCard;
    QVector<CardPanel*> MyLast3Cards;
    QPoint MyBaseCardPos;//最后三张的位置
    GameControl::GameStatus MyGameStatus;
    QTimer* MyTimer;
    AnimationWndow* MyAnimation;
    CardPanel* MyCurrentSelectCard;//当前所选择的牌
    QSet<CardPanel*> MySelectCards;//存储所有选中的牌的窗口
    QRect MyCardsRect;//存储玩家剩余牌显示的区域
    QHash<CardPanel*, QRect> MyUserCards;//记录牌和剩余的位置 根据对象找位置
    CountDown* MyCountDown;
    BGMControl* MyBgm;
};
