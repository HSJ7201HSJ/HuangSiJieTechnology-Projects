//
//  main.cpp
//  HT-2
//
//  Created by HuangSiJie on 2019/4/18.
//  Copyright © 2019 HuangSiJie. All rights reserved.
//
//黄思杰制作的我的世界问题程序
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
//导入头文件（虽然某些可能没用）
using namespace std;
//cout和cin需要的东西
int Q1()
//j第一道题的函数 答案64
{
    using namespace std;
    int X;
    while (1 > 0)
    {
        cout << "我的世界一组是多少？" << endl;
        sleep (2);
        cin >> X;
        if(X == 64)
        {
            sleep(2);
            cout << "恭喜你，答对了。" << endl;
            sleep(2);
            break;
        }
        else
        {
            sleep(2);
            cout << "哎呦答错了，再试一次吧。" << endl;
            sleep(2);
            continue;
        }
    }
    return 0;
}
int Q2()
//第二道题的函数 答案32767
{
    using namespace std;
    int Y;
    while (1 > 0)
    {
        cout << "我的世界经验上限是多少？（同时也是Kill命令的伤害以及C++某种变量的最大值)" << endl;
        sleep (2);
        cin >> Y;
        if(Y == 32767)
        {
            sleep(2);
            cout << "恭喜你，答对了。" << endl;
            sleep(2);
            break;
        }
        else
        {
            sleep(2);
            cout << "哎呦答错了，再试一次吧。" << endl;
            sleep(2);
            continue;
        }
    }
    return 0;
}
int main()
{
    using namespace std;
    cout << "来做几道题吧！" << endl;
    sleep (2);
    //引用题目函数
    Q1(); //答案64
    Q2(); //答案32767
    return 0;
}
