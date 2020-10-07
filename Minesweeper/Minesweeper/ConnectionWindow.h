#pragma once


// Диалоговое окно ConnectionWindow

class ConnectionWindow : public CDialog
{
	DECLARE_DYNAMIC(ConnectionWindow)

public:
	ConnectionWindow(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~ConnectionWindow();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONNECTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	CString m_name;
	CString m_port;
	CString m_server;
	SOCKET	m_sClient;
	char name[128];
	bool success;
	
	afx_msg void OnBnClickedConnect();
	virtual void OnCancel();
};
