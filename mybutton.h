#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QWidget>
#include<QPushButton>

class MyBUtton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyBUtton(QWidget *parent = nullptr);

    void SetImage(QString normal, QString hover, QString pressed);

protected:
    //鼠标按下
    void mousePressEvent(QMouseEvent* ev) override;
    //鼠标释放
    void mouseReleaseEvent(QMouseEvent* ev) override;
    //鼠标进入
    void enterEvent(QEvent* event) override;
    //鼠标离开
    void leaveEvent(QEvent* ev) override;
    //绘图
    void paintEvent(QPaintEvent * ev) override;

signals:


private:
    QString MyNormal;
    QString MyHover;
    QString MyPressed;
    QPixmap MyPixmap;
};

#endif // MYBUTTON_H
