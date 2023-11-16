#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	TCHAR	szRelativePath[MAX_PATH] = L"";
	TCHAR	szCurDirectory[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurDirectory);

	// PathRelativePathTo : szCurDirectory���� strFullPath�� ���� ��� ��θ� ���ؼ� szRelativePath�� ������ ��, ��, ���� ����̺� �������� ����

	PathRelativePathTo(szRelativePath, 
						szCurDirectory, 
						FILE_ATTRIBUTE_DIRECTORY, 
						strFullPath.GetString(), 
						FILE_ATTRIBUTE_DIRECTORY);
	
	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring & wstrPath, list<IMGPATH*>& rPathList)
{
	// CFileFind : mfc���� �����ϴ� ���� �� ��� ���� ���� Ŭ����
	CFileFind		Find;

	wstring		wstrFilePath = wstrPath + L"\\*.*";
	//wstrFilePath = L"D:\\����ȯ\\141��\\Frame141\\Texture\\Stage\\*.*"

	// FindFile : �Ű� ������ �ش��ϴ� ��ο� ������ �ִ��� ������ �Ǵ��ϴ� �Լ�
	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		// FindNextFile : ���� ��� �ȿ��� ���� ���ϵ��� �ִ��� ������ �Ǵ�
		// ���̻� ã���� ���ų� ������ ����� ã���� ��� 0�� ����
		bContinue = Find.FindNextFile();

		// ã�� ������ . �Ǵ� ..�� ��� Ž���� �ǳʶ�
		if(Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathList);
		}

		else
		{
			// ã�� ������ ���� ���ϰ� ���� �ý��� �����̶�� �ǳʶ�
			if(Find.IsSystem())
				continue;

			IMGPATH*	pImgPath = new IMGPATH;
			TCHAR		szPathBuf[MAX_PATH] = L"";

			lstrcpy(szPathBuf, Find.GetFilePath().GetString());
			//szPathBuf = 0x00fdd52c L"D:\\����ȯ\\141��\\Frame141\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack0.png"

			PathRemoveFileSpec(szPathBuf);
			//szPathBuf = 0x00fdd52c L"D:\\����ȯ\\141��\\Frame141\\Texture\\Stage\\Effect\\BossMultiAttack"

			pImgPath->iCount = DirFileCount(szPathBuf);

			/////////////////////////////////////////////////

			// GetFileTitle : ���� �̸��� ������ �Լ�(Ȯ���ڸ� ����)
			wstring	wstrTextureName = Find.GetFileTitle().GetString();
			// wstrTextureName = L"BossMultiAttack0"

			// substr(����, ��) : ���ۿ��� ���� �ش��ϴ� ���ڿ��� ����
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";
			// wstrTextureName = L"BossMultiAttack%d.png"

			TCHAR	szBuf[MAX_PATH] = L"";

			lstrcpy(szBuf, Find.GetFilePath().GetString());
		//	szBuf = 0x00fdd2f8 L"D:\\����ȯ\\141��\\Frame141\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack0.png"
			PathRemoveFileSpec(szBuf);
			// szBuf = 0x00fdd2f8 L"D:\\����ȯ\\141��\\Frame141\\Texture\\Stage\\Effect\\BossMultiAttack"

			// PathCombine(���, ���ڿ�1, ���ڿ�2) : ���ڿ� 1�� ���ڿ� 2�� �̾�ٿ� �ϼ��� ��η� ����� ����
			// ���ڿ�1�� ���ڿ�2 ���̿� '\\'�� �ڵ����� ���Ե�ä ����� ����
			PathCombine(szBuf, szBuf, wstrTextureName.c_str());
			// szBuf = 0x00fdd2f8 L"D:\\����ȯ\\141��\\Frame141\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"
			
			// �ϼ��� ��θ� ����η� ��ȯ�� ��, ����ü ��� ������ ����
			pImgPath->wstrPath = ConvertRelativePath(szBuf);
			// pImgPath->wstrPath = L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

			PathRemoveFileSpec(szBuf);
			// L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\

			// PathFindFileName : �����̸��� ã�ų� ���� ������ ���� �̸��� ã�Ƴ��� �Լ�
			pImgPath->wstrStateKey = PathFindFileName(szBuf);

			PathRemoveFileSpec(szBuf);			
			pImgPath->wstrObjKey = PathFindFileName(szBuf);

			rPathList.push_back(pImgPath);			
			bContinue = 0;
		}
	}
}

int CFileInfo::DirFileCount(const wstring & wstrPath)
{
	CFileFind		Find;

	wstring		wstrFilePath = wstrPath + L"\\*.*";

	
	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());

	int iFileCnt(0);

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		if (Find.IsSystem())
			continue;

		++iFileCnt;
	}

	return iFileCnt;
}
