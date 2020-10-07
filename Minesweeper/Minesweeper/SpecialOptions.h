#pragma once


// диалоговое окно SpecialOptions

class SpecialOptions : public CDialog
{
	DECLARE_DYNAMIC(SpecialOptions)

public:
	SpecialOptions(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~SpecialOptions();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	int m_height;
	int m_width;
	int m_mines;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnEnChangeHeight();
	afx_msg void OnEnChangeMines();
	afx_msg void OnEnChangeWidth();
};
