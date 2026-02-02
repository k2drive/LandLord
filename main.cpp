#include "gamepanel.h"
#include "cards.h"
#include "loading.h"
#include <QApplication>
#include <qdebug.h>
//#include <QResource>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<Cards>("Cards&");//调试阶段 确保信号中的参数是引用传送
    //qRegisterMetaType<Cards>("Cards");//值传送 信号机制 跨函数
    //告诉程序有一个二进制资源文件
    //QResource::registerResource("./resource.rcc");
    Loading w;
    w.show();


    return a.exec();
}
