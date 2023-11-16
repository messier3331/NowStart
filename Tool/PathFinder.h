#pragma once
#include "afxwin.h"
#include "Incude.h"

// CPathFinder 대화 상자입니다.

class CPathFinder : public CDialog
{
	DECLARE_DYNAMIC(CPathFinder)

public:
	CPathFinder(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathFinder();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHFINDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnListBox();
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDestroy();

public:
	CListBox m_ListBox;
	list<IMGPATH*>		m_PathList;
};
