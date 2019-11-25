//=============================================================================
//
// 2DPolygon処理 [2DPolygon.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _2DPOLYGON_H_
#define _2DPOLYGON_H_

#include "main.h"
#include "scene.h"

#define FRAME_SPEED	(1.0f)
//*****************************************************************************
// クラスの定義
//*****************************************************************************
class C2D
{
public:
	typedef enum
	{
		DRAW_TYPE_NORMAL = 0,		//通常描画
		DRAW_TYPE_ADD,				//加算合成
		DRAW_TYPE_SUBTRACT,			//減算合成
		DRAW_TYPE_MULTIPLY,			//乗算合成
		DRAW_TYPE_OVERLAY,			//黒無視加算無し
		DRAW_TYPE_AER,				//黒無視加算無し
		DRAW_TYPE_ZNORMAL = 10,		//Z無視通常
		DRAW_TYPE_ZADD,				//Z無視加算
		DRAW_TYPE_ZSUBTRACT,		//Z無視減算
		DRAW_TYPE_ZMULTIPLY,		//乗算合成
		DRAW_TYPE_ZOVERLAY,			//黒無視加算無し
		DRAW_TYPE_NO,				//描画しない
		DRAW_TYPE_MAX
	}DRAW_TYPE;
	C2D() {};
	~C2D() {};

	static C2D	*Create(void);
	void	Set(D3DXVECTOR3 pos, float fLengthX, float flengthY, float fRot, int nUV, int nUV_X, int nUV_Y
		, D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), DRAW_TYPE Drawtype = DRAW_TYPE_NORMAL);
	void	SetPosition(float fRot);
	void	SetPosition2(float fSize);
	void	SetUV(int nUV, int nUV_X, int nUV_Y);
	void	SetUV_Camera(float fUV_X, float fUV_Y, float m_fDiameter);
	void	SetGageUV(float fMaxLengthX);
	void	SetGage(float fLengthX, int nDirection);
	void	SetColor(D3DXCOLOR col);
	void	PlusColor(D3DXCOLOR col);
	void	SetSideAlpha(float fCol, float fMinCol, int nID);
	void	PlusUV(D3DXVECTOR2 uv);

	HRESULT Init(void);
	void	Uninit(void);
	void	Draw(void);
	void	Update(void);
	void	BindTexture(LPDIRECT3DTEXTURE9 pTex)	 { m_pTex = pTex; }
	static bool		DrawPrepare(DRAW_TYPE DrawType, LPDIRECT3DDEVICE9 &pD3DDevice);	//描画準備処理

	float		&GetfLengthX(void) { return m_fLengthX; }
	float		&GetfLengthY(void) { return m_fLengthY; }
	D3DXVECTOR3 &GetPosition(void) { return m_pos; }
protected://*****************************************************************************
		  //変数宣言//***********************************************************************
private://*****************************************************************************
		//変数宣言//***********************************************************************
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// 頂点バッファへのポインタ
	DRAW_TYPE				m_DrawType;		// 描画する方法(加算合成など)

	D3DXVECTOR3				m_pos;				// ポリゴンの位置
	LPDIRECT3DTEXTURE9		m_pTex;				// テクスチャ番号
	float					m_fLengthX;			// ポリゴンの長さX
	float					m_fLengthY;			// ポリゴンの長さY
};
//*****************************************************************************
// クラスの定義
//*****************************************************************************
class C2DAnim : public C2D
{
public:
	typedef enum
	{
		ANIMATION_NORMAL,
		ANIMATION_LOOP,
		ANIMATION_END,
		ANIMATION_MAX
	}ANIMATION;
	typedef enum
	{
		STATE_NORMAL,
		STATE_FADEOUT,
		STATE_FADEIN,
		STATE_FADEIN_D,
		STATE_FADEOUT_D,
		STATE_MAX
	}STATE;
	C2DAnim() {};
	~C2DAnim() {};

	static C2DAnim	*Create(void);
	void	Set(D3DXVECTOR3 pos, float fLengthX, float flengthY, float fRot, int nPtnAnim, int nPtnWide, int nPtnHeight, int nPtnSpeed, 
		ANIMATION animation = ANIMATION_NORMAL, D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), DRAW_TYPE Drawtype = DRAW_TYPE_NORMAL);
	void	SetState(STATE state, float fCntState = -99.9f);
	void		Update(void);
	ANIMATION	&GetAnim(void) { return m_animation; }
	STATE		&Getstate(void) { return m_state; }
	float		&GetfStateSpd(void) { return m_fStateSpd; }
private://*****************************************************************************
		//変数宣言//***********************************************************************
	int			m_nCntAnim;			//Animationのカウント
	int			m_nPtnAnim;			//指定された座標で表示
	int			m_nPtnHeight;		//画像の縦の分割数
	int			m_nPtnWide;			//画像の横の分割数
	int			m_nPtnSpeed;		//Animation速度
	D3DXCOLOR	m_col;
	ANIMATION	m_animation;	//使用方法

	STATE		m_state;			//状態
	float		m_fCntState;		//状態管理用
	float		m_fCntSize;			//大きさ管理用
	float		m_fStateSpd;		//状態管理用速度
};
//*****************************************************************************
// クラスの定義
//*****************************************************************************
class C2DCameraBG : public C2D
{
public:
	C2DCameraBG() {};
	~C2DCameraBG() {};

	void	Set(D3DXVECTOR3 pos, float fLengthX, float flengthY, float fRot, float fUV_X, float fUV_Y, float fDiameter, D3DXCOLOR col);
	void	Update(void);
private://*****************************************************************************
		//変数宣言//***********************************************************************
	float	m_fUV_X;		//UV_X
	float	m_fUV_Y;		//UV_Y
	float	m_fUV_XPlus;	//Xのプラス値
	float	m_fUV_YPlus;	//Yのプラス値
	float	m_fDiameter;	//カメラ距離の倍率
};
//*****************************************************************************
// クラスの定義
//*****************************************************************************
class C2DPolygon : public C2D
{
public:
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_FADE,
		STATE_FLASH, STATE_FLASH2,
		STATE_FADEIN,
		STATE_FADEOUT,
		STATE_SLIDEIN, STATE_SLIDEOUT,
		STATE_SLIDEIN2, STATE_SLIDEOUT2,
		STATE_BIG, STATE_MEDIUM, STATE_SMALL,
		STATE_MAX
	}STATE;
	C2DPolygon() {};
	~C2DPolygon() {};

	C2DPolygon	*Create(void);
	void	Set(D3DXVECTOR3 pos, float fLengthX, float flengthY, float fRot, int nUV, int nUV_X, int nUV_Y, float fCntState, STATE state, D3DXCOLOR col);
	void	Update(void);

	D3DXCOLOR	&GetColor(void) { return m_col; }
	STATE		&GetState(void) { return m_state; }
	float		&GetfCntState(void) { return m_fCntState; }
private://*****************************************************************************
		//変数宣言//***********************************************************************
	float				m_fCntState;		//STATE管理用
	STATE				m_state;			//使用方法
	D3DXCOLOR m_col;
};
#endif
