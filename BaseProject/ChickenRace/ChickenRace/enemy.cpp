//=============================================================================
//
// 敵の処理（仮） [enemy.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "scene.h"
#include "enemy.h"
#include "manager.h"
#include "object.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"
#include "shadow.h"
#include "tutorial.h"
#include "egg.h"
#include "feed.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_SPEED				(5.0f)
#define PLAYER_DEPTH			(50)		// プレイヤーの幅調整用
#define OBJCT_ANGLE_REVISION	(0.2f)		// 角度補正
#define EFFECT_HIGHT			(250.0f)	// エミッターの高さ
#define FOUNTAIN_UP				(20.0f)		// 噴水の上昇させる値

#define DAMAGE_TIME				(60)		// ダメージを食らっている時間

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
LPD3DXMESH CEnemy::m_pMeshModel = NULL;						//メッシュ情報へのポインタ
LPD3DXBUFFER CEnemy::m_pBuffMatModel = NULL;					//マテリアルの情報へのポインタ
DWORD CEnemy::m_nNumMatModel = {};							//マテリアルの情報数
LPDIRECT3DTEXTURE9 CEnemy::m_pMeshTextures = NULL;
D3DXVECTOR3 CEnemy::m_VtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 CEnemy::m_VtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CEnemy::CEnemy() : CModel3D(ENEMY_PRIOTITY, CScene::OBJTYPE_ENEMY)
{
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 大きさ
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntMove = 0;
	m_nCntDamage = 0;
	m_fSpeed = 0.0f;
	m_nNumEgg = 0;
	m_nCntFrame = 0;
	m_nCountSpeed = 0;

	//卵のポインタを初期化
	for (int nCntEgg = 0; nCntEgg < MAX_EGG; nCntEgg++)
	{
		m_pEgg[nCntEgg] = NULL;
	}

	for (int nCntEggPos = 0; nCntEggPos < MAX_FRAME; nCntEggPos++)
	{
		m_OldEggRot[nCntEggPos] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_abJump[nCntEggPos] = false;
	}
}
//===============================================================================
//　デストラクタ
//===============================================================================
CEnemy::~CEnemy() {}

//===============================================================================
//　生成
//===============================================================================
CEnemy * CEnemy::Create(D3DXVECTOR3 pos)
{
	CEnemy *pEnemy = NULL;

	// NULLチェック
	if (pEnemy == NULL)
	{// メモリの動的確保

		pEnemy = new CEnemy;

		if (pEnemy != NULL)
		{
			// 種類の設定
			pEnemy->BindModel(m_pMeshModel, m_pBuffMatModel, m_nNumMatModel, m_pMeshTextures,
				m_VtxMaxModel, m_VtxMinModel);
			// サイズを代入
			pEnemy->m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			// サイズを親クラスに代入
			pEnemy->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			// オブジェクトクラスの生成
			pEnemy->Init();
			// 位置を代入
			pEnemy->SetPosition(pos);
		}
	}

	return pEnemy;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemy::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//3DモデルのInit
	CModel3D::Init();

	// 位置の初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//位置の代入
	CModel3D::SetPosition(m_pos);

	// 各種情報の代入
	CModel3D::SetScale(m_scale);

	//変数の初期化
	m_pObjBill = NULL;
	m_State = ENEMYSTATE::ENEMYSTATE_NORMAL;
	m_bSpeedDown = false;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntMove = 0;
	m_nCntDamage = 0;
	m_fSpeed = 0.0f;
	m_State = ENEMYSTATE_NORMAL;
	m_nNumEgg = 0;
	m_nCountSpeed = 0;
	m_nCntFrame = -1;
	m_bJumpSave = false;
	m_bDamage = false;

	for (int nCntEggPos = 0; nCntEggPos < MAX_FRAME; nCntEggPos++)
	{
		m_OldEggRot[nCntEggPos] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_abJump[nCntEggPos] = false;
	}

	//卵のポインタを初期化
	for (int nCntEgg = 0; nCntEgg < MAX_EGG; nCntEgg++)
	{
		m_pEgg[nCntEgg] = NULL;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	//オブジェクトビルボード
	m_pObjBill = NULL;

	//3DモデルのUninit
	CModel3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{
	CInputKeyBoard * pInputKeyboard = CManager::GetInput();		//キーボードの取得
	m_pos = CModel3D::GetPosition();
	m_posOld = m_pos;

	CollisionFeed();		// 餌の当たり判定

	CollisionEgg();			// 卵との当たり判定

	ChaseEgg();	// 卵がついてくる処理

	if (pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD5) == true)
	{// 弾発射
		BulletEgg();
	}

	switch (m_State)
	{
	case ENEMYSTATE_NORMAL:
		m_fSpeed = 3.0f;
		break;

	case ENEMYSTATE_SPEEDDOWN:
		m_fSpeed = 1.0f;
		break;

	case ENEMYSTATE_SPEEDUP:
		break;

	case ENEMYSTATE_DAMAGE:
		m_fSpeed = 0.0f;
		break;
	}

	if (m_State == ENEMYSTATE_SPEEDUP)
	{// スピードアイテムを使ったとき
		m_nCountSpeed++;

		/*if (m_nCountSpeed > SPEEDUP_TIME)
		{
		m_State = ENEMYSTATE_NORMAL;
		m_nCountSpeed = 0;
		}*/
	}

	if (m_bDamage == true)
	{
		m_nCntDamage++;

		int nDamageTime = 0;	// 状態が変わる時間の長さ

		if (m_State == ENEMYSTATE_DAMAGE)
		{// 攻撃を食らったとき
			nDamageTime = DAMAGE_TIME;
		}
		else if (m_State == ENEMYSTATE_SPEEDDOWN)
		{// スピードダウンを食らったとき
			nDamageTime = SPEEDDOWN_TIME;
		}

		if (m_nCntDamage > nDamageTime)
		{
			m_State = ENEMYSTATE_NORMAL;
			m_nCntDamage = 0;

			m_bDamage = false;
		}
	}

	m_nCntMove++;

	m_move.z = m_fSpeed;

	if (m_nCntMove >= 120)
	{
		m_move.z *= -1.0f;
	}

	if (m_nCntMove >= 240)
	{
		m_nCntMove = 0;
	}

	m_pos.z += m_move.z;
	m_pos.x += m_move.x;

	CModel3D::SetMove(m_move);
	CModel3D::SetPosition(m_pos);

	//距離の取得
	float fLength = CModel3D::GetLength();

	if (CModel3D::GetDelete() == true) { Uninit(); }
}
//=============================================================================
// 描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//位置の初期化
	D3DXVECTOR3 Modelpos = CModel3D::GetPosition();
	//ゲームの情報
	CManager::MODE pMode = CManager::GetMode();

	//頂点法線の自動正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	//描画処理
	CModel3D::Draw();

	//頂点法線の自動正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//===============================================================================
// Xファイルの読み込み
//===============================================================================
HRESULT CEnemy::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\Enemy\\box.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel, NULL, &m_nNumMatModel, &m_pMeshModel);

	//マテリアル情報からテクスチャの取得
	pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();


	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

						//モデルの最大値・最小値を取得する

	m_VtxMaxModel = D3DXVECTOR3(-10000, -10000, -10000);	//最大値
	m_VtxMinModel = D3DXVECTOR3(10000, 10000, 10000);	//最小値

														//頂点数を取得
	nNumVtx = m_pMeshModel->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMeshModel->GetFVF());

	//頂点バッファのロック
	m_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//頂点座標の代入

														//最大値
		if (vtx.x > m_VtxMaxModel.x)
		{
			m_VtxMaxModel.x = vtx.x;
		}
		if (vtx.y > m_VtxMaxModel.y)
		{
			m_VtxMaxModel.y = vtx.y;
		}
		if (vtx.z > m_VtxMaxModel.z)
		{
			m_VtxMaxModel.z = vtx.z;
		}
		//最小値
		if (vtx.x < m_VtxMinModel.x)
		{
			m_VtxMinModel.x = vtx.x;
		}
		if (vtx.y < m_VtxMinModel.y)
		{
			m_VtxMinModel.y = vtx.y;
		}
		if (vtx.z < m_VtxMinModel.z)
		{
			m_VtxMinModel.z = vtx.z;
		}

		//サイズ文のポインタを進める
		pVtxBuff += sizeFVF;
	}

	//頂点バッファのアンロック
	m_pMeshModel->UnlockVertexBuffer();


	//使っているテクスチャ
	//D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pMeshTextures[0]);

	return S_OK;
}

//===============================================================================
// テクスチャの破棄
//===============================================================================
void CEnemy::UnLoad(void)
{
	// メッシュの開放
	if (m_pMeshModel != NULL)
	{
		m_pMeshModel->Release();
		m_pMeshModel = NULL;
	}
	// マテリアルの開放
	if (m_pBuffMatModel != NULL)
	{
		m_pBuffMatModel->Release();
		m_pBuffMatModel = NULL;
	}

	//テクスチャ
	if (m_pMeshTextures != NULL)
	{
		m_pMeshTextures->Release();
		m_pMeshTextures = NULL;
	}
}
//=============================================================================
// 餌との当たり判定
//=============================================================================
void CEnemy::CollisionFeed(void)
{
	if (m_nNumEgg >= MAX_EGG) { return; }

	CSound *pSound = CManager::GetSound();
	CScene *pScene;

	for (int nCntPriority = 2; nCntPriority <= FEED_PRIOTITY; nCntPriority++)
	{
		// プライオリティーチェック
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// プライオリティー内のリスト構造を最後まで見る
			CScene *pSceneNext = pScene->GetNext();		// 次のオブジェクトを保存

			if (pScene->GetObjType() == OBJTYPE_FEED)
			{// タイプが障害物だったら
				CFeed *pFeed = (CFeed*)pScene;	// オブジェクトクラスのポインタ変数にする

				if (pFeed->CollisionFeed(&m_pos, &m_posOld))
				{// 衝突した
					EggAppear(pFeed);	// 卵出現
					pFeed->Uninit();	// 餌削除
					m_nNumEgg++;
				}
			}

			// Nextに次のSceneを入れる
			pScene = pSceneNext;
		}
	}
}


//=============================================================================
// 卵の出現処理
//=============================================================================
void CEnemy::EggAppear(CFeed *pFeed)
{
	if (m_pEgg[m_nNumEgg] == NULL)
	{
		if (pFeed->GetFeedType() == CFeed::FEEDTYPE_ATTACK)
		{// 攻撃の卵生成
			m_pEgg[m_nNumEgg] = CEgg::Create(m_pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(EGG_SCALE, EGG_SCALE, EGG_SCALE),
				CEgg::EGGTYPE_ATTACK,
				CEgg::BULLETTYPE_ENEMY,
				0);
		}
		else if (pFeed->GetFeedType() == CFeed::FEEDTYPE_ANNOY)
		{// 妨害の卵生成
			m_pEgg[m_nNumEgg] = CEgg::Create(m_pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(EGG_SCALE, EGG_SCALE, EGG_SCALE),
				CEgg::EGGTYPE_ANNOY,
				CEgg::BULLETTYPE_ENEMY,
				0);
		}
		else if (pFeed->GetFeedType() == CFeed::FEEDTYPE_SPEED)
		{// 加速の卵生成
			m_pEgg[m_nNumEgg] = CEgg::Create(m_pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(EGG_SCALE, EGG_SCALE, EGG_SCALE),
				CEgg::EGGTYPE_SPEED,
				CEgg::BULLETTYPE_ENEMY,
				0);
		}
	}

	m_pEgg[m_nNumEgg]->SetState(CEgg::EGGSTATE_CHASE);
}

//=============================================================================
// 卵がついてくる処理
//=============================================================================
void CEnemy::ChaseEgg(void)
{
	//m_nCntFrame++;

	//if (m_nCntFrame >= MAX_FRAME)
	//{
	//	m_nCntFrame = 0;
	//}

	//// 前のプレイヤーの角度を保存
	//m_OldEggRot[m_nCntFrame] = m_rot;
	//m_abJump[m_nCntFrame] = m_bJumpSave;

	//if (m_nNumEgg <= 0) { return; }

	//if (m_nNumEgg >= 1)
	//{// 卵が一個の時
	//	if (m_pEgg[0]->GetState() == CEgg::EGGSTATE_CHASE)
	//	{
	//		// 前の向きを代入
	//		int nData = m_nCntFrame - EGG_POS;

	//		if (nData < 0)
	//		{
	//			nData += MAX_FRAME;
	//		}

	//		// 卵の位置設定
	//		m_pEgg[0]->SetPosition(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE) + m_pos.x,
	//			m_pEgg[0]->SetHeight(),
	//			(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE) + m_pos.z));

	//		// 卵の角度設定
	//		m_pEgg[0]->SetRot(m_OldEggRot[nData]);

	//		if (m_abJump[nData] == true)
	//		{
	//			m_pEgg[0]->Jump(EGGJUMP);
	//		}

	//		//CDebugProc::Print("%.1f : %.1f\n", m_OldEggRot[nData].x, m_OldEggRot[nData].z);
	//	}
	//}
	//if (m_nNumEgg >= 2)
	//{// 卵が二個の時
	// // 二個目
	//	int nData = m_nCntFrame - EGG_POS * 2;

	//	if (nData < 0)
	//	{
	//		nData += MAX_FRAME;
	//	}

	//	m_pEgg[1]->SetPosition(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 2) + m_pos.x,
	//		m_pEgg[1]->SetHeight(),
	//		(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 2) + m_pos.z));

	//	m_pEgg[1]->SetRot(m_OldEggRot[nData]);

	//	if (m_abJump[nData] == true)
	//	{
	//		m_pEgg[1]->Jump(EGGJUMP);
	//	}
	//}
	//if (m_nNumEgg >= 3)
	//{
	//	// 三個目
	//	int nData = m_nCntFrame - EGG_POS * 3;

	//	if (nData < 0)
	//	{
	//		nData += MAX_FRAME;
	//	}

	//	m_pEgg[2]->SetPosition(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 3) + m_pos.x,
	//		m_pEgg[2]->SetHeight(),
	//		(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 3) + m_pos.z));

	//	m_pEgg[2]->SetRot(m_OldEggRot[nData]);

	//	if (m_abJump[nData] == true)
	//	{
	//		m_pEgg[2]->Jump(EGGJUMP);
	//	}
	//}
}

//=============================================================================
// 卵発射処理
//=============================================================================
void CEnemy::BulletEgg(void)
{
	if (m_nNumEgg > 0)
	{// 卵を持っているとき
		if (m_pEgg[0] != NULL && m_pEgg[0]->GetState() == CEgg::EGGSTATE_CHASE)
		{// 一個目の卵に情報が入っていて、プレイヤーについてくる時

			m_pEgg[0]->SetState(CEgg::EGGSTATE_BULLET);	// 状態を弾にする

			m_nNumEgg--;	// 所持数を減らす

			if (m_pEgg[0]->GetType() == CEgg::EGGTYPE_SPEED)
			{
				m_State = ENEMYSTATE_SPEEDUP;
				m_fSpeed += SPEED * 2;

				m_pEgg[0]->Uninit();
				m_pEgg[0] = NULL;
			}

			// 情報入れ替え
			m_pEgg[0] = m_pEgg[1];
			m_pEgg[1] = m_pEgg[2];
			m_pEgg[2] = NULL;
		}
	}
}

//=============================================================================
// 卵との当たり判定
//=============================================================================
void CEnemy::CollisionEgg(void)
{
	CSound *pSound = CManager::GetSound();
	CScene *pScene;

	for (int nCntPriority = 2; nCntPriority <= EGG_PRIOTITY; nCntPriority++)
	{
		// プライオリティーチェック
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// プライオリティー内のリスト構造を最後まで見る
			CScene *pSceneNext = pScene->GetNext();		// 次のオブジェクトを保存

			if (pScene->GetObjType() == OBJTYPE_EGG)
			{// タイプが障害物だったら
				CEgg *pEgg = (CEgg*)pScene;	// オブジェクトクラスのポインタ変数にする

				if (pEgg->GetState() == CEgg::EGGSTATE_BULLET)
				{
					if (pEgg->CollisionEgg(&m_pos, &m_posOld) == true && pEgg->GetBulletType() != CEgg::BULLETTYPE_ENEMY)
					{// 衝突した
						switch (pEgg->GetType())
						{
							// 攻撃
						case CEgg::EGGTYPE_ATTACK:
							// ダメージ状態にする
							if (m_bDamage == false)
							{
								m_bDamage = true;
								m_State = ENEMYSTATE_DAMAGE;
							}
							pEgg->Uninit();	// 卵削除
							break;

							// 減速
						case CEgg::EGGTYPE_ANNOY:
							if (m_bDamage == false)
							{
								m_bDamage = true;
								m_State = ENEMYSTATE_SPEEDDOWN;
							}
							pEgg->Uninit();	// 卵削除
							break;
						}
					}
				}
			}

			// Nextに次のSceneを入れる
			pScene = pSceneNext;
		}
	}
}