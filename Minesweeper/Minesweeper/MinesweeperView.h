
// MinesweeperView.h : ��������� ������ CMinesweeperView
//

#pragma once


class CMinesweeperView : public CView
{
protected:
	//CPoint m_PointOld;
	//CPoint m_PointOrigin;
	CString m_ClassName;
public:
	int m_difficulty;
	SOCKET m_sClient;
	CString username;
	enum { DIF_EASY, DIF_NORMAL, DIF_HARD, DIF_SPEC };
protected: // ������� ������ �� ������������
	CMinesweeperView();
	DECLARE_DYNCREATE(CMinesweeperView)

// ��������
public:
	CMinesweeperDoc* GetDocument() const;

// ��������
public:

// ���������������
public:
	virtual void OnDraw(CDC* pDC);  // �������������� ��� ��������� ����� �������������
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ���������� � ������ ��� ����� ������������

// ����������
public:
	virtual ~CMinesweeperView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ��������� ������� ����� ���������
protected:
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDifficulty1();
	afx_msg void OnUpdateDifficulty1(CCmdUI *pCmdUI);
	afx_msg void OnDifficulty2();
	afx_msg void OnUpdateDifficulty2(CCmdUI *pCmdUI);
	afx_msg void OnDifficulty3();
	afx_msg void OnUpdateDifficulty3(CCmdUI *pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	//afx_msg void OnConnectiontogame();
	void SendMessageToServer(int type,int x,int y);
	void RequestField();
	afx_msg void OnDisconnect();
	//afx_msg void OnUpdateConnectiontogame(CCmdUI *pCmdUI);
	afx_msg void OnDestroy();
//	afx_msg void OnPaint();
	afx_msg void OnConnectiontogame();
};

#ifndef _DEBUG  // ���������� ������ � MinesweeperView.cpp
inline CMinesweeperDoc* CMinesweeperView::GetDocument() const
   { return reinterpret_cast<CMinesweeperDoc*>(m_pDocument); }
#endif

