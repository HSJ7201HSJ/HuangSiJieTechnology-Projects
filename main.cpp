//
//  main.cpp
//  HT-1
//
//  Created by HuangSiJie on 2019/3/15.
//  Copyright © 2019 HuangSiJie. All rights reserved.
//
#include <iostream>
#include <stdio.h>
#include <unistd.h>
using namespace std;
int void1 ()
{
    std :: cout << "梅梅"  ;
    return 0;
}
int void2 ()
{
    return 99999999;
}
int main(int argc, const char * argv[]) {
    int a = 0;
    std :: cout << ">0=爱梅梅器<0=不爱梅梅器" ;
    std :: cin >> a;
    if (a < 0)
    {
        std ::cout << "傻逼";
    }
    else
    {
        std ::cout << "对了\n";
    }
    std::cout << "我爱梅梅器";
    void1 ();
    std:: cin >> a;
    while(1)
    {
        sleep (2);
        void1 ();
        void2 ();
    }
    std::cout << "梅梅器" ;
//    我爱妈妈
    
    return 0;
}
