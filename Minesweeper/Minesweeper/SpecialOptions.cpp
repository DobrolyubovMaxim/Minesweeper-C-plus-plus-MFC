// SpecialOptions.cpp: ���� ����������
//

#include "stdafx.h"
#include "Minesweeper.h"
#include "SpecialOptions.h"
#include "afxdialogex.h"


// ���������� ���� SpecialOptions

IMPLEMENT_DYNAMIC(SpecialOptions, CDialog)

SpecialOptions::SpecialOptions(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG1, pParent)
	, m_height(0)
	, m_width(0)
	, m_mines(0)
{

}

SpecialOptions::~SpecialOptions()
{
}

void SpecialOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HEIGHT, m_height);
	DDX_Text(pDX, IDC_WIDTH, m_width);
	DDX_Text(pDX, IDC_MINES, m_mines);
	DDV_MinMaxInt(pDX, m_mines, 1, 9999);
	DDV_MinMaxInt(pDX, m_height, 9, 100);
	DDV_MinMaxInt(pDX, m_width, 9, 100);
}


BEGIN_MESSAGE_MAP(SpecialOptions, CDialog)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_HEIGHT, &SpecialOptions::OnEnChangeHeight)
	ON_EN_CHANGE(IDC_MINES, &SpecialOptions::OnEnChangeMines)
	ON_EN_CHANGE(IDC_WIDTH, &SpecialOptions::OnEnChangeWidth)
END_MESSAGE_MAP()


// ����������� ��������� SpecialOptions


BOOL SpecialOptions::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �������� �������������� �������������

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����������: �������� ������� OCX ������ ���������� �������� FALSE
}


void SpecialOptions::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // �� �������� CDialog::OnPaint() ��� ��������� ���������

}


void SpecialOptions::OnEnChangeHeight()
{
	m_height = (int)GetDlgItemInt(IDC_HEIGHT);

}


void SpecialOptions::OnEnChangeMines()
{
	m_mines = (int)GetDlgItemInt(IDC_MINES);

}


void SpecialOptions::OnEnChangeWidth()
{
	m_width = (int)GetDlgItemInt(IDC_WIDTH);
}
