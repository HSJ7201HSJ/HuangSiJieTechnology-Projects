/*1. 建立线程运行其他可执行文件

2.关闭任务管理器或者其他窗口

3.ShellExecute的用法

4.打开关闭显示器

5.使鼠标乱跑

1. 建立一个线程 （ CreateThread() ）*/

DWORD WINAPI DestroyWindows(LPVOID) 

{ 

//your code would go here 

} 

int main() 

{ 

CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)&DestroyWindows, 0, 0, NULL); 

while(1) 

{ 

Sleep(10); 

} 

} 
//2. 关闭任务管理器或者其他窗口

TaskMgr = FindWindow(NULL,"Windows Task Manager"); 

if( TaskMgr != NULL ) 

{ 

PostMessage( TaskMgr, WM_CLOSE, (LPARAM)0, (WPARAM)0); 

} 
//程序发出关闭任务管理器的的消息，并不等待继续执行下面的代码

3. ShellExcute

char Notepad[MAX_PATH]="notepad.exe"; 

ShellExecute(NULL,"open",Notepad,NULL,NULL,SW_MAXIMIZE); 
//打开记事本。也可以通过以下代码打开google.

char Website[MAX_PATH] = "http:\\www.google.com
//4. 打开关闭显示器

SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM) 2); 

Sleep(5000); 

SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM) -1); 
//5. 鼠标乱跑

X = rand()%801; 

Y = rand()%601; 

SetCursorPos( X, Y ); 