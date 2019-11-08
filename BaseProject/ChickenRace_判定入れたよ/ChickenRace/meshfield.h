//=============================================================================
//
// メッシュ処理 [meshfield.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "mesh.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHFILE_NAME			"data\\TEXT\\ステージ6\\meshfield.txt"	//読み込むtxtファイルの名前
#define MESHFIELD_PRIOTITY		(2)

//=====================
//  CMeshの派生クラス
//=====================
class CMeshField : public CMesh
{
public://誰でも扱える
	CMeshField();
	~CMeshField();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void TextSave(void);
	static CMeshField *Create(D3DXVECTOR3 pos, int nMeshX, int nMeshZ, float fMeshXUV, float fMeshYUV, float fMeshWidth, float fMeshDepth, float fVtx0, float fVtxMeshX, float fVtxMeshZ, float fVtxMeshXMeshZ, float fVtxSide0, float fVtxSide1, float fVtxSide2, float fVtxSide3, int nTexTypeint, int nMeshType);
	static void CreateMesh(void);

private://個人でのみ使う
	static LPDIRECT3DTEXTURE9		m_pTexture[MAX_MESH_TEXTURE];	//テクスチャへのポインタ
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;						//頂点バッファへのポインタ
	static LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;						//インデックスへのポインタ
	int								m_nCnt;							//カウンター
	int								m_nType;						//メッシュの種類
	float							m_fVtx0;						//pVtx[0]番目の高さを変える
	float							m_fVtxMeshX;					//pVtx[MeshX + 1]番目の高さを変える
	D3DXVECTOR3						m_pos;
	
	//ファイル書き込み用変数
	int								m_nWidthDivide;					//横の分割数を入れる
	int								m_nDepthDivide;					//奥の分割数を入れる
	float							m_fTextXUV;						//横のテクスチャ座標
	float							m_fTextYUV;						//縦のテクスチャ座標
	float							m_fWidthLength;					//横の長さ
	float							m_fDepthLength;					//奥の長さ
	int								m_nSelectNum;					//選んでいるもの
	int								m_nTexType;						//テクスチャの種類
	int								m_nVtxNumber;					//選択している頂点の番号
	float							m_fVtxHeight_No0;				//頂点番号0を操作
	float							m_fVtxHeight_No1;				//頂点番号1を操作
	float							m_fVtxHeight_No2;				//頂点番号2を操作
	float							m_fVtxHeight_No3;				//頂点番号3を操作
	float							m_fVtxSide_No0;					//頂点番号0を操作
	float							m_fVtxSide_No1;					//頂点番号1を操作
	float							m_fVtxSide_No2;					//頂点番号2を操作
	float							m_fVtxSide_No3;					//頂点番号3を操作
};
#endif