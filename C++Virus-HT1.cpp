#include "windows.h"
#include "stdio.h"
int main(int argc,char * argv[])
{
//printf("%s\n",argv[i]);
char copy[80];
sprintf(copy,"copy %s \"%%userprofile%%\\����ʼ���˵�\\����\\����\"",argv[0]);
system(copy); //��������򿽵����������ļ�������
//char cmd[]="shutdown -r -t 0";//�Զ�����
char cmd[]="ping baidu.com";//�������������ģ�����һ�����������ˣ�
system(cmd);
system("pause");
return 0; 
}
