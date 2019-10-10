//=============================================================================
//
// タイヤの処理 [tire.cpp]
// Author : 佐藤安純
//
//=============================================================================
#include "tire.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "gamecamera.h"
#include "game.h"
#include "player.h"
#include "incline.h"
#include "scene.h"
#include "mesh.h"
#include "meshfield.h"
#include "tutorial.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FILE_NAME		("data/MODEL/タイヤ/tire.x")	//読み込むファイル名
#define FILE_TEXTURE	("data/TEXTURE/tire.jpg")		//テクスチャの読み込み
#define ADD_ROT			(0.2f)							//タイヤの回転量
#define OFFSET_WIDTH	(26.0f)							//横幅
#define GRASS_TEXNUM	(3)								//草むらのテクスチャ番号

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTire::m_pTexture = NULL;
float CTire::m_fHight = 0.0f;
bool CTire::m_bJump = false;
bool CTire::m_bPass = false;

//=============================================================================
// 生成処理
//=============================================================================
CTire * CTire::Create(const D3DXVECTOR3 pos)
{
	//インスタンスの生成
	CTire * pModel;
	pModel = new CTire;

	//初期化処理
	pModel->Init();

	//設定処理
	pModel->SetPos(pos);
	pModel->SetFirstPos(pos);

	return pModel;
}

//===============================================================================
//　テクスチャの読み込み
//===============================================================================
void CTire::LoadTexture(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice,
							  FILE_TEXTURE,
							  &m_pTexture);
}

//===============================================================================
//　テクスチャの破棄
//===============================================================================
void CTire::UnloadTexture(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//===============================================================================
//　コンストラクタ
//===============================================================================
CTire::CTire()
{
	m_pMesh = NULL;								//メッシュ情報へのポインタ
	m_pBuffMat = NULL;							//マテリアルの情報へのポインタ
	m_nNumMat = 0;								//マテリアルの情報数
	m_VtxMin, m_VtxMax;							//モデルの最小値・最大値
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_FirstPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//初期位置
	m_pTextures = NULL;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CTire::~CTire(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTire::Init(void)
{
	//変数の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fvtxMaxY = 0.0f;

	CModel::Init(FILE_NAME);
	CModel::BindTexture(m_pTexture);

	//モデルの頂点最大値を取得
	m_fvtxMaxY = GetVtxMax().y;

	m_pIncline = CIncline::Create();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTire::Uninit(void)
{
	if (m_pIncline != NULL)
	{// 傾斜処理の破棄
		m_pIncline->Uninit();
		m_pIncline = NULL;
	}

	m_fHight = 0.0f;	//高さの初期化
	m_bPass = false;	//処理通過フラグを初期化

	CModel::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTire::Update(int nCntTire, float fGravity)
{
	//変数宣言
	float fAngle = 0.0f;
	CManager::MODE mode = CManager::GetMode();
	CPlayer * pPlayer = NULL;

	if (mode == CManager::MODE_GAME)
	{//ゲーム
		pPlayer = CGame::GetPlayer();		//プレイヤーの取得
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//チュートリアル
		pPlayer = CTutorial::GetPlayer();	//プレイヤーの取得
	}

	if (mode == CManager::MODE_RESULT) { return; }

	CPlayer::STATE_SPEED PlayerStateSpeed = pPlayer->GetStateSpeed();
	CPlayer::STATE_HANDLE PlayerHandle = pPlayer->GetStateHandle();
	bool bGravity = false;
	CModel::Update();

	//傾斜処理
	if (nCntTire == 0) { SetIncline(pPlayer); }

	//重力の加算
	if ((PlayerStateSpeed != CPlayer::STATE_SPEED_BRAKS) && (pPlayer->GetSmoke() == false))
	{
		m_move.y -= cosf(0) * fGravity;
		bGravity = true;
	}

	if (bGravity == true)
	{//重力が有効だったら
		if ((PlayerHandle == CPlayer::HANDLE_LEFT) || (PlayerHandle == CPlayer::HANDLE_RIGHT))
		{
			m_move.y -= cosf(0) * fGravity;
		}
	}

	//ジャンプ状態なら重力を足す
	if((!bGravity) && (pPlayer->GetJump() == true))
	{
		m_move.y -= cosf(0) * fGravity; 
	}

	//高さを求める
	if (nCntTire == 0)
	{
		m_move.y = m_fHight;
	}
	else if ((nCntTire == 2))
	{
		m_move.y = m_fHight;
	}
	else if (m_bJump == true)
	{
		m_move.y = m_fHight * 0.25f;
	}

	//移動量の加算
	m_WorldPos.y += m_move.y;

	//高さの更新処理
	UpdateField(pPlayer);

	//タイヤの向きの処理
	if (pPlayer->GetStateHandle() == CPlayer::HANDLE_LEFT)
	{
		//角度を求める
		fAngle = D3DX_PI * 0.7f;
		RemakeAngle(&fAngle);

		//角度の設定
		m_Rot.y = fAngle;
	}
	else if (pPlayer->GetStateHandle() == CPlayer::HANDLE_RIGHT)
	{
		//角度を求める
		fAngle = D3DX_PI * -0.7f;
		RemakeAngle(&fAngle);

		//角度の設定
		m_Rot.y = fAngle;
	}
	else
	{
		fAngle = D3DX_PI;	//角度を求める
		m_Rot.y = fAngle;	//角度の設定
	}

	//タイヤの回転処理
	if (pPlayer->GetStateSpeed() != CPlayer::STATE_SPEED_STOP)
	{
		m_Rot.x -= ADD_ROT;
		RemakeAngle(&m_Rot.z);
	}

	//デバッグ表示
	//CDebugProc::Print("ワールド座標 : X %.2f, Y %.2f, Z %.2f\n", m_WorldPos.x, m_WorldPos.y, m_WorldPos.z);
	//CDebugProc::Print("ジャンプの高さ : %.2f\n", m_fHight);
}

//=============================================================================
// 描画処理
//=============================================================================
void CTire::Draw(float fAlpha)
{
	CModel::Draw(1.0f);

	//ワールド座標の取得
	m_WorldPos = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
}

//=============================================================================
// タイヤの高さ判定
//=============================================================================
void CTire::UpdateField(CPlayer * pPlayer)
{
	CScene *pScene = CScene::GetTop(MESH_PRIOTITY);
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

	//地面に乗っているかどうか
	m_bLand = false;

	//NULLチェック
	while (pScene != NULL)
	{
		//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetObjType() == CScene::OBJTYPE_GROUND)
		{//タイプが地面だったら
			CMeshField *pField = (CMeshField*)pScene;

			if (pField->OnField(m_WorldPos, m_fvtxMaxY))
			{// 傾斜の計算
				float fHight = pField->GetHeightMesh(m_WorldPos);
				
				if (m_WorldPos.y <= fHight + TIRE_HIGHT)
				{
					m_WorldPos.y = fHight + TIRE_HIGHT;	//地面の高さを取得
					m_move.y = 0.0f;					//移動量を初期化する
					m_bLand = true;

					pPlayer->SetGrassEffect(false);	//草のエフェクトを生成状態ではなくする

					//ジャンプ中かどうか
					bool bJumpFlag = pPlayer->GetJump();
					if(bJumpFlag)
					{
						pPlayer->GetPlayerInfoPoint()->nCountTime -= (int)std::round(pPlayer->GetPlayerInfoPoint()->nCountTime * 0.05f);
					}
					else
					{
						if (pField->GetMeshType() == GRASS_TEXNUM)
						{//草むらのテクスチャなら
							pPlayer->SetGrassEffect(true);	//草のエフェクト生成状態にする
						}
					}

					//ジャンプの状態設定
					pPlayer->SetJump(false);
					m_pIncline->SetJumpFlag(false);

					//プレイヤーのｙ軸の移動量を初期化
					D3DXVECTOR3 PlayerMove = pPlayer->GetMove();
					PlayerMove.y = 0.0f;
					pPlayer->SetMove(PlayerMove);
					break;
				}
			}
		}
		//Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

//=============================================================================
// 傾斜の処理
//=============================================================================
void CTire::SetIncline(CPlayer * pPlayer)
{
	//ジャンプフラグの取得
	bool bJump = pPlayer->GetJump();
	m_bJump = bJump;

	if (NULL != m_pIncline)
	{// 傾斜処理が使われているとき
	 //プライオリティーチェック
		CScene *pScene = CScene::GetTop(MESH_PRIOTITY);

		//NULLチェック
		while (pScene != NULL)
		{
			//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
			CScene *pSceneNext = pScene->GetNext();

			if (pScene->GetObjType() == CScene::OBJTYPE_MESH)
			{//タイプが地面だったら
				CMeshField *pField = (CMeshField*)pScene;
				if (pField->OnField(m_WorldPos, m_fvtxMaxY))
				{// 傾斜の計算
					m_pIncline->SetOnField(pField);
					if (!bJump)
					{// ジャンプ中でないとき
						m_pIncline->SetClimbSpeed(0.5f);				//上るスピード
						m_pIncline->Update(m_WorldPos, m_Rot, pPlayer->GetPlayerInfoPoint()->fAccel);	//傾斜のスピード計算

						//スピードの設定
						if (pPlayer->GetState() == CPlayer::STATE_DRIVE)
						{
							pPlayer->GetPlayerInfoPoint()->fAccel = m_pIncline->GetSpeed();
						}
						else
						{
							pPlayer->GetPlayerInfoPoint()->fAccel = m_pIncline->GetSpeed() * -1;
						}

						m_fHight = m_pIncline->GetJumpHeight();			//ジャンプの高さを取得
						bJump = m_pIncline->GetJumpFlag();				//ジャンプフラグの取得
						pPlayer->SetJump(bJump);						//プレイヤーにフラグを設定
						m_bJump = bJump;								//ジャンプ状態にする
					}
					break;
				}
			}
			//Nextに次のSceneを入れる
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// 角度修正
//=============================================================================
void CTire::RemakeAngle(float * pAngle)
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