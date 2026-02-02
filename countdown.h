#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QTimer>
#include <QWidget>

class CountDown : public QWidget
{
    Q_OBJECT
public:
    explicit CountDown(QWidget *parent = nullptr);

    void ShowCountDown();
    //处理 玩家在时间结束之前 打出了牌
    void StopCountDown();

signals:
    void NotMuchTime();
    void timeout();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap MyClock;
    QPixmap MyNumber;
    QTimer* MyTimer;//定时器
    int  MyCount;//总时长

};

#endif // COUNTDOWN_H
