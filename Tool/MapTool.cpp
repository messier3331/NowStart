// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MainFrm.h"
#include "ToolView.h"

// CMapTool 대화 상자입니다.

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


// CMapTool 메시지 처리기입니다.


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

	// AfxGetApp : 메인 쓰레드를 갖고 있는 현재 메인 app을 반환
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	// GetPane(행, 열) : 행과 열에 해당하는 클래스의 정보를 얻어오는 함수 
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

	// AfxGetApp : 메인 쓰레드를 갖고 있는 현재 메인 app을 반환
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	// GetPane(행, 열) : 행과 열에 해당하는 클래스의 정보를 얻어오는 함수 
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

	// AfxGetApp : 메인 쓰레드를 갖고 있는 현재 메인 app을 반환
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	// GetPane(행, 열) : 행과 열에 해당하는 클래스의 정보를 얻어오는 함수 
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	pMainView->m_iDrawID_Drcorate = iDrawID;



	if (iter == m_mapPngImg_Decorate.end())
		return;

	m_Picture_Decorate.SetBitmap(*(iter->second));


	UpdateData(FALSE);


}


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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


	// DragQueryFile : 드롭된 파일 중 0번 인덱스에 해당하는 파일의 전체 경로를 얻어옴
	// 2인자 :  0xffffffff(-1)로 지정되면 드롭된 파일의 개수를 반환
	int	iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		CString	strRelative = CFileInfo::ConvertRelativePath(szFilePath);
		CString	strFileName = PathFindFileName(strRelative);

		lstrcpy(szFileName, strFileName.GetString());
		
		// 확장자 명을 제거
		PathRemoveExtension(szFileName);

		strFileName = szFileName;

		if (rectGround.PtInRect(pt))
			// 마우스로 드래그 앤 드랍한 부분이 m_ListBox_Ground 리스트 박스일 경우
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
			/*마우스로 드래그 앤 드랍 한 부분이 m_ListBox_Wall 리스트 박스일경우*/
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
			/*마우스로 드래그 앤 드랍 한 부분이 m_ListBox_Decorate 리스트 박스일경우*/
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

	// GetCount : 리스트 박스 목록의 개수를 얻어옴
	for (int i = 0; i < m_ListBox_Ground.GetCount(); ++i)
	{
		//	GetText: 인덱스에 해당하는 목록의 문자열을 얻어옴
		m_ListBox_Ground.GetText(i, strName);

		// 현재 문자열의 길이를 픽셀 단위로 변환
		size = pDC->GetTextExtent(strName);

		if (size.cx > iWidth)
			iWidth = size.cx;
	}

	m_ListBox_Ground.ReleaseDC(pDC);

	// GetHorizontalExtent : 리스트 박스가 가로 스크롤 바를 생성할 수 있는 최대 범위를 얻어오는 함수
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
	CFileDialog		Dlg(FALSE,	// TRUE(열기, 불러오기), FALSE(다른 이름으로 저장하기) 모드지정
		L"dat",	// 기본 확장자명
		L"*.dat", // 대화 상자에 표시될 최초 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, //OFN_HIDEREADONLY(읽기 전용 체크박스 숨김) | OFN_OVERWRITEPROMPT(중복 파일 저장 시 경고메세지 띄움)
		L"Data Files(*.dat) | *.dat||", // 대화 상자에 표시될 파일 형식("콤보 박스에 출력될 문자열 | 실제 사용할 필터링 문자열 ||"
		this);	// 부모 윈도우 주소

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
