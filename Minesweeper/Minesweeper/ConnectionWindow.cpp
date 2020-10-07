// ConnectionWindow.cpp: файл реализации
//

#include "stdafx.h"
#include "Minesweeper.h"
#include "ConnectionWindow.h"
#include "afxdialogex.h"

#define DEFAULT_PORT	5150


// Диалоговое окно ConnectionWindow

IMPLEMENT_DYNAMIC(ConnectionWindow, CDialog)

ConnectionWindow::ConnectionWindow(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CONNECTION, pParent)
	, m_port(_T(""))
	, m_server(_T(""))
	, m_name(_T(""))
{

}

ConnectionWindow::~ConnectionWindow()
{
}

void ConnectionWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PORT, m_port);
	DDX_Text(pDX, IDC_SERVER, m_server);
	DDX_Text(pDX, IDC_USERNAME, m_name);
}


BEGIN_MESSAGE_MAP(ConnectionWindow, CDialog)

	ON_BN_CLICKED(IDC_CONNECT, &ConnectionWindow::OnBnClickedConnect)
END_MESSAGE_MAP()


// Обработчики сообщений ConnectionWindow
BOOL ConnectionWindow::OnInitDialog()
{
	CDialog::OnInitDialog();
	success = false;

	char Str[128];
	GetDlgItem(IDC_SERVER)->SetWindowText("localhost");
	sprintf_s(Str, sizeof(Str), "%d", DEFAULT_PORT);
	GetDlgItem(IDC_PORT)->SetWindowText(Str);
	GetDlgItem(IDC_USERNAME)->SetWindowText("Player");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}

void ConnectionWindow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // Не вызывать CDialog::OnPaint() для сообщений рисования

}

void ConnectionWindow::OnBnClickedConnect()
{
	char szServer[128];	// Имя или IP-адрес сервера
	int iPort;			// Порт
	
	WSADATA	wsd;

	struct sockaddr_in 	server;
	struct hostent		*host = NULL;

	char Str[256];

	GetDlgItem(IDC_SERVER)->GetWindowText(szServer, sizeof(szServer));
	GetDlgItem(IDC_PORT)->GetWindowText(Str, sizeof(Str));
	GetDlgItem(IDC_USERNAME)->GetWindowText(name, sizeof(name));
	iPort = atoi(Str);
	if (iPort <= 0 || iPort >= 0x10000)
	{
		AfxMessageBox((LPTSTR)"Port number incorrect");
		return;
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		AfxMessageBox((LPTSTR)"Failed to load Winsock library!");
		return;
	}

	m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sClient == INVALID_SOCKET)
	{
		sprintf_s(Str, sizeof(Str), "socket() failed: %d\n", WSAGetLastError());
		AfxMessageBox((LPTSTR)Str);
		return;
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(iPort);
	server.sin_addr.s_addr = inet_addr(szServer);

	if (server.sin_addr.s_addr == INADDR_NONE)
	{
		host = gethostbyname(szServer);
		if (host == NULL)
		{
			sprintf_s(Str, sizeof(Str), "Unable to resolve server: %s", szServer);
			AfxMessageBox((LPTSTR)Str);
			return;
		}
		CopyMemory(&server.sin_addr, host->h_addr_list[0], host->h_length);
	}

	if (connect(m_sClient, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		sprintf_s(Str, sizeof(Str), "connect() failed: %d", WSAGetLastError());
		AfxMessageBox((LPTSTR)Str);
		return;
	}

	//AfxMessageBox("Successfully connected to server");
	success = true;
	this->OnCancel();
}




void ConnectionWindow::OnCancel()
{
	// TODO: добавьте специализированный код или вызов базового класса

	CDialog::OnCancel();
}
