#include "stdafx.h"
#include "SingleTexture.h"
#include "Device.h"

CSingleTexture::CSingleTexture()
{
}

CSingleTexture::~CSingleTexture()
{
	Release();
}

HRESULT CSingleTexture::Insert_Texture(const TCHAR * pFilePath,
									const TCHAR * pStateKey, 
									const int & iCount)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	// D3DXGetImageInfoFromFile : ������ �̹��� ���Ͽ� ���� ������ ���� ����ü�� ����ϴ� �Լ�

	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		Safe_Delete(m_pTexInfo);

		MSG_BOX(pFilePath);
		return E_FAIL;
	}

	// D3DXCreateTextureFromFileEx : �ؽ�ó �İ�ü�� �����ϴ� �Լ�

	// D3DPOOL_DEFAULT : ���� ������ �޸�, ���� �ӵ��� ��������, ��ġ�� �սǵǾ��� �� �����ϱⰡ ��ٷӴ�.
	// D3DPOOL_MANAGED : dx�� �ڿ� ����, �׷��� �޸𸮸� ��������� ���� �޸𸮿� ���, D3DPOOL_DEFAULT���� �������� �������̴�.
	// D3DPOOL_SYSTEMMEM : ���� �޸𸮸� ����Ͽ� ����
	// D3DPOOL_SCRATCH : ���� �޸𸮸� ��������� dx ��ġ�� ���� �� �� ����
		
	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(), 
											pFilePath,	// ������ �̸� �� ���
											m_pTexInfo->tImgInfo.Width,	    // �ؽ�ó ���� ������
											m_pTexInfo->tImgInfo.Height,    // �ؽ�ó ���� ������
											m_pTexInfo->tImgInfo.MipLevels, // �ؽ�ó �� ���� ����
											0,	// Ư���� ��찡 �ƴ� �̻� 0(�Ϲ� �ؽ�ó �ɼ�) 
											m_pTexInfo->tImgInfo.Format, // �ؽ�ó �ȼ� ���� ����
											D3DPOOL_MANAGED, // dx �ڿ����� � �޸𸮿� ������ ���ΰ�
											D3DX_DEFAULT,	// �̹��� ���͸� ��� ����
											D3DX_DEFAULT,	// �Ӹ��� �̿��� Ȯ�� ��� �� ���͸� ����
											0,	// ������ ����, 0���� ������ ��� �÷�Ű ��ȿȭ
											nullptr,  // �̹��� ���� ���(������ ����ü ���� �޾ƿͼ� ���� ����)
											nullptr, // �ȷ�Ʈ ����(�Ⱦ���)
											&(m_pTexInfo->pTexture)))) // �� ��ü
	{
		Safe_Delete(m_pTexInfo);
		MSG_BOX(L"Single Texture Load Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CSingleTexture::Release()
{
	Safe_Release(m_pTexInfo->pTexture);
	Safe_Delete(m_pTexInfo);
}
