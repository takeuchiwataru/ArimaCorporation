//=============================================================================
//
// 画面演出処理 [DispEffect.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _DISPEFFECT_H_
#define _DISPEFFECT_H_

#include "main.h"
#include "scene.h"
#include "2DPolygon.h"
class CPlayer;
//*****************************************************************************
// マクロの定義
//*****************************************************************************
#define DISP_ANIM_MAX	(32)		//同時に存在できるアニメーション数
#define DISP_SSET		(5)			//水溜まりにはいった時の汚す数
#define DISP_SSIZE		(90.0f)		//水溜まりの一つの基本サイズ
#define DISP_STIME		(20.0f)		//水溜まりの一つの出現時間
#define DISP_BTIME		(60.0f)		//加速演出の終了時間

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CDispEffect : public CScene
{
public:
	typedef enum
	{
		EFFECT_SWAMP,	//水たまり
		EFFECT_BOOST,	//加速
		EFFECT_MAX,
	}EFFECT;
	typedef enum
	{
		TEX_SWAMP, TEX_SWAMP_ONE,	//水たまり
		TEX_BOOST, 					//加速
		TEX_FootSteps, TEX_Shadow,
		TEX_Pin, TEX_Light,
		TEX_MAX,
	}TEX;
	CDispEffect(int nPriority = 5, CScene::OBJTYPE objType = CScene::OBJTYPE_2DPOLYGON) : CScene(nPriority, objType) {};
	~CDispEffect() {};
	static void Load(void);
	static void UnLoad(void);

	static LPDIRECT3DTEXTURE9 &GetpTexAll(TEX tex) { return m_pTexAll[tex]; }
	static CDispEffect	*Create(CPlayer *pPlayer);
	HRESULT Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	void	SetEffect(EFFECT Effect);
private://*****************************************************************************
	void	Set(CPlayer *pPlayer);
	void	AnimUp(void);
	void	GetSize(D3DXVECTOR3 &pos, float &fSizeX, float &fSizeY);
	void	SetDispSwmp(void);
	void	SetDispBoost(void);

	void	ChangeEffect(void);
	C2DAnim	*&SetAnim(bool bSet = true);
	void	SetSwmp(C2DAnim *&pAnim, float fRot = -99.9f);
	//変数宣言//***********************************************************************
	static LPDIRECT3DTEXTURE9	m_pTexAll[TEX_MAX];			// テクスチャ番号

	LPDIRECT3DTEXTURE9		m_pTex;			// テクスチャ番号
	CPlayer *m_pPlayer;
	EFFECT	m_Effect;
	float	m_fCntState;

	C2DAnim	*m_pDispAnim[EFFECT_MAX];	//全体エフェクト
	C2DAnim	*m_pAnim[DISP_ANIM_MAX];	//個別エフェクト
	EFFECT	m_EAnim[DISP_ANIM_MAX];		//個別のエフェクトタイプ
};
#endif
