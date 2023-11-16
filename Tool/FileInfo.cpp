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

	// PathRelativePathTo : szCurDirectory에서 strFullPath로 가는 상대 경로를 구해서 szRelativePath에 저장해 줌, 단, 같은 드라이브 내에서만 가능

	PathRelativePathTo(szRelativePath, 
						szCurDirectory, 
						FILE_ATTRIBUTE_DIRECTORY, 
						strFullPath.GetString(), 
						FILE_ATTRIBUTE_DIRECTORY);
	
	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring & wstrPath, list<IMGPATH*>& rPathList)
{
	// CFileFind : mfc에서 제공하는 파일 및 경로 제어 관련 클래스
	CFileFind		Find;

	wstring		wstrFilePath = wstrPath + L"\\*.*";
	//wstrFilePath = L"D:\\유준환\\141기\\Frame141\\Texture\\Stage\\*.*"

	// FindFile : 매개 변수에 해당하는 경로에 파일이 있는지 유무를 판단하는 함수
	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		// FindNextFile : 동일 경로 안에서 다음 파일들이 있는지 유무를 판단
		// 더이상 찾을게 없거나 마지막 대상을 찾았을 경우 0을 리턴
		bContinue = Find.FindNextFile();

		// 찾은 파일이 . 또는 ..인 경우 탐색을 건너뜀
		if(Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathList);
		}

		else
		{
			// 찾은 파일이 숨김 파일과 같은 시스템 파일이라면 건너뜀
			if(Find.IsSystem())
				continue;

			IMGPATH*	pImgPath = new IMGPATH;
			TCHAR		szPathBuf[MAX_PATH] = L"";

			lstrcpy(szPathBuf, Find.GetFilePath().GetString());
			//szPathBuf = 0x00fdd52c L"D:\\유준환\\141기\\Frame141\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack0.png"

			PathRemoveFileSpec(szPathBuf);
			//szPathBuf = 0x00fdd52c L"D:\\유준환\\141기\\Frame141\\Texture\\Stage\\Effect\\BossMultiAttack"

			pImgPath->iCount = DirFileCount(szPathBuf);

			/////////////////////////////////////////////////

			// GetFileTitle : 파일 이름만 얻어오는 함수(확장자명 제거)
			wstring	wstrTextureName = Find.GetFileTitle().GetString();
			// wstrTextureName = L"BossMultiAttack0"

			// substr(시작, 끝) : 시작에서 끝에 해당하는 문자열을 얻어옴
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";
			// wstrTextureName = L"BossMultiAttack%d.png"

			TCHAR	szBuf[MAX_PATH] = L"";

			lstrcpy(szBuf, Find.GetFilePath().GetString());
		//	szBuf = 0x00fdd2f8 L"D:\\유준환\\141기\\Frame141\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack0.png"
			PathRemoveFileSpec(szBuf);
			// szBuf = 0x00fdd2f8 L"D:\\유준환\\141기\\Frame141\\Texture\\Stage\\Effect\\BossMultiAttack"

			// PathCombine(결과, 문자열1, 문자열2) : 문자열 1과 문자열 2를 이어붙여 완성된 경로로 결과에 저장
			// 문자열1과 문자열2 사이에 '\\'가 자동으로 삽입된채 결과에 저장
			PathCombine(szBuf, szBuf, wstrTextureName.c_str());
			// szBuf = 0x00fdd2f8 L"D:\\유준환\\141기\\Frame141\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"
			
			// 완성된 경로를 상대경로로 변환한 뒤, 구조체 멤버 변수에 저장
			pImgPath->wstrPath = ConvertRelativePath(szBuf);
			// pImgPath->wstrPath = L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

			PathRemoveFileSpec(szBuf);
			// L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\

			// PathFindFileName : 파일이름을 찾거나 가장 마지막 폴더 이름을 찾아내는 함수
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
