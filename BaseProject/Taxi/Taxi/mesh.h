//=============================================================================
//
// メッシュ処理 [meshf.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _MESH_H_
#define _MESH_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MESH_TEXTURE			(3)
#define ANGLE						(1.0f / (CORSS/2))			// 指定した数文のPIを出す式
#define MESH_PRIOTITY				(2)

//=====================
//  CSceneの派生クラス
//=====================
class CMesh : public CScene
{
public://誰でも扱える
	CMesh();
	~CMesh();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMesh *Create();
	void BindTexture(LPDIRECT3DTEXTURE9 Texture);
	void TextSave(void);
	void TextLoad(void);
	void SetPosition(D3DXVECTOR3 pos);
	void SetMeshInfo(int nMeshX, int nMeshZ, float fMeshTexX, float fMeshTexY, float fMeshDepth, float fMeshWidth);
	void SetMeshType(int nType) { m_nType = nType; }
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetVtx(float fVtx0, float fVtxMeshX, float fVtxMeshZ, float fVtxMeshXMeshZ) { m_fVtx0 = fVtx0; m_fVtxMeshX = fVtxMeshX; m_fVtxMeshZ = fVtxMeshZ; m_fVtxMeshXMeshZ = fVtxMeshXMeshZ; }
	void SetVtxSide(float fVtxSide0, float fVtxSide1, float fVtxSide2, float fVtxSide3) { m_fVtxSide0 = fVtxSide0; m_fVtxSide1 = fVtxSide1; m_fVtxSide2 = fVtxSide2; m_fVtxSide3 = fVtxSide3; }
	void Nor(void);
	bool GetHeight(D3DXVECTOR3 &pos);
	float GetHeightMesh(D3DXVECTOR3 pos);
	void SetHeight(D3DXVECTOR3 pos, float fValue, float fRange);

	D3DXVECTOR3 GetNor(D3DXVECTOR3 pos);						// 地面の法線を取得
	float GetInclineAngle(D3DXVECTOR3 pos, float fAngle);		// 傾斜の角度の取得
	bool OnField(D3DXVECTOR3 pos, float fMaxHeight);	// 現在いる地面かどうかの判定

private://個人でのみ使う
	LPDIRECT3DTEXTURE9		m_pTexture;							// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;							// インデックスへのポインタ
	D3DXVECTOR3				m_pos;								// 位置
	D3DXVECTOR3				m_rot;								// 向き
	D3DXMATRIX				m_mtxWorld, m_local;				// ワールドマトリックス
	int						m_nType;							// メッシュフィールドかシリンダーの種類
	int						m_nMeshX;							// メッシュのX分割数
	int						m_nMeshZ;							// メッシュのZ分割数
	float					m_fMeshTexX;						// メッシュのXUV座標
	float					m_fMeshTexY;						// メッシュのYUV座標
	float					m_fMeshDepth;						// メッシュの奥行
	float					m_fMeshWidth;						// メッシュの横幅
	float					m_fVtx0;							// pVtx[0]番目の高さを変える
	float					m_fVtxMeshX;						// pVtx[MeshX + 1]番目の高さを変える
	float					m_fVtxMeshZ;						// pVtx[MeshZ + 1]番目の高さを変える
	float					m_fVtxMeshXMeshZ;					// pVtx[MeshXZ + 1]番目の高さを変える
	float					m_fVtxSide0;						// pVtx[0]番目の高さを変える
	float					m_fVtxSide1;						// pVtx[0]番目の高さを変える
	float					m_fVtxSide2;						// pVtx[0]番目の高さを変える
	float					m_fVtxSide3;						// pVtx[0]番目の高さを変える

};
#endif