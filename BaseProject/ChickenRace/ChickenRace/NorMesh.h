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
#include "manager.h"
//*****************************************************************************
// マクロの定義
//*****************************************************************************
#define Cylinder_PRIORITY	(4)
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
	void	SetCylinder(float fLengthY, float fLengthXZU, float fLengthXZD
		, D3DXCOLOR col = INIT_COL, D3DXVECTOR2 UV0 = D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2 UV1 = D3DXVECTOR2(0.1f, 1.0f));	//セット処理
	void	SetNor(void);	//法線のセット処理
	void	SetMtx(void);

	HRESULT Init(void);
	void	Uninit(void);
	void	Draw(void);
	void		BindTexture(LPDIRECT3DTEXTURE9 pTex) { m_pTex = pTex; }

	D3DXMATRIX  &GetMtx(void) { return m_mtxWorld; }
	D3DXVECTOR3 &GetRot(void) { return m_rot; }
	D3DXVECTOR3 &GetPos(void) { return m_pos; }
	int &GetnCross(void) { return m_nCross; }
	int &GetnVertical(void) { return m_nVertical; }
	int &GetnNumVertex(void) { return m_nNumVertex; }
	int &GetnNumDraw(void) { return m_nNumDraw; }
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
	int						m_nNumDraw;		//描画関数
	bool					m_bMesh;		//張っている面が内側か外側か(左回りか右回りか)
};
//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CCylinder : public CNorMesh, public CScene
{
public:
	typedef enum
	{
		TYPE_BOOST,		//ブースト
		TYPE_RARE,		//レアピヨコ
		TYPE_EATK,		//卵攻撃
		TYPE_HATK,		//ヒヨコ攻撃
		TYPE_HATK_S,	//強ヒヨコ攻撃
		TYPE_HATK_SC,	//強ヒヨコ攻撃 キャラ警告用
		TYPE_HATK_SH,	//強ヒヨコ攻撃 範囲警告用
		TYPE_HDEF,		//ヒヨコ妨害
		TYPE_MAX
	}TYPE;
	typedef enum
	{
		STATE_NORMAL,	//通常
		STATE_SPREAD,	//広がる
		STATE_FADE,		//消えていく
		STATE_TIME,		//時間経過でフェード
		STATE_END,		//終わり
		STATE_MAX
	}STATE;
	CCylinder(int nPriority = Cylinder_PRIORITY) : CScene(nPriority) {};
	~CCylinder() {};

	static CCylinder	*Create(D3DXVECTOR3 &pos, D3DXVECTOR3 &rot, TYPE type);
	void	Set(D3DXVECTOR3 &pos, D3DXVECTOR3 &rot, TYPE type);	//セット処理
	void	SetState(STATE state);	//状態設定処理

	HRESULT Init(void);
	void	Update(void);
	void	Uninit(void);
	void	Draw(void);

private://*****************************************************************************
		//変数宣言//***********************************************************************
	TYPE	m_Type;
	STATE	m_State;		//終了判定
	D3DXVECTOR3 *m_pPos;	//参照する位置
	D3DXVECTOR3 *m_pRot;	//参照する角度
	D3DXVECTOR3 m_AddPos;	//加算位置
	D3DXVECTOR3 m_AddRot;	//加算角度
	D3DXCOLOR	m_col;		//透明度

	float	m_fUV_X;		//XにずらすUVの値
	float	m_fPlusA;		//透明度減少量
	float	m_fCntState;	//状態管理用
	float	m_fCntTime;	//時間管理用
	float	m_fLengthY;		//長さY
	float	m_fLengthXZ[2];	//長さXZ	0:上	1:下

	int		m_nPtn;
	int		m_nSpeed;
	int		m_nCntSpd;
};
#endif
