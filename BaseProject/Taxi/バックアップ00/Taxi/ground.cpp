//=============================================================================
//
// 川の処理 [ground.cpp]
// Author : 有馬　武志
//
//=============================================================================
#include "ground.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "mesh.h"
#include "scene3D.h"
#include "game.h"
#include "player.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define GROUND_TEX_1		"data\\TEXTURE\\海.png"	// ファイルの名前
#define GROUND_MOVE_SPEED	(0.002f)				// テクスチャの移動速度の調整用

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CGround::m_pTexture[MAX_GROUND_TEX] = {};
LPDIRECT3DVERTEXBUFFER9	CGround::m_pVtxBuff = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CGround::CGround() : CScene3D(GROUND_PRIOTITY, CScene::OBJTYPE_GROUND)
{
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_fTexMoveSpeed = 0.0f;
	m_bSound = false;
}
//=============================================================================
// デストラクタ
//=============================================================================
CGround::~CGround()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGround::Init()
{
	CScene3D::Init();

	//種類の設定
	SetTypeNumber(TYPE_GROUND);
	m_fTexMoveSpeed = GROUND_MOVE_SPEED;

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CGround::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_GROUND_TEX; nCount++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}

	CScene3D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CGround::Update(void)
{
	//描画状態の取得
	bool bDraw = CScene::GetDraw();

	//描画状態ではないなら更新しない
	if (!bDraw) { return; }

	CScene3D::Update();

	//変数
	D3DXVECTOR2 UV[4];

	m_nCnt++;

	if (m_rot.x != 0.0f)
	{
		UV[0] = D3DXVECTOR2(0.0f - (m_nCnt * 0.025f), 1.0f);
		UV[1] = D3DXVECTOR2(0.0f - (m_nCnt * 0.025f), 0.0f);
		UV[2] = D3DXVECTOR2(1.0f - (m_nCnt * 0.025f), 1.0f);
		UV[3] = D3DXVECTOR2(1.0f - (m_nCnt * 0.025f), 0.0f);

		CScene3D::SetNor(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	}
	else if (m_rot.z != 0.0f)
	{
		UV[0] = D3DXVECTOR2(0.0f + (m_nCnt * 0.025f), 0.0f);
		UV[1] = D3DXVECTOR2(1.0f + (m_nCnt * 0.025f), 0.0f);
		UV[2] = D3DXVECTOR2(0.0f + (m_nCnt * 0.025f), 1.0f);
		UV[3] = D3DXVECTOR2(1.0f + (m_nCnt * 0.025f), 1.0f);
	}
	else
	{
		if (m_move.x != 0.0f)
		{// テクスチャ設定　左右に流れる
			UV[0] = D3DXVECTOR2(0.0f - (m_nCnt * (m_move.x * m_fTexMoveSpeed)), 0.0f);
			UV[1] = D3DXVECTOR2(5.0f - (m_nCnt * (m_move.x * m_fTexMoveSpeed)), 0.0f);
			UV[2] = D3DXVECTOR2(0.0f - (m_nCnt * (m_move.x * m_fTexMoveSpeed)), 1.0f);
			UV[3] = D3DXVECTOR2(5.0f - (m_nCnt * (m_move.x * m_fTexMoveSpeed)), 1.0f);
		}
		if (m_move.z != 0.0f)
		{// テクスチャ設定 上下に流れる
			UV[0] = D3DXVECTOR2(0.0f + (m_nCnt * (m_move.z * m_fTexMoveSpeed)), 1.0f);
			UV[1] = D3DXVECTOR2(0.0f + (m_nCnt * (m_move.z * m_fTexMoveSpeed)), 0.0f);
			UV[2] = D3DXVECTOR2(1.0f + (m_nCnt * (m_move.z * m_fTexMoveSpeed)), 1.0f);
			UV[3] = D3DXVECTOR2(1.0f + (m_nCnt * (m_move.z * m_fTexMoveSpeed)), 0.0f);
		}
		if (m_move.z == 0.0f && m_move.x == 0.0f)
		{// テクスチャ設定 動きなし
			UV[0] = D3DXVECTOR2(0.0f, 0.0f);
			UV[1] = D3DXVECTOR2(1.0f, 0.0f);
			UV[2] = D3DXVECTOR2(0.0f, 1.0f);
			UV[3] = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	//背景のUV
	CScene3D::SetUV(&UV[0]);
}

//=============================================================================
// 描画処理
//=============================================================================
void CGround::Draw(void)
{
	//CScene3D::Draw();

	CScene3D::Draw();
}
//===============================================================================
//　クリエイト
//===============================================================================
CGround * CGround::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXVECTOR2 size, int nType)
{
	CGround *pGround = NULL;

	//NULLチェック
	if (pGround == NULL)
	{//メモリの動的確保

		pGround = new CGround;

		if (pGround != NULL)
		{
			//初期化処理
			pGround->Init();
			//当たり判定用
			pGround->m_size = size;
			//種類の設定
			pGround->m_nType = nType;
			pGround->SetTexType(nType);
			//位置と大きさの設定
			pGround->SetGroundPosSize(pos, size);
			pGround->m_pos = pos;
			// 移動量の設定
			pGround->SetMove(move);
			pGround->m_move = move;
			// 角度の設定
			pGround->SetRotation(rot * D3DX_PI);
			pGround->m_rot = rot;
			//テクスチャの設定
			pGround->BindTexture(m_pTexture[nType]);
		}
	}

	return pGround;
}
//=============================================================================
// 読み込み処理
//=============================================================================
HRESULT CGround::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャ生成
	D3DXCreateTextureFromFile(pDevice, GROUND_TEX_1, &m_pTexture[0]);

	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CGround::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_GROUND_TEX; nCount++)
	{
		// テクスチャ破棄
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
// 川の流れによる移動量の変化
//=============================================================================
D3DXVECTOR3 CGround::RiverMove(void)
{
	return m_move * 0.5f;
}

//=============================================================================
// 川の音の再生
//=============================================================================
void CGround::Sound(bool bPlay)
{
	if (bPlay)
	{// 音を再生する
		if (!m_bSound)
		{// 音を再生していないとき
			CSound *pSound = CManager::GetSound();
			pSound->PlaySound(CSound::SOUND_LABEL_SE_RIVER);
			m_bSound = true;
		}
	}
	else
	{// 音を消す
		if (m_bSound)
		{// 音を再生しているとき
			CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_RIVER);
			m_bSound = false;
		}
	}
}