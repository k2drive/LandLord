#ifndef ROBTGRAPLORD_H
#define ROBTGRAPLORD_H

#include <QThread>
#include "player.h"

class RobotGrapLord : public QThread
{
    Q_OBJECT
public:
    explicit RobotGrapLord(Player* player,QObject *parent = nullptr);

protected:
    void run() ;

signals:

private:
    Player* MyPlayer;

};

#endif // ROBTGRAPLORD_H
