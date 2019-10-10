//=============================================================================
//
// 3Dパーティクル処理 [3Dparticle.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _3DPARTICLE_H_
#define _3DPARTICLE_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CRotationShader;


//=============================================================================
// 3Dポリゴンクラス
//=============================================================================
class C3DParticle : public CScene
{
public:
	C3DParticle();
	C3DParticle(int nPriority, CScene::OBJTYPE obj);
	~C3DParticle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static C3DParticle * Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; };	//テクスチャの割当て

	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };					//位置の更新
	void SetColor(D3DXCOLOR col);
	void SetMoveTex(float fMoveX, float fMoveY);
	void SetDivision(D3DXVECTOR2 Division) { m_Division = Division; };			//テクスチャの分割数
	void SetUV(int nWidth, int nHight);											//UVの設定
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };								//向きの設定
	D3DXVECTOR3 GetRot(void) { return m_rot; }									//向きの取得
	D3DXVECTOR3 GetPos(void) { return m_pos; };									//位置の取得

private:
	//変数宣言

	//メンバ変数
	LPDIRECT3DTEXTURE9		m_pTexture;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;				//中心座標
	D3DXVECTOR3				m_rot;				//向き
	D3DXVECTOR3				m_size;				//ポリゴンのサイズ
	D3DXVECTOR3				m_vtxMin;			//ポリゴンの最小値
	D3DXVECTOR3				m_vtxMax;			//ポリゴンの最小値
	D3DXMATRIX				m_mtxWorld;			//ワールドマトリックス
	D3DXVECTOR2				m_Division;			//分割数
	CRotationShader *		m_pRotationShader;	//シェーダー情報

	D3DXVECTOR3 m_nor[2];
};
#endif
