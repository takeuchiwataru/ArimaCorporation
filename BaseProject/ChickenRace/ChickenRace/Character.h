//=============================================================================
//
// キャラクター処理 [Character.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "main.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define CHAR_PRIORITY	(0)
#define WOOD_DIS		(40.0f)				//木と木の間の距離（半分）
#define WOOD_LEN		(5.0f)				//木の長さ
//=============================================================================
// 前方宣言
//=============================================================================

//=============================================================================
// プレイヤークラス
//=============================================================================
class CCharcter : public CScene
{
public:
	//モーション種類
	typedef enum
	{
		ANIM_NEUTRAL = 0,		//ニュートラルモーション
		ANIM_MAX					//モーションの最大数
	}Anim;

	//色の状態の種類
	CCharcter(int nPriority = CHAR_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_WINDWOOD) : CScene(nPriority, objType) {};
	~CCharcter() {};
	static void Load(void);
	static void Unload(void);
	static void	FileLoad(void);
	static CCharcter	*Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static void	CollisionAll(CPlayer *pPlayer);

	void				Setting(D3DXVECTOR3 &pos, D3DXVECTOR3 &rot);
	HRESULT				Init(void);
	void				Uninit(void);
	void				Update(void);
	void				Draw(void);
	void				Collision(CPlayer *&pPlayer, D3DXVECTOR3 &Mypos);
	void				CollisionWood(CPlayer *&pPlayer);

	void UpdateMotion(void);
	
	float	&GetfCntState(void) { return m_fCntState; }
private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	float		m_fCntState;

	// モーション関数	新規
	static CPlayer::KEY_INFO	  *m_pKeyInfo[MAX_MOTION];		// キー情報へのポインタ
	static int					  m_nNumParts;					// パーツ数
	static int					  m_aIndexParent[MAX_PARTS];	// 親のインデックス
	static CPlayer::KEY			  m_aKayOffset[MAX_PARTS];		// オフセット情報
	static CPlayer::MOTION_INFO	  m_aMotionInfo[MAX_MOTION];	// モーション情報
	static LPDIRECT3DTEXTURE9	  m_pTexture;					// テクスチャ	
	static int					  m_nNumModel;					//モデル数
	static char					  m_aFileNameModel[MAX_PARTS][256];

	CModel						  **m_apModel;					// パーツモデルのポインタ
	int							  m_nKey;						// 現在のキーナンバー
	int							  m_nCountFlame;				// フレーム数
	int							  m_nMotionType;				// モーションのタイプ(int型)
	bool						  m_bMotionEnd;					// モーション終了
};
#endif
