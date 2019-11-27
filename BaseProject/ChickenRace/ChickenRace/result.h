//=============================================================================
//
// リザルト処理 [result.h]
// Author : 有馬 武志
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
//#include "meshfield.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MAP_OBJECT		(3500)
#define RESULT_WAIT			(120)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CResultCamera;

class CResultUI;

//=====================
// 基本クラス
//=====================
class CResult
{
public:
	CResult();								// コンストラクタ
	~CResult();								// デストラクタ
	HRESULT Init();							// 初期化処理
	void	Uninit(void);					// 終了処理
	void	Update(void);					// 更新処理
	static void	Draw(void);					// 描画処理

	static int GetCounter(void) { return m_nResultCounter; }

	static CResultCamera * GetResultCamera(void) { return m_pResultCamera; }

private:
	typedef struct
	{
		int					m_nType;			// 種類
		int					m_nTexType;			// テクスチャの種類
		int					m_nCollision;		// 当たり判定のONOFF
		D3DXVECTOR3			m_pos;				// 位置
		D3DXVECTOR3			m_rot;				// 回転
		D3DXVECTOR3			m_scale;			// サイズ
	}Map;

	//---------------------------------
	//ファイルを読み込む
	//---------------------------------
	void TextLoad(int nLoadNumber);

	char *ReadLine(FILE *pFile, char *pDst);	//1行読み込み
	char *GetLineTop(char *pStr);				//行の先頭を取得
	int  PopString(char *pStr, char *pDest);	//行の最後を切り捨て

	int m_nSetObjectNum;						//オブジェクトを置いた数

	Map m_Map[MAX_MAP_OBJECT];					//設置するオブジェクトの構造体

	static int			m_nResultCounter;	// タイトルのカウンター

	static CResultCamera	*m_pResultCamera;	//リザルトカメラのポインタ

	CResultUI			*m_pResultUI;		// リザルトUI
};
#endif