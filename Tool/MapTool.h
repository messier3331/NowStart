#pragma once

#include "Incude.h"
#include "afxwin.h"


// CMapTool ��ȭ �����Դϴ�.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapTool();

	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnListBox_Ground();
	afx_msg void OnListBox_Wall();
	afx_msg void OnListBox_Decorate();

	afx_msg void OnDropFiles(HDROP hDropInfo);

	void	Horizontal_Scroll();
	afx_msg void OnDestroy();
	afx_msg void OnSaveData();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();

public:
	CListBox m_ListBox_Ground;
	CListBox m_ListBox_Wall;
	CListBox m_ListBox_Decorate;

	CStatic m_Picture_Ground;
	CStatic m_Picture_Wall;
	CStatic m_Picture_Decorate;

	map<CString, CImage*>		m_mapPngImg_Ground;
	map<CString, CImage*>		m_mapPngImg_Wall;
	map<CString, CImage*>		m_mapPngImg_Decorate;

	CButton m_Pick[3];
	afx_msg void MapUndo();
	afx_msg void MapRedo();
};
