//=============================================================================
//
// メッシュ処理 [mesh.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _NORMESH_H_
#define _NORMESH_H_

#include "main.h"
#include "scene.h"
//*****************************************************************************
// マクロの定義
//*****************************************************************************

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CNorMesh
{
public:
	typedef enum
	{
		TYPE_FIELD,		//地面
		TYPE_WALL,		//壁
		TYPE_CEILING,	//天井
		TYPE_ORBIT,		//オービット
		TYPE_CYLINDER,	//シリンダー
		TYPE_MAX
	}TYPE;
	CNorMesh() {};
	~CNorMesh() {};

	static bool	Create(CNorMesh *&pNorMesh);
	void	Set(TYPE type, D3DXVECTOR3 pos, int nVertical, int nCorss, bool bMesh);	//セット処理
	void	SetNor(void);	//法線のセット処理
	void	SetMtx(void);

	HRESULT Init(void);
	void	Uninit(void);
	void	Draw(void);
	void		BindTexture(LPDIRECT3DTEXTURE9 pTex) { m_pTex = pTex; }

	D3DXMATRIX  &GetMtx(void)	{ return m_mtxWorld; }
	D3DXVECTOR3 &GetRot(void)	{ return m_rot; }
	D3DXVECTOR3 &GetPos(void)	{ return m_pos;}
	int &GetnCross(void)		{ return m_nCross; }
	int &GetnVertical(void)		{ return m_nVertical; }
	int &GetnNumVertex(void) { return m_nNumVertex; }
	LPDIRECT3DVERTEXBUFFER9 &GetpVtxBuff(void) { return m_pVtxBuff; }
protected://*****************************************************************************
	LPDIRECT3DVERTEXBUFFER9 &GetVtxBuff(void) { return m_pVtxBuff; }
private://*****************************************************************************
	void PlusNor(VERTEX_3D *pVtx, VERTEX_3D *pVtxL, VERTEX_3D *pVtxR);	//法線のセット処理
		//変数宣言//***********************************************************************
	TYPE					m_Type;			//タイプ
	D3DXMATRIX				m_mtxWorld;		//ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;		//インデックスバッファのポインタ
	LPDIRECT3DTEXTURE9		m_pTex;		//画像
	D3DXVECTOR3				m_pos;			//位置
	D3DXVECTOR3				m_rot;			//向き
	int						m_nVertical;	//縦の分割数
	int						m_nCross;		//横の分割数
	int						m_nNumVertex;	//頂点数
	int						m_nNumIdx;		//インデックス数
	int						m_nNumPolygon;	//ポリゴン数
	bool					m_bMesh;		//張っている面が内側か外側か(左回りか右回りか)
};
#endif
