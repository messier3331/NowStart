#pragma once
#include "afxwin.h"
#include "Incude.h"

// CUnitTool ��ȭ �����Դϴ�.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUnitTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnAddCharactor();
	afx_msg void OnListBox();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSearchData();

public:	
	// value
	CString m_strTest;
	CString m_strCopy;
	CString m_strName;
	int m_iHp;
	int m_iAttack;	
	
	CString m_strFindName;


public: // control
	CListBox m_ListBox;
	CButton m_Radio[3];
	CButton m_Check[3];
	CButton m_Bitmap;


	map<CString, UNITDATA*>		m_mapUnitData;
	afx_msg void OnDeleteData();
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();
};

// Ž��(edit control ����� ��)
// ����(button�� ����� ��)
// ����, �ҷ����� ���� ��(button�� ����� ��)