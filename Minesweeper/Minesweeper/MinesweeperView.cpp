
// MinesweeperView.cpp : реализация класса CMinesweeperView
//

#include "stdafx.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "Minesweeper.h"
#endif

#include "MinesweeperDoc.h"
#include "MinesweeperView.h"
#include "ConnectionWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CELLSIZE 20
#define DEFAULT_BUFFER	4096

UINT ListenThread(PVOID lpParam);
HWND hWnd_LB;
CMinesweeperView* client;
CMinesweeperDoc* pDoc;

IMPLEMENT_DYNCREATE(CMinesweeperView, CView)

BEGIN_MESSAGE_MAP(CMinesweeperView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_DIFFICULTY_1, &CMinesweeperView::OnDifficulty1)
	ON_UPDATE_COMMAND_UI(ID_DIFFICULTY_1, &CMinesweeperView::OnUpdateDifficulty1)
	ON_COMMAND(ID_DIFFICULTY_2, &CMinesweeperView::OnDifficulty2)
	ON_UPDATE_COMMAND_UI(ID_DIFFICULTY_2, &CMinesweeperView::OnUpdateDifficulty2)
	ON_COMMAND(ID_DIFFICULTY_3, &CMinesweeperView::OnDifficulty3)
	ON_UPDATE_COMMAND_UI(ID_DIFFICULTY_3, &CMinesweeperView::OnUpdateDifficulty3)
	//ON_COMMAND(ID_DIFFICULTY_4, &CMinesweeperView::OnDifficulty4)
	//ON_UPDATE_COMMAND_UI(ID_DIFFICULTY_4, &CMinesweeperView::OnUpdateDifficulty4)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_DISCONNECT, &CMinesweeperView::OnDisconnect)
	//ON_UPDATE_COMMAND_UI(ID_CONNECTIONTOGAME, &CMinesweeperView::OnUpdateConnectiontogame)
	ON_WM_DESTROY()
	//	ON_WM_PAINT()
	ON_COMMAND(ID_CONNECTIONTOGAME, &CMinesweeperView::OnConnectiontogame)
END_MESSAGE_MAP()

// создание/уничтожение CMinesweeperView


CMinesweeperView::CMinesweeperView()
{
	// TODO: добавьте код создания
}

CMinesweeperView::~CMinesweeperView()
{
}

BOOL CMinesweeperView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs
	//CView::OnInitialUpdate();
	m_ClassName = AfxRegisterWndClass
		(CS_HREDRAW | CS_VREDRAW,                // стили окна
			0,                                      // без курсора
			(HBRUSH)::GetStockObject(WHITE_BRUSH), // белый фон 
			0);                                     // без значка
	cs.lpszClass = m_ClassName;

	return CView::PreCreateWindow(cs);
}
// рисование CMinesweeperView

void CMinesweeperView::OnDraw(CDC* pDC)
{
	CMinesweeperDoc* PDoc = GetDocument();
	ASSERT_VALID(PDoc);
	if (!PDoc)
		return;
	PDoc->Draw(pDC);
}

void CMinesweeperView::OnInitialUpdate()
{
	CWnd* m_hWnd = AfxGetApp()->GetMainWnd();
	pDoc = GetDocument();
	pDoc->initField(30, 16, 10, 0);
	m_hWnd->MoveWindow(400, 200, 20 + pDoc->getSizeX()*CELLSIZE, 63 + pDoc->getSizeY()*CELLSIZE);

}

#ifdef _DEBUG
void CMinesweeperView::AssertValid() const
{
	CView::AssertValid();
}

void CMinesweeperView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMinesweeperDoc* CMinesweeperView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMinesweeperDoc)));
	return (CMinesweeperDoc*)m_pDocument;
}
#endif //_DEBUG

void CMinesweeperView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMinesweeperDoc* PDoc = GetDocument();

	PDoc->UpdateAllViews(NULL);
	CView::OnLButtonDown(nFlags, point);
}

void CMinesweeperView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (pDoc->getStart() == 1) {
		SendMessageToServer(0, point.x / CELLSIZE, point.y / CELLSIZE);
		pDoc->UpdateAllViews(NULL);
	}
	CView::OnLButtonUp(nFlags, point);
}

void CMinesweeperView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (pDoc->getStart() == 1) {
		SendMessageToServer(1, point.x / CELLSIZE, point.y / CELLSIZE);
		pDoc->UpdateAllViews(NULL);
	}
	CView::OnRButtonDown(nFlags, point);
}

void CMinesweeperView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного

	CView::OnRButtonUp(nFlags, point);
}

void CMinesweeperView::OnConnectiontogame()
{
	ConnectionWindow connection;

	connection.DoModal();

	m_sClient = connection.m_sClient;
	if (connection.success) {
		username = "2";
		username.Append(connection.name);
		send(m_sClient, username, strlen(username), 0);
		AfxBeginThread(ListenThread, (LPVOID)m_sClient);
	}

}

void CMinesweeperView::SendMessageToServer(int type , int x, int y)//0 - лкм, 1 - пкм, 2 - name
{
	char Str[128];
	sprintf_s(Str, sizeof(Str), "%d %d %d", type, x, y);
	send(m_sClient, Str, strlen(Str), 0);
}

void CMinesweeperView::OnDifficulty1()
{
	// TODO: добавьте свой код обработчика команд
	CMinesweeperDoc* PDoc = GetDocument();
	m_difficulty = DIF_EASY;
	PDoc->setDifficulty(0);

	CWnd* m_hWnd = AfxGetApp()->GetMainWnd();
	m_hWnd->MoveWindow(400, 200, 20 + PDoc->getSizeX()*CELLSIZE, 113 + PDoc->getSizeY()*CELLSIZE);
}

void CMinesweeperView::OnUpdateDifficulty1(CCmdUI *pCmdUI)
{
	// TODO: добавьте свой код обработчика ИП обновления команд
	pCmdUI->SetRadio(m_difficulty == DIF_EASY);
}

void CMinesweeperView::OnDifficulty2()
{
	// TODO: добавьте свой код обработчика команд
	CMinesweeperDoc* PDoc = GetDocument();
	m_difficulty = DIF_NORMAL;
	PDoc->setDifficulty(1);

	CWnd* m_hWnd = AfxGetApp()->GetMainWnd();
	m_hWnd->MoveWindow(400, 200, 20 + PDoc->getSizeX()*CELLSIZE, 113 + PDoc->getSizeY()*CELLSIZE);
}

void CMinesweeperView::OnUpdateDifficulty2(CCmdUI *pCmdUI)
{
	// TODO: добавьте свой код обработчика ИП обновления команд
	pCmdUI->SetRadio(m_difficulty == DIF_NORMAL);
}

void CMinesweeperView::OnDifficulty3()
{
	CMinesweeperDoc* PDoc = GetDocument();
	m_difficulty = DIF_HARD;
	PDoc->setDifficulty(2);

	CWnd* m_hWnd = AfxGetApp()->GetMainWnd();
	m_hWnd->MoveWindow(400, 200, 20 + PDoc->getSizeX()*CELLSIZE, 113 + PDoc->getSizeY()*CELLSIZE);
}

void CMinesweeperView::OnUpdateDifficulty3(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_difficulty == DIF_HARD);
}

UINT ListenThread(PVOID lpParam)
{

	// CMinesweeperDoc* PDoc = client->GetDocument();
	SOCKET sock = (SOCKET)lpParam;
	CWnd* m_hWnd = AfxGetApp()->GetMainWnd();
	CRect rect;
	//char szBuff[DEFAULT_BUFFER];
	char* szBuff = (char*)malloc(DEFAULT_BUFFER * sizeof(char));
	char* beginSzBuff = szBuff;
	int ret, x, y, info, len;
	char Str[200];

	while (1)
	{
		ret = recv(sock, szBuff, DEFAULT_BUFFER, 0);
		if (ret == 0)	// Корректное завершение
			break;
		else if (ret == SOCKET_ERROR)
		{
			sprintf_s(Str, sizeof(Str), "recv() failed: %d", WSAGetLastError());
			AfxMessageBox(Str);
			break;
		}
		szBuff[ret] = '\0';

		for (int i = 0; i < ret; i++) {

			switch (szBuff[i])
			{
			case 's'://размеры поля
				szBuff += i;
				ret -= i;
				i = 0;
				sscanf_s(szBuff, "%c%d%d%d%d", &Str[0],&Str[1], &x, &y, &info);
				pDoc->initField(x, y, info, 1);
				m_hWnd->MoveWindow(400, 200, 20 + pDoc->getSizeX()*CELLSIZE, 63 + pDoc->getSizeY()*CELLSIZE);
				break;
			case 'i'://информация в ячейке открылась
				szBuff += i;
				ret -= i;
				i = 0;
				len = sscanf_s(szBuff, "%c%d%d%d%d", &Str[0], &Str[1], &x, &y, &info);
				pDoc->m_field[y*pDoc->getSizeX() + x] = info;
				pDoc->m_userField[y*pDoc->getSizeX() + x] = 1;
				rect.SetRect(x * CELLSIZE, y * CELLSIZE, (x + 1) * CELLSIZE, (y + 1)* CELLSIZE);
				m_hWnd->InvalidateRect(rect);
				break;
			case 'n'://конец игры ,начало новой
				szBuff += i;
				ret -= i;
				i = 0;
				sscanf_s(szBuff, "%c%d", &Str[0], &Str[1], &info); //info - колво игроков
				szBuff += 1;
				AfxMessageBox(szBuff);
				pDoc->initField(pDoc->m_sizeX, pDoc->m_sizeY, pDoc->m_bombs, 0);
				m_hWnd->Invalidate();
				break;
			}
		}
		//PostMessage(*m_hWnd, WM_PAINT, NULL, NULL);
		//pDoc->UpdateAllViews(NULL);
		szBuff = beginSzBuff;
		//SendMessage(WM_PAINT);
		//Invalidate();
		//UpdateWindow();
		//pDoc->UpdateAllViews(NULL);
		//AfxMessageBox(szBuff);
	}
	return 0;
}

void CMinesweeperView::OnDisconnect()
{
	closesocket(m_sClient);
	WSACleanup();
}

void CMinesweeperView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: добавьте свой код обработчика сообщений
	
	char Str[16];
	sprintf_s(Str, sizeof(Str), "3");
	send(m_sClient, Str, strlen(Str), 0);
	closesocket(m_sClient);
}

