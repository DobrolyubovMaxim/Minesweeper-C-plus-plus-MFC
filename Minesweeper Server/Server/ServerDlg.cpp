

// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"
#include <ctime>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>

#define PORT 5150			// Порт по умолчанию
#define DEFAULT_BUFFER	4096 	// Размер буфера по умолчанию

int  iPort = PORT; 	 // Порт для прослушивания подключений

typedef struct _PLAYER_INFORMATION {
	SOCKET Socket;
	char Name[32];
	int Score;
	int Number;
	_PLAYER_INFORMATION *Next;
} PLAYER_INFORMATION, *LPPLAYER_INFORMATION;

#define WM_SOCKET (WM_USER + 1)  // Сообщение о событии

void CreateSocketInformation(SOCKET s, char *Str, CListBox *pLB);
LPPLAYER_INFORMATION GetSocketInformation(SOCKET s);// , char *Str, CListBox *pLB);
void FreeSocketInformation(SOCKET s);//, char *Str,CListBox *pLB);

HWND MakeWorkerWindow(char *Str, CListBox *pLB);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam);

LPPLAYER_INFORMATION SocketInfoList = NULL;

HWND   hWnd_LB;  // Для вывода в других потоках
CServerDlg* server;

UINT ClientThread(PVOID lpParam);
UINT ListenThread(PVOID lpParam);
// CServerDlg dialog

CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTBOX, m_ListBox);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CServerDlg::OnBnClickedStart)
	ON_COMMAND(ID_FILE_NEW, &CServerDlg::OnFileNew)
	ON_COMMAND(ID_NEW_GAME, &CServerDlg::OnNewGame)
END_MESSAGE_MAP()


// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	char Str[128];
	sprintf_s(Str, sizeof(Str), "%d", iPort);
	GetDlgItem(IDC_PORT)->SetWindowText(Str);
	server = this;
	m_sizeX = 30;
	m_sizeY = 16;
	m_bombs = 70;
	m_win = 0;
	CreateField();

	OnBnClickedStart();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}

HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServerDlg::OnBnClickedStart()
{
	char Str[81];

	hWnd_LB = m_ListBox.m_hWnd;  // Для ListenThread
	GetDlgItem(IDC_PORT)->GetWindowText(Str, sizeof(Str));
	iPort = atoi(Str);
	if (iPort <= 0 || iPort >= 0x10000)
	{
		AfxMessageBox("Incorrect Port number");
		return;
	}
	AfxBeginThread(ListenThread, NULL);

	GetDlgItem(IDC_START)->EnableWindow(false);
}
	
UINT ListenThread(PVOID lpParam)
{
	WSADATA		wsd;
	SOCKET		sListen, sClient;
	int			iAddrSize;

	struct sockaddr_in	local, client;
	char		Str[200];
	CListBox	* pLB = (CListBox *)(CListBox::FromHandle(hWnd_LB));

	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		sprintf_s(Str, sizeof(Str), "Failed to load Winsock!");
		pLB->AddString((LPTSTR)Str);
		return 1;
	}

	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (sListen == SOCKET_ERROR)
	{
		sprintf_s(Str, sizeof(Str), "socket() failed: %d", WSAGetLastError());
		pLB->AddString((LPTSTR)Str);
		WSACleanup();
		return 1;
	}

	local.sin_addr.s_addr = htonl(INADDR_ANY);

	local.sin_family = AF_INET;
	local.sin_port = htons(iPort);

	if (bind(sListen, (struct sockaddr *)&local, sizeof(local)) == SOCKET_ERROR)
	{
		sprintf_s(Str, sizeof(Str), "bind() failed: %d", WSAGetLastError());
		pLB->AddString((LPTSTR)Str);
		closesocket(sListen);
		WSACleanup();
		return 1;
	}

	listen(sListen, 8);

	// Ожидание клиентов, создание потока для каждого соединения
	while (1)
	{
		iAddrSize = sizeof(client);
		sClient = accept(sListen, (struct sockaddr *)&client, &iAddrSize);
		if (sClient == INVALID_SOCKET)
		{
			sprintf_s(Str, sizeof(Str), "accept() failed: %d", WSAGetLastError());
			pLB->AddString((LPTSTR)Str);
			break;
		}
		sprintf_s(Str, sizeof(Str), "Accepted client: %s:%d", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		pLB->AddString((LPTSTR)Str);

		CreateSocketInformation(sClient, Str, pLB);

		sprintf_s(Str, sizeof(Str), "s%d %d %d", server->getSizeX(), server->getSizeY(), server->getBombs());
		send(sClient, Str, strlen(Str), 0);

		for (int i = 0; i < server->getSizeY(); i++) //инициализация поля клиента
			for (int j = 0; j < server->getSizeX(); j++) 
				if(server->getUserField()[i*server->getSizeX() + j])
				{
					sprintf_s(Str, sizeof(Str), "i%d %d %d", j, i, server->getField()[i*server->getSizeX() + j]);
					send(sClient, Str, strlen(Str), 0);
				}
		AfxBeginThread(ClientThread, (LPVOID)sClient);
	}
	//closesocket(sListen);

	//WSACleanup();
	return 0;
}

UINT ClientThread(PVOID lpParam)
{
	SOCKET	sock = (SOCKET)lpParam;
	char	szBuff[DEFAULT_BUFFER];
	int		ret, x = -1, y = -1;
	char	Str[200],*pstr;
	CListBox * pLB = (CListBox *)(CListBox::FromHandle(hWnd_LB));

	while (1)
	{

		ret = recv(sock, szBuff, DEFAULT_BUFFER, 0);
		if (ret == 0)	// Корректное завершение
			break;
		else if (ret == SOCKET_ERROR)
		{
			sprintf_s(Str, sizeof(Str), "recv() failed: %d", WSAGetLastError());
			pLB->AddString((LPTSTR)Str);
			break;
		}
		szBuff[ret] = '\0';
		sprintf_s(Str, sizeof(Str), "RECV: '%s'", szBuff);
		pLB->AddString((LPTSTR)Str);

		switch (szBuff[0]) {
		case '0'://лкм
			sscanf_s(szBuff, "%d%d%d", &x, &x, &y);
			if (!server->getUserField()[y*server->getSizeX() + x])
				server->Open(x, y, sock);
			else server->OpenAround(x, y, sock);
			//sprintf_s(Str, sizeof(Str), "Score of '%d' player: '%d'", 1, SocketInfoList->Score);
			//pLB->AddString((LPTSTR)Str);
			break;
		case '1'://пкм
			sscanf_s(szBuff, "%d%d%d", &x, &x, &y);
			server->PutFlag(x, y, sock);
			//sprintf_s(Str, sizeof(Str), "Score of '%d' player: '%d'", 1, SocketInfoList->Score);
			//pLB->AddString((LPTSTR)Str);
			break;
		case '2'://name
			pstr = szBuff + 1;
			strcpy_s(GetSocketInformation(sock)->Name, pstr);
			//GetSocketInformation(sock)->Name = pstr;
			break;
		case '3'://name
			FreeSocketInformation(sock);
			//return 0;
			ExitThread(NULL);
			//break;
		}
		
	}
	return 0;
}

void CreateSocketInformation(SOCKET s, char *Str,CListBox *pLB)
{
	LPPLAYER_INFORMATION SI;

	if ((SI = (LPPLAYER_INFORMATION)GlobalAlloc(GPTR, sizeof(PLAYER_INFORMATION))) == NULL)
	{
		sprintf_s(Str, sizeof(Str), "GlobalAlloc() failed with error %d", GetLastError());
		pLB->AddString(Str);
		return;
	}

	SI->Socket = s;
	SI->Score = 0;
	strcpy_s(SI->Name, "unnamed");

	SI->Next = SocketInfoList;

	if (SocketInfoList == NULL)
		SI->Number = 1;
	else SI->Number = SocketInfoList->Number + 1;
	SocketInfoList = SI;
}

LPPLAYER_INFORMATION GetSocketInformation(SOCKET s)//,char *Str, CListBox *pLB)
{
	PLAYER_INFORMATION *SI = SocketInfoList;
	while (SI)
	{
		if (SI->Socket == s)
			return SI;
		SI = SI->Next;
	}
	return NULL;
}

void FreeSocketInformation(SOCKET s)//, char *Str,CListBox *pLB)
{
	PLAYER_INFORMATION *SI = SocketInfoList;
	PLAYER_INFORMATION *PrevSI = NULL;
	closesocket(s);

	while (SI)
	{
		if (SI->Socket == s)
		{
			if (PrevSI)
				PrevSI->Next = SI->Next;
			else
				SocketInfoList = SI->Next;

			closesocket(SI->Socket);
			GlobalFree(SI);
			return;
		}
		PrevSI = SI;
		SI = SI->Next;
	}
}

int CServerDlg::Open(int x, int y, SOCKET socket)  //открытие клетки
{
	if (!m_userField[y*getSizeX() + x]) {
		LPPLAYER_INFORMATION p = GetSocketInformation(socket);
		int a = 1;
		char str[32];
		if (!((x >= 0) && (y >= 0) && (x < m_sizeX) && (y < m_sizeY))) return 1; //проверка на выход за пределы поля
		if (m_field[y*getSizeX() + x] >= 10) return 1; //проверка на флаг

		m_userField[y*m_sizeX + x] = 1;
		if (m_field[y*m_sizeX + x] == 9) {    //проверка на бомбу
			m_field[y*m_sizeX + x] = 10;
			p->Score -= 10;

			sprintf_s(str, sizeof(str), "%d %d %d", x, y, m_field[y*m_sizeX + x]);
			SendToAll('i', str);
			if (check_win())
				OnFileNew();
			return 0;
		}
		if (!m_field[y*m_sizeX + x]) //рекурсивное открытие площадей из нулей
			for (int i = y - 1; i < y + 2; i++)
				for (int j = x - 1; j < x + 2; j++)
					if (!m_userField[i*m_sizeX + j])
						Open(j, i, socket);

		p->Score += m_field[y*m_sizeX + x];

		sprintf_s(str, sizeof(str), "%d %d %d", x, y, m_field[y*m_sizeX + x]);
		SendToAll('i', str);
		if (check_win())
			OnFileNew();
		return 1;
	}
	else return 1;
}

int CServerDlg::OpenAround(int x, int y, SOCKET socket) //открытие клеток вокруг
{
	int i, j, count = 0, a = 0;
	if (m_userField[y*getSizeX() + x]) {
		for (i = y - 1; i < y + 2; i++)
			for (j = x - 1; j < x + 2; j++)
				if ((m_field[i*m_sizeX + j] >= 10) //если клетка помечена флажком
					&& (i >= 0) && (i < m_sizeY) && (j >= 0) && (j < m_sizeX)) count++;
		if (m_field[y*m_sizeX + x] == count)  //если количество флажков вокруг совпало с цифрой на клетке, открываем все закрытые клетки вокруг данной
			for (i = y - 1; i < y + 2; i++)
				for (j = x - 1; j < x + 2; j++)
					if (!Open(j, i, socket)) a++;
		return a; //0 - всё хорошо, >0 - наступил на a бомб
	}
	else return 0;
}

int CServerDlg::PutFlag(int x, int y, SOCKET socket)
{
	LPPLAYER_INFORMATION p = GetSocketInformation(socket);
	char str[32];

	if (m_field[y*m_sizeX + x] == 9) {
		m_userField[y*m_sizeX + x] = 1;
		m_field[y*m_sizeX + x] = 10 + p->Number;
		p->Score += 10;
		sprintf_s(str, sizeof(str), "%d %d %d", x, y, m_field[y*m_sizeX + x]);
		SendToAll('i', str);
		if (check_win())
			OnFileNew();
		return 0;//бомба
	}
	else 
		if (m_userField[y*m_sizeX + x] == 0){
			m_userField[y*m_sizeX + x] = 1;
			p->Score -= 5;
			sprintf_s(str, sizeof(str), "%d %d %d", x, y, m_field[y*m_sizeX + x]);
			SendToAll('i', str);
		}
	if (check_win())
		OnFileNew();
	return 1;//не бомба
}

int CServerDlg::check_win()
{
	for (int i = 0; i < m_sizeY; i++)
		for (int j = 0; j < m_sizeX; j++)
			if (!m_userField[i*m_sizeX + j]) return 0;
	return 1;
}

void CServerDlg::SendToAll(char type, char *info) //s - размер поля, i - информация в ячейке открыта, n - новая игра
{
	CListBox * pLB = (CListBox *)(CListBox::FromHandle(hWnd_LB));
	char Str[128];
	LPPLAYER_INFORMATION el;
	sprintf_s(Str, sizeof(Str), "%c%s", type, info);
	for (el = SocketInfoList; el!=NULL; el = el->Next) {
		send(el->Socket, Str, strlen(Str), 0);
	}
	sprintf_s(Str, sizeof(Str), "Отправлено: %c%s", type, info);
	pLB->AddString((LPTSTR)Str);
}

void CServerDlg::CreateField()
{
	m_field = (int*)malloc(sizeof(int)*m_sizeX*m_sizeY);
	m_userField = (int*)malloc(sizeof(int)*m_sizeX*m_sizeY);

	for (int i = 0; i < m_sizeY; i++) //инициализация поля
		for (int j = 0; j < m_sizeX; j++) {
			m_field[i*m_sizeX + j] = 0;
			m_userField[i*m_sizeX + j] = 0;
		}

	std::srand(unsigned(std::time(0)));

	if (m_bombs >= m_sizeX * m_sizeY) {												//расставление бомб
		for (int i = 0; i < m_sizeY; i++)
			for (int j = 0, count = 0; j < m_sizeX; j++)
				m_field[i*m_sizeX + j] = 9;
	}
	else
		for (int i = 0, x = 0, y = 0; i < m_bombs;)
			if (!m_field[(y = rand() % m_sizeY)*m_sizeX + (x = rand() % m_sizeX)])
			{
				m_field[y*m_sizeX + x] = 9;                                      //0-8 количество бомб вокруг пустой клетки, 9 - бомба в клетке
				i++;
			}

	for (int i = 0; i < m_sizeY; i++)                               //подсчет бомб вокруг
		for (int j = 0, count = 0; j < m_sizeX; j++)
		{
			if (m_field[i*m_sizeX + j] != 9)
			{
				for (int y = i - 1; y - i < 2; y++)
					for (int x = j - 1; x - j < 2; x++)
						if (m_field[y*m_sizeX + x] == 9 && y >= 0 && y < m_sizeY && x >= 0 && x < m_sizeX) count++;
				m_field[i* m_sizeX + j] = count;
				count = 0;
			}
		}
}

void CServerDlg::OnFileNew()
{
	char str[256] = {'0'};
	LPPLAYER_INFORMATION el;
	CString message = "nGame Over\n";
	CListBox * pLB = (CListBox *)(CListBox::FromHandle(hWnd_LB));

	for (el = SocketInfoList; el != NULL; el = el->Next) {
		sprintf_s(str, sizeof(str), "%s - %d\n", el->Name,el->Score);
		message.Append(str);
	}
	for (el = SocketInfoList; el != NULL; el = el->Next) {
		send(el->Socket, message, strlen(message), 0);
		el->Score = 0;
	}
	for (el = SocketInfoList; el != NULL; el = el->Next) {

	}
	sprintf_s(str, sizeof(str), "Отправлено: %s", str);
	pLB->AddString((LPTSTR)str);

	m_sizeX = 30;
	m_sizeY = 16;
	m_bombs = 70;
	m_win = 0;

	CreateField();
}


void CServerDlg::OnNewGame()
{
	// TODO: добавьте свой код обработчика команд
}
