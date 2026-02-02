#ifndef ENDINGPANEL_H
#define ENDINGPANEL_H

#include "scorepanel.h"

#include <QLabel>
#include <QPushButton>
#include <QWidget>

class EndingPanel : public QWidget
{
    Q_OBJECT
public:
    explicit EndingPanel(bool isLord, bool isWin, QWidget *parent = nullptr);
    void SetPlayerScore(int left, int right, int me);

signals:
    void ContinueGame();

protected:
    void paintEvent(QPaintEvent* ev);

private:
    QPixmap MyBk;//QPixmap是做了优化的
    QLabel* MyTitle;
    ScorePanel* MyScore;
    QPushButton* MyContinue;


};

#endif // ENDINGPANEL_H
