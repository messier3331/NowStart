// PathFinder.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathFinder.h"
#include "afxdialogex.h"
#include "FileInfo.h"


// CPathFinder ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPathFinder, CDialog)

CPathFinder::CPathFinder(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHFINDER, pParent)
{

}

CPathFinder::~CPathFinder()
{
}

void CPathFinder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CPathFinder, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CPathFinder::OnListBox)
	ON_BN_CLICKED(IDC_BUTTON1, &CPathFinder::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON7, &CPathFinder::OnLoadData)
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CPathFinder::OnListBox()
{
	
}

void CPathFinder::OnSaveData()
{
	wofstream		fout;

	fout.open(L"../Data/ImgPath.txt", ios::out);

	if (!fout.fail())	// ���� ���� ����
	{
		for (auto& iter : m_PathList)
		{
			fout << iter->wstrObjKey << L"|" << iter->wstrStateKey << L"|" << iter->iCount << L"|" << iter->wstrPath << endl;
		}

		fout.close();
	}

	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}

void CPathFinder::OnLoadData()
{
	UpdateData(TRUE);

	wifstream		fin;

	fin.open(L"../Data/ImgPath.txt", ios::in);

	if (!fin.fail())
	{
		TCHAR	szObjKey[MAX_STR] = L"";
		TCHAR	szStateKey[MAX_STR] = L"";
		TCHAR	szCount[MAX_STR] = L"";
		
		TCHAR	szPath[MAX_PATH] = L"";

		wstring	wstrCombined = L"";

		m_ListBox.ResetContent();

		while (true)
		{
			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;
			
			wstrCombined = wstring(szObjKey) + L"|" + szStateKey + L"|" + szCount + L"|" + szPath;

			m_ListBox.AddString(wstrCombined.c_str());
		}

		fin.close();
	}
	
	UpdateData(FALSE);
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);

}


void CPathFinder::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	UpdateData(TRUE);

	CDialog::OnDropFiles(hDropInfo);

	TCHAR		szFilePath[MAX_PATH] = L"";

	int	iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);
		CFileInfo::DirInfoExtraction(szFilePath, m_PathList);
	}

	m_ListBox.ResetContent();

	wstring		wstrCombined = L"";
	TCHAR		szBuf[MAX_STR] = L"";

	for (auto& iter : m_PathList)
	{
		//_itow_s : ������ �����ڵ� ���ڿ� ���·� ��ȯ, 3��° �Ű������� 10������ ��ȯ�ϰڴٴ� �ǹ�
		_itow_s(iter->iCount, szBuf, 10);

		wstrCombined = iter->wstrObjKey + L"|" + iter->wstrStateKey + L"|" + szBuf + L"|" + iter->wstrPath;
		
		m_ListBox.AddString(wstrCombined.c_str());
	}	

	UpdateData(FALSE);
}


void CPathFinder::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	for_each(m_PathList.begin(), m_PathList.end(), CDeleteObj());
	m_PathList.clear();

}
