# 笔记

当一个玩家叫了三分之后  自然成为了地主 ，就没有其他玩家的事情

当Realease是 使用rcc.exe生成Qt二进制文件 .rcc 命令生成rcc需要使用绝对路径。

## 遇到的问题 以及最终解决的方式

1, 发牌遇到发牌正反面未区分，且非单张![](E:\code\Qt\出bug图片集合\屏幕截图 2026-01-16 143436.png)

![](E:\code\Qt\出bug图片集合\屏幕截图 2026-01-17 142431.png)

。发现图片裁剪没有完成 和get函数返回的变量错误。=>在裁剪图片时，需要确定起始点和判断条件 ，get函数的命名中要包含需要得到的参数名字，在return返回中根据名称确定。

2，现在下注的分数无法显示在主窗口中 而是单独显示在一个新的窗口-> 通过Qdebu发现没有进入paintEvent。发现 paintEvent函数拼写错误，paintEvent是虚函数 =>推出结论：使用虚函数要使用override关键字，要不然不发出发事件机制

3，已写出机器人的出牌线程和出牌所需的函数 但是现在运行的时候机器人没有出牌。初步发现：信号量和槽函数有错 或者 connect 没有连接正确，导致Qvector断言=>指针问题。在debug期间 出现一系列问题

下图是strategy类中的FistPlay函数出现错误 根据提示是越界问题 当第一个出牌的机器人玩家 想出的牌的类型在判断的时候 出现vector越界问题，通过Debug输出判断出是当第一个玩家出三带一or二或者三个的时候出现越界错误

> ```c++
> 		qDebug() << "5 ok";
> 		Cards triple = triples.first();
>         qDebug() << "first ok"； 
>        	Card::CardPoint tp = PlayHand(triple).GetCardPoint(); 
> 		qDebug() << "GetCardPoint ok";
> ```

输出：

> 5 ok
> first ok

 通过上述代码和输出 判断是PlayHand构造函数中JudgeCardType函数错误，在PlayHand中if里面的 MyPt = MyTwoCard.at(0);不应该是Two而是Three

![](E:\code\Qt\出bug图片集合\屏幕截图 2026-01-24 200910.png)

![幕截图 2026-01-24 20154](E:\code\Qt\出bug图片集合\屏幕截图 2026-01-24 201548.png)

下图是在OnPlayHand和OnDisposePlayHand这连个槽函数处理错误 错误的发送了下注的信号。 进一笔判断出来：在出牌阶段打出的各种牌型的牌 没有对应的动画 ，然而上次下注的图片只是被隐藏了 ，在switch之后 重写show方法了，使得下注的图片又重新出现。解决方法：将下注产生的分数显示2s之后 清楚掉，下次再次show之后，下注分数区域没有图片是空的，所以没有显示。等到后续其他牌型产生提示信息之后，显示一段时间后 直接清除掉。

![](E:\code\Qt\出bug图片集合\屏幕截图 2026-01-24 155849.png)

下图问题是 牌型未匹配正确 初步判断是strategy类错误。确定具体位置是GetGreaterCards函数根据已出牌的类型，来查询当前出牌玩家有没有和已出牌玩家所出的相同类型的牌型查询错误。进一步查询发现 是FindCardType该函数错误在连对我顺子出现错误，再次debug发现是GetSeqPairOrSeqSingle函数错误 在GetSeqPairOrSeqSingle函数中的顺子判断FindSamePointCards((Card::CardPoint)(point + 1), info.number);不应该是加一而是加i

细想 只有连对和顺子是错误的 ，初步应该是从这两个下手 ，但是没有这样考虑 。结论 ：遇到bug之后 应该仔细观察bug的特征 然后找到问题的大致类型。

![](E:\code\Qt\出bug图片集合\屏幕截图 2026-01-24 200611.png)

![](E:\code\Qt\出bug图片集合\屏幕截图 2026-01-26 174316.png)

~~目前已想到的方法是：将地主机器人的第一次出牌的牌型规定下来 ，对每一个牌型进行debug~~

4，音乐类编写完成之后 ，添加到已有的Gmapanel类中 出现越界bug ，越界查找到 MyPlayer没有初始化 导致其size为空 从而越界，最后结论： MyPlayer 没有初始化![](E:\code\Qt\出bug图片集合\屏幕截图 2026-01-31 213540.png)

5，在Release的文件夹下找到对应程序的.exe双击执行 出现：”由于找不到Qt5Core.dll，无法执行代码 重写安装可能会解决此问题“。”由于找不到Qt5Gui.dll，无法执行代码 重写安装可能会解决此问题“。”由于找不到Qt5Multimedia.dll，无法执行代码 重写安装可能会解决此问题“。”由于找不到Qt5Widgestdll，无法执行代码 重写安装可能会解决此问题“。这种情况 该怎么解决？ 在Qt Create 中可以run => 方法：通过cmd找到Qt目录里面的bin目录判断版本是否为：Qt Deploy Tool 5.14.2，然后使用命令：cd E:\code\Qt\build-LandLord-Desktop_Qt_5_14_2_MinGW_64_bit-Release\release。目的是将缺少的dll拷贝到realease目录中

6，bug描述：机器人都pass之后 玩家出牌只有对子和单张 且必须要为第一次选中 ，当选中飞机or顺子or'连对的时候 不允许出牌 再次选择正确的牌的时候 也不能出牌

## 发布相关事宜

1，将文本文件转换为二进制文件的命令
E:\Qt\Qt5.14.2\5.14.2\mingw73_64\bin\rcc.exe -binary res.qrc -o resource.rcc
在mian函数中加上#include <QResource>和 QResource::registerResource("./resource.rcc"); 并且在Qt项目中移除res.qrc文件但不删除源文件
目的：使得.exe文件很小 资源全到resource文件中。但是无法在Qt项目文件中run,只有将res.qrc加入到源项目中 才可以在Qt项目中运行文件
将得到的小的exe可执行程序 和 resource.rcc文件 放到需要打包发布的文件夹中 

2，在使用下述命令 使得程序所运行的库加载到该文件中
E:\Qt\Qt5.14.2\5.14.2\mingw73_64\bin\windeployqt.exe LandLord.exe

## 待改进

1，给用户提供提示的按钮

