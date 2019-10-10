//=============================================================================
//
// 目的地の処理 [destination.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _DESTINATION_H_
#define _DESTINATION_H_

#include "main.h"
#include "billboord.h"
#include "scene2D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CLogo;

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=====================
//  CScene2Dの派生クラス
//=====================
class CDestination : public CScene2D
{
public://誰でも扱える
	typedef enum
	{	// 目的地の種類
		DESTINATION_SAPPRO = 0,		// 札幌
		DESTINATION_MAIN_STREET,	// 大通り
		DESTINATION_TV_TOWER,		// テレビ塔
		DESTINATION_CLOCKTOWER,		// 時計塔
		DESTINATION_TANUKIKOJI,		// 狸小路
		DESTINATION_NORVESA,		// ノルベサ
		DESTINATION_RED_BRICK,		// 赤レンガ
		DESTINATION_SUSUKI_SRREET,	// すすきの通り
		DESTINATION_FOUNTAIN,		// 噴水
		DESTINATION_TAPIOCAR ,		// タピオカ―
		DESTINATION_HOSPITAL,		// 病院
		DESTINATION_MAX
	}GOALTYPE;

	CDestination();
	~CDestination();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CDestination *Create(int nDestination);
	static void SetLogoState(int nStateType, int nStateFont);

	void SetDelet(bool bDelet) { m_bDelet = bDelet; }
	bool GetDelet(void) { return m_bDelet; }
private:
	static int					m_nNextType;				//状態を貰うときのタイプ
	static int					m_nNextFont;				//状態の文字
	int							m_nStateType;				//状態を貰うときのタイプ
	int							m_nStateFont;				//状態の文字
	int							m_nDestination;				//目的地のタイプ
	D3DXVECTOR2					m_size;						//サイズ
	float						m_fLength;					//距離
	float						m_fAngle;					//角度
	D3DXVECTOR3					m_rot;						//向き
	int							m_nCntTime;
	bool						m_bDelet;					//表示を消すかどうか
	CLogo						*m_pLogo;					// ロゴへのポインタ
};

#endif