
// ServerDlg.h : header file
//

#pragma once


// CServerDlg dialog
class CServerDlg : public CDialogEx
{
protected:
	int* m_field;        //данные о поле
	int* m_userField;   //данные об известной пользователю части поля
	int m_sizeX;
	int m_sizeY;
	int m_bombs;
	int m_win; //1 - выйгрыш, 0 - идёт игра
public:
	int* getField() { return m_field; }
	char* getFieldToString();
	int* getUserField() { return m_userField; }
	int getSizeX() { return m_sizeX; }
	int getSizeY() { return m_sizeY; }
	int getBombs() { return m_bombs; }
	void setWinStatus(int status) { m_win = status; }
	int Open(int x, int y, SOCKET socket);
	int OpenAround(int x, int y, SOCKET socket);
	int PutFlag(int x, int y, SOCKET socket);
	int check_win();
	void setDifficulty(int dif);
	void SendToAll(char type, char* info);
	void CreateField();


// Construction
public:
	CServerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ListBox;
	afx_msg void OnBnClickedStart();
	afx_msg void OnFileNew();
	afx_msg void OnNewGame();
};
