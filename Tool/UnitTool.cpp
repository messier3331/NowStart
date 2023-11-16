// UnitTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_strTest(_T(""))
	, m_strCopy(_T(""))
	, m_strName(_T(""))
	, m_iHp(0)
	, m_iAttack(0)
	, m_strFindName(_T(""))
{

}

CUnitTool::~CUnitTool()
{
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strTest);
	DDX_Text(pDX, IDC_EDIT2, m_strCopy);
	DDX_Text(pDX, IDC_EDIT3, m_strName);
	DDX_Text(pDX, IDC_EDIT4, m_iHp);
	DDX_Text(pDX, IDC_EDIT5, m_iAttack);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);
	DDX_Control(pDX, IDC_CHECK1, m_Check[0]);
	DDX_Control(pDX, IDC_CHECK2, m_Check[1]);
	DDX_Control(pDX, IDC_CHECK3, m_Check[2]);
	DDX_Control(pDX, IDC_BUTTON3, m_Bitmap);
	DDX_Text(pDX, IDC_EDIT6, m_strFindName);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CUnitTool::OnAddCharactor)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnListBox)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT6, &CUnitTool::OnSearchData)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnDeleteData)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON6, &CUnitTool::OnLoadData)
END_MESSAGE_MAP()

void CUnitTool::OnBnClickedButton1()
{
	// TRUE : ���̾�α� �ڽ��κ��� �Էµ� ������ ����	
	// FALSE : ������ ����� ������ ���̾�α� �ڽ��� �ݿ�

	UpdateData(TRUE);

	m_strCopy = m_strTest;

	UpdateData(FALSE);
}

void CUnitTool::OnAddCharactor()
{
	UpdateData(TRUE);

	UNITDATA*		pUnit = new UNITDATA;

	pUnit->strName = m_strName;
	pUnit->iHp = m_iHp;
	pUnit->iAttack = m_iAttack;

	for (int i = 0; i < 3; ++i)
	{
		if (m_Radio[i].GetCheck())
		{
			pUnit->byJobIndex = i;
			break;
		}
	}

	pUnit->byItem = 0x00;

	if (m_Check[0].GetCheck())
		pUnit->byItem |= RUBY;

	if (m_Check[1].GetCheck())
		pUnit->byItem |= DIAMOND;

	if (m_Check[2].GetCheck())
		pUnit->byItem |= SAPPHIRE;

	/*if (dynamic_cast<CButton*>(GetDlgItem(IDC_CHECK1))->GetCheck())
		pUnit->byItem |= RUBY;*/


	// AddString : ����Ʈ �ڽ��� �Ű������� �ش��ϴ� ���ڿ��� �߰���
	m_ListBox.AddString(pUnit->strName);

	m_mapUnitData.insert({ pUnit->strName, pUnit });

	UpdateData(FALSE);
}


void CUnitTool::OnListBox()
{
	UpdateData(TRUE);

	CString		strSelectName;

	// GetCurSel : Ŀ���� ���� ���� �ε��� ���� ��ȯ(0������ �ε����� ����)
	int		iIndex = m_ListBox.GetCurSel();

	// GetText : �ش� �ε����� �ִ� ����Ʈ �ڽ� ���ڿ��� ������ �Լ�
	m_ListBox.GetText(iIndex, strSelectName);

	auto	iter = m_mapUnitData.find(strSelectName);

	if (iter == m_mapUnitData.end())
		return;

	m_strName = iter->second->strName;
	m_iHp = iter->second->iHp;
	m_iAttack = iter->second->iAttack;

	for (int i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE);
		m_Check[i].SetCheck(FALSE);
	}

	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);

	if (iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);
	
	UpdateData(FALSE);
}


void CUnitTool::OnDestroy()
{
	CDialog::OnDestroy();

	for_each(m_mapUnitData.begin(), m_mapUnitData.end(), CDeleteMap());
	m_mapUnitData.clear();
}


BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	HBITMAP		hBitmap = (HBITMAP)LoadImage(nullptr, 
		L"../Texture/JusinLogo1.bmp", 
		IMAGE_BITMAP, 
		100, 50, 
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_Bitmap.SetBitmap(hBitmap);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CUnitTool::OnSearchData()
{
	UpdateData(TRUE);

	auto&	iter = m_mapUnitData.find(m_strFindName);

	if (iter == m_mapUnitData.end())
		return;

	//FindString : ù ��° �Ű� ���� �ε������� �� ��° �Ű� ������ �ش��ϴ� ���ڿ� Ž��
	int		iIndex = m_ListBox.FindString(0, m_strFindName);

	if (LB_ERR == iIndex)
		return;
	
	// �ε��� ���� ������
	m_ListBox.SetCurSel(iIndex);

	m_strName = iter->second->strName;
	m_iHp = iter->second->iHp;
	m_iAttack = iter->second->iAttack;

	for (int i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE);
		m_Check[i].SetCheck(FALSE);
	}

	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);

	if (iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);


	UpdateData(FALSE);
}


void CUnitTool::OnDeleteData()
{
	UpdateData(TRUE);

	CString	strFindName = L"";
	
	int	iIndex = m_ListBox.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	m_ListBox.GetText(iIndex, strFindName);

	auto&	iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	Safe_Delete(iter->second);
	m_mapUnitData.erase(strFindName);

	m_ListBox.DeleteString(iIndex);


	UpdateData(FALSE);
}


void CUnitTool::OnSaveData()
{
	CFileDialog		Dlg(FALSE,	// TRUE(����, �ҷ�����), FALSE(�ٸ� �̸����� �����ϱ�) �������
						L"dat",	// �⺻ Ȯ���ڸ�
						L"*.dat", // ��ȭ ���ڿ� ǥ�õ� ���� ���ϸ�
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, //OFN_HIDEREADONLY(�б� ���� üũ�ڽ� ����) | OFN_OVERWRITEPROMPT(�ߺ� ���� ���� �� ���޼��� ���)
						L"Data Files(*.dat) | *.dat||", // ��ȭ ���ڿ� ǥ�õ� ���� ����("�޺� �ڽ��� ��µ� ���ڿ� | ���� ����� ���͸� ���ڿ� ||"
						this);	// �θ� ������ �ּ�

	TCHAR	szPath[MAX_STR] = L"";

	// ���� ������Ʈ ��θ� ������ �Լ�
	GetCurrentDirectory(MAX_STR, szPath);
	// D:\����ȯ\141��\Frame141\Tool

	// ��ü ��ο��� ���� �̸��� �߶��ִ� �Լ�, ���� ��λ� ���� �̸��� ���ٸ� ���� ������ �����̸��� �߶�
	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data");

	// D:\����ȯ\141��\Frame141\Data
	Dlg.m_ofn.lpstrInitialDir = szPath;
	
	// DoModal : ��ȭ ���ڸ� ����, IDOK �Ǵ� IDCANCEL ��ȯ
	if (IDOK == Dlg.DoModal())
	{
		// GetPathName : ���� ���̾�αװ� ���ñ� ��θ� ��ȯ
		CString		str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD		dwByte(0);
		DWORD		dwStrByte(0);

		for (auto& MyPair : m_mapUnitData)
		{
			dwStrByte = sizeof(TCHAR)* (MyPair.first.GetLength() + 1);

			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, MyPair.first.GetString(), dwStrByte, &dwByte, nullptr);

			WriteFile(hFile, &(MyPair.second->byItem), sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &(MyPair.second->byJobIndex), sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &(MyPair.second->iAttack), sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(MyPair.second->iHp), sizeof(int), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
}


void CUnitTool::OnLoadData()
{
	UpdateData(TRUE);

	CFileDialog		Dlg(TRUE,	// TRUE(����, �ҷ�����), FALSE(�ٸ� �̸����� �����ϱ�) �������
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

	// DoModal : ��ȭ ���ڸ� ����, IDOK �Ǵ� IDCANCEL ��ȯ
	if (IDOK == Dlg.DoModal())
	{

		for (auto& MyPair : m_mapUnitData)
			delete MyPair.second;

		m_mapUnitData.clear();

		// ������ ����Ʈ �ڽ��� ����� �ʱ�ȭ
		m_ListBox.ResetContent();

		CString		str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD		dwByte(0);
		DWORD		dwStrByte(0);
		UNITDATA	tData{};

		while (true)
		{
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

			TCHAR*	pName = new TCHAR[dwStrByte];
			
			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			ReadFile(hFile, &(tData.byItem), sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &(tData.byJobIndex), sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iAttack), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iHp), sizeof(int), &dwByte, nullptr);

			if (0 == dwByte)
			{
				delete[]pName;
				break;
			}

			UNITDATA*		pUnit = new UNITDATA;
			pUnit->strName = pName;

			delete[]pName;
			pName = nullptr;

			pUnit->byItem = tData.byItem;
			pUnit->byJobIndex = tData.byJobIndex;
			pUnit->iAttack = tData.iAttack;
			pUnit->iHp = tData.iHp;

			m_mapUnitData.insert({ pUnit->strName, pUnit });

			m_ListBox.AddString(pUnit->strName);
			
		}
		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}
