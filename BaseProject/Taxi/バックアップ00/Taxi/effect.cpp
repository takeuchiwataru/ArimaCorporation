//=============================================================================
//
// エフェクトの処理 [effect.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "scene.h"
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "game.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	EFFECT_TEXTURE_NAME_1	"data\\TEXTURE\\effect000.jpg"			//読み込むテクスチャファイル名
#define	EFFECT_TEXTURE_NAME_2	"data\\TEXTURE\\kemuri.jpg"				//読み込むテクスチャファイル名
#define	EFFECT_TEXTURE_NAME_3	"data\\TEXTURE\\smoke.png"					//読み込むテクスチャファイル名

#define TEXTURE_SIZE			(50)									//テクスチャサイズ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
int CEffect::m_nCntEffect = 0;
//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3DTEXTURE9	CEffect::m_pTexture[MAX_BILLBOORD_TEX] = {};

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CEffect::CEffect() : CScene3D(EFFECT_PRIORTY, CScene::OBJTYPE_EFFECT)
{
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_EffectType = EFFECTTYPE_NONE;
	m_fRadius = 0.0f;
	m_nLife = 0;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CEffect::~CEffect()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEffect::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CScene3D::Init();

	//これはビルボードかエフェクトかの判別
	CScene3D::SetTypeNumber(CScene3D::TYPE_EFFECT);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEffect::Uninit(void)
{
	CScene3D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CEffect::Update(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	CScene3D::Update();

	//位置の値を親からもらう
	D3DXVECTOR3 pos = CScene3D::GetPosition();
	//移動量の値を親からもらう
	D3DXVECTOR3 move = CScene3D::GetMove();

	//カラーの宣言と初期化
	D3DXCOLOR Color[4];

	//頂点情報の設定
	CScene3D::SetVtxEffect(m_fRadius);

	//4色のうちのどれか
	int nNumber = rand() % 4 + 1;

	switch (m_EffectType)
	{
	case EFFECTTYPE_SPLASHES:	//水しぶき
		if (nNumber == 1) { Color[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 2) { Color[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 3) { Color[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 4) { Color[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }

		//エフェクトのカラー
		CScene3D::SetColor(Color[0]);

		//移動値代入
		pos += move;

		//寿命減少
		m_nLife--;

		if (m_nLife <= 0)
		{//半径が0以下の場合
			CScene3D::Uninit();
		}
		break;
	case EFFECTTYPE_ERUPTION:	//水煙
		if (nNumber == 1) { Color[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 2) { Color[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 3) { Color[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 4) { Color[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }

		//エフェクトのカラー
		CScene3D::SetColor(Color[0]);

		//移動値代入
		pos += move;

		//寿命減少
		m_nLife--;

		//バウンドの処理
		if (pos.y <= 0)
		{
			move.y = (m_posOld.y - pos.y) * 2;
		}
		if (m_nLife <= 0)
		{//半径が0以下の場合
			CScene3D::Uninit();
		}
		break;
	case EFFECTTYPE_SMOKE:		//煙エフェクト
		if (nNumber == 1) { Color[0] = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f); }
		if (nNumber == 2) { Color[1] = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f); }
		if (nNumber == 3) { Color[2] = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f); }
		if (nNumber == 4) { Color[3] = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f); }

		//エフェクトのカラー
		CScene3D::SetColor(Color[0]);

		//移動値代入
		pos.y += move.y;
		//寿命減少
		m_nLife--;

		//半径減少
		if (m_nLife > 0)
		{
			m_fRadius -= 0.05f;
		}
		else
		{
			m_fRadius -= 0.5f;
		}
		if (m_fRadius <= 0)
		{//半径が0以下の場合
			CScene3D::Uninit();
		}
		break;
	case EFFECTTYPE_FIRE:		//火エフェクト
		if (nNumber == 1) { Color[0] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); }
		if (nNumber == 2) { Color[1] = D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f); }
		if (nNumber == 3) { Color[2] = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f); }
		if (nNumber == 4) { Color[3] = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f); }

		//エフェクトのカラー
		CScene3D::SetColor(Color[0]);

		//移動値代入
		pos.y += 0.2f;
		//寿命減少
		m_nLife--;

		//半径減少
		if (m_nLife > 0)
		{
			m_fRadius -= 1.0f;
		}
		else
		{
			m_fRadius -= 1.0f;
		}

		if (m_fRadius <= 0)
		{//半径が0以下の場合
			CScene3D::Uninit();
		}
		break;
	case EFFECTTYPE_POWDER:		//火の粉エフェクト
		if (nNumber == 1) { Color[0] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); }
		if (nNumber == 2) { Color[1] = D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f); }
		if (nNumber == 3) { Color[2] = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f); }
		if (nNumber == 4) { Color[3] = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f); }

		//エフェクトのカラー
		CScene3D::SetColor(Color[0]);

		//移動値代入
		pos.y += 0.2f;
		//寿命減少
		m_nLife--;

		//半径減少
		if (m_nLife > 0)
		{
			m_fRadius -= 0.05f;
		}
		else
		{
			m_fRadius -= 0.05f;
		}
		if (m_fRadius <= 0)
		{//半径が0以下の場合
			CScene3D::Uninit();
		}
		break;
	case EFFECTTYPE_SNOW:		//雪エフェクト
		if (nNumber == 1) { Color[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 2) { Color[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 3) { Color[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 4) { Color[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }

		//エフェクトのカラー
		CScene3D::SetColor(Color[0]);

		//moveに重力
		//move.y = -1.0f;
		//移動値代入
		pos += move;

		if (pos.y <= 0)
		{//位置のｙ軸が0以下の場合
			CScene3D::Uninit();
		}
		break;
	case EFFECTTYPE_RAIN:		//雨エフェクト
		if (nNumber == 1) { Color[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 2) { Color[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 3) { Color[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 4) { Color[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }

		//エフェクトのカラー
		CScene3D::SetColor(Color[0]);

		//moveに重力
		//move.y = -3.0f;
		//移動値代入
		pos += move;

		if (pos.y <= 0)
		{//半径が0以下の場合
			CScene3D::Uninit();
		}
		break;
	}

	//位置ｙ・ライフ・半径が0じゃない場合のみセットする
	if (pos.y >= 0.1 || m_fRadius >= 0.1)
	{//半径が0以下の場合
		//エフェクトの位置
		CScene3D::SetPosition(pos);
		//エフェクトの移動量
		//CScene3D::SetMove(move);
	}

}
//=============================================================================
// 描画処理
//=============================================================================
void CEffect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//位置の値を親からもらう
	D3DXVECTOR3 EffectPos = CScene3D::GetPosition();
	//プレイヤーの位置情報
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	//ゲームの情報
	CManager::MODE pMode = CManager::GetMode();

	//フォグを使用しない
	//pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	//アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//アルファテスト処理
	//デフォルトの場合FALSEになっているからTRUEにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	if (m_EffectType == CEffect::EFFECTTYPE_SMOKE)
	{
		//減算合成の設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	else
	{
		//αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//近くのエフェクト以外を消す
	if (EffectPos.x + 1000.0f >= PlayerPos.x && EffectPos.x - 1000.0f <= PlayerPos.x && pMode == CManager::MODE_GAME)
	{
		CScene3D::Draw();
	}
	else if(pMode != CManager::MODE_GAME)
	{
		CScene3D::Draw();
	}

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//デフォルトの場合FALSEになっているからTRUEにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//フォグを使用する
	//pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
}
//===============================================================================
//　クリエイト
//===============================================================================
CEffect * CEffect::Create(EFFECTTYPE Type,D3DXVECTOR3 pos, int nRadius, int nLife,int nTexType)
{
	CEffect *pEffect = NULL;

	//NULLチェック
	if (pEffect == NULL)
	{//メモリの動的確保

		pEffect = new CEffect;

		if (pEffect != NULL)
		{
			//オブジェクトクラスの生成
			pEffect->Init();
			//エフェクトの種類
			pEffect->m_EffectType = Type;
			//半径の設定
			pEffect->m_fRadius = (float)nRadius;
			//ライフの設定
			pEffect->m_nLife = nLife;
			//テクスチャの割り当て
			pEffect->BindTexture(m_pTexture[nTexType]);
			//位置の割り当て
			pEffect->SetPosition(pos);
			//パーティクルの設定
			pEffect->SetParticleEffect(Type, nRadius);
		}
	}

	return pEffect;
}
//=========================================================================================================================
//パーティクルエフェクト設定処理
//=========================================================================================================================
void CEffect::SetParticleEffect(EFFECTTYPE Type, int nRadius)
{
	//タイプの設定
	m_EffectType = Type;

	//位置の値を親からもらう
	//D3DXVECTOR3 pos = CScene3D::GetPosition();
	//移動量の値を親からもらう
	D3DXVECTOR3 move = CScene3D::GetMove();

	if (m_EffectType == EFFECTTYPE_ERUPTION || m_EffectType == EFFECTTYPE_SMOKE || m_EffectType == EFFECTTYPE_FIRE || m_EffectType == EFFECTTYPE_POWDER)
	{
		//一周分にでるエフェクトの角度
		float fLap = 1.0f / (10 / 2);

		//移動ランダム化
		move.x = sinf((-D3DX_PI * (1.0f * (fLap * (m_nCntEffect + 1)))))* 1.5f;
		move.y = cosf((rand() % 10) / 10.0f) * ((rand() % 4) + 1);
		move.z = cosf((-D3DX_PI * (1.0f * (fLap * (m_nCntEffect + 1)))))* 1.5f;
	}
	else if (m_EffectType == EFFECTTYPE_SPLASHES)
	{
		//一周分にでるエフェクトの角度
		float fLap = 1.0f / (15 / 2);

		//移動ランダム化
		move.x = sinf((-D3DX_PI * (1.0f * (fLap * (m_nCntEffect + 1)))))* 2.0f;
		//move.y = cosf((rand() % 1) / 1.0f) * ((rand() % 4) + 1);
		move.y += 1.0f;
		move.z = cosf((-D3DX_PI * (1.0f * (fLap * (m_nCntEffect + 1)))))* 2.0f;
	}

	m_nCntEffect = (1 + m_nCntEffect)% 100;

	//エフェクトの移動量
	CScene3D::SetMove(move);
}
//===============================================================================
// テクスチャの読み込み
//===============================================================================
HRESULT CEffect::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE_NAME_1, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE_NAME_2, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE_NAME_3, &m_pTexture[2]);

	return S_OK;
}
//===============================================================================
// テクスチャの破棄
//===============================================================================
void CEffect::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_BILLBOORD_TEX; nCount++)
	{
		//テクスチャーの破棄
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}
