#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>

namespace Ui {
class ButtonGroup;
}

class ButtonGroup : public QWidget
{
    Q_OBJECT

public:
    enum Panel{Start,
               PlayCard,
               PassOrPlay,
               CallLord,
               Empty};
    explicit ButtonGroup(QWidget *parent = nullptr);
    ~ButtonGroup();

    //按钮初始化
    void InitButtons();

    //处理page页的切换 bet默认为0
    void SelectPanel(Panel type, int bet = 0);

signals:
    //开始游戏
    void StartGame();
    //出牌
    void PlayHand();
    //不出牌
    void Pass();
    //抢地主 即下注
    void BetPoint(int bet);
private:
    Ui::ButtonGroup *ui;
};

#endif // BUTTONGROUP_H
