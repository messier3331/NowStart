#pragma once

#include "Incude.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice)

public:
	CDevice();
	~CDevice();

public:
	LPDIRECT3DDEVICE9		Get_Device() { return m_pDevice; }
	LPD3DXSPRITE			Get_Sprite() { return m_pSprite; }
	LPD3DXFONT				Get_Font()	 { return m_pFont; }


public:
	HRESULT		Init_Device();

	void		Render_Begin();
	void		Render_End(HWND hWnd = nullptr);

	void		Release();

private:
	void		Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp);
	
	// COM ��ü : ���� ȯ�濡 ��� ���� ȣȯ�� ������ ������Ʈ ����� ���� ms�� ������ ǥ��ȭ�� ���
	//			 �������� Ư�� �ý��ۿ� ���Ͽ� �����ϰ� �۵��ϵ��� COM ��ü�� ���� ��� �Ծ��� ������
private:
	LPDIRECT3D9				m_pSDK;
	LPDIRECT3DDEVICE9		m_pDevice;

	// dx �󿡼� 2D �̹����� ����ϴ� ��ü
	LPD3DXSPRITE			m_pSprite;
	// dx �󿡼� ��Ʈ ����� ����ϴ� ��ü
	LPD3DXFONT				m_pFont;
};

