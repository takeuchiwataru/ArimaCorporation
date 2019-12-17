//=============================================================================
//
// ひよこの処理 [chick.cpp]
// Author : 長山拓実
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "scene.h"
#include "chick.h"
#include "manager.h"
#include "object.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"
#include "shadow.h"
#include "tutorial.h"
#include "ColMesh.h"
#include "egg.h"
#include "particle.h"
#include "model.h"
#include "Character.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FILE_TEXTURE			("data\\TEXTURE\\modeltex\\ニワトリ.jpg")	//テクスチャの読み込み

#define MODEL_SPEED				(5.0f)
#define OBJCT_ANGLE_REVISION	(0.2f)		// 角度補正
#define EFFECT_HIGHT			(250.0f)	// エミッターの高さ
#define FOUNTAIN_UP				(20.0f)		// 噴水の上昇させる値
#define CHICK_SPEED				(15.0f)		// ひよこが飛んでくスピード
#define CHICK_JUMP				(3.5f)		// ジャンプ力
#define CHICK_FALL_TIME			(30)		// ひよこが落ちてくるタイミングの間隔
#define CHICK_FALL_SPEED		(12.0f)		// 落ちてくるひよこの速さ
#define CHICK_PARTICLE			(30)		// パーティクルの数
#define CHICK_UPDOWN_TIME		(5)			// ひよこが上下する間隔の時間
#define CHICK_SPEED_RANGE		(15)		// 加速ひよこの間隔
#define MAX_SMOKE_SPEED			(15)		// キラーひよこ出現時の煙の数
#define ATTACK_TIME				(3)			// 隕石ひよこが落ちるまでの時間
#define MAX_SMOKE_S				(2)			// キラーひよこの煙の数(ながやま修正12/17)
//更新範囲
#define FOUNTAIN_LENGTH			(15000)		//噴水の更新範囲
#define LEAF_LENGTH				(10500)		//葉の更新処理
#define WOOD_LENGTH				(8000)		//木をビルボードに入れ替える距離
#define FLOWER_LENGTH			(5500)		//花壇の描画範囲

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
D3DXVECTOR3				CChick::m_VtxMaxModel[MAX_CHICK] = {};
D3DXVECTOR3				CChick::m_VtxMinModel[MAX_CHICK] = {};

CChick::KEY_INFO		*CChick::m_pKeyInfo[MAX_CHICK_MOTION] = {};		//キー情報へのポインタ
int						CChick::m_nNumParts = 0;						//パーツ数
int						CChick::m_aIndexParent[MAX_CHICK_PARTS] = {};	//親のインデックス
CChick::KEY				CChick::m_aKayOffset[MAX_CHICK_PARTS] = {};		//オフセット情報
CChick::MOTION_INFO		CChick::m_aMotionInfo[MAX_CHICK_MOTION] = {};	//モーション情報

//--------------------------------------------
//グローバル変数
//--------------------------------------------
int g_nChickNumModel;
char g_aChickFileNameModel[MAX_CHICK_PARTS][256];

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CChick::CChick() : CScene(EGG_PRIOTITY, CScene::OBJTYPE_CHICK)
{
	//値の初期化
	m_apModel = NULL;

	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 大きさ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDestAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDiffAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_FNor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_DestPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRangePos = 0.0f;
	m_fRange = 0.0f;
	m_fHeight = 0.0f;
	m_fLength = 0.0f;
	m_nRank = 0;
	m_nNumPlayer = 0;
	m_nDisTimer = 0;
	m_DestRank = 0;
	m_nMap = 0;
	m_nExplosion = 0;
	m_nKey = 0;
	m_nCntUpDown = 0;
	m_nCntAttackTime = 0;
	m_fUpDown = 0.0f;
	m_fDisTime = 0.0f;	// (ながやま修正12/17)
}
//===============================================================================
//　デストラクタ
//===============================================================================
CChick::~CChick() {}

//===============================================================================
//　生成
//===============================================================================
CChick * CChick::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, BULLETTYPE bulletType, STATE state, int nNumPlayer)
{
	CChick *pChick = NULL;

	// NULLチェック
	if (pChick == NULL)
	{// メモリの動的確保

		pChick = new CChick;

		if (pChick != NULL)
		{
			// サイズを代入
			pChick->m_scale = scale;
			// 卵の種類を代入
			pChick->m_type = type;
			// 弾の種類を代入
			pChick->m_bulletType = bulletType;
			// オブジェクトクラスの生成
			pChick->Init();
			//位置の代入
			pChick->m_pos = pos;
			//高さの代入
			pChick->m_fHeight = pos.y;
			// 回転を反映
			pChick->SetRot(rot);
			// 何位の卵か
			pChick->m_nNumPlayer = nNumPlayer;
			// 状態の設定
			pChick->m_state = state;

			CCharcter::ResetCheck(pChick->m_pos, &pChick->m_fCola[0]);
		}
	}

	return pChick;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CChick::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//変数の初期化
	m_pObjBill = NULL;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fHeight = 0.0f;
	m_fDestAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDiffAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_FNor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_DestPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRangePos = 0.0f;
	m_fRange = 0.0f;
	m_fLength = 3.0f;
	m_bJump = false;
	m_bDis = true;
	m_state = STATE_NORMAL;
	m_nRank = 0;
	m_nNumPlayer = 0;
	m_nDisTimer = 0;
	m_DestRank = -1;
	m_nMap = 0;
	m_bAttackS = false;
	m_bExplosion = false;
	m_bHeight = false;
	m_bJumpOk = false;
	m_bSpeedS = false;
	m_bAttackCol = true;
	m_nExplosion = 0;
	m_nAnimnow = CHICK_ANIM_NEUTRAL;	//ニュートラル状態
	m_nCountFlame = 0;
	m_nCntUpDown = 0;
	m_nCntAttackTime = 0;
	m_fUpDown = 10.0f;
	m_fDisTime = DISTIME;	// (ながやま修正12/17)
	if (m_type == TYPE_SPEED)
	{
		m_fDisTime = (60.0f * 0.5f) + 40;
	}
	else if (m_type == TYPE_SPEED_S)
	{
		m_fDisTime = (60.0f * KILLER_TIME);
	}

	CModel::ParentModel(m_apModel, CModel::TYPE_CHICK);
	int &nMaxModel = CModel::GetnModelMax(CModel::TYPE_CHICK);
	for (int nCountIndex = 0; nCountIndex < nMaxModel; nCountIndex++)
	{// 減速鶏のポインタを初期化
		if (m_aIndexParent[nCountIndex] == -1)
		{
			//モデルの親を指定
			m_apModel[nCountIndex]->SetParent(NULL);
		}
		else
		{
			//モデルの親を指定
			m_apModel[nCountIndex]->SetParent(m_apModel[m_aIndexParent[nCountIndex]]);
		}

		m_apModel[nCountIndex]->SetPos(D3DXVECTOR3(m_pos.x + m_aKayOffset[nCountIndex].fposX,
			m_pos.y + m_aKayOffset[nCountIndex].fposY,
			m_pos.z + m_aKayOffset[nCountIndex].fposZ));

		if (m_type == TYPE_ATTACK || m_type == TYPE_ATTACK_S) m_apModel[nCountIndex]->BindTexture(CModel::GetTexAll(CModel::TEX_CHICK_R + TYPE_ATTACK));
		if (m_type == TYPE_ANNOY || m_type == TYPE_ANNOY_S) m_apModel[nCountIndex]->BindTexture(CModel::GetTexAll(CModel::TEX_CHICK_R + TYPE_ANNOY));
		if (m_type == TYPE_SPEED || m_type == TYPE_SPEED_S) m_apModel[nCountIndex]->BindTexture(CModel::GetTexAll(CModel::TEX_CHICK_R + TYPE_SPEED));
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CChick::Uninit(void)
{
	int &nMaxModel = CModel::GetnModelMax(CModel::TYPE_CHICK);
	if (m_apModel != NULL)
	{
		for (int nCount = 0; nCount < nMaxModel; nCount++)
		{
			if (m_apModel[nCount] != NULL)
			{
				//3DモデルのUninit
				m_apModel[nCount]->Uninit();
				delete m_apModel[nCount];
				m_apModel[nCount] = NULL;
			}
		}
		delete[] m_apModel;
		m_apModel = NULL;
	}

	//死亡フラグを立てる
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CChick::Update(void)
{
	m_posOld = m_pos;	//前回の位置を保存する

						// ひよこの動き
	if (Move()) { return; }

	m_nMotionType = m_nAnimnow;

	//モーション更新
	UpdateMotion();
}
//=============================================================================
// 描画処理
//=============================================================================
void CChick::Draw(void)
{
	float fCola;
	CCharcter::DrawCheck(m_pos, &m_fCola[0], fCola);

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

	int &nMaxParts = CModel::GetnModelMax(CModel::TYPE_CHICK);

	if (m_bExplosion == false)
	{
		for (int nCount = 0; nCount < nMaxParts; nCount++)
		{//モデルの描画
			if (m_apModel[nCount] != NULL)
			{
				//描画する
				m_apModel[nCount]->Draw(fCola);
			}
		}
	}
}

//===============================================================================
// Xファイルの読み込み
//===============================================================================
void CChick::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int			nNumVtx = 0;	//頂点数
								//モデルのオフセットと読み込み
	FileLoad();

	//モデルの最大値・最小値を取得する
	for (int nCntModel = 0; nCntModel < MAX_CHICK; nCntModel++)
	{
		m_VtxMaxModel[nCntModel] = D3DXVECTOR3(-10000, -10000, -10000);	//最大値
		m_VtxMinModel[nCntModel] = D3DXVECTOR3(10000, 10000, 10000);	//最小値

		CModel3D::MODEL_TYPE type;

		switch (nCntModel)
		{
		case TYPE_ATTACK:	type = CModel3D::MODEL_TYPE_CHICK;	break;
		case TYPE_ANNOY:	type = CModel3D::MODEL_TYPE_CHICK;	break;
		case TYPE_SPEED:	type = CModel3D::MODEL_TYPE_CHICK;	break;
		case TYPE_ATTACK_S:	type = CModel3D::MODEL_TYPE_CHICK;	break;
		case TYPE_ANNOY_S:	type = CModel3D::MODEL_TYPE_CHICK;	break;
		case TYPE_SPEED_S:	type = CModel3D::MODEL_TYPE_CHICK;	break;
		}

		//LPD3DXMESH &Mesh = CModel3D::MeshLoad(type);
	}
}

//===============================================================================
// テクスチャの破棄
//===============================================================================
void CChick::UnLoad(void)
{

}

//===============================================================================
// ひよこの動き
//===============================================================================
bool CChick::Move(void)
{
	// 使ったとき
	if (Item()) { return true; }

	if ((m_type != TYPE_ANNOY && m_type != TYPE_ATTACK_S && m_type != TYPE_ANNOY_S) || m_state != STATE_BULLET)
	{
		// 重力
		float fGravity = 0.2f;

		if (m_type == TYPE_SPEED && m_state == STATE_BULLET)
		{// 重力
			fGravity = 0.3f;
		}

		m_move.y -= fGravity;
	}

	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;

	if (m_bDis == false)
	{// 消えるまでの処理(ながやま修正12/17)
		m_nDisTimer++;

		if (m_nDisTimer > m_fDisTime)
		{// 消す
			if (m_type == TYPE_SPEED || m_type == TYPE_SPEED_S)
			{
				D3DXVECTOR2 fSize;
				int nNumSmoke = 0;

				if (m_type == TYPE_SPEED)
				{// 加速ひよこのパーティクルの数を設定
					nNumSmoke = MAX_SMOKE;
				}
				else if (m_type == TYPE_SPEED_S)
				{// キラーひよこのパーティクルの数を設定(ながやま修正12/17)
					nNumSmoke = MAX_SMOKE_S;
				}

				for (int nCntParticle = 0; nCntParticle < nNumSmoke; nCntParticle++)
				{// パーティクル
					fSize.x = SMOKE_SIZE + (float)(CServer::Rand() % 3);
					fSize.y = SMOKE_SIZE + (float)(CServer::Rand() % 3);

					CParticle::Create(D3DXVECTOR3((sinf(m_rot.y + D3DX_PI) * -30.0f) + m_pos.x,
						m_pos.y - 2.0f,
						(cosf(m_rot.y + D3DX_PI) * -30.0f) + m_pos.z),
						D3DXVECTOR3(sinf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 3 + 1)), 0.0f, cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 3 + 1))),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
						fSize,
						30,
						CParticle::TEXTURE_SMOKE,
						CParticle::TYPE_TURN,
						m_nNumPlayer);
				}
			}

			m_nDisTimer = 0;
			Uninit();
			return true;
		}
	}

	if (m_state == STATE_CHASE)
	{
		if (m_type != TYPE_ATTACK_S || m_type != TYPE_ANNOY_S)
		{
			//マップとの当たり判定
			CPlayer **pPlayer = NULL;
			switch (CManager::GetMode())
			{
			case CManager::MODE_TITLE:
				pPlayer = CTitle::GetPlayer();
				break;
			case CManager::MODE_GAME:
				pPlayer = CGame::GetPlayer();
				break;
			}

			m_fHeight = CCOL_MESH_MANAGER::GetHeight(m_pos, pPlayer[m_nNumPlayer]->GetnMap());

			if (m_pos.y < m_fHeight)
			{
				m_move.y = 0.0f;
				m_pos.y = m_fHeight;
				//ジャンプの状態設定
				m_bJump = false;
			}
		}
	}
	else if (m_state == STATE_BULLET)
	{
		if (m_type == TYPE_ATTACK || m_type == TYPE_ATTACK_S || m_type == TYPE_SPEED_S)
		{
			//マップとの当たり判定
			CPlayer **pPlayer = NULL;
			switch (CManager::GetMode())
			{
			case CManager::MODE_TITLE:
				pPlayer = CTitle::GetPlayer();
				break;
			case CManager::MODE_GAME:
				pPlayer = CGame::GetPlayer();
				break;
			}

			m_fHeight = CCOL_MESH_MANAGER::GetHeight(m_pos, pPlayer[m_nNumPlayer]->GetnMap());

			if (m_pos.y < m_fHeight)
			{
				m_move.y = 0.0f;
				m_pos.y = m_fHeight;
				//ジャンプの状態設定
				m_bJump = false;

				if (m_type == TYPE_ATTACK_S)
				{
					if (m_bExplosion == false && m_bAttackCol == true)
					{
						m_bExplosion = true;
						D3DXVECTOR2 fSize;

						for (int nCntParticle = 0; nCntParticle < CHICK_PARTICLE; nCntParticle++)
						{// パーティクル生成
							fSize.x = 5.0f + (float)(CServer::Rand() % 5);
							fSize.y = 5.0f + (float)(CServer::Rand() % 5);

							CParticle::Create(m_pos,
								D3DXVECTOR3(sinf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 5 + 1)), cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 5 + 1)), cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 5 + 1))),
								D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
								fSize,
								20,
								CParticle::TEXTURE_STAR,
								CParticle::TYPE_NORMAL,
								m_nNumPlayer);
						}
					}
				}
			}
		}
	}

	return false;
}

//===============================================================================
// ひよこを使用したときの動き
//===============================================================================
bool CChick::Item(void)
{
	if (m_state == STATE_BULLET)
	{// 弾の状態の時
		float fHeight = 0.0f;
		CPlayer **pPlayer = NULL;

		switch (m_type)
		{
			// 攻撃
		case TYPE_ATTACK:
			break;

			// 攻撃
		case TYPE_ATTACK_S:
			if (m_bExplosion == true)
			{// 爆発中
				m_nExplosion++;

				if (m_nExplosion > EXPLOSION_TIME)
				{// 削除
					m_nExplosion = 0;
					Uninit();
					return true;
				}
			}
			break;

			// 減速させる
		case TYPE_ANNOY:
			Annoy();
			break;

			// 減速させる
		case TYPE_SPEED:
			Speed();
			break;

			// 減速させる
		case TYPE_SPEED_S:
			SpeedS();
			break;
		}
		// 飛んでく動き
		Bullet();
	}
	return false;
}

//===============================================================================
// 当たり判定
//===============================================================================
bool CChick::CollisionChick(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld)
{
	//あたっているかあたってないか
	bool bHit = false;

	if (m_state == STATE_BULLET)
	{
		// 各種情報の取得
		float fDepth = PLAYER_DEPTH;

		if (m_type == TYPE_ANNOY)
		{
			fDepth = ANNOY_RANGE;
		}
		if (m_bExplosion == true)
		{
			fDepth = 80.0f;
		}

		//距離計算
		float fDistance = sqrtf(powf(pPos->x - m_pos.x, 2) + powf(pPos->z - m_pos.z, 2));

		if (fDistance < PLAYER_DEPTH + fDepth)
		{//距離チェック
			bHit = true;
		}

		if (bHit == true && m_type == TYPE_ATTACK)
		{
			m_bDis = false;
		}
	}
	return bHit;
}

//=============================================================================
// 高さ判定
//=============================================================================
float CChick::SetHeight(void)
{
	float fHeight = 0.0f;

	CScene *pScene = CScene::GetTop(MESH_PRIOTITY);

	//NULLチェック
	while (pScene != NULL)
	{
		//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetObjType() == CScene::OBJTYPE_GROUND)
		{//タイプが地面だったら
			CMeshField *pField = (CMeshField*)pScene;

			if (pField->OnField(m_pos, 0.0f))
			{// 傾斜の計算
				fHeight = pField->GetHeightMesh(m_pos);

				if (m_bJump == false || (m_bJump == true && m_fHeight < fHeight))
				{
					m_fHeight = fHeight + 15.0f;				//地面の高さを取得
					m_move.y = 0.0f;					//移動量を初期化する

														//ジャンプの状態設定
					m_bJump = false;

					break;
				}
			}
		}
		//Nextに次のSceneを入れる
		pScene = pSceneNext;
	}

	return m_fHeight;
}

//=============================================================================
// ジャンプ
//=============================================================================
void CChick::Jump(float fJump)
{
	// ジャンプ
	if (m_bJump == false)
	{// ジャンプしていない
		m_bJump = true;
		m_move.y += fJump;
	}
}

//=============================================================================
// 飛んでく
//=============================================================================
void CChick::Bullet(void)
{
	if (m_bulletType == BULLETTYPE_PLAYER)
	{
		switch (m_type)
		{
			// 攻撃ひよこ
		case TYPE_ATTACK:
			// 1つ前のプレイヤーに飛んでいく
			Attack();
			break;

			// 強い攻撃ひよこ
		case TYPE_ATTACK_S:
			// 1位に全体攻撃
			AttackS();
			break;

			// 強い減速ひよこ
		case TYPE_ANNOY_S:
			// 1位に全体攻撃
			AnnoyS();
			break;
		}
	}

}

//=============================================================================
// 角度の調節
//=============================================================================
float CChick::AdjustAngle(float rot)
{
	if (rot > D3DX_PI)
	{
		rot -= D3DX_PI * 2.0f;
	}
	if (rot < -D3DX_PI)
	{
		rot += D3DX_PI * 2.0f;
	}

	return rot;
}

//=============================================================================
// 攻撃ひよこ
//=============================================================================
void CChick::Attack(void)
{
	CPlayer **pPlayer = NULL;
	switch (CManager::GetMode())
	{
	case CManager::MODE_TITLE:
		pPlayer = CTitle::GetPlayer();
		break;
	case CManager::MODE_GAME:
		pPlayer = CGame::GetPlayer();
		break;
	}
	if (m_DestRank >= 0)
	{
		// 目的の角度
		m_fDestAngle.y = atan2f(pPlayer[m_DestRank]->GetPos().x - m_pos.x, pPlayer[m_DestRank]->GetPos().z - m_pos.z);

		// 差分
		m_fDiffAngle.y = m_fDestAngle.y - m_rot.y;

		m_fDiffAngle.y = AdjustAngle(m_fDiffAngle.y);

		m_rot.y += m_fDiffAngle.y * 0.5f;

		m_rot.y = AdjustAngle(m_rot.y);

		//モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
		m_move.x = sinf(m_rot.y) * CHICK_SPEED;
		m_move.z = cosf(m_rot.y) * CHICK_SPEED;
	}
	else
	{
		//モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
		m_move.x = sinf(m_rot.y) * -CHICK_SPEED * 0.6f;
		m_move.z = cosf(m_rot.y) * -CHICK_SPEED * 0.6f;
	}

}

//=============================================================================
// 攻撃ひよこ(強い)
//=============================================================================
void CChick::AttackS(void)
{
	CPlayer **pPlayer = NULL;
	switch (CManager::GetMode())
	{
	case CManager::MODE_TITLE:
		pPlayer = CTitle::GetPlayer();
		break;
	case CManager::MODE_GAME:
		pPlayer = CGame::GetPlayer();
		break;
	}
	if (m_bAttackS == false)
	{// １位の周りをまわる
		int nDestRank, nChar;
		int nTarget = 99;
		bool bGoal;
		D3DXVECTOR3 Goal;

		if (m_nCntAttackTime < 60 * ATTACK_TIME)
		{
			for (int nCntChar = 0; nCntChar < MAX_MEMBER; nCntChar++)
			{// 1位のやつを見つける
				bGoal = pPlayer[nCntChar]->GetGoal();

				switch (CManager::GetMode())
				{
				case CManager::MODE_TITLE:
					nDestRank = CTitle::GetRanking(nCntChar);
					break;
				case CManager::MODE_GAME:
					nDestRank = CGame::GetRanking(nCntChar);
					break;
				}

				if (!bGoal && nTarget > nDestRank)
				{//ゴールしていない && ターゲットより順位が上
					nTarget = nDestRank;
					nChar = nCntChar;
				}
			}
			m_DestRank = nChar;
			m_nCntAttackTime++;
			m_fHeight = CCOL_MESH_MANAGER::GetHeight(m_pos, pPlayer[m_DestRank]->GetnMap());

			// 向き設定
			m_rot.x = pPlayer[m_DestRank]->GetRot().x;
			m_rot.y -= 0.06f;
			m_rot.z = pPlayer[m_DestRank]->GetRot().z;

			m_rot.y = AdjustAngle(m_rot.y);

			Goal = D3DXVECTOR3(pPlayer[m_DestRank]->GetPos().x + sinf(m_rot.y + D3DX_PI * 0.5f) * (FALL_CHICK_RANGE / 2),
				pPlayer[m_DestRank]->GetPos().y,
				pPlayer[m_DestRank]->GetPos().z + cosf(m_rot.y + D3DX_PI * 0.5f) * (FALL_CHICK_RANGE / 2));
			m_pos += (Goal - m_pos) * 0.1f;
			if (m_pos.y < m_fHeight)
			{
				m_pos.y = m_fHeight;
			}

			if (m_nCntAttackTime == 1)
			{
				D3DXVECTOR2 fSize = D3DXVECTOR2(0.0f, 0.0f);
				for (int nCntParticle = 0; nCntParticle < MAX_SMOKE; nCntParticle++)
				{
					fSize.x = SMOKE_SIZE + (float)(CServer::Rand() % 3);
					fSize.y = SMOKE_SIZE + (float)(CServer::Rand() % 3);

					CParticle::Create(D3DXVECTOR3((sinf(m_rot.y + D3DX_PI) * -30.0f) + m_pos.x,
						m_pos.y,
						(cosf(m_rot.y + D3DX_PI) * -30.0f) + m_pos.z),
						D3DXVECTOR3(sinf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 3 + 1)), cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 1 + 1)), cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 3 + 1))),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
						fSize,
						30,
						CParticle::TEXTURE_SMOKE,
						CParticle::TYPE_TURN,
						m_DestRank);
				}
			}
		}
		if (m_nCntAttackTime >= 60 * ATTACK_TIME)
		{// 隕石になるタイミング
			int fx = CServer::Rand() % FALL_CHICK_RANGE;
			int fz = CServer::Rand() % FALL_CHICK_RANGE;

			m_pos = D3DXVECTOR3(pPlayer[m_DestRank]->GetPos().x + ((FALL_CHICK_RANGE / 2) - fx),
				pPlayer[m_DestRank]->GetPos().y + 80.0f,
				pPlayer[m_DestRank]->GetPos().z + ((FALL_CHICK_RANGE / 2) - fz));

			if (pPlayer[m_nNumPlayer]->GetCntChick() < CHICK_FALL_NUM)
			{// 5匹まで出す
			 // 落ちるひよこ出現
				pPlayer[m_nNumPlayer]->FallChicks(D3DXVECTOR3(
					(sinf(pPlayer[m_DestRank]->GetRot().y + D3DX_PI)) + pPlayer[m_DestRank]->GetPos().x,
					pPlayer[m_DestRank]->GetPos().y + 80.0f,
					(cosf(pPlayer[m_DestRank]->GetRot().y + D3DX_PI)) + pPlayer[m_DestRank]->GetPos().z));
			}

			m_bAttackS = true;
			m_bExplosion = true;
			pPlayer[m_nNumPlayer]->SetCntChick(0);
		}
	}
	else
	{
		// 移動量を設定
		m_move.y = -CHICK_FALL_SPEED;
	}
}

//=============================================================================
// 減速ひよこ(強い)
//=============================================================================
void CChick::AnnoyS(void)
{
	CPlayer **pPlayer = NULL;
	switch (CManager::GetMode())
	{
	case CManager::MODE_TITLE:
		pPlayer = CTitle::GetPlayer();
		break;
	case CManager::MODE_GAME:
		pPlayer = CGame::GetPlayer();
		break;
	}
	if (m_bAttackS == false)
	{
		m_move.x = pPlayer[m_nNumPlayer]->GetMove().x;
		m_move.y = 3.0f;
		m_move.z = pPlayer[m_nNumPlayer]->GetMove().z;

		if (m_pos.y > pPlayer[m_nNumPlayer]->GetPos().y + 200.0f)
		{// 隕石になるタイミング
			pPlayer[m_nNumPlayer]->AnnoyChicks();
			m_bAttackS = true;
		}
	}

	if (pPlayer[m_nNumPlayer]->GetPlayerState() == CPlayer::PLAYERSTATE_SPEEDUP ||
		pPlayer[m_nNumPlayer]->GetPlayerState() == CPlayer::PLAYERSTATE_SPEEDUP_S)
	{// ながやま修正(12/17)
		m_fDisTime = 0.0f;
	}
}

//=============================================================================
// 加速ひよこ
//=============================================================================
void CChick::Speed(void)
{
	CPlayer **pPlayer = NULL;

	switch (CManager::GetMode())
	{
	case CManager::MODE_TITLE:
		pPlayer = CTitle::GetPlayer();
		break;
	case CManager::MODE_GAME:
		pPlayer = CGame::GetPlayer();
		break;
	}
	m_fHeight = CCOL_MESH_MANAGER::GetHeight(m_pos, pPlayer[m_nNumPlayer]->GetnMap()) + 20.0f;

	if (m_bHeight == false && m_fHeight < m_pos.y)
	{
		m_bHeight = true;
	}
	else if (m_bHeight == false && m_fHeight >= m_pos.y)
	{
		// 向き設定
		m_rot = pPlayer[m_nNumPlayer]->GetRot();
	}

	if (m_bHeight == true)
	{
		if (m_bJump == false || (m_bJump == true && m_fHeight > m_pos.y))
		{
			//ジャンプの状態設定
			m_bJump = false;

			m_move.y = 0.0f;
			m_pos.y = pPlayer[m_nNumPlayer]->GetPos().y + 25.0f;


			m_pos.x = pPlayer[m_nNumPlayer]->GetPos().x;
			m_pos.z = pPlayer[m_nNumPlayer]->GetPos().z;

			Jump(2.5f);

			m_bJumpOk = true;
		}

		if (m_bJump == true && m_bJumpOk == true)
		{
			m_rot.y += 0.1f;

			m_rot.y = AdjustAngle(m_rot.y);
		}
	}

	m_move.x = pPlayer[m_nNumPlayer]->GetMove().x * 1.2f;
	m_move.z = pPlayer[m_nNumPlayer]->GetMove().z * 1.2f;
}

//=============================================================================
// 強い加速ひよこ
//=============================================================================
void CChick::SpeedS(void)
{
	CPlayer **pPlayer = NULL;

	switch (CManager::GetMode())
	{
	case CManager::MODE_TITLE:
		pPlayer = CTitle::GetPlayer();
		break;
	case CManager::MODE_GAME:
		pPlayer = CGame::GetPlayer();
		break;
	}

	if (m_bSpeedS == true)
	{
		float aRangePos[MAX_SPEED_CHICK];
		float fRange = 0.0f;

		for (int nCntChick = 0; nCntChick < MAX_SPEED_CHICK; nCntChick++)
		{// ひよこ出現
			aRangePos[nCntChick] = 0.0f;

			aRangePos[nCntChick] = (-1.0f + (nCntChick * 0.25f));

			fRange = CHICK_SPEED_RANGE;

			CChick::Create(
				D3DXVECTOR3(pPlayer[m_nNumPlayer]->GetPos().x + sinf(m_rot.y + -D3DX_PI * aRangePos[nCntChick]) * fRange,
					pPlayer[m_nNumPlayer]->GetPos().y,
					pPlayer[m_nNumPlayer]->GetPos().z + cosf(m_rot.y + -D3DX_PI * aRangePos[nCntChick]) * fRange),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(1.0f, 1.0f, 1.0f),
				CChick::TYPE_SPEED_S,
				CChick::BULLETTYPE_PLAYER,
				CChick::STATE_BULLET,
				m_nNumPlayer);

			if (nCntChick == MAX_SPEED_CHICK - 1)
			{
				m_bSpeedS = false;
			}
		}

		int nCntChick = 0;

		CScene *pScene;
		// プライオリティーチェック
		pScene = CScene::GetTop(EGG_PRIOTITY);
		while (pScene != NULL)
		{// プライオリティー内のリスト構造を最後まで見る
			CScene *pSceneNext = pScene->GetNext();		// 次のオブジェクトを保存

			if (pScene->GetObjType() == OBJTYPE_CHICK)
			{// タイプが障害物だったら
				CChick *pChick = (CChick*)pScene;	// オブジェクトクラスのポインタ変数にする

				if (pChick->GetState() == CChick::STATE_BULLET && pChick->GetType() == CChick::TYPE_SPEED_S && pChick->GetDis() == true)
				{
					pChick->m_bDis = false;
					pChick->m_fRangePos = aRangePos[nCntChick];
					pChick->m_fRange = fRange;

					nCntChick++;
				}
			}
			// Nextに次のSceneを入れる
			pScene = pSceneNext;
		}

		D3DXVECTOR2 fSize;

		for (int nCntParticle = 0; nCntParticle < MAX_SMOKE_SPEED; nCntParticle++)
		{
			fSize.x = SMOKE_SIZE + (float)(CServer::Rand() % 3);
			fSize.y = SMOKE_SIZE + (float)(CServer::Rand() % 3);

			CParticle::Create(D3DXVECTOR3((sinf(m_rot.y + D3DX_PI) * -30.0f) + pPlayer[m_nNumPlayer]->GetPos().x,
				pPlayer[m_nNumPlayer]->GetPos().y - 2.0f,
				(cosf(m_rot.y + D3DX_PI) * -30.0f) + pPlayer[m_nNumPlayer]->GetPos().z),
				D3DXVECTOR3(sinf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 6 + 1)), 0.0f, cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 6 + 1))),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				fSize,
				30,
				CParticle::TEXTURE_SMOKE,
				CParticle::TYPE_TURN,
				m_nNumPlayer);
		}
	}

	m_pos = D3DXVECTOR3(pPlayer[m_nNumPlayer]->GetPos().x + sinf(m_rot.y + -D3DX_PI * m_fRangePos) * m_fRange,
		pPlayer[m_nNumPlayer]->GetPos().y,
		pPlayer[m_nNumPlayer]->GetPos().z + cosf(m_rot.y + -D3DX_PI * m_fRangePos) * m_fRange);// ながやま修正(12/17)

	m_rot = pPlayer[m_nNumPlayer]->GetRot();
}

//=============================================================================
// 減速ひよこ
//=============================================================================
void CChick::Annoy(void)
{
	CPlayer **pPlayer = NULL;

	switch (CManager::GetMode())
	{
	case CManager::MODE_TITLE:
		pPlayer = CTitle::GetPlayer();
		break;
	case CManager::MODE_GAME:
		pPlayer = CGame::GetPlayer();
		break;
	}

	m_fHeight = CCOL_MESH_MANAGER::GetHeight(m_pos, pPlayer[m_nNumPlayer]->GetnMap());

	m_nCntUpDown++;

	float fUpDown = 0.4f;

	if (m_nCntUpDown <= CHICK_UPDOWN_TIME)
	{
		fUpDown *= -1;
	}

	if (m_nCntUpDown >= CHICK_UPDOWN_TIME * 2)
	{
		m_nCntUpDown = 0;
	}

	m_fUpDown += fUpDown;

	// 向き設定
	m_rot.x = pPlayer[m_nNumPlayer]->GetRot().x;
	m_rot.y -= 0.08f;
	m_rot.z = pPlayer[m_nNumPlayer]->GetRot().z;

	m_rot.y = AdjustAngle(m_rot.y);

	m_pos.x = pPlayer[m_nNumPlayer]->GetPos().x + sinf(m_rot.y + D3DX_PI * 0.5f) * ANNOY_RANGE;
	m_pos.y = pPlayer[m_nNumPlayer]->GetPos().y + m_fUpDown;
	m_pos.z = pPlayer[m_nNumPlayer]->GetPos().z + cosf(m_rot.y + D3DX_PI * 0.5f) * ANNOY_RANGE;

	if (m_pos.y < m_fHeight)
	{
		m_pos.y = m_fHeight;
	}
}


//=============================================================================
// プレイヤーのモーション
//=============================================================================
void CChick::UpdateMotion(void)
{
	//モーション
	KEY *pKey, *pNextKey;
	float fRateMotion;
	float fDiffMotion;
	D3DXVECTOR3 rotmotion;
	D3DXVECTOR3 posmotion;

	//キーが最大数を上回らないように
	if (m_aMotionInfo[m_nMotionType].nNumKey <= m_nKey)
	{
		m_nKey = 0;
	}

	//モーション更新
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_apModel[nCntParts] != NULL)
		{
			//現在のキーを取得
			pKey = &m_pKeyInfo[m_nMotionType][m_nKey].aKey[nCntParts];
			//次のキーを取得
			pNextKey = &m_pKeyInfo[m_nMotionType][(m_nKey + 1) % m_aMotionInfo[m_nMotionType].nNumKey].aKey[nCntParts];

			//現在のキーから次のキーへの再生フレーム数におけるモーションカウンターの相対値を算出
			fRateMotion = (float)m_nCountFlame / (float)m_pKeyInfo[m_nMotionType][m_nKey].nFrame;

			//ROT
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->frotX - pKey->frotX;
			//相対値を差分を使って各要素の値を算出
			rotmotion.x = pKey->frotX + (fDiffMotion * fRateMotion);

			//POS
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->fposX - pKey->fposX;
			//相対値を差分を使って各要素の値を算出
			posmotion.x = pKey->fposX + (fDiffMotion * fRateMotion);


			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->frotY - pKey->frotY;
			//相対値を差分を使って各要素の値を算出
			rotmotion.y = pKey->frotY + (fDiffMotion * fRateMotion);
			//POS
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->fposY - pKey->fposY;
			//相対値を差分を使って各要素の値を算出
			posmotion.y = pKey->fposY + (fDiffMotion * fRateMotion);


			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->frotZ - pKey->frotZ;
			//相対値を差分を使って各要素の値を算出
			rotmotion.z = pKey->frotZ + (fDiffMotion * fRateMotion);
			//POS
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->fposZ - pKey->fposZ;
			//相対値を差分を使って各要素の値を算出
			posmotion.z = pKey->fposZ + (fDiffMotion * fRateMotion);


			//パーツを動かす
			m_apModel[nCntParts]->SetRot(rotmotion);

			//POS
			m_apModel[nCntParts]->SetPos(D3DXVECTOR3(m_aKayOffset[nCntParts].fposX + posmotion.x,
				m_aKayOffset[nCntParts].fposY + posmotion.y,
				m_aKayOffset[nCntParts].fposZ + posmotion.z));
		}
	}

	//ループの判定
	switch (m_aMotionInfo[m_nMotionType].bLoop)
	{
	case true:
		//ループする
		//フレームを進める
		m_nCountFlame++;
		//キーの更新
		if (m_nCountFlame >= m_pKeyInfo[m_nMotionType][m_nKey].nFrame)
		{
			if (m_aMotionInfo[m_nMotionType].nNumKey - 1 == m_nKey)
			{
				m_nKey = 0;
			}
			else
			{
				m_nKey += 1;
			}
			m_nCountFlame = 0;
		}

		break;
	case false:
		//ループしない
		if (m_aMotionInfo[m_nMotionType].nNumKey - 1 > m_nKey)
		{//フレームを進める
			m_nCountFlame++;
		}
		else if (m_aMotionInfo[m_nMotionType].nNumKey - 1 == m_nKey)
		{
			//if (m_nAnimnow == PLAYERANIM_ATTACK)
			//{//攻撃モーション
			//	m_nAttackDelay++;
			//	if (m_nAttackDelay > 20)
			//	{
			//		m_bAttack = false;
			//		m_nAttackDelay = 0;
			//	}
			//}
			m_bMotionEnd = true;
		}
		//キーの更新
		if (m_nCountFlame >= m_pKeyInfo[m_nMotionType][m_nKey].nFrame)
		{
			if (m_aMotionInfo[m_nMotionType].nNumKey > m_nKey)
			{
				m_nKey += 1;
			}
			m_nCountFlame = 0;
		}
		break;
	}

#ifdef  _DEBUG
	/*CDebugProc::Print(" Numキー  : (%d)\n", m_nKey);
	CDebugProc::Print(" m_nCountFlame  : (%d)\n", m_nCountFlame);*/

#endif

}

//=============================================================================
// ファイル読み込み
//=============================================================================
void CChick::FileLoad(void)
{
	//デバイスを取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//ファイル読み込み用変数
	FILE *pFile;		//ファイルポインタ
	char *pStrcur;		//現在の先頭の文字列
	char aLine[256];	//文字列
	char aStr[256];		//一時保存文字列
	int nIndex = 0;		//現在のインデックス
	int nWord = 0;		//ポップで返された値を保持

#if 1
						//ファイルを開く 読み込み
	pFile = fopen(FILE_NAME_CHICK, "r");
	//NULLチェック
	if (pFile != NULL)
	{
		for (int nCntMotion = 0; nCntMotion < MAX_CHICK_MOTION; )
		{
			//文字列の先頭を設定
			pStrcur = ReadLine(pFile, &aLine[0]);
			//文字列を取り出す
			strcpy(aStr, pStrcur);

			//文字列のデータ 比較する文字列 比較する文字数
			if (memcmp(pStrcur, "NUM_MODEL = ", strlen("NUM_MODEL = ")) == 0)
			{
				//頭出し
				pStrcur += strlen("NUM_MODEL = ");
				//文字列の先頭を設定
				strcpy(aStr, pStrcur);
				//文字列抜き出し
				g_nChickNumModel = atoi(pStrcur);

				for (int nCntModel = 0; nCntModel < g_nChickNumModel; nCntModel++)
				{
					//文字列の先頭を設定
					pStrcur = ReadLine(pFile, &aLine[0]);
					//文字列を取り戻す
					strcpy(aStr, pStrcur);
					if (memcmp(pStrcur, "MODEL_FILENAME = ", strlen("MODEL_FILENAME = ")) == 0)
					{
						int nNumber = nCntModel + CModel::PARTS_CHICK_BODY;
						LPD3DXBUFFER &m_pBuffMat = CModel::GetpBuffMat(nNumber);
						DWORD &m_nNumMat = CModel::GetnNumMat(nNumber);
						LPD3DXMESH &m_pMesh = CModel::GetpMesh(nNumber);

						//頭出し
						pStrcur += strlen("MODEL_FILENAME = ");

						//文字列を設定
						strcpy(aStr, pStrcur);

						//必要な文字列の最後の文字までの文字数を数える
						int nNullNum = PopString(pStrcur, &aStr[0]);

						//文字列を取り戻す
						strcpy(aStr, pStrcur);

						//最後の文字にNULL文字を入れる
						aStr[nNullNum - 1] = '\0';

						//対象の文字列から抜き出し
						strcpy(&g_aChickFileNameModel[nCntModel][0], aStr);

						// Xファイルの読み込み
						D3DXLoadMeshFromX(&g_aChickFileNameModel[nCntModel][0],
							D3DXMESH_SYSTEMMEM,
							pDevice,
							NULL,
							&m_pBuffMat,
							NULL,
							&m_nNumMat,
							&m_pMesh);

						//CModel::SetModelType(CModel::TYPE_CHICK_BODY + nCntModel);
					}
				}
				//文字列の先頭を設定
				pStrcur = ReadLine(pFile, &aLine[0]);
				//文字列を取り出す
				strcpy(aStr, pStrcur);
			}


			//文字列のデータ 比較する文字列 比較する文字数
			if (memcmp(pStrcur, "CHARACTERSET", 12) == 0)
			{
				while (1)
				{
					//文字列の先頭を設定
					pStrcur = ReadLine(pFile, &aLine[0]);
					//文字列を取り出す
					strcpy(aStr, pStrcur);
					if (memcmp(pStrcur, "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("NUM_PARTS = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_nNumParts = atoi(pStrcur);
					}
					if (memcmp(pStrcur, "PARTSSET", strlen("PARTSSET")) == 0)
					{
						while (1)
						{
							//文字列の先頭を設定
							pStrcur = ReadLine(pFile, &aLine[0]);

							//INDEXを読み込み
							if (memcmp(pStrcur, "INDEX = ", strlen("INDEX = ")) == 0)
							{
								//頭出し
								pStrcur += strlen("INDEX = ");
								//文字列の先頭を設定
								strcpy(aStr, pStrcur);
								//文字列抜き出し
								nIndex = atoi(pStrcur);
							}
							//PARENTを読み込み
							if (memcmp(pStrcur, "PARENT = ", strlen("PARENT = ")) == 0)
							{
								//頭出し
								pStrcur += strlen("PARENT = ");
								//文字列の先頭を設定
								strcpy(aStr, pStrcur);
								//文字列抜き出し
								m_aIndexParent[nIndex] = atoi(pStrcur);
							}
							//POSを読み込み
							if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
							{
								//頭出し
								pStrcur += strlen("POS = ");
								//文字列の先頭を設定
								strcpy(aStr, pStrcur);

								//文字数を返してもらう
								nWord = PopString(pStrcur, &aStr[0]);
								//文字列変換
								m_aKayOffset[nIndex].fposX = (float)atof(pStrcur);
								//文字数分進める
								pStrcur += nWord;

								//文字数を返してもらう
								nWord = PopString(pStrcur, &aStr[0]);
								//文字列変換
								m_aKayOffset[nIndex].fposY = (float)atof(pStrcur);
								//文字数分進める
								pStrcur += nWord;

								//文字数を返してもらう
								nWord = PopString(pStrcur, &aStr[0]);
								//文字列変換
								m_aKayOffset[nIndex].fposZ = (float)atof(pStrcur);

							}
							//ROTを読み込み
							if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
							{
								//頭出し
								pStrcur += strlen("ROT = ");
								//文字列の先頭を設定
								strcpy(aStr, pStrcur);

								//文字数を返してもらう
								nWord = PopString(pStrcur, &aStr[0]);
								//文字列変換
								m_aKayOffset[nIndex].frotX = (float)atof(pStrcur);

								//文字数分進める
								pStrcur += nWord;
								//文字数を返してもらう
								nWord = PopString(pStrcur, &aStr[0]);
								//文字列変換
								m_aKayOffset[nIndex].frotY = (float)atof(pStrcur);

								//文字数分進める
								pStrcur += nWord;
								//文字数を返してもらう
								nWord = PopString(pStrcur, &aStr[0]);
								//文字列変換
								m_aKayOffset[nIndex].frotZ = (float)atof(pStrcur);
							}
							//パーツセット終了
							else if (memcmp(pStrcur, "END_PARTSSET", strlen("END_PARTSSET")) == 0)
							{
								break;
							}
						}
					}
					//キャラクターセット終了
					else if (memcmp(pStrcur, "END_CHARACTERSET", strlen("END_CHARACTERSET")) == 0)
					{
						break;
					}
				}
				//文字列の先頭を設定
				pStrcur = ReadLine(pFile, &aLine[0]);
				//文字列を取り出す
				strcpy(aStr, pStrcur);
			}

			//モーション読み込み
			if (memcmp(pStrcur, "MOTIONSET", strlen("MOTIONSET")) == 0)
			{
				//頭出し
				pStrcur += strlen("MOTIONSET");

				while (1)
				{
					//文字列の先頭を設定
					pStrcur = ReadLine(pFile, &aLine[0]);
					//文字列を取り出す
					strcpy(aStr, pStrcur);

					if (memcmp(pStrcur, "LOOP = ", strlen("LOOP = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("LOOP = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);

						switch (atoi(pStrcur))
						{
						case 0:
							//文字列抜き出し
							m_aMotionInfo[nCntMotion].bLoop = false;
							break;
						case 1:
							//文字列抜き出し
							m_aMotionInfo[nCntMotion].bLoop = true;
							break;
						}
						//文字列の先頭を設定
						pStrcur = ReadLine(pFile, &aLine[0]);
					}
					if (memcmp(pStrcur, "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("NUM_KEY = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_aMotionInfo[nCntMotion].nNumKey = atoi(pStrcur);

						//文字列の先頭を設定
						pStrcur = ReadLine(pFile, &aLine[0]);
						//文字列を取り出す
						strcpy(aStr, pStrcur);
					}

					//キーの設定
					for (int nCntKey = 0; nCntKey < m_aMotionInfo[nCntMotion].nNumKey;)
					{
						if (memcmp(pStrcur, "KEYSET", strlen("KEYSET")) == 0)
						{
							//頭出し
							pStrcur += strlen("KEYSET");
							//文字列の先頭を設定
							strcpy(aStr, pStrcur);
							//文字列の先頭を設定
							pStrcur = ReadLine(pFile, &aLine[0]);

							if (memcmp(pStrcur, "FRAME = ", strlen("FRAME = ")) == 0)
							{
								//頭出し
								pStrcur += strlen("FRAME = ");

								m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].nFrame = atoi(pStrcur);

								//文字列の先頭を設定
								strcpy(aStr, pStrcur);
								//文字列の先頭を設定
								pStrcur = ReadLine(pFile, &aLine[0]);
							}

							//パーツ分回す
							for (int nCntParts = 0; nCntParts < m_nNumParts;)
							{
								if (memcmp(pStrcur, "KEY", strlen("KEY")) == 0)
								{
									//文字列の先頭を設定
									pStrcur = ReadLine(pFile, &aLine[0]);

									if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
									{
										//頭出し
										pStrcur += strlen("POS = ");
										//文字列の先頭を設定
										strcpy(aStr, pStrcur);

										//文字数を返してもらう
										nWord = PopString(pStrcur, &aStr[0]);
										//POS.X代入
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposX = (float)atof(pStrcur);
										//文字数分進める
										pStrcur += nWord;

										//文字数を返してもらう
										nWord = PopString(pStrcur, &aStr[0]);
										//POS.Y代入
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposY = (float)atof(pStrcur);
										//文字数分進める
										pStrcur += nWord;

										//文字数を返してもらう
										nWord = PopString(pStrcur, &aStr[0]);
										//POS.Z代入
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposZ = (float)atof(pStrcur);
										//文字列の先頭を設定
										pStrcur = ReadLine(pFile, &aLine[0]);
									}
									//ROTを読み込み
									if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
									{
										//頭出し
										pStrcur += strlen("ROT = ");
										//文字列の先頭を設定
										strcpy(aStr, pStrcur);

										//文字数を返してもらう
										nWord = PopString(pStrcur, &aStr[0]);
										//RotX
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotX = (float)atof(pStrcur);
										//文字数分進める
										pStrcur += nWord;

										//文字数を返してもらう
										nWord = PopString(pStrcur, &aStr[0]);
										//RotY
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotY = (float)atof(pStrcur);
										//文字数分進める
										pStrcur += nWord;

										//文字数を返してもらう
										nWord = PopString(pStrcur, &aStr[0]);
										//RotZ
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotZ = (float)atof(pStrcur);

										//文字列の先頭を設定
										pStrcur = ReadLine(pFile, &aLine[0]);
									}
									if (memcmp(pStrcur, "END_KEY", strlen("END_KEY")) == 0)
									{
										//頭出し
										pStrcur += strlen("END_KEY");
										//文字列の先頭を設定
										strcpy(aStr, pStrcur);
										//文字列の先頭を設定
										pStrcur = ReadLine(pFile, &aLine[0]);
										//パーツのカウントを進める
										nCntParts++;
									}
								}
								else
								{
									//文字列の先頭を設定
									pStrcur = ReadLine(pFile, &aLine[0]);
								}
							}
							if (memcmp(pStrcur, "END_KEYSET", strlen("END_KEYSET")) == 0)
							{
								//文字列の先頭を設定
								pStrcur = ReadLine(pFile, &aLine[0]);
								//カウントを進める
								nCntKey++;
							}
						}
						else
						{
							//文字列の先頭を設定
							pStrcur = ReadLine(pFile, &aLine[0]);
						}

					}
					if (memcmp(pStrcur, "END_MOTIONSET", strlen("END_MOTIONSET")) == 0)
					{
						//モーションの情報をセット
						m_pKeyInfo[nCntMotion] = &m_aMotionInfo[nCntMotion].aKayInfo[0];
						nCntMotion++;
						break;
					}
				}
			}
			//スクリプトの終わり
			if (memcmp(pStrcur, "END_SCRIPT	", strlen("END_SCRIPT")) == 0)
			{
				break;
			}
		}
	}
	else
	{	//ファイルが開けなかった
		printf("ファイルが開けませんでした\n");
	}
	//ファイルを閉じる
	fclose(pFile);
#endif

}

//=============================================================================
//　ファイル読み込み無効文を排除
//=============================================================================
char *CChick::ReadLine(FILE *pFile, char *pDst)
{
	while (1)
	{
		//１行分読み込み
		fgets(&pDst[0], 256, pFile);

		//文字列のデータ 比較する文字列 比較する文字数
		if (memcmp(pDst, "#", strlen("#")) == 0)
		{
			pDst += strlen("\n");
		}
		//文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pDst, "\t", strlen("\t")) == 0)
		{
			pDst += strlen("\t");
			while (1)
			{
				if (memcmp(pDst, "\t", strlen("\t")) == 0)
				{
					pDst += strlen("\t");
				}
				else
				{
					break;
				}
			}
			break;
		}
		//文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pDst, " ", strlen(" ")) == 0)
		{
			pDst += strlen(" ");
			while (1)
			{
				if (memcmp(pDst, " ", strlen(" ")) == 0)
				{
					pDst += strlen(" ");
				}
				else
				{
					break;
				}
				break;
			}
		}
		//文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pDst, "\n", strlen("\n")) == 0)
		{
			pDst += strlen("\n");
		}
		else
		{
			break;
		}
	}
	return pDst;
}

//=============================================================================
//　ファイル読み込み先頭を排除
//=============================================================================
char * CChick::GetLineTop(char * pStr)
{
	while (1)
	{
		//文字列のデータ 比較する文字列 比較する文字数
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{
			pStr += strlen(" ");
			while (1)
			{
				if (memcmp(pStr, " ", strlen(" ")) == 0)
				{
					pStr += strlen(" ");
				}
				else
				{
					break;
				}
				break;
			}
		}
		//文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{
			pStr += strlen("\t");
			while (1)
			{
				if (memcmp(pStr, "\t", strlen("\t")) == 0)
				{
					pStr += strlen("\t");
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}
	return pStr;
}

//=============================================================================
//　文字数を返す
//=============================================================================
int CChick::PopString(char * pStr, char * pDest)
{
	int nWord = 0;

	while (1)
	{	//頭出し
		pStr += 1;
		nWord += 1;
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{	//頭出し
			pStr = "\0";
			nWord += 1;
			break;
		}
		if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{	//頭出し
			pStr = "\0";
			nWord += (int)strlen("\t");
			break;
		}
		//文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pStr, "\n", strlen("\n")) == 0)
		{
			//頭出し
			nWord += (int)strlen("\n");
			break;
		}
	}
	strcpy(pDest, pStr);
	//文字列の数を返す
	return nWord;
}