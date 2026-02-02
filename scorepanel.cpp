
#include "scorepanel.h"
#include "ui_scorepanel.h"

const QString MYCOLOR[] = {"black","white", "red", "blue", "greem"};

ScorePanel::ScorePanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ScorePanel)
{
    ui->setupUi(this);
    //标签存储到容器中
    MyList << ui->MyScore << ui->LeftScore << ui->RightScore
           << ui->meTitle << ui->leftTitle << ui->rightTitle
           << ui->score1 << ui->score2 << ui->score3;
}
ScorePanel::~ScorePanel()
{
    delete ui;
}

void ScorePanel::SetScores(int left, int right, int user)
{
    ui->LeftScore->setText(QString::number(left));
      ui->RightScore->setText(QString::number(right));
      ui->MyScore->setText(QString::number(user));
}

void ScorePanel::SetMyFontSize(int point)
{
    QFont font("微软雅黑", point, QFont::Bold);
    //遍历容器 将自定义类型给其label
    for(int i = 0; i < MyList.size(); i++)
    {
        MyList[i]->setFont(font);
    }
}

void ScorePanel::SetMyFontColor(ScorePanel::FontColor color)
{
    QString style = QString("QLabel{color:%1}").arg(MYCOLOR[color]);
    for(int i = 0; i < MyList.size(); i++)
    {

        MyList[i]->setStyleSheet(style);
    }
}
