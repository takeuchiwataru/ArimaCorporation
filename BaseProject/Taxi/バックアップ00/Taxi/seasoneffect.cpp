//=============================================================================
//
// 季節エフェクトの処理 [seasoneffect.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "scene.h"
#include "seasoneffect.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define VECTOR3_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// D3DXVECTOR3の初期値
#define VECTOR2_ZERO		(D3DXVECTOR2(0.0f, 0.0f))			// D3DXVECTOR2の初期値
#define MOVE				(D3DXVECTOR3(4.0f, 2.5f, 0.0f))		// 移動量
#define MOVE_CHANGE_TAIME	(70)								// 移動量を変える時間
#define ADD_ROT_X			(0.05f)								// 回転量(X軸)
#define ADD_ROT_Y			(0.1f)								// 回転量(X軸)
#define PRIORTYV_4			(4)									// 優先順位(4)
#define PRIORTYV_6			(6)									// 優先順位(6)
#define TEX_ANIM_X			(4)									// テクスチャアニメーションX軸
#define TEX_ANIM_Y			(1)									// テクスチャアニメーションY軸
#define SLOWING_DOWN		(0.02f)								// 減速
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
int CSeasonEffect::m_nCntEffect = 0;
//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CSeasonEffect::CSeasonEffect(int nPriorityv) : CScene2D(nPriorityv, CScene::OBJTYPE_EFFECT)
{
	m_pVtxBuff		= NULL;
	m_pos			= VECTOR3_ZERO;
	m_posOld		= VECTOR3_ZERO;
	m_rot			= VECTOR3_ZERO;
	m_size			= VECTOR2_ZERO;
	m_SeasonType	= SEASONTYPE_NONE;
	m_fRadius		= 0.0f;
	m_fAngle		= 0.0f;
	m_fLength		= 0.0f;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CSeasonEffect::~CSeasonEffect()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSeasonEffect::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CScene2D::Init();
	// テクスチャの設定
	CScene2D::SetTexture((int)m_SeasonType - 1, TEX_ANIM_X, TEX_ANIM_Y, 1);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSeasonEffect::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CSeasonEffect::Update(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	CScene2D::Update();

	//位置の値を親からもらう
	D3DXVECTOR3 pos = CScene2D::GetPosition();
	// サイズの値を親からもらう
	D3DXVECTOR2 size = CScene2D::GetSize();
	//移動量の値を親からもらう
	D3DXVECTOR3 move = CScene2D::GetMove();

	m_fLength = sqrtf((size.x + size.x) * (size.y + size.y));
	// 角度を出す
	m_fAngle = atan2f(size.x / 2, size.y / 2);
	m_rot.x += ADD_ROT_X;
	m_rot.y = ADD_ROT_Y;

	m_nCountTime++;
	if ((m_nCountTime % MOVE_CHANGE_TAIME) == 0) { move.x *= -1; }

	move.x += (0.0f - move.x) * SLOWING_DOWN;

	//移動値代入
	pos.x += sinf(move.x);
	pos.y += move.y;

	//位置ｙが0じゃない場合のみセットする
	if (pos.y <= SCREEN_HEIGHT)
	{//半径が0以下の場合
		// テクスチャの設定
		CScene2D::SetTexture((int)m_SeasonType - 1, TEX_ANIM_X, TEX_ANIM_Y, 1);
		// 回転処理
		CScene2D::SetSpin(pos, m_fAngle, m_fLength, m_rot);
		//エフェクトの移動量
		CScene2D::SetMove(move);
	}
	else { Uninit(); }

}
//=============================================================================
// 描画処理
//=============================================================================
void CSeasonEffect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// カリングしない

	CScene2D::Draw();

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング(元に戻す)
}
//===============================================================================
//　クリエイト
//===============================================================================
CSeasonEffect * CSeasonEffect::Create(int Type,D3DXVECTOR3 pos, int nRadius,int nTexType)
{
	CSeasonEffect *pSeasonEffect = NULL;

	//NULLチェック
	if (pSeasonEffect == NULL)
	{//メモリの動的確保
		// プライベート設定
		int nPriorityv = PRIORTYV_4;
		if (nRadius >= RADIUS_CONDIUIONS) { nPriorityv = PRIORTYV_6; }
		pSeasonEffect = new CSeasonEffect(nPriorityv);

		if (pSeasonEffect != NULL)
		{
			//エフェクトの種類
			pSeasonEffect->m_SeasonType = (SEASONTYPE)Type;
			//オブジェクトクラスの生成
			pSeasonEffect->Init();
			//半径の設定
			pSeasonEffect->m_fRadius = (float)nRadius;
			//テクスチャの割り当て
			pSeasonEffect->BindTexture(*CTexture::GetTexture(nTexType));
			//位置の割り当て
			pSeasonEffect->SetPosSize(pos, D3DXVECTOR2(pSeasonEffect->m_fRadius, pSeasonEffect->m_fRadius));
			//移動量の割り当て
			pSeasonEffect->SetMove(MOVE);
		}
	}
	return pSeasonEffect;
}