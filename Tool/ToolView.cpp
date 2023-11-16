
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "TextureMgr.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView() : m_pTerrain(nullptr), m_iDrawID_Ground(0), m_iDrawID_Wall(0), m_iDrawID_Drcorate(0),
m_bGround(false), m_bWall(false), m_bDecorate(false)
{


}

CToolView::~CToolView()
{
}

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// SetScrollSizes : ��ũ�� �� ����� �����ϴ� scrollview Ŭ������ ��� �Լ�
	// MM_TEXT : �ȼ� ������ ��ũ�� ���� ũ�⸦ ����
	// ��ũ�� ����, ���� ������
	// CSize : ����� ����ϴ� Ŭ����

	SetScrollSizes(MM_TEXT, CSize(TILEX * TILECX, TILEY * TILECY / 2));




	// AfxGetMainWnd : ������ ���� ������ ������ ��ȯ�ϴ� ���� �Լ�
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWnd{};

	// GetWindowRect : ���� �������� RECT ������ ������ �Լ�
	pMainFrm->GetWindowRect(&rcWnd);

	// SetRect : ������ �Ű� ������� rect�� �����ϴ� �Լ�
	// ���� 0,0 �������� �ٽ� ��Ʈ ������ �������ϰ� ����

	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT	rcMainView{};

	// GetClientRect : ���� view â�� rect ������ ������ �Լ�
	GetClientRect(&rcMainView);

	float	fRowFrm = float(rcWnd.right - rcMainView.right);
	float	fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	//SetWindowPos : �Ű� ������� ���Ӱ� ������ ��ġ�� ũ�⸦ �����ϴ� �Լ�
	//SetWindowPos(��ġ�� �������� Z������ ���� ������, X��ǥ, Y��ǥ, ���� ũ��,���� ũ��, ��ġ�� �������� ũ�� �� ��ġ ���� �ɼ�)
	
	//������ �Ű� ������ NULL�� ��� : ���� ������ ���� ����
	//������ �Ű� ������ SWP_NOZORDER�� ��� : ���� ������ �����ϰ���

	pMainFrm->SetWindowPos(nullptr, 0, 0, int(WINCX + fRowFrm), int(WINCY + fColFrm), SWP_NOZORDER);


	g_hWnd = m_hWnd;

	if (FAILED(CDevice::Get_Instance()->Init_Device()))
		return;

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Cube.png", TEX_SINGLE, L"Cube")))
	{
		AfxMessageBox(L"Cube Img Insert Failed");
		return;
	}

	m_pTerrain = new CTerrain;
	m_pTerrain->Initialize();

	m_pTerrain->Set_MainView(this);

}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnLButtonDown(nFlags, point);

	if (m_bGround)
	{
		m_pTerrain->Picking_Grass(D3DXVECTOR3((float)point.x + GetScrollPos(0),
			(float)point.y + GetScrollPos(1),
			0.f), m_iDrawID_Ground);
	}
	else if (m_bWall)
	{
		m_pTerrain->Add_Wall(D3DXVECTOR3((float)point.x + GetScrollPos(0),
			(float)point.y + GetScrollPos(1),
			0.f), m_iDrawID_Wall);
	}
	else if (m_bDecorate)
	{


	}


	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

	pMiniView->Invalidate(FALSE);

	// Invalidate : ȣ�� �� �����쿡 WM_PAINT��  WM_ERASEBKGND �޼����� �߻� ��Ŵ
	// WM_ERASEBKGND : ����� ������ �޼���

	// �Ű� ������ FALSE�� ��� : WM_PAINT �޼����� �߻�
	// �Ű� ������ TRUE�� ��� : WM_PAINT�� WM_ERASEBKGND �޼��� �� �� �߻�

	Invalidate(FALSE);
}

void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CScrollView::OnRButtonDown(nFlags, point);


	if (m_bWall)
	{
		m_pTerrain->Delete_Wall(D3DXVECTOR3((float)point.x + GetScrollPos(0),
			(float)point.y + GetScrollPos(1),
			0.f));
	}
	else if (m_bDecorate)
	{

	}

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));
	pMiniView->Invalidate(FALSE);


	Invalidate(FALSE);
}


void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnMouseMove(nFlags, point);

	/*if (m_bGround && GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_pTerrain->Picking_Grass(D3DXVECTOR3((float)point.x + GetScrollPos(0),
			(float)point.y + GetScrollPos(1),
			0.f),
			m_iDrawID_Ground);
	}
	else if (m_bWall && GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_pTerrain->Add_Wall(D3DXVECTOR3((float)point.x + GetScrollPos(0),
			(float)point.y + GetScrollPos(1),
			0.f), m_iDrawID_Wall);
	}*/





	Invalidate(FALSE);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

	pMiniView->Invalidate(FALSE);
	
}

void CToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.


/// ���⼭ ���� ����


	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	

#pragma region ����
	// Draw : ��ũ�� ������ 2D ��������Ʈ �̹����� ���
	/*CDevice::Get_Instance()->Get_Sprite()->Draw(m_pSingle->Get_Texture()->pTexture, // �ؽ�ó �İ�ü
	nullptr, // ����� �̹��� ������ ���� RECT ����ü ������, NULL�� ��� �̹����� 0, 0�������� ���
	nullptr, // ����� �̹����� �߽����� VECTOR3 ����ü ������, NULL�� ��� 0, 0�� �߽� ��ǥ
	nullptr, // �̹����� ����� ��ġ, VECTOR3 ����ü ������, NULL�� ��� ��ũ�� �� 0, 0 ��ǥ�� ���
	D3DCOLOR_ARGB(255, 255, 255, 255));	// ����� ���� �̹����� �ռ��� ���� ��, 0xffffffff�� �Ѱ��� ��� ���� ������ ����*/

	//float	fCenterX = m_pSingle->Get_Texture()->tImgInfo.Width / 2.f;
	//float	fCenterY = m_pSingle->Get_Texture()->tImgInfo.Height / 2.f;
#pragma endregion ����	

	CDevice::Get_Instance()->Render_Begin();
	
	m_pTerrain->Render();

	CDevice::Get_Instance()->Render_End();
}

#pragma region �Ƚ�

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}


// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


#pragma endregion �Ƚ�


void CToolView::OnDestroy()
{
	CScrollView::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	
	Safe_Delete(m_pTerrain);

	CTextureMgr::Destroy_Instance();
	CDevice::Destroy_Instance();

}



