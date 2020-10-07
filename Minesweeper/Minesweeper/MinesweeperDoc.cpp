
// MinesweeperDoc.cpp : реализация класса CMinesweeperDoc
//

#include "stdafx.h"
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>

#define CELLSIZE 20

#ifndef SHARED_HANDLERS
#include "Minesweeper.h"
#endif

#include "SpecialOptions.h"
#include "ConnectionWindow.h"
#include "MinesweeperDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMinesweeperDoc
CImage mine, redFlag, blueFlag, yellowFlag, orangeFlag, whiteFlag, greenFlag, lightBlueFlag, violetFlag, BrownFlag;

IMPLEMENT_DYNCREATE(CMinesweeperDoc, CDocument)

BEGIN_MESSAGE_MAP(CMinesweeperDoc, CDocument)
	ON_COMMAND(ID_DIFFICULTY_4, &CMinesweeperDoc::OnDifficulty4)
END_MESSAGE_MAP()


// создание/уничтожение CMinesweeperDoc

CMinesweeperDoc::CMinesweeperDoc()
{
	
}

CMinesweeperDoc::~CMinesweeperDoc()
{

}

// сериализация CMinesweeperDoc

void CMinesweeperDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранения
	}
	else
	{
		// TODO: добавьте код загрузки
	}
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CMinesweeperDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CMinesweeperDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задайте содержимое поиска из данных документа. 
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CMinesweeperDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// диагностика CMinesweeperDoc

#ifdef _DEBUG
void CMinesweeperDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMinesweeperDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

void CMinesweeperDoc::Draw(CDC *PDC)
{
	CString s;
	CRect cell;
	CBrush greyBrush(RGB(192, 192, 192));
	CBrush whiteBrush(RGB(255, 255, 255));
	CBrush redBrush(RGB(255, 0, 0));
	CBrush* oldbrush = PDC->SelectObject(&greyBrush);
	PDC->SetBkColor(RGB(192, 192, 192));

	//cell.SetRect(12, 12, 58, 38); //бомбы
	//PDC->Rectangle(cell);
	//s.Format("%d", m_bombs);
	//PDC->DrawText(s, cell, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//cell.SetRect(CELLSIZE*m_sizeX / 2 - 13, 12, CELLSIZE*m_sizeX / 2 + 13, 38);        //лицо
	//PDC->Rectangle(cell);
	//if (game_start == 1 && m_lmb == 0) PDC->DrawText("o_o", cell, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//if (game_start == 1 && m_lmb == 1) PDC->DrawText("0o0", cell, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	PDC->SetBkColor(RGB(255, 255, 255));

	for (int i = 0; i < m_sizeY; i++)
		for (int j = 0; j < m_sizeX; j++)
			DrawCell(PDC, j, i);
	PDC->SelectObject(&oldbrush);

}

void CMinesweeperDoc::DrawCell(CDC* PDC, int x, int y)
{
	CString s;
	CRect cell;
	CBrush greyBrush(RGB(192, 192, 192));
	CBrush whiteBrush(RGB(255, 255, 255));
	CBrush* oldbrush = PDC->SelectObject(&greyBrush);

	if (m_userField[y*m_sizeX + x])
	{
		PDC->SelectObject(&whiteBrush);
		cell.SetRect(x * CELLSIZE, y * CELLSIZE, (x + 1) * CELLSIZE, (y + 1)* CELLSIZE);
		if(m_field[y*m_sizeX + x] <= 10)
			PDC->SetBkColor(RGB(255, 255, 255));
		else PDC->SetBkColor(RGB(192, 192, 192));
		PDC->Rectangle(cell);

		if (m_field[y*m_sizeX + x] > 8) 
			cell.SetRect(x * CELLSIZE + 1, y * CELLSIZE + 1, (x + 1) * CELLSIZE - 1, (y + 1)* CELLSIZE - 1);
		
		switch (m_field[y*m_sizeX + x])
		{
		case 1:
			PDC->SetTextColor(RGB(0, 0, 255));
			break;
		case 2:
			PDC->SetTextColor(RGB(0, 128, 0));
			break;
		case 3:
			PDC->SetTextColor(RGB(255, 0, 0));
			break;
		case 4:
			PDC->SetTextColor(RGB(0, 0, 128));
			break;
		case 5:
			PDC->SetTextColor(RGB(128, 0, 0));
			break;
		case 6:
			PDC->SetTextColor(RGB(0, 128, 128));
			break;
		case 7:
			PDC->SetTextColor(RGB(0, 0, 0));
			break;
		case 8:
			PDC->SetTextColor(RGB(128, 128, 128));
			break;
		case 10:
			mine.Draw(*PDC, cell);
			break;
		case 11:
			redFlag.Draw(*PDC, cell);
			break;
		case 12:
			blueFlag.Draw(*PDC, cell);
			break;
		case 13:
			yellowFlag.Draw(*PDC, cell);
			break;
		case 14:
			orangeFlag.Draw(*PDC, cell);
			break;
		case 15:
			greenFlag.Draw(*PDC, cell);
			break;
		case 16:
			whiteFlag.Draw(*PDC, cell);
			break;
		case 17:
			lightBlueFlag.Draw(*PDC, cell);
			break;
		case 18:
			violetFlag.Draw(*PDC, cell);
			break;
		case 19:
			BrownFlag.Draw(*PDC, cell);
			break;
		}
		if (m_field[y*m_sizeX + x] >= 1 && m_field[y*m_sizeX + x] <= 8) {
			s.Format("%d", m_field[y*m_sizeX + x]);
			PDC->DrawText(s, cell, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	else
	{
		PDC->SelectObject(&greyBrush);
		cell.SetRect(x * CELLSIZE, y * CELLSIZE, (x + 1) * CELLSIZE, (y + 1)* CELLSIZE);
		PDC->Rectangle(cell);
		//PDC->SetBkColor(RGB(255, 255, 255));
	}
}

void CMinesweeperDoc::initField(int x, int y, int bombs, bool images)
{
	m_sizeX = x;
	m_sizeY = y;
	m_bombs = bombs;
	game_start = 1;
	m_field = (int*)malloc(sizeof(int)*m_sizeX*m_sizeY);
	m_userField = (int*)malloc(sizeof(int)*m_sizeX*m_sizeY);
	for (int i = 0; i < m_sizeY; i++) //инициализация поля
		for (int j = 0; j < m_sizeX; j++) {
			m_field[i*m_sizeX + j] = 0;
			m_userField[i*m_sizeX + j] = 0;
		}
	if (images) {
		mine.Load(_T("..\\resources\\mine.png"));
		redFlag.Load(_T("..\\resources\\flag white.png"));
		blueFlag.Load(_T("..\\resources\\flag blue.png"));
		yellowFlag.Load(_T("..\\resources\\flag yellow.png"));
		orangeFlag.Load(_T("..\\resources\\flag orange.png"));
		whiteFlag.Load(_T("..\\resources\\flag white.png"));
		greenFlag.Load(_T("..\\resources\\flag green.png"));
		lightBlueFlag.Load(_T("..\\resources\\flag light blue.png"));
		violetFlag.Load(_T("..\\resources\\flag violet.png"));
		BrownFlag.Load(_T("..\\resources\\flag brown.png"));
	}
}

void CMinesweeperDoc::setDifficulty(int dif)
{
	switch (dif)
	{
	case 0:
		m_sizeX = 9;
		m_sizeY = 9;
		m_bombs = m_allBombs = 10;
		break;
	case 1:
		m_sizeX = 16;
		m_sizeY = 16;
		m_bombs = 40;
		break;
	case 2:
		m_sizeX = 30;
		m_sizeY = 16;
		m_bombs = m_allBombs = 99;
		break;
	case 3:
		m_sizeX = 30;
		m_sizeY = 16;
		m_bombs = m_allBombs = 99;
	}
	DeleteContents();
	OnNewDocument();
	UpdateAllViews(0);
}

void CMinesweeperDoc::OnDifficulty4()
{
	SpecialOptions OptDlg;
	OptDlg.m_mines = m_allBombs;
	OptDlg.m_height = m_sizeY;
	OptDlg.m_width = m_sizeX;

	if (OptDlg.DoModal() == IDOK)
	{
		m_allBombs = OptDlg.m_mines;
		m_sizeY = OptDlg.m_height;
		m_sizeX = OptDlg.m_width;
		UpdateAllViews(NULL);
		OnNewDocument();
		CWnd* m_hWnd = AfxGetApp()->GetMainWnd();
		m_hWnd->MoveWindow(400, 200, 20 + m_sizeX*CELLSIZE, 113 + m_sizeY*CELLSIZE);
	}
	// инициализация переменных класса:
}
