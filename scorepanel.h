
#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QLabel>
#include <QWidget>

namespace Ui {
class ScorePanel;
}

class ScorePanel : public QWidget
{
    Q_OBJECT

public:
    enum FontColor
    {
        Black,
        White,
        Red,
        Blue,
        Green
    };
    explicit ScorePanel(QWidget *parent = nullptr);
    ~ScorePanel();

    //设置玩家得分
    void SetScores  (int left, int right, int user);
    //设置字体大小
    void SetMyFontSize(int point);
    //设置字体颜色
    void SetMyFontColor(FontColor color);

private:
    Ui::ScorePanel *ui;
    QVector<QLabel*> MyList;//添加一个容器 保存其实例对象的地址
};

#endif // SCOREPANEL_H
