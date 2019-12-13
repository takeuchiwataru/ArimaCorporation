//=============================================================================
//
// モデル専用の処理 [model.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
#include "toonshader.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MODEL			(25)
#define MAX_MODEL_TEXTURE	(4)

//=====================
//  CModel 単独クラス
//=====================
class CModel
{
public://誰でも扱える

	typedef enum
	{// オブジェクトの種類
		PARTS_CHICKEN_BODY = 0,		//体
		PARTS_CHICKEN_HEAD,			//頭
		PARTS_CHICKEN_ASS,			//尻尾
		PARTS_CHICKEN_WINGSR_0,		//右羽元
		PARTS_CHICKEN_WINGSR_1,		//右羽先
		PARTS_CHICKEN_WINGSL_0,		//左羽元
		PARTS_CHICKEN_WINGSL_1,		//左羽先
		PARTS_CHICKEN_LEGR,			//右腿
		PARTS_CHICKEN_FOOTR,		//右足
		PARTS_CHICKEN_LEGL,			//左腿
		PARTS_CHICKEN_FOOTL,		//左足

		PARTS_CHICKEN_11,			//ハッピーボーイ
		PARTS_CHICKEN_12,			//ハット帽子
		PARTS_CHICKEN_13,			//フェザーボーン
		PARTS_CHICKEN_14,			//海賊帽子
		PARTS_CHICKEN_15,			//兜
		PARTS_CHICKEN_16,			//ヘルメット
		PARTS_CHICKEN_17,			//ベレー帽
		PARTS_CHICKEN_18,			//ピエロ帽子

		PARTS_CHICK_BODY,			//体
		PARTS_CHICK_HEAD,			//頭
		PARTS_CHICK_LEGR,			//右腿
		PARTS_CHICK_FOOTR,			//右足
		PARTS_CHICK_LEGL,			//左腿
		PARTS_CHICK_FOOTL,			//左足

		PARTS_WOOD_LEG,				//足
		PARTS_WOOD_BODY_R,			//右半身
		PARTS_WOOD_LONG_R,			//長い右腕
		PARTS_WOOD_SHORT_R,			//短い右腕
		PARTS_WOOD_LEAF_R,			//右の草
		PARTS_WOOD_BODY_L,			//左半身
		PARTS_WOOD_LONG_L,			//長い左腕
		PARTS_WOOD_SHORT_L,			//短い左腕
		PARTS_WOOD_LEAF_L,			//左の草
		PARTS_MAX,					//最大数
	}PARTS_TYPE;

	typedef enum
	{// オブジェクトの種類
		TYPE_CHICKEN = 0,			//鳥
		TYPE_CHICK,					//ひよこ
		TYPE_WOOD,
		TYPE_MAX,					//
	}TYPE;

	typedef enum
	{// テクスチャの種類
		TEX_HAPPYBOY = 0,			//ハッピボーイ
		TEX_HAT,					//ハット帽子
		TEX_FEATHERBONE,			//フェザーボーン
		TEX_PIRATE,					//海賊
		TEX_KABUTO,					//兜
		TEX_HELMET,					//ヘルメット
		TEX_BERET,					//ベレー
		TEX_CLOWN,					//ピエロ
		TEX_MAX,					//最大数
	}TEXTURE;


	CModel();
	~CModel();
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(float fAlpha);
	static CModel * Create(const D3DXVECTOR3 pos, char FileName[40], D3DXVECTOR3 Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }

	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_Rot; }

	void SetFirstPos(D3DXVECTOR3 FirstPos) { m_FirstPos = FirstPos; };
	void SetParent(CModel *pModel) { m_pParent = pModel; }
	void AddPos(D3DXVECTOR3 Pos) { m_Pos = Pos + m_FirstPos; };
	void BindModel(LPD3DXMESH mesh, LPD3DXBUFFER buff, DWORD nummat) { *m_pMesh = mesh; *m_pBuffMat = buff; *m_nNumMat = nummat; }
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTextures = pTexture; }
	D3DXMATRIX &GetMtxWorld(void) { return m_mtxWorld; }
	D3DXVECTOR3 GetPos(void) { return m_Pos; }

	static D3DXVECTOR3 GetVtxMax(int nNumber) { return m_VtxMax[nNumber]; }
	static D3DXVECTOR3 GetVtxMin(int nNumber) { return m_VtxMin[nNumber]; }
	static int &GetnModelMax(TYPE type) { return m_nModelMax[type]; }
	static LPD3DXBUFFER &GetpBuffMat(int nNumber) { return m_pBuffMat[nNumber]; }
	static LPD3DXMESH &GetpMesh(int nNumber) { return m_pMesh[nNumber]; }
	static DWORD &GetnNumMat(int nNumber) { return m_nNumMat[nNumber]; }

	D3DXVECTOR3 GetScale(void) { return m_Scale; }
	void SetColor(D3DXCOLOR col);
	static void Load(void);
	static void SetParts(void);
	static void ParentModel(CModel **&apModel, TYPE type, int nSelectParts = 0);
	static void PartsTypeUnLoad(void);
	void SetType(PARTS_TYPE partstype) { m_Type = partstype; }

private://派生クラスでも使える
	void CreateXFile(char FileName[40]);									//Xファイルの生成
	static D3DMATERIAL9			*m_pMeshMaterials[PARTS_MAX];
	static LPDIRECT3DTEXTURE9	*m_pShaderMeshTextures[PARTS_MAX];			//シェーダー用
	static LPDIRECT3DTEXTURE9	*m_pMeshTextures[MAX_MODEL_TEXTURE];		// シェーダー用
	static LPD3DXBUFFER			m_pBuffMat[PARTS_MAX];						//マテリアルの情報へのポインタ
	static LPD3DXMESH			m_pMesh[PARTS_MAX];							//メッシュ情報へのポインタ
	static DWORD				m_nNumMat[PARTS_MAX];						//マテリアルの情報数
	static D3DXVECTOR3			m_VtxMin[PARTS_MAX], m_VtxMax[PARTS_MAX];							//モデルの最小値・最大値
	static PARTS_TYPE			*m_partstype[TYPE_MAX];
	static int					m_nModelMax[TYPE_MAX];								// モデルの種類

	PARTS_TYPE					m_Type;										// モデルの種類
	LPDIRECT3DTEXTURE9			m_pTextures;								//テクスチャ
	D3DXMATRIX					m_mtxWorld;									//ワールドマトリックス
	D3DXVECTOR3					m_Pos;										//位置
	D3DXVECTOR3					m_Rot;										//向き
	D3DXVECTOR3					m_FirstPos;									//初期位置
	D3DXVECTOR3					m_Scale;									//拡大、縮小率
	CModel						*m_pParent;									//親モデルへのポインタ
	bool						m_bTexMat;									//シェーダーに使うテクスチャとマテリアル

protected://個人のみ使える

};
#endif