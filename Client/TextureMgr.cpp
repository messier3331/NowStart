#include "stdafx.h"
#include "TextureMgr.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEXINFO * CTextureMgr::Get_Texture(const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCount)
{
	auto iter = find_if(m_MapTex.begin(), m_MapTex.end(), 
		[&](auto& MyPair)->bool 
		{
			if (pObjKey == MyPair.first)
				return true;

			return false;
		});

	if (iter == m_MapTex.end())
		return nullptr;

	return iter->second->Get_Texture(pStateKey, iCount);
}

HRESULT CTextureMgr::Read_ImgPath(const wstring & wstrPath)
{
	wifstream		fin;

	fin.open(wstrPath, ios::in);

	if (!fin.fail())
	{
		TCHAR	szObjKey[MAX_STR] = L"";
		TCHAR	szStateKey[MAX_STR] = L"";
		TCHAR	szCount[MAX_STR] = L"";

		TCHAR	szPath[MAX_PATH] = L"";

		while (true)
		{
			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			int	iCount = _ttoi(szCount);

			if (FAILED(Insert_Texture(szPath, TEX_MULTI, szObjKey, szStateKey, iCount)))
			{
				MSG_BOX(L"�̹��� �޸��� Insert ����");
				return E_FAIL;
			}

		}

		fin.close();
	}

	return S_OK;
}

HRESULT CTextureMgr::Insert_Texture(const TCHAR * pFilePath, TEXTYPE eType, const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCount)
{
	auto iter = find_if(m_MapTex.begin(), m_MapTex.end(),
		[&](auto& MyPair)->bool
	{
		if (pObjKey == MyPair.first)
			return true;

		return false;
	});

	if (iter == m_MapTex.end())
	{
		CTexture*		pTexture = nullptr;

		switch (eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->Insert_Texture(pFilePath, pStateKey, iCount)))
		{
			MSG_BOX(pFilePath);
			return E_FAIL;
		}

		m_MapTex.insert({ pObjKey, pTexture });
	}


	return S_OK;
}

void CTextureMgr::Release()
{
	for_each(m_MapTex.begin(), m_MapTex.end(), [](auto& MyPair)
	{
		if (MyPair.second)
		{
			delete MyPair.second;
			MyPair.second = nullptr;
		}
	});

	m_MapTex.clear();
}
