#ifndef LOADING_H
#define LOADING_H

#include <QWidget>

class Loading : public QWidget
{
    Q_OBJECT
public:
    explicit Loading(QWidget *parent = nullptr);

signals:
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap MyBk;
    QPixmap MyProgress;//加载进度条
    int MyDist = 15;//存储进度条宽度初始值为默认宽度

};

#endif // LOADING_H
