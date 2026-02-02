#include "endingpanel.h"
#include "gamepanel.h"
#include "playhand.h"
#include "ui_gamepanel.h"
#include<QPainter>
#include<QRandomGenerator>
#include<QPoint>
#include<QDebug>
#include<QPixmap>
#include <QMouseEvent>
#include <QPropertyAnimation>

GamePanel::GamePanel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GamePanel)
{
    ui->setupUi(this);


    //背景图 随机获得背景图
    int num = QRandomGenerator::global()->bounded(10);
    QString path = QString(":/images/background-%1.png").arg(num + 1);
    MyBkIMage.load(path);
    //qDebug() << "laod" << path;
    //窗口的标题大小
    this->setWindowTitle("欢乐斗地主");
    this->setFixedSize(1000,650);//窗口大小
    //实例化游戏控制对象
    GameControlInit();
    //玩家得分 更新
    UpdatePlayerScore();
    //切割游戏图片
    InitCardMap();
    //初始化游戏中的按钮
    InitBUttonsGroup();
    //初始化玩家在窗口的上下文环境
    InitPlayerConnectext();

    //扑克牌场景初始化
    InitGameScene();

    //倒计时窗口初始化
    InitCountDown();


    //定时器实例化
    MyTimer = new QTimer(this);
    connect(MyTimer, &QTimer::timeout, this, &GamePanel::OnDispatchChard);

    //动画窗口实例化
    MyAnimation = new AnimationWndow(this);
    //bgm实例化
    MyBgm = new BGMControl(this);

    //    qDebug() << "parent =" << MyAnimation->parent();
    //    qDebug() << "parent visible =" << static_cast<QWidget*>(MyAnimation->parent())->isVisible();

    //ShowEndingScorePanel();//仅测试完结动画所需要的
//    //特效显示测试
//    QTimer::singleShot(500, this, [=]()
//    {
//        ShowAnimation(Bomb);
//    });

}

GamePanel::~GamePanel()
{
    delete ui;
}

void GamePanel::GameControlInit()
{
    //this指向父对象 在父对象销毁的时候 这个对象自动销毁 自动释放内存
    MyGameControl = new GameControl(this);
    MyGameControl->PlayerInit();
    //得到三个玩家的实例
    Robot* LRobot = MyGameControl->GetLeftRobot();
    Robot* RRobot = MyGameControl->GetRightRobot();
    UserPlayer * Us = MyGameControl->GetUserPlayer();
    //顺序是：左机器人 右机器人 当前玩家
    MyPlayerLIst << LRobot << RRobot << Us;

    connect(MyGameControl,& GameControl::PlayerStatusChanged, this, &GamePanel::OnPlayerStatusChangd);//发射信号玩家状态改变 槽函数处理状态改变的功能
    connect(MyGameControl, &GameControl::NotifyGrabLordBet, this, &GamePanel::OnGrabLordBet);//下注 信号与处理
    connect (MyGameControl , &GameControl::GameStatusChanged, this, & GamePanel::GameStatusPrecess);//游戏的状态改变
    connect (MyGameControl , &GameControl::NotifyPlayHand, this, & GamePanel::OnDisposePlayHand);

    connect(LRobot, &Player::NotifyPickCards, this, &GamePanel::DisPosCard);
    connect(RRobot, &Player::NotifyPickCards, this, &GamePanel::DisPosCard);
    connect(Us, &Player::NotifyPickCards, this, &GamePanel::DisPosCard);
}

void GamePanel::UpdatePlayerScore()
{
    ui->scorePanel->SetScores(MyPlayerLIst[0]->GetScore(),
            MyPlayerLIst[1]->GetScore(),
            MyPlayerLIst[2]->GetScore());
}

void GamePanel::InitCardMap()
{
    //加载大图
    QPixmap PixMap(":/images/card.png");
    //计算每张图片大小
    //高度除5 宽度除13
    MyCardSize.setWidth(PixMap.width() / 13);
    MyCardSize.setHeight(PixMap.height() / 5);

    //取出三种图片 正常花色 背景图 大小王
    MyCardBackImage = PixMap.copy(2*MyCardSize.width(), 4*MyCardSize.height(),//指定背景图片的横纵轴坐标
                                  MyCardSize.width(), MyCardSize.height());//每张扑克牌的大小
    //循环遍历出每张牌
    for(int i = 0, suit = Card::SuitBegin + 1; suit < Card::SuitEnd; suit++, i++)//花色
    {
        for(int j = 0, pt = Card::CardBegin + 1; pt < Card::CardSj; pt++, j++)
        {
            Card card((Card::CardPoint)pt, (Card::CardSuit)suit);//构造函数
            //裁剪图片
            CropImage(PixMap,j * MyCardSize.width(), i * MyCardSize.height(), card);
        }
    }
    //大小王裁剪
    Card c;
    c.setPoint(Card::CardSj);
    c.setsuit(Card::SuitBegin);
    CropImage(PixMap, 0, 4 * MyCardSize.height(),c);

    c.setPoint(Card::CardBj);
    CropImage(PixMap, MyCardSize.width(), 4 * MyCardSize.height(),c);
}

void GamePanel::CropImage(QPixmap &pix, int x, int y, Card& c)
{
    //裁剪出单张图片
    QPixmap sub = pix.copy(x, y, MyCardSize.width(), MyCardSize.height());
    //实例化对象
    CardPanel * panel = new CardPanel(this);
    //完善单张图片信息
    panel->SetImage(sub, MyCardBackImage);
    //设置单张图片
    panel->SetCard(c);
    panel->hide();
    //将裁剪出来的每张图片存到map中
    MyCardMap.insert(c, panel);//有可能会被覆盖重复
    connect(panel, &CardPanel::CardSelected, this, &GamePanel::OnCardSelected);
}

void GamePanel::InitBUttonsGroup()
{

    ui->BtnGroup->InitButtons();
    ui->BtnGroup->SelectPanel(ButtonGroup::Start );

    //处理按钮组发射的信号 进行连接
    connect(ui->BtnGroup, & ButtonGroup::StartGame, this, [=](){
        //界面初始化
        //隐藏开始按钮

            ui->BtnGroup->SelectPanel(ButtonGroup::Empty);
        //清空玩家得分

                    MyGameControl->ClearPlyerScore();
        UpdatePlayerScore();//更新玩家得分
        //修改游戏到发牌状态
           GameStatusPrecess(GameControl::DisPatchCard);
        //播放背景音乐
        MyBgm->StartBGM(80);//音量为80

    });
    connect(ui->BtnGroup, & ButtonGroup::PlayHand, this, &GamePanel::OnUserPlayHand);
    connect(ui->BtnGroup, & ButtonGroup::Pass, this,  &GamePanel::OnUserPass);
    connect(ui->BtnGroup, & ButtonGroup::BetPoint, this, [=](int bet){
        MyGameControl->GetUserPlayer()->GrabLordBet(bet);
        ui->BtnGroup->SelectPanel(ButtonGroup::Empty);//隐藏按钮
    });
}

void GamePanel::InitPlayerConnectext()
{
    // 1. 放置玩家扑克牌的区域
    const QRect cardsRect[] =
    {
        // x, y, width, height
        QRect(90, 130, 100, height() - 200),                    // 左侧机器人
        QRect(rect().right() - 190, 130, 100, height() - 200),  // 右侧机器人
        QRect(250, rect().bottom() - 120, width() - 500, 100)   // 当前玩家
    };
    const QRect playHandRect[] =
    {
        QRect(260, 150, 100, 100),                              // 左侧机器人
        QRect(rect().right() - 360, 150, 100, 100),             // 右侧机器人
        QRect(150, rect().bottom() - 290, width() - 300, 105)   // 当前玩家
    };
    const QPoint roleImgPos[] =
    {
        QPoint(cardsRect[0].left()-80, cardsRect[0].height() / 2 + 20),     // 左侧机器人
        QPoint(cardsRect[1].right()+10, cardsRect[1].height() / 2 + 20),    // 右侧机器人
        QPoint(cardsRect[2].right()-10, cardsRect[2].top() - 10)            // 当前玩家
    };

    //循环
    //得到非机器人玩家的索引
    int idex = MyPlayerLIst.indexOf(MyGameControl->GetUserPlayer());
    for(int i = 0; i < MyPlayerLIst.size(); i++)
    {
        PlayerContext context;
        //通过索引判断什么类型的玩家 进行扑克牌的排列
        context.Align = i == idex? Horizontal:Vertical;
        context.IsFrontSide = i == idex? true:false;///正面是true反面是false
        context.CardRect = cardsRect[i];
        context.PlayHanRect = playHandRect[i];
        //提示信息
        context.Info = new QLabel(this);

        context.Info->resize(160, 98);
        context.Info->hide();//默认隐藏
        //显示到出牌区域的中心位置
        QRect rect = playHandRect[i];
        //计算出中心位置
        QPoint pt(rect.left() + (rect.width() - context.Info->width()) / 2,
                  rect.top() + (rect.height() - context.Info->height()) / 2);
        context.Info->move(pt);
        //玩家头像
        context.RoleImg = new QLabel(this);
        context.RoleImg->resize(84, 120);
        context.RoleImg->hide();
        context.RoleImg->move(roleImgPos[i]);
        MyContextMap.insert(MyPlayerLIst.at(i), context);//使用at有越界检查且为const
    }

    //qDebug() << "Left cardRect =" << cardRect[0];
    //qDebug() << "Right cardRect =" << cardRect[1];
    //qDebug() << "User cardRect =" << cardRect[2];

}

void GamePanel::InitGameScene()
{
    //发牌区的扑克牌
    MyBaseCard = new CardPanel(this);
    //只需要显示背面
    MyBaseCard->SetImage(MyCardBackImage,MyCardBackImage);
    //发牌过程中移动的扑克牌
    MyMoveCard = new CardPanel(this);
    MyMoveCard->SetImage(MyCardBackImage, MyCardBackImage);
    //最后三张底牌 用于窗口显示
    for(int i = 0; i < 3; i++)
    {
        CardPanel* panel = new CardPanel(this);
        panel->SetImage(MyCardBackImage, MyCardBackImage);
        MyLast3Cards.push_back(panel);
        panel->hide();
    }
    //计算扑克牌的位置
    MyBaseCardPos = QPoint((width() - MyCardSize.width()) / 2,
                           height() / 2 - 100);
    //移动位置
    MyBaseCard->move(MyBaseCardPos);
    MyMoveCard->move(MyBaseCardPos);

    //最后三张牌的位置计算 base为基准坐标
    int base = (width() - 3 * MyCardSize.width() - 2 * 10) / 2;

    //确定最后三张牌的位置 每张牌间距10像素 i 为第几张牌
    for (int i = 0; i < 3; i++)
    {
        MyLast3Cards[i]->move(base + (MyCardSize.width() + 10) * i, 20);
    }
}

void GamePanel::GameStatusPrecess(GameControl::GameStatus status)
{
    //记录游戏状态、
    MyGameStatus = status;
    //处理游戏状态
    switch(status)
    {
    case GameControl::DisPatchCard://发牌
        StartDispathCard();
        break;
    case GameControl::CallingLord://叫地主
    {
        //取出底牌数据
        CardList last3Card = MyGameControl->GetSurplusCArds().ToCardList();
        //给底牌窗口设置图片
        //遍历容器
        for(int i = 0; i < last3Card.size(); i++)
        {
            QPixmap front = MyCardMap[last3Card.at(i)]->Image();//image获得正面图片
            MyLast3Cards[i]->SetImage(front, MyCardBackImage);
            MyLast3Cards[i]->hide();

        }
        //开始叫地主
        MyGameControl->StartLordCard();
    }
        break;
    case GameControl::PlayingHand://出牌
    {
        //隱藏发牌区底牌和移动的牌
        MyBaseCard->hide();
        MyMoveCard->hide();
        //显示留给地主的三张牌
        for(int i = 0; i < MyLast3Cards.size(); i++)
        {
            MyLast3Cards.at(i)->show();
        }
        //隐藏各个玩家抢地主过程中的提示信息
        for(int i = 0; i < MyPlayerLIst.size(); i++)
        {
            PlayerContext &context = MyContextMap[MyPlayerLIst.at(i)];
            context.Info->hide();
            Player * player = MyPlayerLIst.at(i);
            //显示各个玩家的头像
            QPixmap pixmap = LoadRoleImage(player->GetSex(), player->GetDirection(), player->GetRole());
            context.RoleImg->setPixmap(pixmap);
            context.RoleImg->show();
        }
    }
        break;
    default:
        break;
    }
}

void GamePanel::StartDispathCard()
{
    //重置每张卡牌的数据
    for(auto it = MyCardMap.begin(); it != MyCardMap.end(); it++)
    {
        it.value()->SetSelected(false);//非选中
        it.value()->setFrontSide(true);//正面是真
        it.value()->hide();
    }
    //隐藏三张底牌
    for(int i = 0; i < MyLast3Cards.size(); i++)
    {
        MyLast3Cards.at(i)->hide();
    }
    //重置玩家窗口上下文信息
    int idex = MyPlayerLIst.indexOf(MyGameControl->GetUserPlayer());
    for(int i = 0; i < MyPlayerLIst.size(); i++)
    {
        MyContextMap[MyPlayerLIst.at(i)].LastCards.clear();
        MyContextMap[MyPlayerLIst.at(i)].Info->hide();
        MyContextMap[MyPlayerLIst.at(i)].RoleImg->hide();//玩家头像隐藏
        MyContextMap[MyPlayerLIst.at(i)].IsFrontSide = i == idex ? true:false;
    }
    //重置所有玩家的卡牌数据
    MyGameControl->RestCardData();

    //显示底牌
    MyBaseCard->show();
    //隐藏
    ui->BtnGroup->SelectPanel(ButtonGroup::Empty);
    //启动定时器
    MyTimer->start(11);//发牌动画速度
    //播放背景音 发牌的声音
    MyBgm->PlayAssistMusic(BGMControl::Dispatch);

}



void GamePanel::CardMoveStep(Player* player, int curpos)
{
    //得到每个玩家的扑克牌展示区域
    QRect CardRect = MyContextMap[player].CardRect;
    //季赛每个玩家的单元步长
    int unit[] = {
        (MyBaseCardPos.x() - CardRect.right()) / 100,
        (CardRect.left() -MyBaseCardPos.x()) / 100,
        (CardRect.top() - MyBaseCardPos.y()) / 100
    };
    //每次窗口移动的时候 每个玩家对应牌的实时坐标位置
    QPoint pos[] =
    {
        QPoint(MyBaseCardPos.x() - curpos * unit[0], MyBaseCardPos.y()),
        QPoint(MyBaseCardPos.x() + curpos * unit[1], MyBaseCardPos.y()),
        QPoint(MyBaseCardPos.x(), MyBaseCardPos.y() + curpos * unit[2]),
    };

    //移动扑克牌窗口
    int idex = MyPlayerLIst.indexOf(player);//取出当前玩家的下标
    MyMoveCard->move(pos[idex]);

    //临界状态的处理 在开始的时候 显示，在到达区域时候 隐藏
    if(curpos == 0)
    {
        MyMoveCard->show();
    }
    if(curpos == 100)
    {

        MyMoveCard->hide();
    }

}

void GamePanel::DisPosCard(Player *player, const Cards &cards)
{

    Cards & mycard = const_cast<Cards&>(cards);
    CardList list = mycard.ToCardList();
    for(int i = 0; i < list.size(); i++)
    {
        CardPanel* panel = MyCardMap[list.at(i)];
        panel->SetOwner(player);
    }
    //更新扑克牌在窗口的显示
    UpdatePlayerCards(player);
}

void GamePanel::UpdatePlayerCards(Player *player)
{
    Cards cards  = player->GetCards();//得到剩余的牌
    CardList list = cards.ToCardList();//存储到容器中

    //对象初始化
    MyCardsRect = QRect();
    MyUserCards.clear();
    //取出展示扑克牌的区域
    int cardSpace = 20;
    QRect cardsRect = MyContextMap[player].CardRect;
    for(int i = 0; i < list.size(); i++)
    {
        CardPanel* panel = MyCardMap[list.at(i)];
        panel->show();
        panel->raise();

        panel->setFrontSide(MyContextMap[player].IsFrontSide);

        //根据当前玩家显示正面还是背面 从而确定是水平or竖直展示
        if(MyContextMap[player].Align == Horizontal)
        {
            int leftX = cardsRect.left() + (cardsRect.width() - (list.size() - 1) * cardSpace - panel->width())/2;
            int topY = cardsRect.top() + (cardsRect.height() - MyCardSize.height()) / 2;
            //窗口的移动
            if(panel->IsSelected())
            {
                topY -= 10;
            }
            panel->move(leftX + cardSpace *i, topY);
            MyCardsRect = QRect(leftX,topY, cardSpace * i + MyCardSize.width(), MyCardSize.height());
            int CurrentWidth = 0;
            if(list.size() - 1 == i)
            {
                //当前是最后一张牌 完整显示的
                CurrentWidth = MyCardSize.width();
            }
            else
            {
                //非最后一张牌
                CurrentWidth = cardSpace;
            }
            QRect cardRect(leftX + cardSpace * i, topY,CurrentWidth, MyCardSize.height());
            MyUserCards.insert(panel, cardRect);
        }
        else//机器人 即垂直
        {
            int leftX = cardsRect.left() + (cardsRect.width() - MyCardSize.width()) / 2;
            int topY = cardsRect.top() + (cardsRect.height() - (list.size() - 1) * cardSpace - panel->height()) / 2;
            panel->move(leftX,topY + i* cardSpace);

        }
    }

    //显示玩家打出的牌
    //得到当前玩家的出牌区域以及本轮打出的牌
    QRect PlayCardRect = MyContextMap[player].PlayHanRect;
    Cards lastcards = MyContextMap[player].LastCards;
    if(!lastcards.IsEmpty())
    {
        //显示牌之间的间隙
        int playSpacing = 24;
        //类型转换和存储到容器中
        CardList lastCardList = lastcards.ToCardList();
        CardList::ConstIterator itplayed = lastCardList.constBegin();
        for(int i = 0;itplayed != lastCardList.constEnd();itplayed++,i++)
        {
            //根据卡牌数据找到对应的窗口
            CardPanel *panel = MyCardMap[*itplayed];
            //显示正面花色以及点数
            panel->setFrontSide(true);
            panel->raise();//显示同级别的上方 为了堆叠
            //将打出的牌移动到出牌区域
            if(MyContextMap[player].Align == Horizontal)
            {
                int LeftBase = PlayCardRect.left() +
                        (PlayCardRect.width() - (lastCardList.size() - 1) * playSpacing - panel->width()) / 2;
                int top = PlayCardRect.top()+(PlayCardRect.height() - panel->height()) / 2;
                panel->move(LeftBase + i * playSpacing, top);
            }
            else
            {
                int left = PlayCardRect.left() +  (PlayCardRect.width() -  panel->width()) / 2;
                int top = PlayCardRect.top();
                panel->move(left, top + i * playSpacing);
            }
            panel->show();
        }
    }
    //      if (!MyContextMap.contains(player))
    //          return;

    //      const auto &ctx = MyContextMap[player];
    //      Cards lastcards = ctx.LastCards;
    //      if (lastcards.IsEmpty())
    //          return;

    //      CardList lastCardList = lastcards.ToCardList();
    //      int playSpacing = 24;

    //      int i = 0;
    //      for (auto it = lastCardList.cbegin(); it != lastCardList.cend(); ++it, ++i)
    //      {
    //          if (!MyCardMap.contains(*it))
    //              continue;

    //          CardPanel *panel = MyCardMap[*it];
    //          if (!panel) continue;

    //          panel->setFrontSide(true);
    //          panel->raise();
    //          panel->adjustSize();

    //          if (ctx.Align == Horizontal)
    //          {
    //              int LeftBase = ctx.PlayHanRect.left()
    //                  + (ctx.PlayHanRect.width()
    //                  - (lastCardList.size() - 1) * playSpacing
    //                  - panel->width()) / 2;

    //              int top = ctx.PlayHanRect.top()
    //                  + (ctx.PlayHanRect.height() - panel->height()) / 2;

    //              panel->move(LeftBase + i * playSpacing, top);
    //          }
    //          else
    //          {
    //              int left = ctx.PlayHanRect.left()
    //                  + (ctx.PlayHanRect.width() - panel->width()) / 2;

    //              int top = ctx.PlayHanRect.top() + i * playSpacing;
    //              panel->move(left, top);
    //          }

    //          panel->show();
    //      }

}

QPixmap GamePanel::LoadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role)
{
    //找到图片
    QVector<QString> lordMan;
    QVector<QString> lordWoman;
    QVector<QString> famerMan;
    QVector<QString> famerWoman;
    lordMan << ":/images/lord_man_1.png"  <<  ":/images/lord_man_2.png";
    lordWoman << ":/images/lord_woman_1.png"  <<  ":/images/lord_woman_2.png";
    famerMan <<":/images/farmer_man_1.png"  <<  ":/images/farmer_man_2.png";
    famerWoman << ":/images/farmer_woman_1.png"  <<  ":/images/farmer_woman_2.png";

    //加载图片 QImage 提供镜像
    QImage image;
    int random = QRandomGenerator::global()->bounded(2);//生成随机数
    if(sex == Player::Man && role == Player::Lord)
    {
        image.load(lordMan.at(random));
    }
    else if(sex == Player::Woman && role == Player::Lord)
    {
        image.load(lordWoman.at(random));
    }
    else if(sex == Player::Man && role == Player::Famer)
    {
        image.load(famerMan.at(random));
    }
    else if(sex == Player::Woman && role == Player::Famer)
    {
        image.load(famerWoman.at(random));
    }
    QPixmap pixmap;
    //头像方向
    if(direct == Player::Left)
    {
        pixmap = QPixmap::fromImage(image);
    }
    else
    {
        pixmap = QPixmap::fromImage(image.mirrored(true, false));
    }

    return pixmap;
}


void GamePanel::OnDispatchChard()
{

    //记录扑克牌的位置 不能清零
    static int CurMovePos = 0;
    //当前玩家
    Player* CurPlayer = MyGameControl->GetCurrentPlayer();


    if(CurMovePos >= 100)
    {

        CurMovePos = 0;
        //给玩家发一张牌
        Card card = MyGameControl->TakeOneCard();
        CurPlayer->StoreDispatchCard(card);
        Cards cs(card);
        //DisPosCard(CurPlayer,cs);


        //切换玩家
        MyGameControl->SetCurrentPlayer(CurPlayer->GetNextPlayer());
        //qDebug() << "current is" << CurPlayer;

        //发牌动画
        CardMoveStep(CurPlayer, CurMovePos);
        //判断牌是否发完了
        if(MyGameControl->GetSurplusCArds().cardCount() == 3)
        {
            //终止定时器
            MyTimer->stop();
            //切换游戏状态
            GameStatusPrecess(GameControl::CallingLord);
            //结束发牌的处理音乐
            MyBgm->StopAssistMusic();
            return;
        }
    }
    //移动动画
    CardMoveStep(CurPlayer, CurMovePos);
    //移动扑克牌
    CurMovePos += 15;

}

void GamePanel::OnPlayerStatusChangd(Player *player, GameControl::PlayerStatus status)
{

    //判定当前是什么状态
    switch(status)
    {
    case GameControl::ThinkingForCallLord:
        //切换按钮组必须是用户玩家
        if(player == MyGameControl->GetUserPlayer())
        {

            ui->BtnGroup->SelectPanel(ButtonGroup::CallLord, MyGameControl->GetPlayerMaxBet());
        }
        break;
    case GameControl::ThinkingForPlayHand:
        //隐藏上一轮玩家打出的牌
        HidePlayerDropCards(player);

        if(player == MyGameControl->GetUserPlayer())
        {
            //取出出牌玩家的对象
            Player* pendPlayer = MyGameControl->GetPendPlayer();
            if(pendPlayer == MyGameControl->GetUserPlayer() || pendPlayer == nullptr)//玩家对象  为空也是用户玩家
            {
                ui->BtnGroup->SelectPanel(ButtonGroup::PlayCard);
            }
            else
            {
                ui->BtnGroup->SelectPanel(ButtonGroup::PassOrPlay);
            }
        }
        else
        {
            ui->BtnGroup->SelectPanel(ButtonGroup::Empty);
        }
        break;
    case GameControl::Wining:
        //停止播放背景音乐
        MyBgm->StopBGM();
        MyContextMap[MyGameControl->GetLeftRobot()].IsFrontSide = true;
        MyContextMap[MyGameControl->GetRightRobot()].IsFrontSide = true;
        UpdatePlayerCards(MyGameControl->GetLeftRobot());//刷新页面
        UpdatePlayerCards(MyGameControl->GetRightRobot());//刷新页面
        //更新玩家得分
        UpdatePlayerScore();
        MyGameControl->SetCurrentPlayer(player);//下次是第一次叫地主
        ShowEndingScorePanel();

        break;
    default:
        break;
    }

}

void GamePanel::OnGrabLordBet(Player *player, int bet, bool flag)
{

    //显示抢地主区域
    PlayerContext& context = MyContextMap[player];
    //bet为0 则玩家放弃叫地主了
    if(bet == 0)
    {
        QPixmap px(":/images/buqinag.png");
        context.Info->setPixmap(px);
    }
    else
    {
        if(flag)//第一次抢地主
        {
            QPixmap px1(":/images/jiaodizhu.png");
            context.Info->setPixmap(px1);
        }
        else//第二次抢地主
        {
            QPixmap px2(":/images/qiangdizhu.png");
            context.Info->setPixmap(px2);
        }


        //显示加地主对应的分数
        ShowAnimation(Bet, bet);
    }

    context.Info->show();
    //qDebug() << "点击后" << context.Info;
    //播放对应的背景音乐
    MyBgm->PlayerRobolordMusic(bet, (BGMControl::RoleSex)player->GetSex(), flag);

}

void GamePanel::OnDisposePlayHand(Player *player,  Cards &cards)
{
    //qDebug() << "OnDisposePlayHand ok";

    //存储玩家打出的牌
    auto it = MyContextMap.find(player);
    it->LastCards = cards;

    //根据牌型播放对应的游戏特效 根据当前的牌的得到对应的牌型
    PlayHand hand(cards);

    PlayHand::HandType type = hand.GetHandType();

    if(type == PlayHand::Hand_Plane||
            type == PlayHand::Hand_Plane_Two_Pair||
            type == PlayHand::Hand_Plane_Two_singe)
    {
        ShowAnimation(Plane, 0);
    }
    else if(type == PlayHand::Hand_Seq_Pair)
    {
        ShowAnimation(AnimationType::LianDui,0);
    }
    else if(type == PlayHand::Hand_Seq_Single)
    {
        ShowAnimation(ShunZi,0);
    }
    else if(type == PlayHand::Hand_Bomb)
    {
        ShowAnimation(Bomb,0);
    }
    else if(type == PlayHand::Hand_Bomb_Jokers)
    {
        ShowAnimation(JokerBomb,0);
    }
    //如果玩家打出的牌是空牌（不出牌）显示提示信息
    if(cards.IsEmpty())
    {
        it->Info->setPixmap(QPixmap(":/images/pass.png"));
        it->Info->show();
        //播放不要的音乐
        MyBgm->PlayPassMusic((BGMControl::RoleSex)player->GetSex());
    }
    else
    {
        //不为空 则需要播放对应的牌型和点数
        //判断是不是第一个出牌
        if(MyGameControl->GetPendPlayer() == player || MyGameControl->GetPendPlayer() == nullptr)
        {
            //第一个出牌
            MyBgm->PlayCardMusic(cards, true, (BGMControl::RoleSex)player->GetSex());
        }
        else
        {
            //非第一个出牌
            MyBgm->PlayCardMusic(cards, false, (BGMControl::RoleSex)player->GetSex());
        }

    }
    //更新玩家剩余的牌
    UpdatePlayerCards(player);
    //播放提示音乐
    //判断玩家手中剩余牌的数量
    if(player->GetCards().cardCount() == 2)
    {
        MyBgm->PlayLastMusic(BGMControl::Last2, (BGMControl::RoleSex)player->GetSex());
    }
    else if(player->GetCards().cardCount() == 1)
    {
         MyBgm->PlayLastMusic(BGMControl::Last1, (BGMControl::RoleSex)player->GetSex());
    }

}

void GamePanel::OnCardSelected(Qt::MouseButton button)
{
    //判断游戏状态
    if(MyGameStatus == GameControl::DisPatchCard||
            MyGameStatus == GameControl::CallingLord)
    {
        return ;//如果是叫地主or发牌则不需要
    }

    //判断发射玩家是否为用户
    CardPanel *panel = static_cast<CardPanel*>(sender());    //得到对应的信号发射者
    if(panel->Owner() != MyGameControl->GetUserPlayer())
    {
        return;//不是用户玩家
    }
    //保存当前选中的牌的窗口对象
    MyCurrentSelectCard = panel;
    //判断参数是鼠标左键还是右键  
    if(button == Qt::LeftButton)
    {
        //设置扑克牌的选中状态
        panel->SetSelected(!panel->IsSelected());
        if (!panel->IsSelected())
            MySelectCards.insert(panel);
        else
            MySelectCards.remove(panel);
        //更新扑克牌在窗口的显示
        UpdatePlayerCards(panel->Owner());
        //保存or删除扑克牌的对象
        //若panel在MySelectCards 中则加入 ，不在则删除
        QSet<CardPanel*>::const_iterator it = MySelectCards.find(panel);//常量迭代器
        if(it == MySelectCards.constEnd())//没有找到添加到容器中
        {
            MySelectCards.insert(panel);
        }
        else if(it == MySelectCards.constBegin())//在容器中 则删除
        {
            MySelectCards.erase(it);
        }
        //调用播放出牌的音乐
        MyBgm->PlayAssistMusic(BGMControl::SelectCard);
    }
    else if(button == Qt::RightButton)
    {
        //调用出牌按钮的槽函数
        OnUserPlayHand();
    }
}

void GamePanel::OnUserPlayHand()
{

    //判断游戏状态
    if(MyGameStatus != GameControl::PlayingHand)
    {
        return ;
    }
    //判断是机器人还是用户
    if(MyGameControl->GetCurrentPlayer() != MyGameControl->GetUserPlayer())
    {
        return ;
    }
    //判断出的牌是否为空
    if(MySelectCards.isEmpty())
    {
        return ;
    }
    //得到要打出的牌的类型
    Cards cs;
    //遍历容器 得到出的牌
    for(auto it = MySelectCards.begin(); it != MySelectCards.end(); it++)
    {
        //的到数据
        cs.add((*it)->Card());

    }
    PlayHand hand(cs);
    PlayHand::HandType type = hand.GetHandType();
    if(type == PlayHand::Hand_Unknown)
    {
        //不是有效的牌型 不能出牌
        qDebug() << " invalid hand";
        //重置当前用户的选牌状态
        for (auto panel:MySelectCards)
        {
            panel->SetSelected(false);
        }
        MySelectCards.clear();
        //更新
        UpdatePlayerCards(MyGameControl->GetUserPlayer());
        return;
    }
    //判断当前玩家的牌能不能压住上一家的牌
    //判断用户玩家是不是刚才出牌的玩家
    Cards pendCards = MyGameControl->GetPendCards();
    Player* pendPlayer = MyGameControl->GetPendPlayer();
    Player* curPlayer  = MyGameControl->GetUserPlayer();


    if (!pendCards.IsEmpty()&& pendPlayer != curPlayer)
    {
        PlayHand pendHand(pendCards);
        if (!hand.CanBeat(pendHand))
        {
            qDebug() << "[OnUserPlayHand] cannot beat pending hand";
            return;
        }
    }
    else
    {
        qDebug() << "[OnUserPlayHand] first play, no need to beat";
    }
    MyCountDown->StopCountDown();
    //通过玩家对象出牌
    MyGameControl->GetUserPlayer()->PlayHand(cs);
    //清空容器
    MySelectCards.clear();
}

void GamePanel::OnUserPass()
{
    MyCountDown->StopCountDown();
    //判断是机器人还是用户
    Player* curPlayer = MyGameControl->GetCurrentPlayer();
    Player* userPlayer = MyGameControl->GetUserPlayer();
    if(curPlayer != userPlayer)
    {
        //不是用户
        return;
    }
    //当前用户玩家是不是上一次出牌玩家
    Player* pendPlayer = MyGameControl->GetPendPlayer();
    if(pendPlayer == userPlayer || pendPlayer == nullptr)
    {
        //全局第一次出牌or是同一出牌玩家 不能跳过
        return;
    }
    //打出一个空的Cards对象
    Cards empty;
    userPlayer->PlayHand(empty);
    //清空用户选择的牌(玩家可能选择了牌 但是没有出牌）
    //对容器遍历 将选中的牌 取消掉
    for(auto it = MySelectCards.begin(); it != MySelectCards.end(); it++)
    {
        (*it)->SetSelected(false);
    }
    MySelectCards.clear();
    //更新玩家待出牌区域的牌
    UpdatePlayerCards(userPlayer);
}

void GamePanel::ShowAnimation(AnimationType type, int bet)
{


    switch(type)
    {
    case AnimationType::LianDui:
    case AnimationType::ShunZi:
        MyAnimation->setFixedSize(250, 150);
        MyAnimation->move((width() - MyAnimation->width()) / 2, 200);
        MyAnimation->ShowSequence((AnimationWndow::Type) type);
        break;
    case AnimationType::Plane:
        MyAnimation->setFixedSize(800, 75);
        MyAnimation->move((width() - MyAnimation->width()) / 2, 200);
        MyAnimation->ShowPlane();
        break;
    case AnimationType::Bomb:
        MyAnimation->setFixedSize(180, 200);
        MyAnimation->move((width() - MyAnimation->width()) / 2, (height() - MyAnimation->height()) / 2 - 70);
        MyAnimation->ShowBomb();
        break;
    case AnimationType::JokerBomb:
        MyAnimation->setFixedSize(250, 200);
        MyAnimation->move((width() - MyAnimation->width()) / 2, (height() - MyAnimation->height()) / 2 - 70);
        MyAnimation->ShowJokerBomb();
        break;
    case AnimationType::Bet:
        MyAnimation->setFixedSize(160,98);
        MyAnimation->move((width() - MyAnimation->width()) / 2, (height() - MyAnimation->height()) / 2- 140);
        MyAnimation->ShowBetSore(bet);
        break;
    default:
        break;
    }

    MyAnimation->show();
    //qDebug() << "visible =" << MyAnimation->isVisible();
    //qDebug() << "size=" << MyAnimation->size();


}

void GamePanel::HidePlayerDropCards(Player *player)
{
    //找到该玩家在游戏窗口的信息
    auto it = MyContextMap.find(player);
    if(it != MyContextMap.end())
    {
        //确保打出的牌不为空
        if(it->LastCards.IsEmpty())//为空返回真 不为空则是假
        {
            it->Info->hide();//隐藏提示信息
        }
        else //非空 找到打出的牌  然后删掉
        {
            //将多张牌变为单张
            CardList list = it->LastCards.ToCardList();
            for(auto last = list.begin(); last != list.end(); last++)
            {
                MyCardMap[*last]->hide();
            }
        }
        it->LastCards.clear();
    }

}

void GamePanel::ShowEndingScorePanel()
{
    //调试的显示
    bool islord = MyGameControl->GetUserPlayer()->GetRole() == Player::Lord ? true:false;
    bool iswin = MyGameControl->GetUserPlayer()->GetIsWin();
    EndingPanel* panel = new EndingPanel(islord, islord, this);
    panel->show();
    panel->move((width() - panel->width()) / 2, - panel->height());
    panel->SetPlayerScore(MyGameControl->GetLeftRobot()->GetScore(),
                          MyGameControl->GetRightRobot()->GetScore(),
                          MyGameControl->GetUserPlayer()->GetScore());


    QPropertyAnimation *animation = new QPropertyAnimation(panel, "geometry", this);
    //动画持续时间
    animation->setDuration(1500);//1.5s
    //设置窗口的起始位置和终止位置
    animation->setStartValue(QRect(panel->x(), panel->y(), panel->width(), panel->height()));
    animation->setEndValue(QRect((width() - panel->width()) / 2,
                                 (height() - panel->height()) / 2,
                                 panel->width(), panel->height()));
    //播放结束音乐
    if(iswin)
    {
        MyBgm->PlayEndingMusic(true);
    }
    else
    {
        MyBgm->PlayEndingMusic(false);
    }
    //设置窗口的运动曲线
    animation->setEasingCurve(QEasingCurve(QEasingCurve::OutBounce));
    //播放动画效果
    animation->start();

    //处理窗口信号
    connect(panel, &EndingPanel::ContinueGame, this, [=]
    {
        panel->close();
        //删除
        panel->deleteLater();
        animation->deleteLater();
        ui->BtnGroup->SelectPanel(ButtonGroup::Empty);
        GameStatusPrecess(GameControl::DisPatchCard);
        //播放背景音乐
        MyBgm->StartBGM(80);
    });
}

void GamePanel::InitCountDown()
{
    //对对象进行实例化
    MyCountDown = new CountDown(this);
    MyCountDown->move((width() - MyCountDown->width()) / 2, (height() - MyCountDown->height()) / 2 + 30);
    //对倒计时5s时和0时进行匿名槽函数的处理
    connect(MyCountDown, &CountDown::NotMuchTime, this, [=]{//剩余5s
        //播放提示音乐
        MyBgm->PlayAssistMusic(BGMControl::Alert);
    });
    //时间结束后 玩家自动出最小的牌
    connect(MyCountDown, &CountDown::timeout, this, [=]() {
        Player* user = MyGameControl->GetUserPlayer();
        if (user == MyGameControl->GetCurrentPlayer()) {
            // 当前玩家是用户，出最小牌
            Cards cs;
            cs.add(user->GetCards().GetMinCard()); // 假设 GetMinCard() 返回一张最小卡
            user->PlayHand(cs);
            // 清空选牌状态
            for (auto panel : MySelectCards)
            {
                 panel->SetSelected(false);
            }

            MySelectCards.clear();
            UpdatePlayerCards(user);
        }
    });

    UserPlayer* userPlayer = MyGameControl->GetUserPlayer();//得到用户玩家实例对象
    connect(userPlayer, &UserPlayer::StartCountDown, this, [=]{
        //判断当前玩家是不是上次出牌玩家
        if(MyGameControl->GetPendPlayer() != userPlayer && MyGameControl->GetPendPlayer() != nullptr)
        {
            MyCountDown->ShowCountDown();
        }
    });
}


void GamePanel::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)
    QPainter p(this);
    p.drawPixmap(rect(),MyBkIMage);
}

void GamePanel::mouseMoveEvent(QMouseEvent *ev)
{
    //采用buttons 是要继续移动不是一瞬间的发生 起拖拽的作用
    if(ev->buttons() & Qt::LeftButton)//看鼠标左键是否参与移动
    {
        QPoint pt = ev->pos();//得到位置
        //看当前的点是否在区域里面
        if(!MyCardsRect.contains(pt))
        {
            //不在
            MyCurrentSelectCard = nullptr;
        }
        else
        {
            //在区域里
            //得到key值
            QList<CardPanel*> list = MyUserCards.keys();
            //对多有扑克篇遍历得到位置坐标
            for(int i = 0; i < list.size(); i++)
            {
                CardPanel* panel = list.at(i);
                //区域对应的位置 判断是否在其上
                if(MyUserCards[panel].contains(pt) && MyCurrentSelectCard != panel)//只让点一次
                {
                    //点击这张牌
                    panel->Clicked();//调用这个方法
                    MyCurrentSelectCard = panel;
                }
            }
        }
    }
}


