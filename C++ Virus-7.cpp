/*1. �����߳�����������ִ���ļ�

2.�ر����������������������

3.ShellExecute���÷�

4.�򿪹ر���ʾ��

5.ʹ�������

1. ����һ���߳� �� CreateThread() ��*/

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
//2. �ر����������������������

TaskMgr = FindWindow(NULL,"Windows Task Manager"); 

if( TaskMgr != NULL ) 

{ 

PostMessage( TaskMgr, WM_CLOSE, (LPARAM)0, (WPARAM)0); 

} 
//���򷢳��ر�����������ĵ���Ϣ�������ȴ�����ִ������Ĵ���

3. ShellExcute

char Notepad[MAX_PATH]="notepad.exe"; 

ShellExecute(NULL,"open",Notepad,NULL,NULL,SW_MAXIMIZE); 
//�򿪼��±���Ҳ����ͨ�����´����google.

char Website[MAX_PATH] = "http:\\www.google.com
//4. �򿪹ر���ʾ��

SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM) 2); 

Sleep(5000); 

SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM) -1); 
//5. �������

X = rand()%801; 

Y = rand()%601; 

SetCursorPos( X, Y ); 