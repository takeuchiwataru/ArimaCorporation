//=============================================================================
//
// オービット処理 [Orbit.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _ORBIT_H_
#define _ORBIT_H_

#include "main.h"
#include "scene.h"
#include "2DPolygon.h"
//*****************************************************************************
// マクロの定義
//*****************************************************************************
#define ORBIT_PRIORITY				(2)			//軌跡の優先順位
#define ORBIT_RESET					(10)		//軌跡の開始フレーム

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CEfcOrbit : public CScene
{
public:
	typedef enum
	{
		TYPE_NONE,		//通常
		TYPE_FADE,		//フェード
		TYPE_MAX
	}TYPE;
	CEfcOrbit(int nPrioryity = ORBIT_PRIORITY) : CScene::CScene(nPrioryity)
	{
		m_pmtxWorld = NULL;
		m_bDelete = NULL;
	};
	~CEfcOrbit() {};

	static CEfcOrbit	*Create(void);
	CEfcOrbit	*Set(D3DXMATRIX &mtxWorld, D3DXVECTOR3 length0, D3DXVECTOR3 length1, bool &bDelete, D3DXCOLOR col, int nUV, int nHeight, int nReset);
	HRESULT	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	void	ReSet(void);
	static void	ReSetAll(void);
	static void	Delete(bool bEnd);
	static void	DeleteAll(void);
	void		BindTexture(LPDIRECT3DTEXTURE9 pTex) { m_pTex = pTex; }
	C2D::DRAW_TYPE &GetDrawType(void) { return m_DrawType; }
	float		&GetfCol(void) { return m_fPlusCol; }
	TYPE		&GetType(void) { return m_Type; }

private://*****************************************************************************
	void	SetVtx(bool bAll);		//最新の頂点を計算して代入
	void	SwapVtx(void);		//頂点の入れ替え
								//変数宣言//***********************************************************************
	TYPE						m_Type;
	C2D::DRAW_TYPE			m_DrawType;		// 描画する方法(加算合成など)
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファ
	LPDIRECT3DTEXTURE9		m_pTex;		//画像
	int						m_nNumPolygon;	//ポリゴン数

	D3DXMATRIX		*m_pmtxWorld;	//親のワールドマトリックス
	D3DXMATRIX		m_mtxWorld[2];	//ワールドマトリックス
	D3DXVECTOR3		m_posold;		//前の位置
	D3DXCOLOR		m_col;			//色
	int				m_nUV;			//画像のUV位置
	int				m_nHeight;		//画像の高さの分割数
	int				m_nCross;		//横の分割数
	int				m_nReset;		//リセットされた場合のカウント用
	int				m_nInitReset;	//リセットの保存用
	float			m_fPlusCol;		//薄れゆく色
	float			m_fUV;			//薄れゆく色
	float			m_fEnd;			//破棄までの経過時間
	bool			m_bEnd;			//終了判定
	bool			*m_bDelete;		//消える判定
};
#endif