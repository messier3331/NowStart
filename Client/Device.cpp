#include "stdafx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice() : m_pSDK(nullptr), m_pDevice(nullptr), m_pSprite(nullptr), m_pFont(nullptr)
{
}

CDevice::~CDevice()
{
	Release();
}

HRESULT CDevice::Init_Device()
{
	// 1. ��ü ��ü(��ġ�� ������ ��ü : LPDIRECT3D9) ����
	
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	
	// 2. ��ġ�� ���� ���� ����

	D3DCAPS9			DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps      : ��ġ�� ���� ������ ������ �Լ�
	// D3DADAPTER_DEFAULT : ������ �������� �⺻ �׷��� ī�带 �ǹ�

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		MSG_BOX(L"GetDeviceCaps");
		return E_FAIL;
	}

	DWORD	vp(0);	// vertex processing

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;

	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	
	// 3. ��ġ�� ������ ��ü(LPDIRECT3DDEVICE9) ����

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	Set_Parameters(d3dpp);

	// CreateDevice : �׷��� ��ġ�� ������ ����̽� ��ü�� ����

	/*CreateDevice(� �׷��� ī�带 ������ ���ΰ�, 
					� ������ ��ġ�� ������ ���ΰ�,
					��ġ�� ����� ������,
					���� ���
					��� ȯ��
					��� ������ ������ �������� �ּ�)*/

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									g_hWnd,
									vp,
									&d3dpp,
									&m_pDevice)))
	{
		MSG_BOX(L"CreateDevice Failed");
		return E_FAIL;
	}

	// ��������Ʈ ����
	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		MSG_BOX(L"D3DXCreateSprite Failed");
		return E_FAIL;
	}

	// ��Ʈ ����

	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 20;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"�ü�");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		MSG_BOX(L"D3DXCreateFontIndirect Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::Render_Begin()
{
	m_pDevice->Clear(0,			// ��Ʈ �迭�� ���� ����
					nullptr,	// ��Ʈ �迭�� �̸�(null�� ��� ȭ�� ��ü ������ ����)
					D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // ���ٽ�, ���̹���, �ĸ� ���� ��� ����ڴٴ� �ǹ�
					D3DCOLOR_ARGB(255, 0, 0, 255),	// �ĸ� ���۸� ä�� ����
					1.f,		// ���� ���� �ʱ�ȭ ��
					0);			// ���ٽ� ���� �ʱ�ȭ ��

	m_pDevice->BeginScene();
	// 2D �̹����� �׸� �� �ֵ��� �׸��� ��ġ ��ü�� �غ�, ���� �ɼ��� �����׽�Ʈ�� ��ȿ�� ���¿��� ���ĺ����� ����ϰڴٴ� �ɼ�
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CDevice::Render_End(HWND hWnd)
{
	m_pSprite->End();

	m_pDevice->EndScene();

	// �ĸ� ���۰� ���� ���۷� ��ȯ�Ǿ� ȭ�鿡 ����Ѵ�.
	// 1~2. ���� ü���� D3DSWAPEFFECT_COPY�� �ۼ����� ���� �� NULL
	// 3. ��� ��� ������ �ڵ�, NULL�� ��� D3DPRESENT_PARAMETERS ����ü�� ������ �ڵ��� ��� 
	// 4. ���� ü���� D3DSWAPEFFECT_COPY�� �ۼ����� ���� �� NULL
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
}

void CDevice::Release()
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);

	Safe_Release(m_pDevice);
	Safe_Release(m_pSDK);
}

void CDevice::Set_Parameters(D3DPRESENT_PARAMETERS & d3dpp)
{
	d3dpp.BackBufferWidth  = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount  = 1;
	
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;	

	// D3DSWAPEFFECT_DISCARD : ���� ü�� ���
	// D3DSWAPEFFECT_FLIP : ���� �ϳ��� �������鼭 ����ϴ� ���
	// D3DSWAPEFFECT_COPY : ���� ���۸��� ������ ���� ���

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	
	d3dpp.hDeviceWindow = g_hWnd;

	d3dpp.Windowed = TRUE;		// â ���� �������� ��üȭ�� ���� �������� ����(TRUE �� ���, â ���)

	// DX�� �ڵ����̷� ����, ���ٽ� ���۸� ����� ������ ���ΰ�
	d3dpp.EnableAutoDepthStencil = TRUE;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// ��üȭ�� �ÿ��� ���Ǵ� ��� ������
	
	// ��ü ȭ�� �� ���� ����� ������� ���� ���� ����
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	// ����� ������� ���� �ÿ��� ���� ����

	// D3DPRESENT_INTERVAL_IMMEDIATE : ��� �ÿ�
	// D3DPRESENT_INTERVAL_DEFAULT : ����� ������� ���� ��

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
