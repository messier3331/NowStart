// MyForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM), m_MySheet(L"����")
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnUnitTool)
	ON_BN_CLICKED(IDC_BUTTON7, &CMyForm::OnMapTool)
	ON_BN_CLICKED(IDC_BUTTON8, &CMyForm::OnProperty)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON9, &CMyForm::OnPathFind)
END_MESSAGE_MAP()


// CMyForm �����Դϴ�.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm �޽��� ó�����Դϴ�.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// CreatePointFont(��Ʈ ũ��, �۾�ü)
	m_Font.CreatePointFont(180, L"�ü�");

	// GetDlgItem : ���̾�α׿� ��ġ�� ���ҽ��� id���� ���� ������ �Լ�
	GetDlgItem(IDC_BUTTON1)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON7)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON8)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON9)->SetFont(&m_Font);

	if (nullptr == m_MySheet.GetSafeHwnd())
		m_MySheet.Create(0, WS_OVERLAPPEDWINDOW);

}


void CMyForm::OnUnitTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// id�� �ش��ϴ� ���̾�α� �ڽ��� ����
	// GetSafeHwnd : ���� ���̾�α� ������ �ڵ��� ��ȯ

	if(nullptr == m_UnitTool.GetSafeHwnd())
		m_UnitTool.Create(IDD_UNITTOOL);
	
	// â ������� ���
	m_UnitTool.ShowWindow(SW_SHOW);
}

void CMyForm::OnMapTool()
{
	if (nullptr == m_MapTool.GetSafeHwnd())
		m_MapTool.Create(IDD_MAPTOOL);

	// â ������� ���
	m_MapTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnProperty()
{
	m_MySheet.ShowWindow(SW_SHOW);
}


void CMyForm::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	ShowWindow(SW_HIDE);

	//CFormView::OnClose();
}


void CMyForm::OnPathFind()
{
	if (nullptr == m_PathFinder.GetSafeHwnd())
		m_PathFinder.Create(IDD_PATHFINDER);

	// â ������� ���
	m_PathFinder.ShowWindow(SW_SHOW);

}
