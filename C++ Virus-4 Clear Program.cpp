//START
#include <windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;
//此程序非常危险！请小心运行！
//请用VS运行
//保留所有权利,HuangSiJieTechnology Co. LTD. 2019-05-27
void revir()
{ 
WIN32_FIND_DATA FindFileData;
HANDLE hFind = FindFirstFile("*.c*", &FindFileData);

string FileCode[500];

//病毒函数调用应当插在"main"后面的第一个"{"后面 
//在C++中main有int main(),int main(int arg,char **r)等形式
//依次清除宿主文件 
while (true) 
{ 
   //加载文件
   int FileLen = 0; //宿主代码长度
   int MainLoc = 0; //主函数位置 
  
   ifstream ibe(FindFileData.cFileName);
   getline(ibe,FileCode[0]);
  
   if(FileCode[0] == "//START")//当前文件被感染过 
   {
    //病毒代码要过虑掉 
    for(FileLen = 0;getline(ibe,FileCode[0]);)
    {
     if(FileCode[FileLen] == "//END")
     {
      break;
     }
    }
    //宿主未感染前的代码
    for(FileLen = 0;getline(ibe,FileCode[FileLen]);FileLen++)
    {
     if(FileCode[FileLen].find("main") != -1)
     {
      MainLoc = FileLen;
     }
    }
   
    //清除主文件中的函数调用
    for (int i = MainLoc;i < FileLen;i++) 
    {
     if(FileCode[i].find('{') != -1)
     {
      FileCode[i + 1] = "";
      break;
     }
    }
    ibe.close();
   
    ofstream be(FindFileData.cFileName);
    for(int i = 0;i < FileLen;i++)
    {
     be<<FileCode[i]<<endl;
    }
    be.close();
    cout<<FindFileData.cFileName<<":病毒清除成功/n";
   }
   cout<<FindFileData.cFileName<<":没有被感染!/n";
   if (FindNextFile(hFind, &FindFileData) == false) break;
}
}
int main()
{
revir();
return 0;
}