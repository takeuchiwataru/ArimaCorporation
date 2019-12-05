//=============================================================================
//
// 判定付加メッシュ処理 [ColMesh.cpp]
// Author : Ryo Sugimoto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall突破
#include <stdio.h>				//インクルドファイル
#include <string.h>
#include "ColMesh.h"
#include "RoadPointer.h"
#include "manager.h"
#include "player.h"
#include "DispEffect.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COL_WALL_PLUS		(2.0f)		//壁の判定のプラス値
#define COL_RECOIL			(0.0f)		//壁ヒット時の反動	(1.0 = 100％)		
#define COL_MOVE_SLOWING	(0.5f)		//壁ヒット時の減速　(1.0 = 100％)	

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CCOL_MESH_MANAGER *CCOL_MESH_MANAGER::m_pColMesh[CCOL_MESH_MANAGER::TYPE_MAX];

//==================================================================================================//
//    * 判定付加メッシュの初期化関数 *
//==================================================================================================//
HRESULT CCOL_MESH::Init(void)
{
	m_pos = INIT_VECTOR;
	m_nNumVertical = 0;
	m_nNumWidth = 0;
	m_nNumVtx = 0;
	m_pMesh = NULL;
	m_pVtx = NULL;
	m_bSelect = NULL;
	m_Type = TYPE_NORMAL;
	m_Effect = EFFECT_NORMAL;
	return S_OK;
}
//==================================================================================================//
//    * 判定付加メッシュの生成関数 *
//==================================================================================================//
bool	CCOL_MESH::Create(CCOL_MESH *&pColMesh)
{
	pColMesh = NULL;
	pColMesh = new CCOL_MESH;
	if (pColMesh != NULL) { pColMesh->Init(); return true; }

	return false;
}
//==================================================================================================//
//    * 判定付加メッシュの終了関数 *
//==================================================================================================//
void	CCOL_MESH::Uninit(void)
{
	if (m_pMesh != NULL) { m_pMesh->Uninit(); }
	if (m_pVtx != NULL)
	{
		delete[] m_pVtx;
		delete[] m_bSelect;
		m_pVtx = NULL;
		m_bSelect = NULL;
	}
	delete this;
}
//==================================================================================================//
//    * 判定付加メッシュの更新関数 *
//==================================================================================================//
void	CCOL_MESH::Update(void)
{
	switch (m_Effect)
	{
	case EFFECT_RIVER:	//UV変更

		break;
	}
}
//==================================================================================================//
//    * 判定付加メッシュの描画関数 *
//==================================================================================================//
void	CCOL_MESH::Draw(void)
{
	if (m_pMesh != NULL)
	{//メッシュの描画
	 //if (CManager::GetPlayer(0)->GetpColEdit()->GetMode() != CCOL_EDIT::MODE_GAME)
	 //{
	 //	//if (m_Effect == EFFECT_NORMAL && m_bField) {}
	 //	m_pMesh->Draw();
	 //}
		if (m_Effect != EFFECT_NORMAL && m_Effect != EFFECT_DROP)
		{
			m_pMesh->Draw();
		}
	}
}
//==================================================================================================//
//    * 判定付加メッシュの削除関数 *
//==================================================================================================//
void	CCOL_MESH::Delete(void)
{
	if (m_pMesh != NULL)
	{//メッシュの終了
		m_pMesh->Uninit();
	}
	Uninit();
}
//==================================================================================================//
//    * 判定付加メッシュの仮設定関数 *
//==================================================================================================//
void	CCOL_MESH::Set(bool bField, D3DXVECTOR3 pos)
{
	int nCntVtx = 0;

	m_pos = pos;
	m_bField = bField;
	m_nNumVertical = 1;
	m_nNumWidth = 1;
	m_nNumVtx = (m_nNumVertical + 1) * (m_nNumWidth + 1);

	if (m_pVtx == NULL)
	{
		m_pVtx = new D3DXVECTOR3[m_nNumVtx];
		m_bSelect = new bool[m_nNumVtx];
	}

	for (int nCntY = 0; nCntY < m_nNumVertical + 1; nCntY++)
	{
		for (int nCntX = 0; nCntX < m_nNumWidth + 1; nCntX++)
		{
			if (m_bField) { m_pVtx[nCntVtx] = D3DXVECTOR3((float)-nCntX, 0.0f, (float)nCntY) * INIT_LENGTH; }
			else
			{
				m_pVtx[nCntVtx] = D3DXVECTOR3(0.0f, (float)nCntY, (float)nCntX) * INIT_LENGTH;
				m_pVtx[nCntVtx].y -= 5.0f;
			}
			m_bSelect[nCntVtx] = false;
			nCntVtx++;
		}
	}
	SetMesh();
}
//==================================================================================================//
//    * 判定付加メッシュの見た目生成関数 *
//==================================================================================================//
void	CCOL_MESH::SetMesh(void)
{
	if (m_pMesh != NULL)
	{
		if (m_pMesh->GetnNumVertex() != m_nNumVtx)
		{//頂点数が違う場合作り直し
			m_pMesh->Uninit();
			m_pMesh = NULL;
		}
	}

	if (m_pMesh == NULL)
	{//生成されていないなら生成
		if (CNorMesh::Create(m_pMesh))
		{
			m_pMesh->Set((m_bField ? CNorMesh::TYPE_FIELD : CNorMesh::TYPE_WALL), m_pos, m_nNumVertical, m_nNumWidth, true);
		}
	}

	LPDIRECT3DVERTEXBUFFER9 &pVtxBuff = m_pMesh->GetpVtxBuff();
	int						nCntVtx = 0;
	VERTEX_3D				*pVtx;									//頂点情報へのポインタ

	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//頂点バッファをロックし頂点データへのポインタを取得
	for (int nCntY = 0; nCntY < m_nNumVertical + 1; nCntY++)
	{//頂点を代入する
		for (int nCntX = 0; nCntX < m_nNumWidth + 1; nCntX++)
		{
			pVtx[nCntVtx].pos = m_pVtx[nCntVtx];
			nCntVtx++;
		}
	}
	pVtxBuff->Unlock();	//頂点バッファのアンロックする

	m_pMesh->GetPos() = m_pos;
	m_pMesh->SetMtx();
	//m_pMesh->BindTexture();
}
//==================================================================================================//
//    * 判定付加メッシュの追加関数 *
//==================================================================================================//
int	CCOL_MESH::AddWidth(int nValue)
{
	if (m_nNumWidth + nValue < 1) { return 0; }

	//新作生成
	int		nNumVtx = m_nNumVtx;
	int		nNumWidth = m_nNumWidth;
	int		nAddNumber;
	D3DXVECTOR3 *pVtx;
	bool		*pbData;
	pVtx = new D3DXVECTOR3[nNumVtx];
	pbData = new bool[nNumVtx];

	//引継ぎ
	for (int nCount = 0; nCount < nNumVtx; nCount++)
	{
		pVtx[nCount] = m_pVtx[nCount];
	}
	delete[] m_pVtx;
	delete[] m_bSelect;

	//追加分生成
	m_nNumWidth += nValue;
	m_nNumVtx = (m_nNumVertical + 1) * (m_nNumWidth + 1);
	m_pVtx = new D3DXVECTOR3[m_nNumVtx];
	m_bSelect = new bool[m_nNumVtx];


	for (int nNumZ = 0; nNumZ < m_nNumVertical + 1; nNumZ++)
	{
		for (int nNumX = 0; nNumX < m_nNumWidth + 1; nNumX++)
		{
			if (nNumX > nNumWidth)
			{//新しいデータ
				m_pVtx[nNumZ * (m_nNumWidth + 1) + nNumX] = m_pVtx[nNumZ * (m_nNumWidth + 1) + (nNumX - 1)] + m_pVtx[nNumZ * (m_nNumWidth + 1) + (nNumX - 1)] - m_pVtx[nNumZ * (m_nNumWidth + 1) + (nNumX - 2)];
				m_bSelect[nNumZ * (m_nNumWidth + 1) + nNumX] = true;
			}
			else
			{//前回のデータ
				m_pVtx[nNumZ * (m_nNumWidth + 1) + nNumX] = pVtx[nNumZ * (nNumWidth + 1) + nNumX];
				m_bSelect[nNumZ * (m_nNumWidth + 1) + nNumX] = false;
			}
		}
	}
	//for (int nCount = 0; nCount < nNumVtx; nCount++)
	//{
	//	m_pVtx[nCount] = pVtx[nCount];
	//}
	delete[] pVtx;
	SetMesh();
	ResetSelect();
	for (int nNumZ = 0; nNumZ < m_nNumVertical + 1; nNumZ++)
	{
		for (int nNumX = 0; nNumX < m_nNumWidth + 1; nNumX++)
		{
			if (nNumX >= m_nNumWidth)
			{
				nAddNumber = nNumX;
				Select(nNumZ * (m_nNumWidth + 1) + nNumX);
			}
		}
	}
	return nAddNumber;
}
//==================================================================================================//
//    * 判定付加メッシュの追加関数 *
//==================================================================================================//
int	CCOL_MESH::AddVertical(int nValue)
{
	if (m_nNumVertical + nValue < 1) { return 0; }

	//新作生成
	int		nNumVtx = m_nNumVtx;
	int		nNumVertical = m_nNumVertical;
	int		nAddNumber;
	D3DXVECTOR3 *pVtx;
	bool		*pbData;
	pVtx = new D3DXVECTOR3[nNumVtx];
	pbData = new bool[nNumVtx];

	//引継ぎ
	for (int nCount = 0; nCount < nNumVtx; nCount++)
	{
		pVtx[nCount] = m_pVtx[nCount];
	}
	delete[] m_pVtx;
	delete[] m_bSelect;

	//追加分生成
	m_nNumVertical += nValue;
	m_nNumVtx = (m_nNumVertical + 1) * (m_nNumWidth + 1);
	m_pVtx = new D3DXVECTOR3[m_nNumVtx];
	m_bSelect = new bool[m_nNumVtx];


	for (int nNumZ = 0; nNumZ < m_nNumVertical + 1; nNumZ++)
	{
		for (int nNumX = 0; nNumX < m_nNumWidth + 1; nNumX++)
		{
			if (nNumZ > nNumVertical)
			{//今回のデータ
				m_pVtx[nNumZ * (m_nNumWidth + 1) + nNumX] = m_pVtx[(nNumZ - 1) * (m_nNumWidth + 1) + nNumX] + m_pVtx[(nNumZ - 1) * (m_nNumWidth + 1) + nNumX] - m_pVtx[(nNumZ - 2) * (m_nNumWidth + 1) + nNumX];
				m_bSelect[nNumZ * (m_nNumWidth + 1) + nNumX] = true;
			}
			else
			{//前回のデータ
				m_pVtx[nNumZ * (m_nNumWidth + 1) + nNumX] = pVtx[nNumZ * (m_nNumWidth + 1) + nNumX];
				m_bSelect[nNumZ * (m_nNumWidth + 1) + nNumX] = false;
			}
		}
	}
	delete[] pVtx;
	SetMesh();
	ResetSelect();
	for (int nNumZ = 0; nNumZ < m_nNumVertical + 1; nNumZ++)
	{
		for (int nNumX = 0; nNumX < m_nNumWidth + 1; nNumX++)
		{
			if (nNumZ >= m_nNumVertical)
			{
				nAddNumber = nNumZ;
				Select(nNumZ * (m_nNumWidth + 1) + nNumX);
			}
		}
	}
	return nAddNumber;
}
//==================================================================================================//
//    * 判定付加メッシュの縦行削除関数 *
//==================================================================================================//
int		CCOL_MESH::DeleteLineV(int nWidth)
{
	if (m_nNumWidth <= 1) { return 0; }

	//新作生成
	int		nNumVtx = m_nNumVtx;
	int		nNumWidth = m_nNumWidth;
	int		nAddNumber = 0;
	D3DXVECTOR3 *pVtx;
	bool		*pbData;
	pVtx = new D3DXVECTOR3[nNumVtx];
	pbData = new bool[nNumVtx];

	//引継ぎ
	for (int nCount = 0; nCount < nNumVtx; nCount++)
	{
		pVtx[nCount] = m_pVtx[nCount];
	}
	delete[] m_pVtx;
	delete[] m_bSelect;

	//追加分生成
	m_nNumWidth--;
	m_nNumVtx = (m_nNumVertical + 1) * (m_nNumWidth + 1);
	m_pVtx = new D3DXVECTOR3[m_nNumVtx];
	m_bSelect = new bool[m_nNumVtx];


	for (int nNumZ = 0; nNumZ < m_nNumVertical + 1; nNumZ++)
	{
		for (int nNumX = 0; nNumX < m_nNumWidth + 1; nNumX++)
		{
			nAddNumber = 0;
			if (nNumX == nWidth) { nAddNumber = 1; }
			m_pVtx[nNumZ * (m_nNumWidth + 1) + nNumX] = pVtx[nNumZ * (nNumWidth + 1) + (nNumX + nAddNumber)];
		}
	}
	delete[] pVtx;
	SetMesh();
	ResetSelect();

	for (int nNumZ = 0; nNumZ < m_nNumVertical + 1; nNumZ++)
	{
		for (int nNumX = 0; nNumX < m_nNumWidth + 1; nNumX++)
		{
			if (nNumZ == nWidth)
			{
				nAddNumber = nNumZ;
				Select(nNumZ * (m_nNumWidth + 1) + nNumX);
			}
		}
	}
	return nAddNumber;
}
//==================================================================================================//
//    * 判定付加メッシュの横行削除関数 *
//==================================================================================================//
int		CCOL_MESH::DeleteLineW(int nVertical)
{
	if (m_nNumVertical <= 1) { return 0; }

	//新作生成
	int		nNumVtx = m_nNumVtx;
	int		nAddNumber = 0;
	D3DXVECTOR3 *pVtx;
	bool		*pbData;
	pVtx = new D3DXVECTOR3[nNumVtx];
	pbData = new bool[nNumVtx];

	//引継ぎ
	for (int nCount = 0; nCount < nNumVtx; nCount++)
	{
		pVtx[nCount] = m_pVtx[nCount];
	}
	delete[] m_pVtx;
	delete[] m_bSelect;

	//追加分生成
	m_nNumVertical--;
	m_nNumVtx = (m_nNumVertical + 1) * (m_nNumWidth + 1);
	m_pVtx = new D3DXVECTOR3[m_nNumVtx];
	m_bSelect = new bool[m_nNumVtx];


	for (int nNumZ = 0; nNumZ < m_nNumVertical + 1; nNumZ++)
	{
		for (int nNumX = 0; nNumX < m_nNumWidth + 1; nNumX++)
		{
			nAddNumber = 0;
			if (nNumZ == nVertical) { nAddNumber = 1; }
			m_pVtx[nNumZ * (m_nNumWidth + 1) + nNumX] = pVtx[(nNumZ + nAddNumber) * (m_nNumWidth + 1) + nNumX];
			m_bSelect[nNumZ * (m_nNumWidth + 1) + nNumX] = false;
		}
	}
	delete[] pVtx;
	SetMesh();
	ResetSelect();

	for (int nNumZ = 0; nNumZ < m_nNumVertical + 1; nNumZ++)
	{
		for (int nNumX = 0; nNumX < m_nNumWidth + 1; nNumX++)
		{
			if (nNumZ == nVertical)
			{
				nAddNumber = nNumZ;
				Select(nNumZ * (m_nNumWidth + 1) + nNumX);
			}
		}
	}
	return nAddNumber;
}
//==================================================================================================//
//    * 判定付加メッシュの追加関数 *
//==================================================================================================//
void	CCOL_MESH::SpinRot(float fAddRot, float fScale)
{
	int			nNumSpin = 0;
	float		fRot, fDistance;
	D3DXVECTOR3 pos = INIT_VECTOR;

	//中心点を計算
	for (int nCount = 0; nCount < m_nNumVtx; nCount++)
	{
		if (m_bSelect[nCount])
		{
			pos += m_pVtx[nCount];
			nNumSpin++;
		}
	}
	pos /= (float)nNumSpin;

	//中心に沿って回す
	for (int nCount = 0; nCount < m_nNumVtx; nCount++)
	{
		if (m_bSelect[nCount])
		{
			//距離計算
			fDistance = sqrtf(powf(pos.x - m_pVtx[nCount].x, 2) + powf(pos.z - m_pVtx[nCount].z, 2));

			//角度計算
			fRot = atan2f(m_pVtx[nCount].x - pos.x, m_pVtx[nCount].z - pos.z) + fAddRot;

			//中心から距離分移動
			m_pVtx[nCount] = pos + D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * (fDistance * fScale);
		}
	}
}
//==================================================================================================//
//    * 判定付加メッシュの削除関数 *
//==================================================================================================//
void	CCOL_MESH::Delete(int nNumber)
{

}
//==================================================================================================//
//    * 判定付加メッシュの移動関数 *
//==================================================================================================//
void	CCOL_MESH::Move(D3DXVECTOR3 move)
{
	for (int nCount = 0; nCount < m_nNumVtx; nCount++)
	{
		if (m_bSelect[nCount]) { m_pVtx[nCount] += move; }
	}
	SetMesh();
}
//==================================================================================================//
//    * 判定付加メッシュの移動関数 *
//==================================================================================================//
void	CCOL_MESH::SetWall(D3DXVECTOR3 &pos, D3DXVECTOR3 &Vtx, int &nNumWidth)
{
	D3DXVECTOR3 Pos = pos - m_pos;

	if (nNumWidth > m_nNumWidth)
	{
		AddWidth(1);
		nNumWidth = m_nNumWidth;
	}

	float fPosY = m_pVtx[(m_nNumWidth + 1) + nNumWidth].y - m_pVtx[nNumWidth].y;
	m_pVtx[nNumWidth] = Vtx + D3DXVECTOR3(0.0f, -5.0f, 0.0f) + Pos;
	m_pVtx[(m_nNumWidth + 1) + nNumWidth] = Vtx + D3DXVECTOR3(0.0f, 300.0f - 5.0f, 0.0f) + Pos;

	SetMesh();
}
//==================================================================================================//
//    * 判定付加メッシュの選択関数 *
//==================================================================================================//
void	CCOL_MESH::Select(int nNumber)
{
	LPDIRECT3DVERTEXBUFFER9 &pVtxBuff = m_pMesh->GetpVtxBuff();
	VERTEX_3D				*pVtx;									//頂点情報へのポインタ

	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//頂点バッファをロックし頂点データへのポインタを取得
	m_bSelect[nNumber] = (m_bSelect[nNumber] ? false : true);
	pVtx[nNumber].col = (m_bSelect[nNumber] ? D3DXCOLOR(1.0f, 0.1f, 0.02f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f));
	pVtxBuff->Unlock();	//頂点バッファのアンロックする
}
//==================================================================================================//
//    * 判定付加メッシュのラインX選択関数 *
//==================================================================================================//
void	CCOL_MESH::LineSelectX(int nNumX, int nNumZ, LINE line)
{
	LPDIRECT3DVERTEXBUFFER9 &pVtxBuff = m_pMesh->GetpVtxBuff();
	VERTEX_3D				*pVtx;									//頂点情報へのポインタ

	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//頂点バッファをロックし頂点データへのポインタを取得

	for (int nCnt = 0; nCnt < m_nNumWidth + 1; nCnt++)
	{
		if (line == LINE_ALL || (line == LINE_UP && nCnt >= nNumX) || (line == LINE_DOWN && nCnt <= nNumX))
		{
			m_bSelect[nNumZ * (m_nNumWidth + 1) + nCnt] = true;
			pVtx[nNumZ * (m_nNumWidth + 1) + nCnt].col = D3DXCOLOR(1.0f, 0.1f, 0.02f, 1.0f);
		}
	}
	pVtxBuff->Unlock();	//頂点バッファのアンロックする
}
//==================================================================================================//
//    * 判定付加メッシュのラインZ選択関数 *
//==================================================================================================//
void	CCOL_MESH::LineSelectZ(int nNumX, int nNumZ, LINE line)
{
	LPDIRECT3DVERTEXBUFFER9 &pVtxBuff = m_pMesh->GetpVtxBuff();
	VERTEX_3D				*pVtx;									//頂点情報へのポインタ

	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//頂点バッファをロックし頂点データへのポインタを取得

	for (int nCnt = 0; nCnt < m_nNumVertical + 1; nCnt++)
	{
		if (line == LINE_ALL || (line == LINE_UP && nCnt >= nNumZ) || (line == LINE_DOWN && nCnt <= nNumZ))
		{
			m_bSelect[nCnt * (m_nNumWidth + 1) + nNumX] = true;
			pVtx[nCnt * (m_nNumWidth + 1) + nNumX].col = D3DXCOLOR(1.0f, 0.1f, 0.02f, 1.0f);
		}
	}
	pVtxBuff->Unlock();	//頂点バッファのアンロックする
}
//==================================================================================================//
//    * 判定付加メッシュの移動関数 *
//==================================================================================================//
void	CCOL_MESH::ResetSelect(void)
{
	LPDIRECT3DVERTEXBUFFER9 &pVtxBuff = m_pMesh->GetpVtxBuff();
	VERTEX_3D				*pVtx;									//頂点情報へのポインタ
	D3DXCOLOR col;

	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//頂点バッファをロックし頂点データへのポインタを取得

	switch (m_Effect)
	{
	case EFFECT_NORMAL: col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); break;
	case EFFECT_GRASS: col = D3DXCOLOR(0.1f, 1.0f, 0.1f, 1.0f); break;
	case EFFECT_RIVER: col = D3DXCOLOR(0.0f, 0.1f, 1.0f, 0.7f); break;
	case EFFECT_SWAMP: col = D3DXCOLOR(0.15f, 0.5f, 1.0f, 0.7f); break;
	case EFFECT_BOOST: col = D3DXCOLOR(0.9f, 0.35f, 0.1f, 0.7f); break;
	case EFFECT_DROP: col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); break;
	}
	for (int nCount = 0; nCount < m_nNumVtx; nCount++)
	{
		m_bSelect[nCount] = false;
		if (m_bField)
		{
			pVtx[nCount].col = col;
		}
		else { pVtx[nCount].col = D3DXCOLOR(1.0f, 0.98f, 0.02f, 0.75f); }
	}
	pVtxBuff->Unlock();	//頂点バッファのアンロックする
}
//==================================================================================================//
//    * 判定付加メッシュのポイント設定関数 *
//==================================================================================================//
void	CCOL_MESH::SetPoint(bool bWK)
{
	CRoad_Manager *pManager = CRoad_Manager::GetManager();

	if (pManager == NULL) { CRoad_Manager::Create(pManager); }
	CRoad_Pointer *pTop = NULL;

	if (bWK) { pTop = pManager->GetWKTop(); pManager->GetbWKData() = true; }
	else { pTop = pManager->GetTop(); }

	if (pTop != NULL) { pTop->Uninit(); pTop = NULL; }

	CRoad_Manager::GetManager()->GetnWKData() = 0;
	for (int nCnt = 0; nCnt < m_nNumWidth + 1; nCnt++)
	{
		pTop = CRoad_Pointer::Plus(pTop, m_pVtx[1 * (m_nNumWidth + 1) + nCnt] + m_pos, m_pVtx[0 * (m_nNumWidth + 1) + nCnt] + m_pos);
		if (bWK && nCnt == 0) { pManager->GetpWKTop() = pTop; }
	}
	CRoad_Manager::GetManager()->GetnWKData() = -1;

	pManager->GetbWKData() = false;
}
//==================================================================================================//
//    * 判定付加メッシュの保存関数 *
//==================================================================================================//
void	CCOL_MESH::Save(FILE *pFile)
{
	fprintf(pFile, "%d %d %d %d\n", m_nNumWidth, m_nNumVertical, m_Type, m_Effect);
	fprintf(pFile, "%f %f %f\n", m_pos.x, m_pos.y, m_pos.z);
	fprintf(pFile, "%d\n", (m_bField ? 1 : 0));
	for (int nCount = 0; nCount < m_nNumVtx; nCount++)
	{//チップの保存
		fprintf(pFile, "%f %f %f\n", m_pVtx[nCount].x, m_pVtx[nCount].y, m_pVtx[nCount].z);
	}
}
//==================================================================================================//
//    * 判定付加メッシュの読み込み関数 *
//==================================================================================================//
void	CCOL_MESH::Load(FILE *pFile)
{
	int nField;
	if (m_pVtx != NULL)
	{//頂点の削除
		delete[] m_pVtx;
	}

	fscanf(pFile, "%d %d %d %d\n", &m_nNumWidth, &m_nNumVertical, &m_Type, &m_Effect);
	fscanf(pFile, "%f %f %f\n", &m_pos.x, &m_pos.y, &m_pos.z);
	fscanf(pFile, "%d\n", &nField);
	m_bField = (nField == 0 ? false : true);
	m_nNumVtx = (m_nNumVertical + 1) * (m_nNumWidth + 1);
	m_pVtx = new D3DXVECTOR3[m_nNumVtx];
	m_bSelect = new bool[m_nNumVtx];
	for (int nCount = 0; nCount < m_nNumVtx; nCount++)
	{//チップの保存
		fscanf(pFile, "%f %f %f\n", &m_pVtx[nCount].x, &m_pVtx[nCount].y, &m_pVtx[nCount].z);
		m_bSelect[nCount] = false;
	}
	SetMesh();
	ResetSelect();
}
//=============================================================================
//メッシュ床のあたり判定
//=============================================================================
bool CCOL_MESH::MeshField(CPlayer *&pPlayer)
{
	D3DXVECTOR3 &pos = pPlayer->Getpos();
	D3DXVECTOR3 &rot = pPlayer->Getrot();
	D3DXVECTOR3 &posold = pPlayer->Getposold();
	D3DXVECTOR3 &move = pPlayer->Getmove();
	bool		&bJump = pPlayer->GetbJump();
	D3DXVECTOR3 &FNor = pPlayer->GetFNor();
	EFFECT		&Effect = pPlayer->GgetFEffect();

	bool		bLand = true;
	D3DXVECTOR3 WKnor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 WKm_pos = m_pos;
	D3DXVECTOR3 WKpos = pos - WKm_pos + D3DXVECTOR3(sinf(rot.y), 0.0f, cosf(rot.y)) * 10.0f;
	D3DXVECTOR3 Vec[8];

	int				nCntCross = 0;
	int				nCntVertical = 0;
	D3DXVECTOR3		VtxPos[4];

	while (1)
	{
		VtxPos[0] = m_pVtx[nCntVertical * (m_nNumWidth + 1) + nCntCross];
		VtxPos[1] = m_pVtx[nCntVertical * (m_nNumWidth + 1) + nCntCross + 1];
		VtxPos[2] = m_pVtx[(nCntVertical + 1) * (m_nNumWidth + 1) + nCntCross];
		VtxPos[3] = m_pVtx[(nCntVertical + 1) * (m_nNumWidth + 1) + nCntCross + 1];

		if (FieldCheck(VtxPos[0], VtxPos[1], VtxPos[2], VtxPos[3], WKpos))
		{//四頂点で範囲内なら

			switch (m_Effect)
			{
			case EFFECT_BOOST:	//加速
				if (!bJump) { move *= 1.35f; Effect = m_Effect; }
				break;
			case EFFECT_SWAMP:	//減速
				if (!bJump)
				{
					move *= 0.93f; Effect = m_Effect;

					if (60 < pPlayer->GetSpeedCounter())
						pPlayer->SetSpeedCounter(60);
				}
				break;
			case EFFECT_GRASS:
			case EFFECT_NORMAL:
			case EFFECT_RIVER:
			case EFFECT_DROP:
				WKpos.y = FieldCollision(VtxPos[0], VtxPos[1], VtxPos[2], VtxPos[3], pos - WKm_pos, pos, WKnor);
				pPlayer->GetpShadow()->SetShadow(D3DXVECTOR3(pos.x, WKpos.y + WKm_pos.y + 3.0f, pos.z));
				if (WKpos.y + WKm_pos.y >= pos.y - (!bJump ? 11.5f : 0.0f) && WKpos.y + WKm_pos.y <= posold.y + 30.0f)
				{//貫通していたら
					Effect = m_Effect;
					if (m_Effect == EFFECT_DROP) { return bLand; }
					if (m_Effect == EFFECT_SWAMP)
					{
						move *= 0.93f; Effect = m_Effect;

						if (60 < pPlayer->GetSpeedCounter())
							pPlayer->SetSpeedCounter(60);

						return bLand;
					}
					if (m_Effect == EFFECT_GRASS)
					{
						move *= 0.93f;

						if (60 < pPlayer->GetSpeedCounter())
							pPlayer->SetSpeedCounter(60);
					}
					pos.y = WKpos.y + WKm_pos.y;
					move.y = 0.0f;
					FNor = WKnor;
					bLand = false;
					return bLand;
				}
				break;
			}
		}

		nCntCross++;
		if (m_nNumWidth <= nCntCross)
		{
			nCntCross = 0;
			nCntVertical++;
			if (nCntVertical >= m_nNumVertical) { break; }
		}
	}
	return bLand;
}
//=============================================================================
//メッシュ床の高さ獲得判定
//=============================================================================
bool	CCOL_MESH::GetHeight(D3DXVECTOR3 pos, float &fHeight)
{
	bool		bLand = true;
	D3DXVECTOR3 WKnor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 WKm_pos = m_pos;
	D3DXVECTOR3 WKpos = pos - WKm_pos;

	D3DXVECTOR3 Vec[8];

	int				nCntCross = 0;
	int				nCntVertical = 0;
	D3DXVECTOR3		VtxPos[4];

	while (1)
	{
		VtxPos[0] = m_pVtx[nCntVertical * (m_nNumWidth + 1) + nCntCross];
		VtxPos[1] = m_pVtx[nCntVertical * (m_nNumWidth + 1) + nCntCross + 1];
		VtxPos[2] = m_pVtx[(nCntVertical + 1) * (m_nNumWidth + 1) + nCntCross];
		VtxPos[3] = m_pVtx[(nCntVertical + 1) * (m_nNumWidth + 1) + nCntCross + 1];

		if (FieldCheck(VtxPos[0], VtxPos[1], VtxPos[2], VtxPos[3], WKpos))
		{//四頂点で範囲内なら

			switch (m_Effect)
			{
			case EFFECT_GRASS:
			case EFFECT_NORMAL:
				fHeight = FieldCollision(VtxPos[0], VtxPos[1], VtxPos[2], VtxPos[3], pos - WKm_pos, pos, WKnor);
				fHeight += WKm_pos.y;
				return true;
			}
		}

		nCntCross++;
		if (m_nNumWidth <= nCntCross)
		{
			nCntCross = 0;
			nCntVertical++;
			if (nCntVertical >= m_nNumVertical) { break; }
		}
	}
	return false;
}
//=============================================================================
//内側外側のチェック判定
//=============================================================================
bool CCOL_MESH::FieldCheck(D3DXVECTOR3 &FposUL, D3DXVECTOR3 &FposUR, D3DXVECTOR3 &FposDL, D3DXVECTOR3 &FposDR, D3DXVECTOR3& pos)
{
	D3DXVECTOR3 Vec[8];

	Vec[0] = FposUL - FposUR;
	Vec[1] = pos - FposUR;
	Vec[2] = FposUR - FposDR;
	Vec[3] = pos - FposDR;
	Vec[4] = FposDR - FposDL;
	Vec[5] = pos - FposDL;
	Vec[6] = FposDL - FposUL;
	Vec[7] = pos - FposUL;

	if ((Vec[0].z * Vec[1].x) - (Vec[0].x * Vec[1].z) <= 0 &&
		(Vec[2].z * Vec[3].x) - (Vec[2].x * Vec[3].z) <= 0 &&
		(Vec[4].z * Vec[5].x) - (Vec[4].x * Vec[5].z) <= 0 &&
		(Vec[6].z * Vec[7].x) - (Vec[6].x * Vec[7].z) <= 0
		)
	{//四角範囲内なら
		return true;
	}
	return false;
}
//=============================================================================
//起伏判定
//=============================================================================
float CCOL_MESH::FieldCollision(D3DXVECTOR3 &FposUL, D3DXVECTOR3 &FposUR, D3DXVECTOR3 &FposDL, D3DXVECTOR3 &FposDR, D3DXVECTOR3 WKpos, D3DXVECTOR3& pos, D3DXVECTOR3 &FNor)
{
	bool bLand = false;
	D3DXVECTOR3 Vtx[4];
	D3DXVECTOR3 vec0, vec1;
	float		fData;
	D3DXVECTOR3 nor;
	vec0 = FposDR - FposUL;
	vec1 = WKpos - FposUL;

	if ((vec0.z * vec1.x) - (vec0.x * vec1.z) <= 0)
	{//三角形区分で右上側
	 //CManager::GetDLog()->Printf(CDebugLog::MODE_INFO, "右上\n");
		Vtx[0] = FposUR;
		Vtx[1] = FposUL;
		Vtx[2] = FposDR;
	}
	else
	{//三角形区分で左下側
	 //CManager::GetDLog()->Printf(CDebugLog::MODE_INFO, "左下\n");
		Vtx[0] = FposDL;
		Vtx[1] = FposDR;
		Vtx[2] = FposUL;
	}
	fData = Vtx[0].y; Vtx[0].y -= fData; Vtx[1].y -= fData; Vtx[2].y -= fData;

	vec0 = Vtx[2] - Vtx[0];
	vec1 = Vtx[1] - Vtx[0];
	D3DXVec3Cross(&nor, &vec0, &vec1);	//外積計算
	D3DXVec3Normalize(&nor, &nor);		//正規化にする
	D3DXVECTOR3 vec;
	vec = WKpos - Vtx[0];

	if (nor.y != 0)
	{
		//WKpos.y = ((nor.x * vec.x) + (nor.z * vec.z)) / -nor.y;
		WKpos.y = ((nor.x * vec.x) + (nor.z * vec.z)) / -nor.y;
	}
	else
	{
		//WKpos.y = 0.0f;
		WKpos.y = 0.0f;
	}
	FNor = nor;
	WKpos.y += fData;

	return WKpos.y;
}
//=============================================================================
//メッシュ壁のあたり判定
//=============================================================================
void CCOL_MESH::MeshWall(CPlayer *&pPlayer, int &nCntHit, bool bReflection)
{
	//上のみ見る(2頂点のうち)
	int nVtxY = 0;	//比較する頂点を取得
	D3DXVECTOR3& pos = pPlayer->GetPos();
	for (int nCount = 0; nCount < m_nNumWidth; nCount++)
	{//メッシュWallのXZのみを見る
		if (WallCollision(m_pVtx[nVtxY * (m_nNumWidth + 1) + nCount] + m_pos, m_pVtx[nVtxY * (m_nNumWidth + 1) + nCount + 1] + m_pos
			, m_pVtx[(nVtxY + 1) * (m_nNumWidth + 1) + nCount] + m_pos, m_pVtx[(nVtxY + 1) * (m_nNumWidth + 1) + nCount + 1] + m_pos
			, pPlayer, nCntHit, bReflection) == 2)
		{
			return;
		}
	}
}
//=============================================================================
//線と線のあたり判定(壁)判定
//=============================================================================
int CCOL_MESH::WallCollision(D3DXVECTOR3 Wpos0, D3DXVECTOR3 Wpos1, D3DXVECTOR3 WUpos0, D3DXVECTOR3 WUpos1, CPlayer *&pPlayer, int &nCntHit, bool &bReflection)
{
	D3DXVECTOR3& pos = pPlayer->Getpos();
	D3DXVECTOR3 posold = pPlayer->Getposold();
	D3DXVECTOR3 &move = pPlayer->Getmove();
	float& fLength = pPlayer->GetfLength();

	D3DXVECTOR3 Wpos2;			//差分縮める用
	D3DXVECTOR3 Wpos3;			//差分縮める用
	float		fDistance;		//めり込んだ距離
	float		fPercent;		//距離から出す壁から壁の％
	float		fPower;
	float		fCntZ;
	float		fWKrot = 0.0f;
	float		fAngle[5];
	float		fSave = pos.y;
	D3DXVECTOR3 Vec;
	int		nEnd = 0;	//終了判定

						//角度計算
	fAngle[0] = atan2f(Wpos1.x - Wpos0.x, Wpos1.z - Wpos0.z);

	//キャラなら貫通対策
	if (fLength >= 3.0f) { posold -= D3DXVECTOR3(sinf(fAngle[0] + D3DX_PI * 0.5f), 0.0f, cosf(fAngle[0] + D3DX_PI * 0.5f)) * (COL_WALL_PLUS); }

	//判定対象の幅分壁を伸ばす
	Wpos2 = pos;
	Wpos0 += D3DXVECTOR3(sinf(fAngle[0] - D3DX_PI * 0.75f), 0.0f, cosf(fAngle[0] - D3DX_PI * 0.75f)) * fLength;
	Wpos1 += D3DXVECTOR3(sinf(fAngle[0] - D3DX_PI * 0.25f), 0.0f, cosf(fAngle[0] - D3DX_PI * 0.25f)) * fLength;

	//判定用角度計算
	fAngle[1] = atan2f(Wpos2.x - Wpos0.x, Wpos2.z - Wpos0.z);
	fAngle[2] = atan2f(Wpos0.x - posold.x, Wpos0.z - posold.z);
	fAngle[3] = atan2f(Wpos2.x - posold.x, Wpos2.z - posold.z);
	fAngle[4] = atan2f(Wpos1.x - posold.x, Wpos1.z - posold.z);

	if (AngleCheck(fAngle[0], fAngle[1]) <= 0 &&	//現在の位置が壁の向こうか
		AngleCheck(fAngle[0], fAngle[2]) <= 0 &&	//前の位置が壁の手前側
		AngleCheck(fAngle[3], fAngle[2]) <= 0 &&	//移動方向が壁時計回り手前頂点の内側
		AngleCheck(fAngle[3], fAngle[4]) >= 0		//移動方向が壁時計回り置く頂点の内側
		)
	{//交差チェック
	 //距離計算　距離で％も計算
		fDistance = sqrtf(powf((Wpos2.x - Wpos0.x), 2) + powf((Wpos2.z - Wpos0.z), 2));
		fPercent = fDistance / sqrtf(powf((Wpos1.x - Wpos0.x), 2) + powf((Wpos1.z - Wpos0.z), 2));
		fPercent = (Wpos1.y - Wpos0.y) * fPercent;
		if (pos.y < Wpos0.y + fPercent - 30.0f || pos.y > fPercent + WUpos0.y)
		{//Y軸が範囲外なら
			nEnd = 1;
			return nEnd;
		}

		nCntHit++;
		if (nCntHit < 2 || bReflection)
		{//2回以上ヒット
		 //交点を求める
			nEnd = 1;

			if (fLength >= 3.0f)
			{
				//反射計算
				//差分を反転させて壁の向きに引く
				fWKrot = fAngle[0] - fAngle[3]; //壁角度からプレイヤーの移動方向を引いて反転
				if (fWKrot > D3DX_PI) { fWKrot -= D3DX_PI * 2.0f; }
				if (fWKrot < -D3DX_PI) { fWKrot += D3DX_PI * 2.0f; }
				fAngle[3] = fAngle[0] + fWKrot;

				//fWKrot = fAngle[0] - fAngle[3]; //壁角度からプレイヤーの移動方向を引いて反転
				//if (fWKrot > D3DX_PI) { fWKrot -= D3DX_PI * 2.0f; }
				//if (fWKrot < -D3DX_PI) { fWKrot += D3DX_PI * 2.0f; }
				//fAngle[3] += fWKrot * 2.0f;	//行きたい角度
				//if (fAngle[3] > D3DX_PI) { fAngle[3] -= D3DX_PI * 2.0f; }
				//if (fAngle[3] < -D3DX_PI) { fAngle[3] += D3DX_PI * 2.0f; }
			}
			else
			{
				nEnd = 2;
				//交点をずらさない
				fAngle[3] = atan2f(posold.x - Wpos2.x, posold.z - Wpos2.z);

				fAngle[1] = atan2f(pos.x - Wpos0.x, pos.z - Wpos0.z);	//posoldと壁原点の角度を取る
				fWKrot = D3DX_PI * -0.5f - fAngle[1];					//それと-1.57の差分を求める
				if (fWKrot > D3DX_PI) { fWKrot -= D3DX_PI * 2.0f; }
				if (fWKrot < -D3DX_PI) { fWKrot += D3DX_PI * 2.0f; }
				//元角度計算は行きたい角度と壁角度
				//それらを差分後角度で伸ばし1.0Ｙ分を取得
				Wpos1 = D3DXVECTOR3(sinf(fWKrot + fAngle[0]), 0.0f, cosf(fWKrot + fAngle[0])) * 1.0f;
				Wpos2 = D3DXVECTOR3(sinf(fWKrot + fAngle[3]), 0.0f, cosf(fWKrot + fAngle[3])) * 1.0f;
				Wpos1.x = Wpos1.x / Wpos1.z;
				Wpos2.x = Wpos2.x / Wpos2.z;
				fCntZ = fDistance / (-Wpos1.x + Wpos2.x);
				//(距離 / Ｘの和) = 回数 = 求めたい距離
				Wpos3 = D3DXVECTOR3(fCntZ * Wpos1.x, 0.0f, fCntZ);
				fDistance = sqrtf(powf((Wpos3.x), 2) + powf((Wpos3.z), 2));
			}
			//値代入
			Wpos0 = D3DXVECTOR3(Wpos0.x, pos.y, Wpos0.z) + D3DXVECTOR3(sinf(fAngle[0]), 0.0f, cosf(fAngle[0])) * fDistance;
			if (bReflection) { pos = Wpos0 + D3DXVECTOR3(sinf(fAngle[3]), 0.0f, cosf(fAngle[3])) * (COL_WALL_PLUS); }
			else
			{//主にキャラ向けの反射処理	壁体当たり後に壁沿いに歩け、突っかからないよう調整
				fPower = ((powf(move.x, 2) + powf(move.z, 2)) * (COL_RECOIL)+2.0f);
				if (fPower > 12.0f) { fPower = 12.0f; }
				pos = Wpos0
					+ D3DXVECTOR3(sinf(fAngle[0] - D3DX_PI * 0.5f), 0.0f, cosf(fAngle[0] - D3DX_PI * 0.5f)) * (0.1f)
					+ D3DXVECTOR3(sinf(fAngle[3]), 0.0f, cosf(fAngle[3])) * (1.0f);
				move = D3DXVECTOR3(move.x * (1.0f - COL_MOVE_SLOWING), move.y, move.z * (1.0f - COL_MOVE_SLOWING)) + D3DXVECTOR3(sinf(fAngle[3]), 0.0f, cosf(fAngle[3])) * fPower;
			}
		}
		else
		{
			nEnd = 2;
			//CManager::GetDLog()->Printf(CDebugLog::MODE_LOG, "戻った！\n");
			pos.x = posold.x;
			pos.z = posold.z;
		}
		pos.y = fSave;
	}

	return nEnd;
}
//=============================================================================
//角度と角度のチェック判定
//=============================================================================
int CCOL_MESH::AngleCheck(float fAngle0, float fAngle1)
{
	float fWKRot = fAngle0;
	fAngle0 = 0.0f;
	fAngle1 -= fWKRot;

	if (fAngle1 > D3DX_PI) { fAngle1 -= D3DX_PI * 2.0f; }
	if (fAngle1 < -D3DX_PI) { fAngle1 += D3DX_PI * 2.0f; }

	if (fAngle0 > fAngle1) { return 1; }
	else if (fAngle0 < fAngle1) { return -1; }
	return 0;
}
//=============================================================================
//角度と角度のチェック判定
//=============================================================================
bool	CCOL_MESH::CrossCheck(D3DXVECTOR3 &Wpos0, D3DXVECTOR3 &Wpos1, D3DXVECTOR3 &pos, D3DXVECTOR3 &posold)
{
	float fAngle[5];

	fAngle[0] = atan2f(Wpos1.x - Wpos0.x, Wpos1.z - Wpos0.z);
	fAngle[1] = atan2f(pos.x - Wpos0.x, pos.z - Wpos0.z);
	fAngle[2] = atan2f(Wpos0.x - posold.x, Wpos0.z - posold.z);
	fAngle[3] = atan2f(pos.x - posold.x, pos.z - posold.z);
	fAngle[4] = atan2f(Wpos1.x - posold.x, Wpos1.z - posold.z);

	if (AngleCheck(fAngle[0], fAngle[1]) <= 0 &&	//現在の位置が壁の向こうか
		AngleCheck(fAngle[0], fAngle[2]) <= 0 &&	//前の位置が壁の手前側
		AngleCheck(fAngle[3], fAngle[2]) <= 0 &&	//移動方向が壁時計回り手前頂点の内側
		AngleCheck(fAngle[3], fAngle[4]) >= 0		//移動方向が壁時計回り置く頂点の内側
		)
	{
		return true;
	}
	return false;
}
//==================================================================================================//
//    * 判定付加管理の全読み込み関数 *
//==================================================================================================//
void CCOL_MESH_MANAGER::LoadMap(void)
{
	CRoad_Manager *&pManager = CRoad_Manager::GetManager();

	Load(TYPE_HALF0);
	Load(TYPE_BRIDGE);
	Load(TYPE_HALF1);

	if (pManager == NULL)
	{//生成
		CRoad_Manager::Create(pManager);
	}
	pManager->LoadMap();

	//画像読み込み
	CDispEffect::Load();
}
//==================================================================================================//
//    * 判定付加管理の読み込み関数 *
//==================================================================================================//
void CCOL_MESH_MANAGER::EndMap(void)
{
	CRoad_Manager *&pManager = CRoad_Manager::GetManager();
	if (pManager != NULL) { pManager->Uninit(); }
	CDispEffect::UnLoad();
	UnLoad();
}
//==================================================================================================//
//    * 判定付加管理の読み込み関数 *
//==================================================================================================//
void	CCOL_MESH_MANAGER::Load(TYPE type)
{
	if (type == TYPE_MAX) { return; }
	if (m_pColMesh[type] != NULL) { return; }
	CCOL_MESH_MANAGER::Create(m_pColMesh[type]);
	m_pColMesh[type]->Loadtxt(type);
}
//==================================================================================================//
//    * 判定付加管理の読み込んだもの破棄関数 *
//==================================================================================================//
void CCOL_MESH_MANAGER::UnLoad(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		if (m_pColMesh[nCount] == NULL) { continue; }
		m_pColMesh[nCount]->Uninit();
	}
}
//==================================================================================================//
//    * 判定付加メッシュの生成関数 *
//==================================================================================================//
bool	CCOL_MESH_MANAGER::Create(CCOL_MESH_MANAGER *&pColManager)
{
	pColManager = NULL;
	pColManager = new CCOL_MESH_MANAGER;
	if (pColManager != NULL) { pColManager->Init(); return true; }

	return false;
}
//==================================================================================================//
//    * 判定付加管理の初期化関数 *
//==================================================================================================//
HRESULT	CCOL_MESH_MANAGER::Init(void)
{
	m_pField = NULL;
	m_pWall = NULL;
	m_nNumField = 0;
	m_nNumWall = 0;
	m_Type = TYPE_MAX;
	return S_OK;
}
//==================================================================================================//
//    * 判定付加管理の終了関数 *
//==================================================================================================//
void	CCOL_MESH_MANAGER::Uninit(void)
{
	if (m_Type != TYPE_MAX)
	{//管理元のポインタ破棄
		m_pColMesh[m_Type] = NULL;
	}
	if (m_pField != NULL)
	{//地面の削除
		for (int nCount = 0; nCount < m_nNumField; nCount++)
		{
			m_pField[nCount]->Uninit();
			m_pField[nCount] = NULL;
		}
		delete[] m_pField;
		m_nNumField = 0;
	}
	if (m_pWall != NULL)
	{//壁の削除
		for (int nCount = 0; nCount < m_nNumWall; nCount++)
		{
			m_pWall[nCount]->Uninit();
			m_pWall[nCount] = NULL;
		}
		delete[] m_pWall;
		m_pWall = 0;
	}

	//死亡フラグを立てる
	CScene::Release();
}
//==================================================================================================//
//    * 判定付加管理の更新関数 *
//==================================================================================================//
void	CCOL_MESH_MANAGER::Update(void)
{
	for (int nCount = 0; nCount < m_nNumField; nCount++)
	{//フィールドの更新
		m_pField[nCount]->Update();
	}
	for (int nCount = 0; nCount < m_nNumWall; nCount++)
	{//フィールドの更新
		m_pWall[nCount]->Update();
	}
}
//==================================================================================================//
//    * 判定付加管理の描画関数 *
//==================================================================================================//
void	CCOL_MESH_MANAGER::Draw(void)
{
	for (int nCount = 0; nCount < m_nNumField; nCount++)
	{//フィールドの更新
		m_pField[nCount]->Draw();
	}
	for (int nCount = 0; nCount < m_nNumWall; nCount++)
	{//フィールドの更新
		m_pWall[nCount]->Draw();
	}
}
//==================================================================================================//
//    * 判定付加管理の保存関数 *
//==================================================================================================//
void	CCOL_MESH_MANAGER::Save(void)
{
	FILE *pFile = NULL;
	pFile = fopen("data/TEXT/COL_MESH/Save.txt", "w");

	if (pFile != NULL)
	{//Save　成功
		fprintf(pFile, "%d %d\n", m_nNumField, m_nNumWall);

		for (int nCount = 0; nCount < m_nNumField; nCount++)
		{//フィールドの更新
			m_pField[nCount]->Save(pFile);
		}
		for (int nCount = 0; nCount < m_nNumWall; nCount++)
		{//フィールドの更新
			m_pWall[nCount]->Save(pFile);
		}
		fclose(pFile);
	}
}
//==================================================================================================//
//    * 判定付加管理の読み込み関数 *
//==================================================================================================//
void	CCOL_MESH_MANAGER::Loadtxt(TYPE type)
{
	FILE *pFile = NULL;
	char aMesh[128] = "data/TEXT/COL_MESH/";

	switch (type)
	{
	case CCOL_MESH_MANAGER::TYPE_HALF0:		strcat(&aMesh[0], "Half_0.txt");	break;
	case CCOL_MESH_MANAGER::TYPE_BRIDGE:	strcat(&aMesh[0], "Bridge.txt");	break;
	case CCOL_MESH_MANAGER::TYPE_HALF1:		strcat(&aMesh[0], "Half_1.txt");	break;
	case CCOL_MESH_MANAGER::TYPE_MAX:		strcat(&aMesh[0], "Save.txt");		break;
	}

	pFile = fopen(&aMesh[0], "r");

	if (pFile != NULL)
	{//Save　成功
		if (m_pField != NULL)
		{//地面の削除
			for (int nCount = 0; nCount < m_nNumField; nCount++)
			{
				m_pField[nCount]->Uninit();
				m_pField[nCount] = NULL;
			}
			delete[] m_pField;
			m_nNumField = 0;
		}
		if (m_pWall != NULL)
		{//壁の削除
			for (int nCount = 0; nCount < m_nNumWall; nCount++)
			{
				m_pWall[nCount]->Uninit();
				m_pWall[nCount] = NULL;
			}
			delete[] m_pWall;
			m_pWall = 0;
		}

		m_Type = type;
		fscanf(pFile, "%d %d\n", &m_nNumField, &m_nNumWall);
		m_pField = new CCOL_MESH*[m_nNumField];
		m_pWall = new CCOL_MESH*[m_nNumWall];

		for (int nCount = 0; nCount < m_nNumField; nCount++)
		{//フィールドの更新
			CCOL_MESH::Create(m_pField[nCount]);
			m_pField[nCount]->Load(pFile);
		}
		for (int nCount = 0; nCount < m_nNumWall; nCount++)
		{//フィールドの更新
			CCOL_MESH::Create(m_pWall[nCount]);
			m_pWall[nCount]->Load(pFile);
		}
		ResetCollar();
		fclose(pFile);
	}
}
//==================================================================================================//
//    * 判定付加管理の静的代入関数 *
//==================================================================================================//
void	CCOL_MESH_MANAGER::Set(TYPE type)
{
	if (type >= TYPE_MAX) { return; }
	m_Type = type;
	m_pColMesh[m_Type] = this;
}
//==================================================================================================//
//    * 判定付加管理の静的代入関数 *
//==================================================================================================//
void	CCOL_MESH_MANAGER::ResetCollar(void)
{
	for (int nCount = 0; nCount < m_nNumField; nCount++)
	{//フィールドの更新
		m_pField[nCount]->ResetSelect();
	}
}
//==================================================================================================//
//    * 判定付加管理の読み込み関数 *
//==================================================================================================//
bool	CCOL_MESH_MANAGER::Collision(CPlayer *pPlayer)
{
	bool	bLand = true;
	int		&nMap = pPlayer->GetnMap();

	WNumCollision(pPlayer);
	if (nMap >= 0) { if (!FNumCollision(pPlayer)) { bLand = false; } }
	return bLand;
}
//==================================================================================================//
//    * 判定付加管理の高さ取得関数 *
//==================================================================================================//
float	CCOL_MESH_MANAGER::GetHeight(D3DXVECTOR3 &pos, int &nMap)
{
	if (nMap >= TYPE_MAX) { return true; }

	bool bHit = false;
	float fHeight;
	if (m_pColMesh[nMap] != NULL)
	{
		for (int nCount = 0; nCount < m_pColMesh[nMap]->m_nNumField; nCount++)
		{
			CCOL_MESH *&pMesh = m_pColMesh[nMap]->GetpField()[nCount];

			if (pMesh->GetHeight(pos, fHeight))
			{
				bHit = true;
			}
		}
	}
	if (!bHit) { fHeight = pos.y; }

	return fHeight;
}
//==================================================================================================//
//    * 判定付加管理の壁判定関数 *
//==================================================================================================//
void	CCOL_MESH_MANAGER::WNumCollision(CPlayer *&pPlayer)
{
	int &nNumber = pPlayer->GetnMap();
	if (nNumber >= TYPE_MAX) { return; }

	if (m_pColMesh[nNumber] != NULL)
	{
		int nCntHit = 0;
		for (int nCount = 0; nCount < m_pColMesh[nNumber]->m_nNumWall; nCount++)
		{
			CCOL_MESH *&pMesh = m_pColMesh[nNumber]->GetpWall()[nCount];
			pMesh->MeshWall(pPlayer, nCntHit, false);
		}

	}
}
//==================================================================================================//
//    * 判定付加管理の地面判定関数 *
//==================================================================================================//
bool	CCOL_MESH_MANAGER::FNumCollision(CPlayer *&pPlayer)
{
	int &nNumber = pPlayer->GetnMap();
	if (nNumber >= TYPE_MAX) { return true; }

	bool bLand = true;
	if (m_pColMesh[nNumber] != NULL)
	{
		for (int nCount = 0; nCount < m_pColMesh[nNumber]->m_nNumField; nCount++)
		{
			CCOL_MESH *&pMesh = m_pColMesh[nNumber]->GetpField()[nCount];
			if (!pMesh->MeshField(pPlayer)) { bLand = false; }
		}

	}
	return bLand;
}
//==================================================================================================//
//    * 判定付加管理の壁判定関数 *
//==================================================================================================//
void	CCOL_MESH_MANAGER::WCollision(void)
{

}
//==================================================================================================//
//    * 判定付加管理の地面追加関数 *
//==================================================================================================//
int		CCOL_MESH_MANAGER::AddField(D3DXVECTOR3 pos)
{
	//生成
	CCOL_MESH	**pField;
	pField = new CCOL_MESH*[m_nNumField];

	for (int nCount = 0; nCount < m_nNumField; nCount++)
	{//引継ぎ
		pField[nCount] = m_pField[nCount];
		m_pField[nCount] = NULL;
	}
	delete[] m_pField;	//終了

	m_pField = new CCOL_MESH*[m_nNumField + 1];
	for (int nCount = 0; nCount < m_nNumField; nCount++)
	{//返し
		m_pField[nCount] = pField[nCount];
		pField[nCount] = NULL;
	}
	delete[] pField;	//終了

	CCOL_MESH *pColMesh = NULL;
	if (CCOL_MESH::Create(pColMesh)) { pColMesh->Set(true, pos); }
	m_pField[m_nNumField] = pColMesh;
	m_nNumField++;

	return m_nNumField - 1;
}
//==================================================================================================//
//    * 判定付加管理の壁追加関数 *
//==================================================================================================//
int	CCOL_MESH_MANAGER::AddWall(D3DXVECTOR3 pos)
{
	//生成
	CCOL_MESH	**pWall;
	pWall = new CCOL_MESH*[m_nNumWall];

	for (int nCount = 0; nCount < m_nNumWall; nCount++)
	{//引継ぎ
		pWall[nCount] = m_pWall[nCount];
		m_pWall[nCount] = NULL;
	}
	delete[] m_pWall;	//終了

	m_pWall = new CCOL_MESH*[m_nNumWall + 1];
	for (int nCount = 0; nCount < m_nNumWall; nCount++)
	{//返し
		m_pWall[nCount] = pWall[nCount];
		pWall[nCount] = NULL;
	}
	delete[] pWall;	//終了

	CCOL_MESH *pColMesh = NULL;
	if (CCOL_MESH::Create(pColMesh)) { pColMesh->Set(false, pos); }
	m_pWall[m_nNumWall] = pColMesh;
	m_nNumWall++;

	return m_nNumWall - 1;
}
//==================================================================================================//
//    * 判定付加管理の個別削除関数 *
//==================================================================================================//
void	CCOL_MESH_MANAGER::DeleteField(int nNumber)
{
	//生成
	CCOL_MESH	**pField;
	int			nCntSave;
	pField = new CCOL_MESH*[m_nNumField - 1];

	nCntSave = 0;
	for (int nCount = 0; nCount < m_nNumField; nCount++)
	{//引継ぎ
		if (nCount == nNumber) { m_pField[nCount]->Uninit();  continue; }
		pField[nCntSave] = m_pField[nCount];
		nCntSave++;
		m_pField[nCount] = NULL;
	}
	delete[] m_pField;	//終了

	m_nNumField--;
	m_pField = new CCOL_MESH*[m_nNumField];
	for (int nCount = 0; nCount < m_nNumField; nCount++)
	{//返し
		m_pField[nCount] = pField[nCount];
		pField[nCount] = NULL;
	}
	delete[] pField;	//終了
}
//==================================================================================================//
//    * 判定付加管理の個別削除関数 *
//==================================================================================================//
void	CCOL_MESH_MANAGER::DeleteWall(int nNumber)
{
	//生成
	CCOL_MESH	**pWall;
	int			nCntSave;
	pWall = new CCOL_MESH*[m_nNumWall - 1];

	nCntSave = 0;
	for (int nCount = 0; nCount < m_nNumWall; nCount++)
	{//引継ぎ
		if (nCount == nNumber) { m_pWall[nCount]->Uninit();  continue; }
		pWall[nCntSave] = m_pWall[nCount];
		nCntSave++;
		m_pWall[nCount] = NULL;
	}
	delete[] m_pWall;	//終了

	m_nNumWall--;
	m_pWall = new CCOL_MESH*[m_nNumWall];
	for (int nCount = 0; nCount < m_nNumWall; nCount++)
	{//返し
		m_pWall[nCount] = pWall[nCount];
		pWall[nCount] = NULL;
	}
	delete[] pWall;	//終了
}
