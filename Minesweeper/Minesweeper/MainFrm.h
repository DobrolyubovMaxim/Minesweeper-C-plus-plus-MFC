
// MainFrm.h : ��������� ������ CMainFrame
//

#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // ������� ������ �� ������������
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ��������
public:

// ��������
public:

// ���������������
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����������
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// ��������� ������� ����� ���������
protected:
	DECLARE_MESSAGE_MAP()

};


