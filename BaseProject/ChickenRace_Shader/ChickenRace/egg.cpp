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
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EGG_NAME_000	"data\\MODEL\\Weapon\\egg.x"			// 読み込むモデルファイル

#define TEXTURE_EGG_1	"data\\TEXTURE\\modeltex\\egg02.jpg"	//読み込むテクスチャファイル
#define TEXTURE_EGG_2	"data\\TEXTURE\\modeltex\\egg00.jpg"	//読み込むテクスチャファイル
#define TEXTURE_EGG_3	"data\\TEXTURE\\modeltex\\egg01.jpg"	//読み込むテクスチャファイル

#define MODEL_SPEED				(5.0f)
#define PLAYER_DEPTH			(50)		// プレイヤーの幅調整用
#define PLAYER_HEIGHT			(100.0f)	// プレイヤーの背の高さ
#define OBJCT_ANGLE_REVISION	(0.2f)		// 角度補正
#define EFFECT_HIGHT			(250.0f)	// エミッターの高さ
#define FOUNTAIN_UP				(20.0f)		// 噴水の上昇させる値

#define EGG_SPEED				(40.0f)		// 卵が飛んでくスピード

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
LPD3DXMESH CEgg::m_pMeshModel = NULL;						//メッシュ情報へのポインタ
LPD3DXBUFFER CEgg::m_pBuffMatModel = NULL;					//マテリアルの情報へのポインタ
DWORD CEgg::m_nNumMatModel = NULL;							//マテリアルの情報数
LPDIRECT3DTEXTURE9 CEgg::m_pMeshTextures[MAX_EGG_TEXTURE] = {};
D3DXVECTOR3 CEgg::m_VtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 CEgg::m_VtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CEgg::CEgg() : CModel3D(EGG_PRIOTITY, CScene::OBJTYPE_EGG)
{
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 大きさ
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDestAngle = 0.0f;
	m_fDiffAngle = 0.0f;
	m_fHeight = 0.0f;
	m_nRank = 0;
	m_nNumPlayer = 0;
	m_nHatchingTimer = 0;
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
			// 種類の設定
			pEgg->BindModel(m_pMeshModel, m_pBuffMatModel, m_nNumMatModel, m_pMeshTextures[eggType],
				m_VtxMaxModel, m_VtxMinModel);
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

	// 位置の初期化
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//位置の代入
	CModel3D::SetPosition(pos);

	// 各種情報の代入
	CModel3D::SetScale(m_scale);

	//変数の初期化
	m_pObjBill = NULL;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fHeight = 0.0f;
	m_fDestAngle = 0.0f;
	m_fDiffAngle = 0.0f;
	m_bJump = false;
	m_eggState = EGGSTATE_NORMAL;
	m_nRank = 0;
	m_nNumPlayer = 0;
	m_nHatchingTimer = 0;
	m_bThrow = false;
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
	D3DXVECTOR3 pos = CModel3D::GetPosition();

	m_nHatchingTimer++;

	pos = Item(pos);

	m_move.y -= cosf(0) * 0.4f;
	m_fHeight += m_move.y;

	SetHeight();

	pos.y = m_fHeight;
	pos.x += m_move.x;
	pos.z += m_move.z;

	CModel3D::SetMove(m_move);
	CModel3D::SetPosition(pos);

	CDebugProc::Print("%.1f : %.1f : %.1f\n", pos.x, pos.y, pos.z);
	CDebugProc::Print("%.1f\n", m_move.y);

	if (m_bJump == true)
	{
		CDebugProc::Print("ジャンプ : 〇\n");
	}
	else
	{
		CDebugProc::Print("ジャンプ : ×\n");
	}

	//距離の取得
	float fLength = CModel3D::GetLength();

	if (CModel3D::GetDelete() == true) { Uninit(); }
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

	// 色変更
	if (m_eggType == EGGTYPE_ATTACK)
	{
		CModel3D::Setcol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else if (m_eggType == EGGTYPE_ANNOY)
	{
		CModel3D::Setcol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	}
	else if (m_eggType == EGGTYPE_SPEED)
	{
		CModel3D::Setcol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}

	//描画処理
	CModel3D::Draw();

	//頂点法線の自動正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//===============================================================================
// Xファイルの読み込み
//===============================================================================
HRESULT CEgg::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;

	// Xファイルの読み込み
	D3DXLoadMeshFromX(EGG_NAME_000, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel, NULL, &m_nNumMatModel, &m_pMeshModel);

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
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EGG_1, &m_pMeshTextures[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EGG_2, &m_pMeshTextures[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EGG_3, &m_pMeshTextures[2]);

	return S_OK;
}

//===============================================================================
// テクスチャの破棄
//===============================================================================
void CEgg::UnLoad(void)
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
	for (int nCntTex = 0; nCntTex < MAX_EGG_TEXTURE; nCntTex++)
	{
		//テクスチャ
		if (m_pMeshTextures[nCntTex] != NULL)
		{
			m_pMeshTextures[nCntTex]->Release();
			m_pMeshTextures[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// 使ったときの処理
//=============================================================================
D3DXVECTOR3 CEgg::Item(D3DXVECTOR3 pos)
{
	if (m_eggState == EGGSTATE_BULLET)
	{
		float fHeight = 0.0f;

		switch (m_eggType)
		{
			// 攻撃
		case EGGTYPE_ATTACK:
			fHeight = 30.0f;
			break;

			// 加速
		case EGGTYPE_SPEED:
			CPlayer **pPlayer = NULL;
			pPlayer = CGame::GetPlayer();

			pos = D3DXVECTOR3(pPlayer[m_nNumPlayer]->GetPos().x, pPlayer[m_nNumPlayer]->GetPos().y, pPlayer[m_nNumPlayer]->GetPos().z);

			m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

			m_rot.z = D3DX_PI * 0.5f;

			m_rot.x += 0.5f;

			AdjustAngle(m_rot.x);

			CModel3D::SetRot(m_rot);
			CModel3D::ScaleVtxCornerPos(m_scale);

			if (pPlayer[m_nNumPlayer]->GetPlayerState() == CPlayer::PLAYERSTATE_NORMAL)
			{// 加速状態じゃなくなったら削除
				Uninit();
			}
			break;
		}

		Bullet();
	}

	return pos;
}

//===============================================================================
// 当たり判定
//===============================================================================
bool CEgg::CollisionEgg(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//あたっているかあたってないか
	bool bHit = false;

	// 各種情報の取得
	D3DXVECTOR3 ModelPos = CModel3D::GetPosition();		// 位置
	D3DXVECTOR3 VtxMax = CModel3D::GetVtxMax();			// モデルの最大値
	D3DXVECTOR3 VtxMin = CModel3D::GetVtxMin();			// モデルの最小値
	D3DXVECTOR3 rot = CModel3D::GetRot();

	D3DXVECTOR3 ModelMax = CModel3D::GetPosition() + CModel3D::GetVtxMax();	// 位置込みの最大値
	D3DXVECTOR3 ModelMin = CModel3D::GetPosition() + CModel3D::GetVtxMin();	// 位置込みの最小値

	float fDepth = PLAYER_DEPTH;

	if (m_eggType == EGGTYPE_ATTACK)
	{
		fDepth = PLAYER_DEPTH/* * 2*/;
	}

	if (pPos->x >= ModelMin.x - fDepth && pPos->x <= ModelMax.x + fDepth)
	{// Zの範囲内にいる
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

	return bHit;
}

//=============================================================================
// 高さ判定
//=============================================================================
float CEgg::SetHeight(void)
{
	float fHeight = 0.0f;

	CScene *pScene = CScene::GetTop(MESH_PRIOTITY);

	D3DXVECTOR3 pos = CModel3D::GetPosition();

	//NULLチェック
	while (pScene != NULL)
	{
		//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetObjType() == CScene::OBJTYPE_GROUND)
		{//タイプが地面だったら
			CMeshField *pField = (CMeshField*)pScene;

			if (pField->OnField(pos, 0.0f))
			{// 傾斜の計算
				fHeight = pField->GetHeightMesh(CModel3D::GetPosition());

				if (m_bJump == false || (m_bJump == true && m_fHeight < fHeight))
				{
					float fEggHeight = 15.0f;

					if (m_eggType == EGGTYPE_SPEED && m_eggState == EGGSTATE_BULLET)
					{
						fEggHeight = 30.0f;
					}
					m_fHeight = fHeight + fEggHeight;					//地面の高さを取得
					m_move.y = 0.0f;					//移動量を初期化する

														//ジャンプの状態設定
					m_bJump = false;
					CModel3D::SetMove(m_move);
					CModel3D::SetPosition(D3DXVECTOR3(pos.x, fHeight, pos.z));

					if (m_bThrow == true)
					{
						Uninit();
					}

					break;
				}
			}
		}
		//Nextに次のSceneを入れる
		pScene = pSceneNext;
	}

	return fHeight;
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

	CModel3D::SetMove(m_move);
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
			m_move.x = sinf(m_rot.y) * EGG_SPEED;
			m_move.z = cosf(m_rot.y) * EGG_SPEED;

			m_rot.x = D3DX_PI * 0.5f;

			CModel3D::SetRot(m_rot);
		}
	}
	else if (m_bulletType == BULLETTYPE_ENEMY)
	{
		CScene *pScene = CScene::GetTop(3);

		D3DXVECTOR3 pos = CModel3D::GetPosition();
		m_rot = CModel3D::GetRot();

		//NULLチェック
		while (pScene != NULL)
		{
			//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
			CScene *pSceneNext = pScene->GetNext();

			if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER && m_eggType == EGGTYPE_ATTACK)
			{//タイプが敵だったら
				CPlayer *pPlayer = (CPlayer*)pScene;

				// 目的の角度
				m_fDestAngle = atan2f(pPlayer->GetPos().x - pos.x, pPlayer->GetPos().z - pos.z);

				// 差分
				m_fDiffAngle = m_fDestAngle - m_rot.y;

				if (m_fDiffAngle > D3DX_PI)
				{
					m_fDiffAngle -= D3DX_PI * 2.0f;
				}
				if (m_fDiffAngle < -D3DX_PI)
				{
					m_fDiffAngle += D3DX_PI * 2.0f;
				}

				m_rot.y += m_fDiffAngle * 0.05f;

				if (m_rot.y > D3DX_PI)
				{
					m_rot.y -= D3DX_PI * 2.0f;
				}
				if (m_rot.y < -D3DX_PI)
				{
					m_rot.y += D3DX_PI * 2.0f;
				}

				//モデルの移動	モデルの移動する角度(カメラの向き + 角度) * 移動量
				m_move.x = sinf(m_rot.y) * EGG_SPEED;
				m_move.z = cosf(m_rot.y) * EGG_SPEED;

				m_rot.x = D3DX_PI * 0.5f;
			}
			//Nextに次のSceneを入れる
			pScene = pSceneNext;
		}
		CModel3D::SetRot(m_rot);
	}
}

//=============================================================================
// 角度の調節
//=============================================================================
void CEgg::AdjustAngle(float rot)
{
	if (rot > D3DX_PI)
	{
		rot -= D3DX_PI * 2.0f;
	}
	if (rot < -D3DX_PI)
	{
		rot += D3DX_PI * 2.0f;
	}
}