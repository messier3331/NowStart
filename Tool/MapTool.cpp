// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MainFrm.h"
#include "ToolView.h"

// CMapTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox_Ground);
	DDX_Control(pDX, IDC_LIST2, m_ListBox_Wall);
	DDX_Control(pDX, IDC_LIST3, m_ListBox_Decorate);

	DDX_Control(pDX, IDC_PICTURE, m_Picture_Ground);
	DDX_Control(pDX, IDC_PICTURE2, m_Picture_Wall);
	DDX_Control(pDX, IDC_PICTURE3, m_Picture_Decorate);

	DDX_Control(pDX, IDC_RADIO1, m_Pick[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Pick[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Pick[2]);


}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnListBox_Ground)
	ON_LBN_SELCHANGE(IDC_LIST2, &CMapTool::OnListBox_Wall)
	ON_LBN_SELCHANGE(IDC_LIST3, &CMapTool::OnListBox_Decorate)
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnSaveData)
	ON_BN_CLICKED(IDC_RADIO1, &CMapTool::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMapTool::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CMapTool::OnBnClickedRadio3)

	ON_BN_CLICKED(IDC_BUTTON3, &CMapTool::MapUndo)
	ON_BN_CLICKED(IDC_BUTTON10, &CMapTool::MapRedo)
END_MESSAGE_MAP()


// CMapTool �޽��� ó�����Դϴ�.


void CMapTool::OnListBox_Ground()
{
	UpdateData(TRUE);

	CString		strSelectName;

	int		iIndex = m_ListBox_Ground.GetCurSel();

	m_ListBox_Ground.GetText(iIndex, strSelectName);

	auto	iter = m_mapPngImg_Ground.find(strSelectName);


	int iSkip = strSelectName.Find(_T("Grass"));
	if (iSkip != -1)
	{
		iSkip += _tcslen(_T("Grass"));
	}

	CString strName = strSelectName.Mid(iSkip);
	int iDrawID = _ttoi(strName);

	// AfxGetApp : ���� �����带 ���� �ִ� ���� ���� app�� ��ȯ
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	// GetPane(��, ��) : ��� ���� �ش��ϴ� Ŭ������ ������ ������ �Լ� 
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	pMainView->m_iDrawID_Ground = iDrawID;



	if (iter == m_mapPngImg_Ground.end())
		return;

	m_Picture_Ground.SetBitmap(*(iter->second));


	UpdateData(FALSE);
}

void CMapTool::OnListBox_Wall()
{
	UpdateData(TRUE);

	CString		strSelectName;

	int		iIndex = m_ListBox_Wall.GetCurSel();

	m_ListBox_Wall.GetText(iIndex, strSelectName);

	auto	iter = m_mapPngImg_Wall.find(strSelectName);

	int iSkip = strSelectName.Find(_T("Grass"));
	if (iSkip != -1)
	{
		iSkip += _tcslen(_T("Grass"));
	}

	CString strName = strSelectName.Mid(iSkip);
	int iDrawID = _ttoi(strName);

	// AfxGetApp : ���� �����带 ���� �ִ� ���� ���� app�� ��ȯ
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	// GetPane(��, ��) : ��� ���� �ش��ϴ� Ŭ������ ������ ������ �Լ� 
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	pMainView->m_iDrawID_Wall = iDrawID;


	if (iter == m_mapPngImg_Wall.end())
		return;

	m_Picture_Wall.SetBitmap(*(iter->second));


	UpdateData(FALSE);


}

void CMapTool::OnListBox_Decorate()
{

	UpdateData(TRUE);

	CString		strSelectName;

	int		iIndex = m_ListBox_Decorate.GetCurSel();


	m_ListBox_Decorate.GetText(iIndex, strSelectName);


	auto	iter = m_mapPngImg_Decorate.find(strSelectName);

	int iSkip = strSelectName.Find(_T("Grass"));
	if (iSkip != -1)
	{
		iSkip += _tcslen(_T("Grass"));
	}

	CString strName = strSelectName.Mid(iSkip);
	int iDrawID = _ttoi(strName);

	// AfxGetApp : ���� �����带 ���� �ִ� ���� ���� app�� ��ȯ
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	// GetPane(��, ��) : ��� ���� �ش��ϴ� Ŭ������ ������ ������ �Լ� 
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	pMainView->m_iDrawID_Drcorate = iDrawID;



	if (iter == m_mapPngImg_Decorate.end())
		return;

	m_Picture_Decorate.SetBitmap(*(iter->second));


	UpdateData(FALSE);


}


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	UpdateData(TRUE);
	
	CDialog::OnDropFiles(hDropInfo);
	
	TCHAR		szFilePath[MAX_PATH] = L"";
	TCHAR		szFileName[MAX_STR] = L"";

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	CRect rectGround, rectWall, rectDecorate;
	m_ListBox_Ground.GetWindowRect(&rectGround);
	m_ListBox_Wall.GetWindowRect(&rectWall);
	m_ListBox_Decorate.GetWindowRect(&rectDecorate);
	ScreenToClient(&rectGround);
	ScreenToClient(&rectWall);
	ScreenToClient(&rectDecorate);


	// DragQueryFile : ��ӵ� ���� �� 0�� �ε����� �ش��ϴ� ������ ��ü ��θ� ����
	// 2���� :  0xffffffff(-1)�� �����Ǹ� ��ӵ� ������ ������ ��ȯ
	int	iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		CString	strRelative = CFileInfo::ConvertRelativePath(szFilePath);
		CString	strFileName = PathFindFileName(strRelative);

		lstrcpy(szFileName, strFileName.GetString());
		
		// Ȯ���� ���� ����
		PathRemoveExtension(szFileName);

		strFileName = szFileName;

		if (rectGround.PtInRect(pt))
			// ���콺�� �巡�� �� ����� �κ��� m_ListBox_Ground ����Ʈ �ڽ��� ���
		{
			auto iter = m_mapPngImg_Ground.find(strFileName);

			if (iter == m_mapPngImg_Ground.end())
			{
				CImage* pPngImg = new CImage;

				pPngImg->Load(strRelative);
				m_mapPngImg_Ground.insert({ strFileName, pPngImg });
				m_ListBox_Ground.AddString(strFileName);
			}

		}
		else if (rectWall.PtInRect(pt))
			/*���콺�� �巡�� �� ��� �� �κ��� m_ListBox_Wall ����Ʈ �ڽ��ϰ��*/
		{
			auto iter = m_mapPngImg_Wall.find(strFileName);

			if (iter == m_mapPngImg_Wall.end())
			{
				CImage* pPngImg = new CImage;

				pPngImg->Load(strRelative);
				m_mapPngImg_Wall.insert({ strFileName, pPngImg });
				m_ListBox_Wall.AddString(strFileName);
			}
		}
		else if (rectDecorate.PtInRect(pt))
			/*���콺�� �巡�� �� ��� �� �κ��� m_ListBox_Decorate ����Ʈ �ڽ��ϰ��*/
		{
			auto iter = m_mapPngImg_Decorate.find(strFileName);

			if (iter == m_mapPngImg_Decorate.end())
			{
				CImage* pPngImg = new CImage;

				pPngImg->Load(strRelative);
				m_mapPngImg_Decorate.insert({ strFileName, pPngImg });
				m_ListBox_Decorate.AddString(strFileName);
			}
		}
			
	}

	Horizontal_Scroll();

	UpdateData(FALSE);
}

void CMapTool::Horizontal_Scroll()
{
	CString			strName;
	CSize			size;

	int		iWidth(0);

	CDC*	pDC = m_ListBox_Ground.GetDC();

	// GetCount : ����Ʈ �ڽ� ����� ������ ����
	for (int i = 0; i < m_ListBox_Ground.GetCount(); ++i)
	{
		//	GetText: �ε����� �ش��ϴ� ����� ���ڿ��� ����
		m_ListBox_Ground.GetText(i, strName);

		// ���� ���ڿ��� ���̸� �ȼ� ������ ��ȯ
		size = pDC->GetTextExtent(strName);

		if (size.cx > iWidth)
			iWidth = size.cx;
	}

	m_ListBox_Ground.ReleaseDC(pDC);

	// GetHorizontalExtent : ����Ʈ �ڽ��� ���� ��ũ�� �ٸ� ������ �� �ִ� �ִ� ������ ������ �Լ�
	if (iWidth > m_ListBox_Ground.GetHorizontalExtent())
		m_ListBox_Ground.SetHorizontalExtent(iWidth);

}



void CMapTool::OnDestroy()
{
	CDialog::OnDestroy();

	for_each(m_mapPngImg_Ground.begin(), m_mapPngImg_Ground.end(), [](auto& MyPair)
	{
		MyPair.second->Destroy();
		Safe_Delete(MyPair.second);
	});
	m_mapPngImg_Ground.clear();

	for_each(m_mapPngImg_Wall.begin(), m_mapPngImg_Wall.end(), [](auto& MyPair)
		{
			MyPair.second->Destroy();
			Safe_Delete(MyPair.second);
		});
	m_mapPngImg_Wall.clear();

	for_each(m_mapPngImg_Decorate.begin(), m_mapPngImg_Decorate.end(), [](auto& MyPair)
		{
			MyPair.second->Destroy();
			Safe_Delete(MyPair.second);
		});
	m_mapPngImg_Decorate.clear();

	
}


void CMapTool::OnSaveData()
{
	CFileDialog		Dlg(FALSE,	// TRUE(����, �ҷ�����), FALSE(�ٸ� �̸����� �����ϱ�) �������
		L"dat",	// �⺻ Ȯ���ڸ�
		L"*.dat", // ��ȭ ���ڿ� ǥ�õ� ���� ���ϸ�
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, //OFN_HIDEREADONLY(�б� ���� üũ�ڽ� ����) | OFN_OVERWRITEPROMPT(�ߺ� ���� ���� �� ���޼��� ���)
		L"Data Files(*.dat) | *.dat||", // ��ȭ ���ڿ� ǥ�õ� ���� ����("�޺� �ڽ��� ��µ� ���ڿ� | ���� ����� ���͸� ���ڿ� ||"
		this);	// �θ� ������ �ּ�

	TCHAR	szPath[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		CString		str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD		dwByte(0);

		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView* pMainView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

		vector<TILE*>& vecTile = pMainView->m_pTerrain->Get_VecTile();

		if (vecTile.empty())
			return;

		char cTile = 'T';

		for (auto& iter : vecTile)
		{
			WriteFile(hFile, &cTile, sizeof(char), &dwByte, nullptr);
			WriteFile(hFile, iter, sizeof(TILE), &dwByte, nullptr);
		}

		vector<TILE*>& vecWall = pMainView->m_pTerrain->Get_VecWall();
		if (vecWall.empty())
			return;

		cTile = 'W';

		for (auto& iter : vecWall)
		{
			WriteFile(hFile, &cTile, sizeof(char), &dwByte, nullptr);
			WriteFile(hFile, iter, sizeof(TILE), &dwByte, nullptr);
		}


		CloseHandle(hFile);
	}
}


void CMapTool::OnBnClickedRadio1()
{
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	pMainView->m_bGround = true;
	pMainView->m_bWall = false;
	pMainView->m_bDecorate = false;
}

void CMapTool::OnBnClickedRadio2()
{
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	pMainView->m_bGround = false;
	pMainView->m_bWall = true;
	pMainView->m_bDecorate = false;
}

void CMapTool::OnBnClickedRadio3()
{
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	pMainView->m_bGround = false;
	pMainView->m_bWall = false;
	pMainView->m_bDecorate = true;
}


void CMapTool::MapUndo()
{
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	pMainView->m_pTerrain->Undo();
	pMainView->Invalidate(FALSE);
}


void CMapTool::MapRedo()
{
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	pMainView->m_pTerrain->Redo();
	pMainView->Invalidate(FALSE);
}
