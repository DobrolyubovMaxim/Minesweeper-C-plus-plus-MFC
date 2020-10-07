
// Minesweeper.h : главный файл заголовка для приложения Minesweeper
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CMinesweeperApp:
// О реализации данного класса см. Minesweeper.cpp
//

class CMinesweeperApp : public CWinApp
{
public:
	CMinesweeperApp();


// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMinesweeperApp theApp;
