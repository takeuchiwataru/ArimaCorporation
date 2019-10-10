//=============================================================================
//
// 人NPCの処理 [humannpc.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _CHUMANNPC_H_
#define _CHUMANNPC_H_

#include "main.h"
#include "humanbace.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_PARTS	(7)		//読み込むパーツ数

//=============================================================================
// 人NPCクラス
//=============================================================================
class CHumanNPC : public CHumanBace
{
public:
	CHumanNPC();
	~CHumanNPC();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CHumanNPC * Create(D3DXVECTOR3 pos, MODEL_TYPE type);

private:
	void Escape(void);	//逃げる処理

	bool m_bEscape;		//逃げたかどうか
};
#endif;