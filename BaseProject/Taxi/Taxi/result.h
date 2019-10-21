//=============================================================================
//
// リザルト処理 [result.h]
// Author : 有馬 武志
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "meshfield.h"
#include "object.h"
#include "model.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RANKING				(5)		// 満足度の数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayer;
class CObject;
class CModel;
class CMeshField;
class CPolygon3D;
class CBillBoord;
class CEffect;
class CWall;
class CItem;
class CShadow;
class CResultCamera;
class CLoadTextMotion;
class CTotalScore;

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
	static void	Draw(void);						// 描画処理

	//ファイルを読み込む
	void TextLoad(int nLoadNumber);
	void MeshTextLoad(int nLoadNumber);

	char *ReadLine(FILE *pFile, char *pDst);	//1行読み込み
	char *GetLineTop(char *pStr);				//行の先頭を取得
	int  PopString(char *pStr, char *pDest);	//行の最後を切り捨て

	// トータルスコアと満足度の取得
	static void SetGrades(int nTotalScore, int nLevel) { m_nGetTotalScoer = nTotalScore; m_nGetLevel = nLevel; }

	static CLoadTextMotion * GetPlayerMotion(void) { return m_pPlayerMotion; }	//プレイヤーのモーションの取得
	static CResultCamera * GetCamera(void) { return m_pCamera; }
	static int GetFirstScore(void) { return m_aTotalScore[0]; }

private:
	//テキストの値を管理する場所
	typedef struct
	{
		int					m_nType;			// 種類
		int					m_nTexType;			// テクスチャの種類
		int					m_nCollision;		// 当たり判定のONOFF
		D3DXVECTOR3			m_pos;				// 位置
		D3DXVECTOR3			m_rot;				// 回転
		D3DXVECTOR3			m_scale;			// サイズ
	}Map;

	//テキストの値を管理する場所
	typedef struct
	{
		int					m_nTexType;						// テクスチャの種類
		int					m_nWidthDivide;					// 横の分割数を入れる
		int					m_nDepthDivide;					// 奥の分割数を入れる
		float				m_fWidthLength;					// 横の長さ
		float				m_fDepthLength;					// 奥の長さ
		float				m_fTexXUV;						// XUV座標
		float				m_fTexYUV;						// YUV座標
		float				m_fVtxHeight_No0;				// 頂点番号0を操作
		float				m_fVtxHeight_No1;				// 頂点番号1を操作
		float				m_fVtxHeight_No2;				// 頂点番号2を操作
		float				m_fVtxHeight_No3;				// 頂点番号3を操作
		float				m_fVtxSide_No0;					// 頂点番号0を操作
		float				m_fVtxSide_No1;					// 頂点番号1を操作
		float				m_fVtxSide_No2;					// 頂点番号2を操作
		float				m_fVtxSide_No3;					// 頂点番号3を操作
		D3DXVECTOR3			m_pos;							// 位置
	}Mesh;

	//テキストの値を管理する場所
	typedef struct
	{
		D3DXVECTOR3			m_pos;				// 位置
		D3DXVECTOR3			m_rot;				// 回転
		float				m_fWidthDivide;		// 横幅
		float				m_fHightDivide;		// 縦幅
		int					m_nTexType;			// テクスチャの種類
	}Wall;

	//テキストの値を管理する場所
	typedef struct
	{
		int					m_nType;			// 種類
		int					m_nTexType;			// テクスチャの種類
		int					m_nCollision;		// 当たり判定のONOFF
		D3DXVECTOR3			m_pos;				// 位置
		D3DXVECTOR3			m_rot;				// 回転
		D3DXVECTOR3			m_scale;			// サイズ
		int					m_nRoute;			// ルートの設定
	}Npc;

	//テキストの値を管理する場所
	typedef struct
	{
		int					nNum;				// 番号
		D3DXVECTOR3			pos;				// 位置
	}Point;

	void LoadScore(void);

	static CMeshField		*m_pMeshField;		// メッシュフィールドの ポインタ
	static CObject			*m_pObject;			// オブジェクトの ポインタ
	static CResultCamera	*m_pCamera;			// リザルトカメラへのポインタ
	static int				m_nGetTotalScoer;	// トータルスコア取得用
	static int				m_nGetLevel;		// 満足度レベル取得用
	static CLoadTextMotion	*m_pPlayerMotion;	//プレイヤーのモーション読み込み
	static int			m_aTotalScore[MAX_RANKING];

	int						m_nCntTime;			// カウンター
	int						m_nSetCnt;
	bool					m_bPress;			// 押下したかどうか
	int m_nSetObjectNum;					//オブジェクトを置いた数
	int m_nSetMeshFieldNum;					//メッシュフィールドを置いた数
	int	m_nSetWallNum;						//壁の置いた数
	Map m_Map[MAX_MAP_OBJECT];					//設置するオブジェクトの構造体
	Mesh m_Mesh[MAX_MAP_MESH];				//設置するメッシュフィールドの構造体
	Wall m_aWall[MAX_MAP_WALL];				//設置する壁の構造体

};
#endif