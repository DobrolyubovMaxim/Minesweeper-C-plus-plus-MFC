
// Minesweeper.h : ������� ���� ��������� ��� ���������� Minesweeper
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"       // �������� �������


// CMinesweeperApp:
// � ���������� ������� ������ ��. Minesweeper.cpp
//

class CMinesweeperApp : public CWinApp
{
public:
	CMinesweeperApp();


// ���������������
public:
	virtual BOOL InitInstance();

// ����������
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMinesweeperApp theApp;
