#pragma once

#include "Incude.h"

class CToolView;
class CTerrain
{
public:
	CTerrain();
	~CTerrain();

public:
	void		Initialize(void);
	void		Update();
	void		Render();
	void		Mini_Render();
	void		Release();

public:

	vector<TILE*>&			Get_VecTile() { return m_vecTile; }
	vector<TILE*>&			Get_VecWall() { return m_vecWall; }


	void		Tile_Change(const D3DXVECTOR3& vPos, const int& iDrawID);
	int			Get_TileIndex(const D3DXVECTOR3& vPos);

	void		Picking_Grass(const D3DXVECTOR3& vPos, const int& Draw);
	void		Add_Wall(const D3DXVECTOR3& vPos, const int& Draw);
	void		Delete_Wall(const D3DXVECTOR3& vPos);

	void		SaveUndo();
	void		Undo();
	void		Redo();

	void		Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }

private:
	bool		Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool		Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);
	void		DeleteStack(stack<vector<TILE*>> S);

private:
	vector<TILE*>		m_vecTile;
	vector<TILE*>		m_vecWall;
	vector<TILE*>		m_vecDeco;

	stack<vector<TILE*>> m_sUndoTile;	// ���ư��� ( Ÿ�ϵ��� ��� ���� )
	stack<vector<TILE*>> m_sUndoWall;	// ���ư��� ( ������ ��� ���� )
	stack<vector<TILE*>> m_sUndoDeco;	// ���ư��� ( ���ڵ��� ��� ���� )

	stack<vector<TILE*>> m_sRedoTile;	// �ٽ��ϱ� ( Ÿ�ϵ��� ��� ���� )
	stack<vector<TILE*>> m_sRedoWall;	// �ٽ��ϱ� ( ������ ��� ���� )
	stack<vector<TILE*>> m_sRedoDeco;	// �ٽ��ϱ� ( ���ڵ��� ��� ���� )

	CToolView*			m_pMainView;
};

