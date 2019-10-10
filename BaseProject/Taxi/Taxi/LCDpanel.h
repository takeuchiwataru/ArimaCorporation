//=============================================================================
//
// 液晶パネルの処理 [LDCpanel.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _LCDPANEL_H_
#define _LCDPANEL_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LCD_TEXTURE		(11)			// テクスチャ数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObject;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CLCDpanel : public CScene3D
{
public:
	CLCDpanel();
	~CLCDpanel();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fScale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);
	static void Unload(void);

	static CLCDpanel* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fScale = 1.0f);	// 生成処理

	float GetScale(void) { return m_fScale; };
	void SetScale(float fScale);

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[MAX_LCD_TEXTURE];		// テクスチャへのポインタ
	int								m_nCntDisp;			// 表示画面切替用カウンタ
	int								m_nCurTex;			// 現在表示しているテクスチャ番号
	CObject							*m_pObject;			// 液晶の筐体のモデル
	float							m_fScale;			// スケールの倍率
};
#endif