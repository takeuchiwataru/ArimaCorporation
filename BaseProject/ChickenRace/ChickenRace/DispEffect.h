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
#define DISP_ANIM_MAX	(12)
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
		TEX_SWAMP		, TEX_SWAMP_ONE,	//水たまり
		TEX_BOOST		, 					//加速
		TEX_FootSteps	, 
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

	void	SetAnim(void);
	void	SetSwmp(C2DAnim *&pAnim);
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
