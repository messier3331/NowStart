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
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnListBox)
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnSaveData)
END_MESSAGE_MAP()


// CMapTool �޽��� ó�����Դϴ�.


void CMapTool::OnListBox()
{
	UpdateData(TRUE);

	CString		strSelectName;

	int		iIndex = m_ListBox.GetCurSel();

	m_ListBox.GetText(iIndex, strSelectName);

	auto	iter = m_mapPngImg.find(strSelectName);


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

	pMainView->m_iDrawID = iDrawID;

	if (iter == m_mapPngImg.end())
		return;

	m_Picture.SetBitmap(*(iter->second));


	UpdateData(FALSE);
}


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	UpdateData(TRUE);
	
	CDialog::OnDropFiles(hDropInfo);
	
	TCHAR		szFilePath[MAX_PATH] = L"";
	TCHAR		szFileName[MAX_STR] = L"";

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

		auto iter = m_mapPngImg.find(strFileName);

		if (iter == m_mapPngImg.end())
		{
			CImage*	pPngImg = new CImage;
			pPngImg->Load(strRelative);

			m_mapPngImg.insert({ strFileName, pPngImg });
			m_ListBox.AddString(strFileName);
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

	CDC*	pDC = m_ListBox.GetDC();

	// GetCount : ����Ʈ �ڽ� ����� ������ ����
	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		//	GetText: �ε����� �ش��ϴ� ����� ���ڿ��� ����
		m_ListBox.GetText(i, strName);

		// ���� ���ڿ��� ���̸� �ȼ� ������ ��ȯ
		size = pDC->GetTextExtent(strName);

		if (size.cx > iWidth)
			iWidth = size.cx;
	}

	m_ListBox.ReleaseDC(pDC);

	// GetHorizontalExtent : ����Ʈ �ڽ��� ���� ��ũ�� �ٸ� ������ �� �ִ� �ִ� ������ ������ �Լ�
	if (iWidth > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iWidth);

}



void CMapTool::OnDestroy()
{
	CDialog::OnDestroy();

	for_each(m_mapPngImg.begin(), m_mapPngImg.end(), [](auto& MyPair)
	{
		MyPair.second->Destroy();
		Safe_Delete(MyPair.second);
	});
	m_mapPngImg.clear();

	
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
