#ifndef ANIMATIONWNDOW_H
#define ANIMATIONWNDOW_H

#include <QWidget>

class AnimationWndow : public QWidget
{
    Q_OBJECT
public:
    enum Type
    {
        Sequence,
        Pair
    };
    explicit AnimationWndow(QWidget *parent = nullptr);

    //显示下注分数
    void ShowBetSore(int bet);
    //显示顺子和连对
    void ShowSequence(Type type);
    //王炸和炸弹可以合并代码
    //显示王炸
    void ShowJokerBomb();
    //显示炸弹
    void ShowBomb();
    //显示飞机
    void ShowPlane();

signals:

protected:
    //事件处理函数
    void paintEvent(QPaintEvent* ev) override;

private:
    QPixmap MyPixmap;
    int MyIndex = 0;//用于炸弹图片的切换
    int MyX = 0;

};

#endif // ANIMATIONWNDOW_H
