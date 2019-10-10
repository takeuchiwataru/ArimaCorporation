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
#define MESHFILE_NAME			"data\\TEXT\\MeshCreate.txt"	//読み込むtxtファイルの名前
//#define MESH_DATA				(10)

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
	static CMeshField *Create(D3DXVECTOR3 pos, int nMeshX, int nMeshZ, float fMeshXUV, float fMeshYUV, float fMeshWidth, float fMeshDepth,float fVtx0, float fVtxMeshX, float fVtxMeshZ, float fVtxMeshXMeshZ, int nTexTypeint,int nMeshType);
	/*static*/ int GetMeshType(void) { return m_nMeshType; }
	void DeleteMeshField(void);

private://個人でのみ使う
	//typedef struct
	//{
	//	D3DXVECTOR3			InitPos;					//Posの初期値
	//	int					nDivision_X;				//分割数X
	//	int					nDivision_Z;				//分割数Z
	//	float				fDepth;						//奥行
	//	float				fWidth;						//横幅
	//	int					nTexType;					//テクスチャのTypeの設定
	//	int					nMeshType;					//メッシュフィールドかメッシュシリンダーの種類
	//}MeshField;

	static LPDIRECT3DTEXTURE9		m_pTexture[MAX_MESH_TEXTURE];	// テクスチャへのポインタ
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;						// 頂点バッファへのポインタ
	static LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;						// インデックスへのポインタ
	int								m_nCnt;
	int								m_nType;						// メッシュフィールドかシリンダー
	int								m_nMeshType;					// メッシュの種類
	float							m_fVtx0;						// pVtx[0]番目の高さを変える
	float							m_fVtxMeshX;					// pVtx[MeshX + 1]番目の高さを変える
	D3DXVECTOR3						m_pos;							// 位置

};
#endif