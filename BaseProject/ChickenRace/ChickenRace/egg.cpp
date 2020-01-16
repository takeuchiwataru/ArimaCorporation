//=============================================================================
//
// 卵の処理 [egg.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "scene.h"
#include "egg.h"
#include "manager.h"
#include "object.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"
#include "shadow.h"
#include "tutorial.h"
#include "ColMesh.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_SPEED				(5.0f)
#define OBJCT_ANGLE_REVISION	(0.2f)		// 角度補正
#define EFFECT_HIGHT			(250.0f)	// エミッターの高さ
#define FOUNTAIN_UP				(20.0f)		// 噴水の上昇させる値
#define EGG_SPEED				(15.0f)		// 卵が飛んでくスピード
#define EXPLOSION_RANGE			(80)		// 爆発の範囲
#define EGG_PARTICLE			(30)		// パーティクルの数
#define EGG_DIS_TIME			(30)		// 消えるまでの時間
#define EGG_GRAVITY				(0.2f)		// 重力

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
D3DXVECTOR3 CEgg::m_VtxMaxModel[MAX_EGG] = {};
D3DXVECTOR3 CEgg::m_VtxMinModel[MAX_EGG] = {};

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CEgg::CEgg() : CModel3D(EGG_PRIOTITY, CScene::OBJTYPE_EGG)
{
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 大きさ
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_FNor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDestAngle = 0.0f;
	m_fDiffAngle = 0.0f;
	m_fHeight = 0.0f;
	m_fLength = 0.0f;
	m_nRank = 0;
	m_nNumPlayer = 0;
	m_nHatchingTimer = 0;
	m_nExplosion = 0;
	m_nMap = 0;
	m_nDisTimer = 0;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CEgg::~CEgg() {}

//===============================================================================
//　生成
//===============================================================================
CEgg * CEgg::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, EGGTYPE eggType, BULLETTYPE bulletType, int nNumPlayer)
{
	CEgg *pEgg = NULL;

	// NULLチェック
	if (pEgg == NULL)
	{// メモリの動的確保

		pEgg = new CEgg;

		if (pEgg != NULL)
		{
			// モデルの設定
			pEgg->SetModelType(MODEL_TYPE_EGG);
			// テクスチャの設定
			pEgg->SetTextureType(eggType + TEXTURE_TYPE_EGG_K);
			// サイズを代入
			pEgg->m_scale = scale;
			// サイズを親クラスに代入
			pEgg->SetScale(scale);
			// 卵の種類を代入
			pEgg->m_eggType = eggType;
			// 弾の種類を代入
			pEgg->m_bulletType = bulletType;
			// オブジェクトクラスの生成
			pEgg->Init();
			// 位置を代入
			pEgg->SetPosition(pos);
			pEgg->m_pos = pos;
			pEgg->m_fHeight = pos.y;
			// 回転を反映
			pEgg->SetRot(rot);
			// 何位の卵か
			pEgg->m_nNumPlayer = nNumPlayer;
		}
	}

	return pEgg;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEgg::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//3DモデルのInit
	CModel3D::Init();

	// 各種情報の代入
	CModel3D::SetScale(m_scale);

	//変数の初期化
	m_pObjBill = NULL;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_FNor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fHeight = 0.0f;
	m_fDestAngle = 0.0f;
	m_fDiffAngle = 0.0f;
	m_fLength = 3.0f;
	m_bJump = false;
	m_eggState = EGGSTATE_NORMAL;
	m_nRank = 0;
	m_nNumPlayer = 0;
	m_nHatchingTimer = 0;
	m_nMap = 0;
	m_nExplosion = 0;
	m_nDisTimer = 0;
	m_bThrow = false;
	m_bExplosion = false;
	m_bDis = true;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEgg::Uninit(void)
{
	//オブジェクトビルボード
	m_pObjBill = NULL;

	//3DモデルのUninit
	CModel3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEgg::Update(void)
{
	m_nHatchingTimer++;

	m_posOld = m_pos;	//前回の位置を保存する

	if (m_bDis == false)
	{
		m_nDisTimer++;

		if (m_nDisTimer > 60 * EGG_DIS_TIME)
		{// 消す
			m_nDisTimer = 0;
			Uninit();
		}
	}

	Item();

	Move();

	CModel3D::SetMove(m_move);
	CModel3D::SetPosition(D3DXVECTOR3(m_pos.x, m_pos.y + 10.0f, m_pos.z));
	CModel3D::SetRot(m_rot);
}

//=============================================================================
// 描画処理
//=============================================================================
void CEgg::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//位置の初期化
	D3DXVECTOR3 Modelpos = CModel3D::GetPosition();
	//ゲームの情報
	CManager::MODE pMode = CManager::GetMode();

	//頂点法線の自動正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	if (m_bExplosion == false)
	{
		//描画処理
		CModel3D::Draw();
	}

	//頂点法線の自動正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//===============================================================================
// Xファイルの読み込み
//===============================================================================
HRESULT CEgg::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int			nNumVtx = 0;						//頂点数
	DWORD		sizeFVF;							//頂点フォーマットのサイズ
	BYTE		*pVtxBuff;							//頂点バッファへのポインタ

													//モデルの最大値・最小値を取得する
	for (int nCntModel = 0; nCntModel < MAX_EGG; nCntModel++)
	{
		m_VtxMaxModel[nCntModel] = D3DXVECTOR3(-10000, -10000, -10000);	//最大値
		m_VtxMinModel[nCntModel] = D3DXVECTOR3(10000, 10000, 10000);	//最小値

		CModel3D::MODEL_TYPE type;
		switch (nCntModel)
		{
		case EGGTYPE_ATTACK:type = MODEL_TYPE_EGG;	break;
		case EGGTYPE_ANNOY:	type = MODEL_TYPE_EGG;	break;
		case EGGTYPE_SPEED:	type = MODEL_TYPE_EGG;	break;
		}

		LPD3DXMESH &Mesh = MeshLoad(type);

		if (Mesh != NULL)
		{
			//頂点数を取得
			nNumVtx = Mesh->GetNumVertices();

			//頂点フォーマットのサイズを取得
			sizeFVF = D3DXGetFVFVertexSize(Mesh->GetFVF());

			//頂点バッファのロック
			Mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//頂点座標の代入

																//最大値
				if (vtx.x > m_VtxMaxModel[nCntModel].x)
				{
					m_VtxMaxModel[nCntModel].x = vtx.x;
				}
				if (vtx.y > m_VtxMaxModel[nCntModel].y)
				{
					m_VtxMaxModel[nCntModel].y = vtx.y;
				}
				if (vtx.z > m_VtxMaxModel[nCntModel].z)
				{
					m_VtxMaxModel[nCntModel].z = vtx.z;
				}
				//最小値
				if (vtx.x < m_VtxMinModel[nCntModel].x)
				{
					m_VtxMinModel[nCntModel].x = vtx.x;
				}
				if (vtx.y < m_VtxMinModel[nCntModel].y)
				{
					m_VtxMinModel[nCntModel].y = vtx.y;
				}
				if (vtx.z < m_VtxMinModel[nCntModel].z)
				{
					m_VtxMinModel[nCntModel].z = vtx.z;
				}

				//サイズ文のポインタを進める
				pVtxBuff += sizeFVF;
			}

			//頂点バッファのアンロック
			Mesh->UnlockVertexBuffer();
		}
	}

	return S_OK;
}

//===============================================================================
// テクスチャの破棄
//===============================================================================
void CEgg::UnLoad(void)
{

}

//=============================================================================
// 使ったときの処理
//=============================================================================
void CEgg::Item(void)
{
	if (m_eggState == EGGSTATE_BULLET)
	{
		switch (m_eggType)
		{
			// 攻撃
		case EGGTYPE_ATTACK:
			if (m_bExplosion == true)
			{// 爆発中
				m_nExplosion++;

				if (m_nExplosion > EXPLOSION_TIME)
				{// 削除
					m_nExplosion = 0;
					Uninit();
				}
			}
			break;

			// 加速
		case EGGTYPE_SPEED:
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

			m_pos = D3DXVECTOR3(pPlayer[m_nNumPlayer]->GetPos().x, pPlayer[m_nNumPlayer]->GetPos().y + 5.0f, pPlayer[m_nNumPlayer]->GetPos().z);

			m_rot.z = D3DX_PI * 0.5f;
			m_rot.x += 0.5f;
			m_rot.x = AdjustAngle(m_rot.x);

			m_rot.y = pPlayer[m_nNumPlayer]->GetRot().y;

			if (pPlayer[m_nNumPlayer]->GetPlayerState() == CPlayer::PLAYERSTATE_NORMAL)
			{// 加速状態じゃなくなったら削除
				Uninit();
			}
			break;
		}

		Bullet();
	}
}

//===============================================================================
// 動き
//===============================================================================
void CEgg::Move(void)
{
	float fGravity = EGG_GRAVITY;

	if (m_eggType == EGGTYPE_ATTACK && m_eggState == EGGSTATE_BULLET)
	{
		fGravity = EGG_GRAVITY + 0.1f;
	}
	else if (m_eggType == EGGTYPE_ANNOY && m_eggState == EGGSTATE_BULLET)
	{
		fGravity = 0.0f;
	}

	m_move.y -= cosf(0) * fGravity;

	m_pos.x += m_move.x;
	m_pos.z += m_move.z;
	m_pos.y += m_move.y;

	if (m_eggState == EGGSTATE_CHASE || m_eggType == EGGTYPE_ANNOY || m_eggType == EGGTYPE_ATTACK || m_eggType == EGGTYPE_SPEED)
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
			//ジャンプの状態設定
			m_bJump = false;

			m_move.y = 0.0f;
			m_pos.y = m_fHeight;

			if (m_eggType == EGGTYPE_ATTACK && m_eggState == EGGSTATE_BULLET)
			{// 卵を消す
				if (m_bExplosion == false)
				{
					m_bExplosion = true;
					D3DXVECTOR2 fSize;

					for (int nCntParticle = 0; nCntParticle < EGG_PARTICLE; nCntParticle++)
					{
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
					CCylinder::Create(m_pos, m_rot, CCylinder::TYPE_EATK);
				}
			}
		}
	}
}

//===============================================================================
// 当たり判定
//===============================================================================
bool CEgg::CollisionEgg(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld)
{
	//あたっているかあたってないか
	bool bHit = false;

	if (m_eggState == EGGSTATE_BULLET)
	{
		// 各種情報の取得
		D3DXVECTOR3 ModelPos = CModel3D::GetPosition();		// 位置
		D3DXVECTOR3 VtxMax = m_VtxMaxModel[m_eggType];		// モデルの最大値
		D3DXVECTOR3 VtxMin = m_VtxMinModel[m_eggType];		// モデルの最小値
		D3DXVECTOR3 rot = CModel3D::GetRot();

		D3DXVECTOR3 ModelMax = ModelPos + VtxMax;			// 位置込みの最大値
		D3DXVECTOR3 ModelMin = ModelPos + VtxMin;			// 位置込みの最小値

		float fDepth = PLAYER_DEPTH;

		if (m_eggType == EGGTYPE_ATTACK)
		{
			if (m_bExplosion == true)
			{
				fDepth = EXPLOSION_RANGE;
			}
		}

		if (pPos->x >= ModelMin.x - fDepth && pPos->x <= ModelMax.x + fDepth)
		{// Zの範囲内にいるw
			if (pPos->z >= ModelMin.z - fDepth && pPos->z <= ModelMax.z + fDepth)
			{// Xの範囲内にいる
				if (pPosOld->y >= ModelMax.y && pPos->y <= ModelMax.y)
				{// オブジェクトの上から当たる場合
					bHit = true;
				}
				else if (pPosOld->y + PLAYER_HEIGHT <= ModelMin.y && pPos->y + PLAYER_HEIGHT >= ModelMin.y)
				{// オブジェクトの下から当たる場合
					bHit = true;
				}

				if (!(pPos->y >= ModelMax.y) && !(pPos->y + PLAYER_HEIGHT <= ModelMin.y))
				{// オブジェクト横との当たり判定
					bHit = true;
				}
			}
		}

		// 位置の代入
		CModel3D::SetPosition(ModelPos);
	}
	return bHit;
}

//=============================================================================
// ジャンプ
//=============================================================================
void CEgg::Jump(float fJump)
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
void CEgg::Bullet(void)
{
	if (m_bulletType == BULLETTYPE_PLAYER)
	{
		if (m_eggType == EGGTYPE_ATTACK)
		{//タイプが敵だったら
		 //モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
			float fEggSpeed = EGG_SPEED;

			if (m_bExplosion == true)
			{
				fEggSpeed = 0.0f;
			}

			m_move.x = sinf(m_rot.y) * fEggSpeed;
			m_move.z = cosf(m_rot.y) * fEggSpeed;

			m_rot.x = D3DX_PI * 0.5f;

			if (m_pos.y < -3000.0f)
			{
				Uninit();
			}

			CModel3D::SetRot(m_rot);
		}
	}
}

//=============================================================================
// 角度の調節
//=============================================================================
float CEgg::AdjustAngle(float rot)
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