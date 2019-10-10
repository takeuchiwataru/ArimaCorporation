//=============================================================================
//
// コンボ処理 [combo.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _COMBO_H_
#define _COMBO_H_

#include "scene2D.h"
#include "texture.h"
//=============================================================================
// 前方宣言
//=============================================================================
class CNumber;
class CLogo;
//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_COMBO		(2)		// コンボの桁数
#define COMBO_DISTIME	(180)	// 表示時間

//=============================================================================
// クラス定義
//=============================================================================
class CCombo : public CScene2D
{
public:
	typedef enum
	{
		TYPE_THROUGH = 0,	// すれ違い時
		TYPE_JUMP,			// ジャンプ時
		TYPE_DRIFT,			// ドリフト時
		TYPR_MAX
	}TYPE;

	CCombo();
	~CCombo();

	static CCombo *Create(TYPE type);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void AddCombo(int nCombo) { m_nComboNum += nCombo; }	// コンボの加算
	void ResetCombo(void) { m_nComboNum = 0; m_nCntFrame = 0; }		// コンボのリセット
	static int GetCombo(void) { return m_nComboNum; }
	int GetFream(void) {return m_nCntFrame;}					// コンボ時のフレーム取得
	void SetType(TYPE type) { CScene2D::BindTexture(*CTexture::GetTexture(CTexture::TYPE_THROUGH + (int)type)); }
	TYPE GetType(void) { return m_type; }
private:
	int PowerCalculation(int nData);		// べき乗の計算
	void NumMove(void);						// コンボ時の数字移動

	static int					m_nComboNum;				// コンボの総数
	int							m_nComboOld;
	int							m_nCntFrame;				// コンボ時のフレーム
	int							m_DisNumber;				// 表示する数(コンボ数用)
	int							m_nDiSTime;					// 表示時間
	D3DXVECTOR2					m_NumPos;					// コンボ数の現在位置
	D3DXVECTOR2					m_size;
	CNumber						*m_apNumber[MAX_COMBO];
	CLogo						*m_pLogo;
	TYPE						m_type;
};
#endif