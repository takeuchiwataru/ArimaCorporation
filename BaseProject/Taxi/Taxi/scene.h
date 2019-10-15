//=============================================================================
//
// メイン処理 [scene.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_PRIORITY	(8)			//優先順位の最大数

//=====================
//	   基本クラス
//=====================
class CScene
{
public://誰でも扱える
	typedef enum
	{
		OBJTYPE_NONE = 0,		//使っていない状態
		OBJTYPE_BILLBOORD,		//ビルボード
		OBJTYPE_EFFECT,			//エフェクト
		OBJTYPE_2DPOLYGON,		//2D画像
		OBJTYPE_3DPOLYGON,		//3D画像
		OBJTYPE_3DMODEL,		//3Dモデル
		OBJTYPE_OBJECT,			//3Dオブジェクト
		OBJTYPE_SHADOWMODEL,	//ステンシルシャドウ
		OBJTYPE_PAUSE,			//ポーズ
		OBJTYPE_SERECT,			//セレクト
		OBJTYPE_FADE,			//フェード
		OBJTYPE_PLAYER,			//プレイヤー
		OBJTYPE_ENEMY,			//敵
		OBJTYPE_WALL,			//壁
		OBJTYPE_GROUND,			//地面
		OBJTYPE_MESH,			//メッシュ
		OBJTYPE_MESHORBIT,		//軌跡
		OBJTYPE_MESHFILED,		//メッシュフィールド
		OBJTYPE_SHADOW,			//影
		OBJTYPE_LOGO,			//UI系
		OBJTYPE_TIME,			//時間
		OBJTYPE_SCORE,			//スコア
		OBJTYPE_TOTALSCORE,		//トータルスコア
		OBJTYPE_CYLINDER,		//シリンダー
		OBJTYPE_FEED,			//餌
		OBJTYPE_MAX,			//最大数
	}OBJTYPE;

	CScene(int nPriority = 3, OBJTYPE objType = OBJTYPE_NONE);
	virtual ~CScene();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	static void ReleseAll(void);
	static void NotFadeReleseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	void SetObjType(OBJTYPE ObjType);
	static CScene *&GetTop(int nPriority);
	CScene *&GetNext(void) { return m_pNext; }
	bool &GetDeath(void) { return m_pDeath; }
	OBJTYPE GetObjType(void);
	static CScene *GetScene(int nPriNumber);
	static void SetUpdatePri(int nUpdatePri);

	//描画フラグ
	void SetDraw(bool bDraw) { m_bDraw = bDraw; };
	bool GetDraw(void) { return m_bDraw; };

private://個人でのみ使う
	static CScene   *m_pTop[NUM_PRIORITY];	//先頭のオブジェクトへのポインタ
	static CScene   *m_pCur[NUM_PRIORITY];	//現在のオブジェクトへのポインタ
	CScene			*m_pPrev;				//前のオブジェクトへのポインタ
	CScene			*m_pNext;				//次のオブジェクトへのポインタ
	OBJTYPE			m_ObjType;				//オブジェクトの種類
	static int		m_nNumAll;				//敵の総数
	int				m_nID;					//番号
	int				m_nPriority;			//優先順位の番号
	bool			m_pDeath;				//死亡フラグ
	bool			m_bDraw; ;				//描画フラグ
	static int      m_nUpdatePri;
	static bool		m_bOnOff;

protected:
	void Release(void);
	void Delete(void);
};
#endif