//************************************************************
//
//	スタート合図の処理[startsignal.h]
//	Author : Shun Yokomichi
//
//************************************************************
#ifndef _STARTSIGNAL_H_
#define _STARTSIGNAL_H_

#include "main.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
// マクロ定義
//************************************************************
#define MAX_SIGNAL		(2)		// スタート合図の数
//************************************************************
// 前方宣言
//************************************************************
class CLogo;

//************************************************************
// クラス
//************************************************************
class CStartSignal
{
public:
	typedef enum
	{
		SIGNAL_HERE = 0,
		SIGNAL_WE,
		SIGNAL_GO,
		SIGNAL_NONE,
	}SIGNAL;

	CStartSignal();
	~CStartSignal();
	static CStartSignal *Create(D3DXVECTOR2 size, SIGNAL type);
	HRESULT Init();
	void Uninit();
	void Updata();
	void Draw();

	bool GetEnd(void) { return m_bEnd; }
private:
	void SignalUpdata(bool bFrag, int nType);
	void StartUpdata(void);

	CLogo			*m_pSignal[MAX_SIGNAL];
	CLogo			*m_pStart;

	SIGNAL			m_SignalType;		// 合図の状態
	D3DXVECTOR2		m_size;				// サイズ
	float			m_fColA;			// 透明度の色
	bool			m_bEnd;				// 終了のお知らせ
	bool			m_bColA;			
	int				m_nCntDisplay;		// 表示時間
};

#endif // !_STARTSIGNAL_H_
