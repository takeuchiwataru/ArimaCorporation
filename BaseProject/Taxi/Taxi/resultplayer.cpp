//=============================================================================
//
// リザルトプレイヤーの処理 [resultplayer.cpp]
// Author : shun yokomichi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "resultplayer.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camerabace.h"
#include "scene.h"
#include "model.h"
#include "motion.h"
#include "loadText.h"
#include "tire.h"
#include "wall.h"
#include "incline.h"
#include "taillamp.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))				//ベクトルの初期化
#define END_POS			(D3DXVECTOR3(530.0f, 0.0f, -120.0f))		// 止まる位置
#define FAILE_NAME		("data\\TEXT\\Player\\PlayerState.txt")		//読み込むファイル名
#define FILE_TEXTURE	("data\\TEXTURE\\modeltex\\carup.jpg")		//テクスチャの読み込み
#define LEFT_FRONT_POS	(D3DXVECTOR3(-26.0f, 3.0f, 50.0f))			//左前タイヤ位置
#define LEFT_BACK_POS	(D3DXVECTOR3(-26.0f, 3.0f, -50.0f))			//左後ろタイヤ位置
#define RIGHT_FRONT_POS	(D3DXVECTOR3(26.0f, 3.0f, 50.0f))			//右前タイヤ位置
#define RIGHT_BACK_POS	(D3DXVECTOR3(26.0f, 3.0f, -50.0f))			//右後ろタイヤ位置
#define GRAVITY_FRONT	(0.5f)										//前方の重力
#define GRAVITY_BACK	(0.2f)										//後方の重力

//タイヤの番号
#define LEFT_FRONT		(0)											//左前タイヤ
#define LEFT_BACK		(1)											//左後ろタイヤ
#define RIGHT_FRONT		(2)											//右前タイヤ
#define RIGHT_BACK		(3)											//右後ろタイヤ

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CModel * CResultPlayer::m_pModel = NULL;	//モデルのパーツポインタ
int	CResultPlayer::m_nMaxModel = 0;
int CResultPlayer::m_nMaxParts = 0;
int CResultPlayer::m_nMaxMotion = 0;
D3DXVECTOR3 CResultPlayer::m_pos = VECTOR_ZERO;
CMotion::MOTION_INFO * CResultPlayer::m_pMotionInfo = NULL;
LPDIRECT3DTEXTURE9 CResultPlayer::m_pTexture = NULL;

//=============================================================================
// 生成処理
//=============================================================================
CResultPlayer * CResultPlayer::Create(const D3DXVECTOR3 pos)
{
	//インスタンスの生成
	CResultPlayer * pPlayer;
	pPlayer = new CResultPlayer;

	//初期化処理
	pPlayer->Init();

	//設定処理
	pPlayer->Set(pos, VECTOR_ZERO);
	return pPlayer;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CResultPlayer::CResultPlayer() : CScene(3, OBJTYPE_PLAYER) {}

//=============================================================================
// デストラクタ
//=============================================================================
CResultPlayer::~CResultPlayer() {}

//=============================================================================
// Xファイルの読み込み
//=============================================================================
void CResultPlayer::LoadModel(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice,
		FILE_TEXTURE,
		&m_pTexture);

	//モーション情報の取得
	CLoadTextMotion * pLoadTextMotion = CResult::GetPlayerMotion();

	//モデル情報を取得
	CLoadTextMotion::MODEL_INFO ModelInfo = pLoadTextMotion->GetModelInfo();
	m_nMaxModel = ModelInfo.nMaxModel;	//モデルの最大数の取得
	m_nMaxParts = ModelInfo.nMaxParts;	//モデルのパーツ最大数の取得

	//モデルの生成
	for (int nCntParts = 0; nCntParts < ModelInfo.nMaxParts; nCntParts++)
	{
		m_pModel = CModel::Create(ModelInfo.pOfSetPos[nCntParts], &ModelInfo.paFileName[nCntParts][0]);

		//テクスチャの割当て
		if (m_pModel != NULL) { m_pModel->BindTexture(m_pTexture); }
	}

	//モデルの親設定
	for (int nCntParts = 0; nCntParts < ModelInfo.nMaxParts; nCntParts++)
	{
		if (nCntParts == 0)
		{
			m_pModel->SetParent(NULL);
			break;
		}
	}
}

//=============================================================================
// Xファイルの破棄
//=============================================================================
void CResultPlayer::UnloadModel(void)
{
	if (m_pModel != NULL)
	{
		delete m_pModel;
		m_pModel = NULL;
	}

	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResultPlayer::Init(void)
{
	//変数の初期化
	m_pos = VECTOR_ZERO;					//中心座標
	m_OldPos = VECTOR_ZERO;					//前回の座標
	m_move = VECTOR_ZERO;					//移動
	m_rot = VECTOR_ZERO;					//向き
	m_vtxMaxModel = VECTOR_ZERO;			//モデルの頂点最大値
	m_vtxMinModel = VECTOR_ZERO;			//モデルの頂点最小値
	m_pMotion = NULL;						//モーションポインタ
	m_pText = NULL;							//プレイヤーのテキストポインタ
	m_state = STATE_DRIVE;					//状態設定
	m_StateSpeed = STATE_SPEED_STOP;		//スピードの状態設定
	m_PlayerInfo.nCountTime = 0;			//カウンター
	m_PlayerInfo.fAccel = 3.5f;				//加速値
	m_PlayerInfo.fBraks = 0.1f;				//減速値
	m_PlayerInfo.fAddRot = 0.02f;			//加える回転値
	m_PlayerInfo.fDistance = 0.0f;			//距離
	m_PlayerInfo.fWindy = 0.05f;			//風圧
	m_PlayerInfo.FirstPos = VECTOR_ZERO;	//初期位置

	//タイヤのポインタを初期化
	for (int nCntTire = 0; nCntTire < MAX_TIRE; nCntTire++)
	{
		m_pTire[nCntTire] = NULL;
	}

	CreateTire();			//タイヤの生成

	for (int nCntTailLamp = 0; nCntTailLamp < 2; nCntTailLamp++)
	{// テールランプの生成
		m_apTailLamp[nCntTailLamp] = CTailLamp::Create(D3DXVECTOR3(22.0f - (nCntTailLamp * 44.0f), 35.0f, -85.0f), D3DXVECTOR2(10.0f, 10.0f), &m_mtxWorld);
	}

	//モデルの最小値・最大値の取得
	m_vtxMaxModel = m_pModel->GetVtxMax();
	m_vtxMinModel = m_pModel->GetVtxMin();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResultPlayer::Uninit(void)
{
	//タイヤモデルの破棄
	for (int nCntTire = 0; nCntTire < MAX_TIRE; nCntTire++)
	{
		if (m_pTire[nCntTire] != NULL)
		{
			m_pTire[nCntTire]->Uninit();
			delete m_pTire[nCntTire];
			m_pTire[nCntTire] = NULL;
		}
	}

	//テキストの破棄
	if (m_pText != NULL)
	{
		m_pText->Uninit();
		delete m_pText;
		m_pText = NULL;
	}

	//死亡フラグを立てる
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResultPlayer::Update(void)
{
	//変数宣言
	D3DXVECTOR3 TirePos[MAX_TIRE];

	//タイヤの更新処理
	for (int nCntTire = 0; nCntTire < MAX_TIRE; nCntTire++)
	{
		if (m_pTire[nCntTire] == NULL) { continue; }

		if ((nCntTire % 2) == 0)
		{
			m_pTire[nCntTire]->Update(nCntTire, GRAVITY_FRONT);
		}
		else
		{
			m_pTire[nCntTire]->Update(nCntTire, GRAVITY_BACK);
		}

		if (m_StateSpeed == STATE_SPEED_STOP)
		{
			m_pTire[nCntTire]->SetRot(VECTOR_ZERO);
		}
		else
		{
			m_pTire[nCntTire]->SetRot(D3DXVECTOR3(m_pTire[nCntTire]->GetRot().x, VECTOR_ZERO.y, VECTOR_ZERO.z));
		}

		TirePos[nCntTire] = m_pTire[nCntTire]->GetWorldPos();
	}

	DirectingCount();
	UpdateMove();			// 移動処理
}

//=============================================================================
// 描画処理
//=============================================================================
void CResultPlayer::Draw(void)
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX		  mtxRot, mtxTrans;			// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//モデルの描画
	if (m_pModel != NULL)
	{
		//描画する
		m_pModel->Draw(1.0f);
	}

	//タイヤの描画
	for (int nCntTire = 0; nCntTire < MAX_TIRE; nCntTire++)
	{
		if (m_pTire[nCntTire] != NULL) { m_pTire[nCntTire]->Draw(1.0); }
	}
}

//=============================================================================
// リザルト演出時のカウント
//=============================================================================
void CResultPlayer::DirectingCount(void)
{
	if (m_pos.z <= END_POS.z - 100.0f)
	{
		SetStateSpeed(STATE_SPEED_ACCEL);
		m_move.z += cosf(atan2f(END_POS.x - m_pos.x, END_POS.z - m_pos.z)) * 0.8f;
	}
	else
	{
		m_move.z -= cosf(atan2f(END_POS.x - m_pos.x, END_POS.z - m_pos.z)) * 0.8f;
		if (m_move.z <= 0.0f) { m_move.z = 0.0f; SetStateSpeed(STATE_SPEED_STOP); }
	}
}
//=============================================================================
// 移動処理
//=============================================================================
void CResultPlayer::UpdateMove(void)
{
	m_OldPos = m_pos;	//前回の位置を保存する

	RemakeAngle(&m_rot.y);

	// 重力
	m_move.y -= cosf(0) * 0.4f;

	//中心座標更新
	m_pos.x += m_move.x;
	//m_pos.y += m_move.y;
	m_pos.z += m_move.z;

	//減速
	m_PlayerInfo.fAccel += (0.0f - m_PlayerInfo.fAccel) * m_PlayerInfo.fWindy;
	m_move.x += (0.0f - m_move.x) * m_PlayerInfo.fWindy;
	m_move.z += (0.0f - m_move.z) * m_PlayerInfo.fWindy;

	//停止判定
	D3DXVECTOR3 fDiffuse = m_pos - m_OldPos;

	if (fDiffuse.x < 0.10f && fDiffuse.x > -0.10f)
	{
		if (fDiffuse.z < 0.10f && fDiffuse.z > -0.10f)
		{
			SetStateSpeed(STATE_SPEED_STOP);
		}
	}
}

//=============================================================================
// 状態の設定
//=============================================================================
void CResultPlayer::SetState(CResultPlayer::STATE state)
{
	//値の反転
	if (m_state != state)
	{
		m_PlayerInfo.fAccel *= -0.5f;
		m_PlayerInfo.fBraks *= -1;
		m_fMaxSpeed *= -1;

		CSound *pSound = CManager::GetSound();

		if (STATE_REVERSE == state)
		{// バック音
			pSound->StopSound(CSound::SOUND_LABEL_SE_ACCEL);
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE_BACK);

			if (STATE_SPEED_STOP != m_StateSpeed)
			{// テールランプをバック用に点灯
				SetTailLampType(CTailLamp::TYPE_BACK);
			}
		}
		else if (STATE_DRIVE == state)
		{
			pSound->StopSound(CSound::SOUND_LABEL_SE_BACK);
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ACCEL);

			//テールランプを消す
			SetTailLampType(CTailLamp::TYPE_NONE);
		}
	}

	//状態の設定
	m_state = state;
}

//=============================================================================
// スピードの状態設定
//=============================================================================
void CResultPlayer::SetStateSpeed(CResultPlayer::STATE_SPEED state)
{
	//数値の設定
	if (m_StateSpeed != state)
	{
		m_PlayerInfo.nCountTime = 0;

		CSound *pSound = CManager::GetSound();

		if (STATE_SPEED_ACCEL == state && STATE_DRIVE == m_state)
		{// アクセル音
			pSound->PlaySound(CSound::SOUND_LABEL_SE_ACCEL);
		}
		else if (STATE_SPEED_BRAKS == state)
		{// ブレーキ音
			pSound->PlaySound(CSound::SOUND_LABEL_SE_BRAKING);
			pSound->StopSound(CSound::SOUND_LABEL_SE_ACCEL);
		}
	}

	//状態の設定
	m_StateSpeed = state;
}

//=============================================================================
// 設定処理
//=============================================================================
void CResultPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_pos = pos;
}

//=============================================================================
// タイヤの生成
//=============================================================================
void CResultPlayer::CreateTire(void)
{
	//タイヤの位置を設定
	D3DXVECTOR3 TirePos[MAX_TIRE];

	TirePos[LEFT_FRONT] = LEFT_FRONT_POS;
	TirePos[LEFT_BACK] = LEFT_BACK_POS;
	TirePos[RIGHT_FRONT] = RIGHT_FRONT_POS;
	TirePos[RIGHT_BACK] = RIGHT_BACK_POS;

	//タイヤの生成
	for (int nCntTire = 0; nCntTire < MAX_TIRE; nCntTire++)
	{
		m_pTire[nCntTire] = NULL;
		m_pTire[nCntTire] = CTire::Create(TirePos[nCntTire]);

		if (m_pTire[nCntTire] != NULL) { m_pTire[nCntTire]->SetParent(m_pModel); }	//親子関係の設定
	}
}
//=============================================================================
// 角度修正
//=============================================================================
void CResultPlayer::RemakeAngle(float * pAngle)
{
	//目的の角度修正
	if (*pAngle < -D3DX_PI)
	{
		*pAngle += D3DX_PI * 2.0f;
	}
	if (*pAngle > D3DX_PI)
	{
		*pAngle -= D3DX_PI * 2.0f;
	}
}
//=============================================================================
// テールランプの種類の設定
//=============================================================================
void CResultPlayer::SetTailLampType(CTailLamp::TYPE type)
{
	for (int nCntTailLamp = 0; nCntTailLamp < 2; nCntTailLamp++)
	{// テールランプの数分ループ
		if (NULL != m_apTailLamp[nCntTailLamp])
		{// テールランプを透明にする
			m_apTailLamp[nCntTailLamp]->SetType(type);
		}
	}
}