//START
#include <windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;
//�˳���ǳ�Σ�գ���С�����У�
//����VS����
//��������Ȩ��,HuangSiJieTechnology Co. LTD. 2019-05-27
void revir()
{ 
WIN32_FIND_DATA FindFileData;
HANDLE hFind = FindFirstFile("*.c*", &FindFileData);

string FileCode[500];

//������������Ӧ������"main"����ĵ�һ��"{"���� 
//��C++��main��int main(),int main(int arg,char **r)����ʽ
//������������ļ� 
while (true) 
{ 
   //�����ļ�
   int FileLen = 0; //�������볤��
   int MainLoc = 0; //������λ�� 
  
   ifstream ibe(FindFileData.cFileName);
   getline(ibe,FileCode[0]);
  
   if(FileCode[0] == "//START")//��ǰ�ļ�����Ⱦ�� 
   {
    //��������Ҫ���ǵ� 
    for(FileLen = 0;getline(ibe,FileCode[0]);)
    {
     if(FileCode[FileLen] == "//END")
     {
      break;
     }
    }
    //����δ��Ⱦǰ�Ĵ���
    for(FileLen = 0;getline(ibe,FileCode[FileLen]);FileLen++)
    {
     if(FileCode[FileLen].find("main") != -1)
     {
      MainLoc = FileLen;
     }
    }
   
    //������ļ��еĺ�������
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
    cout<<FindFileData.cFileName<<":��������ɹ�/n";
   }
   cout<<FindFileData.cFileName<<":û�б���Ⱦ!/n";
   if (FindNextFile(hFind, &FindFileData) == false) break;
}
}
int main()
{
revir();
return 0;
}