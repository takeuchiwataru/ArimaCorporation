//=============================================================================
//
// プレイヤーの処理 [player.cpp]
// Author : 坂川 詠祐
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camerabace.h"
#include "scene.h"
#include "model.h"
#include "motion.h"
#include "loadText.h"
#include "wall.h"
#include "incline.h"
#include "sound.h"
#include "tutorial.h"
#include "gamecamera.h"
#include "object.h"
#include "feed.h"
#include "egg.h"
#include "billboord.h"
#include "chick.h"
#include "ColMesh.h"
#include "ModelEffect.h"
#include "DispEffect.h"
#include "particle.h"
#include "mesh.h"
#include "meshfield.h"
#include "Orbit.h"
#include "Character.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define VECTOR_ZERO			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))				//ベクトルの初期化
#define ROLLOVER_STOP		(0.6f)										//横転防止角度
#define DECELERATION		(0.5f)										//減速の割合
#define EGG_RANGE			(25.0f)										// 卵とプレイヤーの距離
#define EGG_POS				(7)											// 卵同士の間隔の広さ（増やすと広くなる）
#define CHICK_SCALE			(D3DXVECTOR3(1.0f, 1.0f, 1.0f))				//ひよこの大きさ
#define THROW				(11.0f)										// 卵を投げる力
#define EGG_RAND			(2.0f)										// 卵に乗るときのジャンプ力
#define EGG_HEIGHT			(40.0f)										// 卵に乗ったように見える高さ
#define SPEED_CHICK			(0.4f)										// 加速する量ひよこ
#define SPEED_EGG			(0.2f)										// 加速する量卵
#define SPEED_COUNT_ANNOY	(8.0f)										// 減速状態
#define SPEED_COUNT_DAMAGE	(0.0f)											// ダメージ状態
#define SPEED_COUNT_UP		(9.0f)										// 加速状態
#define EGGJUMP				(2.0f)										// 卵のジャンプ力
#define ANNOY_PARTICLE		(5)											// 減速エフェクトの出る間隔
#define COL_PARTICLE		(30)										// 卵やひよこが当たったときに出るエフェクトの量
#define COL_PARTICLE_S		(10)										// 隕石ひよこが当たったときに出るエフェクトの量
#define CHICK_BORDER		(6)											// 強いひよこが出る順位のライン
#define HATCH_TIME			(600.0f)									// 孵化するまでの時間
#define CHICK_SPEEDJUMP		(6.5f)										// ひよこに乗るときのジャンプ
#define ANNOY_TO_PLAYER		(40.0f)										// 減速ひよことプレイヤーの間隔
#define SCHICK_TIME			(5)										// 強いひよことひよこの出る時間の間隔
#define STRONG_TIME			(45)										// 強いひよこが出るようになる時間

// プレイヤー情報
#define PLAYER_ACCEL		(0.5f)										// 加速値（前進）
#define PLAYER_BRAKS		(0.75f)										// 加速値（後進）
#define PLAYER_DOWN			(0.08f)										// 減速度
#define PLAYER_ADDROT		(0.005f)									// 回転量
#define PLAYER_DOWNROT		(0.2f)										// 回転量
																		   
#define PLAYER_JUMP			(2.0f)										// 回転量
#define PLAYER_GRAVITY		(0.09f)										// 回転量

#define WIND_TIME			(15.0f)										//風の持続時間
#define WIND_POW			(2.0f)										//風の強さ
#define PLAYER_STRIKE		(0.4f)										//衝撃の強さ
#define PLAYER_STRPLUS		(0.4f)										//タックル加速度
#define PLAYER_STRDOWN		(50.0f)										//タックル最大減速値
#define PLAYER_POWDOWN		(0.0385f)									//タックル加速度の毎F減少値
#define PLAYER_POWMAX		(0.25f)										//タックル加速度の最大値
#define PLAYER_SPDUP		(1.0f)										//アクセル
#define PLAYER_SPDDOWN		(0.3f)										//アクセルの減速

#define PLAYER_ADDROT_INIT	(0.65f)										//回転初期
#define PLAYER_DRIFT_ACCEL	(0.32f)										//ドリフト、アクセル
#define PLAYER_DRIFT_DOWN	(0.25f)										//ドリフト、ダウン
#define PLAYER_MAX_ROAD		(65.0f)										//インアウトの最大値

#define PLAYER_Cap			(D3DXVECTOR3(0.0f, 8.2f, -2.0f))			//帽子の位置
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		CPlayer::m_pTexture[TEXTURE_MAX] = { NULL };
CChick					*CPlayer::m_pAnnoyChick[MAX_MEMBER] = {};

CPlayer::KEY_INFO		*CPlayer::m_pKeyInfo[MAX_MOTION] = {};		//キー情報へのポインタ
int						CPlayer::m_nNumParts = 0;					//パーツ数
int						CPlayer::m_aIndexParent[MAX_PARTS] = {};	//親のインデックス
CPlayer::KEY			CPlayer::m_aKayOffset[MAX_PARTS] = {};		//オフセット情報
CPlayer::MOTION_INFO	CPlayer::m_aMotionInfo[MAX_MOTION] = {};	//モーション情報
bool					CPlayer::m_bSChick = false;
int						CPlayer::m_nCntSTime = 0;

//--------------------------------------------
//グローバル変数
//--------------------------------------------
int g_nNumModel;
char g_aFileNameModel[MAX_PARTS_CAP][256];

//=============================================================================
// 生成処理
//=============================================================================
CPlayer * CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nPlayerNum, int nControllerNum, int nCharacterNum, PLAYERTYPE playerType)
{
	//インスタンスの生成
	CPlayer * pPlayer;
	pPlayer = new CPlayer;

	//初期化処理
	pPlayer->m_nCharacterNum = nCharacterNum;
	pPlayer->m_nPlayerNum = nPlayerNum;
	pPlayer->Init();


	if (playerType == PLAYERTYPE_PLAYER)
	{//画面エフェクトの生成
		if (CClient::GetpmyClient() != NULL)
		{//オンラインなら
			if (nPlayerNum == CClient::GetnID()) 
			{ pPlayer->m_pDispEffect = CDispEffect::Create(pPlayer); }
		}
		else { pPlayer->m_pDispEffect = CDispEffect::Create(pPlayer); }
	}
	pPlayer->m_fRoad = nPlayerNum * ((PLAYER_MAX_ROAD * 2.0f) / 8.0f) - PLAYER_MAX_ROAD;

	pPlayer->m_nControllerNum = nControllerNum;
	pPlayer->m_PlayerType = playerType;

	//設定処理
	pPlayer->Set(pos, VECTOR_ZERO);
	pPlayer->SetRot(rot);

	return pPlayer;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer() : CScene(0, OBJTYPE_PLAYER)
{
	//値の初期化
	m_apModel = NULL;

	m_nPlayerNum = 0;					// プレイヤー番号
	m_nControllerNum = 0;				// コントローラー番号

	m_pPlayerNum = NULL;
	m_pPlayerpos = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer() {}

//=============================================================================
// ファイルの読み込み
//=============================================================================
void CPlayer::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		char cName[256] = {};

		switch (nCntTex)
		{
		case TEXTURE_NUMBER:
			strcpy(cName, "data\\TEXTURE\\game\\charselect\\icon.png");
			break;
		case TEXTURE_ICON:
			strcpy(cName, "data\\TEXTURE\\game\\play\\icon.png");
			break;
		}

		D3DXCreateTextureFromFile(pDevice, cName, &m_pTexture[nCntTex]);
	}

	//モデルのオフセットと読み込み
	FileLoad();
	CModel::SetParts();	//ニワトリ親子の読み込み

	CCharcter::Load();
}

//=============================================================================
// ファイルの破棄
//=============================================================================
void CPlayer::Unload(void)
{
	CModel::PartsTypeUnLoad();

	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
	CCharcter::Unload();
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
	//変数の初期化
	m_OldPos = VECTOR_ZERO;					//前回の座標
	m_move = VECTOR_ZERO;					//移動
	m_pos = VECTOR_ZERO;					//位置
	m_rot = VECTOR_ZERO;					//向き
	m_nCntFrame = -1;						//向き
	m_pMotion = NULL;						//モーションポインタ
	m_StateSpeed = STATE_SPEED_STOP;		//スピードの状態設定
	m_StateHandle = HANDLE_MAX;				//ハンドルの状態
	m_PlayerInfo.fCountTime = 0;			//カウンター
	m_PlayerInfo.fAccel = PLAYER_ACCEL;		//加速値（前進）
	m_PlayerInfo.fBraks = PLAYER_BRAKS;		//加速値（後進）
	m_PlayerInfo.fDown = PLAYER_DOWN;		//減速値
	m_PlayerInfo.fAddRot = PLAYER_ADDROT;	//加える回転値
	m_PlayerInfo.fDistance = 0.0f;			//距離
	m_PlayerInfo.FirstPos = VECTOR_ZERO;	//初期位置
	m_bJump = true;							//ジャンプ状態
	m_bControl = false;						//コントローラーフラグ
	m_fvtxMaxY = 0.0f;						//モデル頂点の最大値（Y）
	m_fSpeed = 0.0f;						// 速さ
	m_nCountSpeed = 0;
	m_nCountTime = 0;						// 時間カウンター
	m_bCrcleCarIn = false;
	m_bShake = true;
	m_nNumEgg = 0;
	m_nNumChick = 0;
	m_nNumItem = 0;
	m_bJumpSave = false;
	m_bJumpFlag = false;
	m_nCntDamage = 0;
	m_bDamage = false;
	m_State = PLAYERSTATE_NORMAL;
	m_nPlayerRank = 0;
	m_nCntChick = 0;
	m_nDestRank = 0;
	m_nAnnoySTimer = 0;
	m_nCntParticle = 0;
	m_nCntSTime = 0;
	m_bAnnoyS = false;
	m_bSChick = false;
	m_bSpeedEgg = false;

	m_nDriftCounter = 0;		// ドリフトカウント

	for (int nCount = 0; nCount < MAX_PLAYCOL; nCount++) { m_fCola[nCount] = 1.0f; }
	if (CManager::GetMode() == CManager::MODE_TITLE || CManager::GetMode() == CManager::MODE_GAME)
	{
		m_pPoint = CRoad_Manager::GetManager()->GetTop(0);
		m_pEnmPoint = CRoad_Manager::GetManager()->GetTop(1);
	}

	m_nStartFrame = (CServer::Rand() % 20);
	m_nStartCounter = 0;

	m_pDispEffect = NULL;
	m_FEffect = CCOL_MESH::EFFECT_NORMAL;
	m_bDrop = false;				// おちた
	m_nDropCounter = 0;					// おちたカウント
	m_fLength = 20.0f;
	m_bDivided = false;
	m_nMap = 0;
	m_nNumRoad = 0;

	m_WindMove = INIT_VECTOR;
	m_fCntWind = 0.0f;;
	m_FNor = INIT_VECTOR;
	m_fTilt = 0.0f;
	m_fCTiltV = 0.0f;
	m_fCTiltW = 0.0f;
	m_fPosY = 0.0f;
	m_fRotOld = 0.0f;
	m_fPower = 0.0f;
	m_fTackle = 0.0f;
	m_fVecUZ = 0.0f;
	m_fStick = 1.0f;
	m_bJumpOld = m_bJump;
	m_bSJump = false;
	m_pFMesh = NULL;

	m_fCountFlame = 0;
	m_nKey = 0;

	m_bGoal = false;					// ゴール
	m_fAddRot = 0.0f;					// 加算角度
	m_fCntAho = 0.0f;
	m_Induction = INDUCTION_MAX;
	m_fFeedRot = 0.0f;
	m_FeedType = (CFeed::FEEDTYPE)(CServer::Rand() % CFeed::FEEDTYPE_MAX);

	m_nSelectNum = 0;					// 選択
	m_nSelectCounter = 0;				// 選択カウント
	m_nCntSky = 0;
	m_fCntTackle = 0.0f;
	m_fUseTime = 0.0f;

	// プレイヤー番号（追従）
	if (CManager::GetMode() == CManager::MODE_GAME && CGame::GetGameMode() == CGame::GAMEMODE_PLAY ||
		CManager::GetMode() == CManager::MODE_RESULT)
	{
		if (m_pPlayerNum == NULL)
			m_pPlayerNum = CBillBoord::Create(m_pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR2(10.0f, 10.0f));
		if (m_pPlayerNum != NULL)
			m_pPlayerNum->BindTexture(m_pTexture[TEXTURE_NUMBER]);
	}

	if (CManager::GetMode() == CManager::MODE_GAME && CGame::GetGameMode() == CGame::GAMEMODE_PLAY)
	{
		if (m_pPlayerpos == NULL)
			m_pPlayerpos = CBillBoord::Create(m_pos + D3DXVECTOR3(0.0f, 500.0f, 0.0f), D3DXVECTOR2(800.0f, 800.0f), true);
		if (m_pPlayerpos != NULL)
			m_pPlayerpos->BindTexture(m_pTexture[TEXTURE_ICON]);
	}

	for (int nCntEggPos = 0; nCntEggPos < MAX_FRAME; nCntEggPos++)
	{
		m_OldEggRot[nCntEggPos] = VECTOR_ZERO;
		m_abJump[nCntEggPos] = false;
	}

	//卵のポインタを初期化
	for (int nCntEgg = 0; nCntEgg < MAX_EGG; nCntEgg++)
	{
		m_pEgg[nCntEgg] = NULL;
		m_pChick[nCntEgg] = NULL;

		m_bulletType[nCntEgg] = BULLET_EGG_ATTACK;
	}

	for (int nCntChick = 0; nCntChick < MAX_MEMBER; nCntChick++)
	{// 減速鶏のポインタを初期化
		m_pAnnoyChick[nCntChick] = NULL;
	}

	CModel::ParentModel(m_apModel, CModel::TYPE_CHICKEN, m_nCharacterNum);
	int &nMaxModel = CModel::GetnModelMax(CModel::TYPE_CHICKEN);
	for (int nCountIndex = 0; nCountIndex < nMaxModel - 1; nCountIndex++)
	{
		m_apModel[nCountIndex]->BindTexture(CDispEffect::GetpTexAll(CDispEffect::TEX_White));

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

		m_apModel[nCountIndex]->BindTexture(CModel::GetTexAll(CModel::TEX_CHICKEN));
	}
	ResetMotion();

	m_apModel[MAX_PARTS]->BindTexture(CModel::GetTexAll(m_nCharacterNum));
	m_apModel[MAX_PARTS]->SetParent(m_apModel[CModel::PARTS_CHICKEN_HEAD - CModel::PARTS_CHICKEN_BODY]);
	m_apModel[MAX_PARTS]->AddPos(PLAYER_Cap);


	m_pShadow = NULL;
	m_pShadow = C3DPolygon::Create(C3DPolygon::TYPE_Shadow, m_pos, D3DXVECTOR3(0.0f, m_rot.y, 0.0f));
	m_pShadow->SetTexture(1, 1, 1, 1);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	CEfcOrbit::ReSetAll();

	if (m_pPlayerNum != NULL)
	{
		m_pPlayerNum->Uninit();
		m_pPlayerNum = NULL;
	}

	if (m_pPlayerpos != NULL)
	{
		m_pPlayerpos->Uninit();
		m_pPlayerpos = NULL;
	}

	//タイヤモデルの破棄
	for (int nCntEgg = 0; nCntEgg < MAX_EGG; nCntEgg++)
	{
		if (m_pEgg[nCntEgg] != NULL)
		{
			m_pEgg[nCntEgg]->Uninit();
			m_pEgg[nCntEgg] = NULL;
		}

		if (m_pChick[nCntEgg] != NULL)
		{
			m_pChick[nCntEgg]->Uninit();
			m_pChick[nCntEgg] = NULL;
		}
	}

	for (int nCntChick = 0; nCntChick < MAX_MEMBER; nCntChick++)
	{
		if (m_pAnnoyChick[nCntChick] != NULL)
		{
			m_pChick[nCntChick] = NULL;
		}
	}

	if (m_apModel != NULL)
	{
		int &nMaxModel = CModel::GetnModelMax(CModel::TYPE_CHICKEN);
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

	if (m_pDispEffect != NULL)
	{
		m_pDispEffect->Uninit();
		m_pDispEffect = NULL;
	}

	//死亡フラグを立てる
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	switch (m_PlayerType)
	{
	case PLAYERTYPE_PLAYER:
	case PLAYERTYPE_ENEMY:
		UpdateRace();
		break;
	case PLAYERTYPE_SELECT:
		UpdateSelect();
		break;
	case PLAYERTYPE_RESULT:
		UpdateResult();
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	float fCola;
	if (!CCharcter::DrawCheck(m_pos, &m_fCola[0], fCola)) { return; }

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
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y + m_fPosY, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_apModel == NULL) { return; }
	int &nMaxModel = CModel::GetnModelMax(CModel::TYPE_CHICKEN);
	for (int nCount = 0; nCount < nMaxModel; nCount++)
	{//モデルの描画
		if (m_apModel[nCount] != NULL)
		{
			//描画する
			m_apModel[nCount]->Draw(fCola);
		}
	}
}


//=============================================================================
// レースの更新処理
//=============================================================================
void CPlayer::UpdateRace(void)
{
	//変数宣言
	D3DXVECTOR3 TirePos[MAX_TIRE];
	CManager::MODE mode = CManager::GetMode();
	CGameCamera * pGameCamera = NULL;
	m_bShake = true;
	m_fRotOld = m_rot.y;
	m_bJumpOld = m_bJump;

	if (m_bControl)
	{//コントロールキー
		if (m_bGoal == false)
		{
			m_nCntSky++;
			if (m_PlayerType == PLAYERTYPE_PLAYER && CManager::GetAging() == false)
			{
				if (m_State != PLAYERSTATE_SPEEDUP_S) { ControlKey(); }
				else { UpdateKiller(); }
			}
			else
			{
				if (m_State != PLAYERSTATE_SPEEDUP_S) { UpdateAI(); }
				else { UpdateKiller(); }
			}
		}
		else
		{
			SetStateHandle(HANDLE_MAX);
		}
	}
	else
	{//ハンドルを握っていない状態にする
		SetStateHandle(HANDLE_MAX);
		SetStateSpeed(STATE_SPEED_DOWN);
	}
	m_fInduction = 9999.9f;
	m_Induction = INDUCTION_MAX;

	//タイムアップ状態なら以降は更新しない
	//if (CTime::GetTime() == 0 && CManager::MODE_GAME == CManager::GetMode()) { return; }

	UpdateMove();			// 移動処理
	UpVecUZ();				// 角度Z方向の計算

	UpdateField();

	CollisionFeed();		// 餌の当たり判定

	CollisionEgg();			// 卵との当たり判定

	CollisionChick();		// ひよことの当たり判定

	ChaseEgg();				// 卵がついてくる処理

	CollisionCharacter();	// キャラクター同士の当たり判定
	CCharcter::CollisionAll(this);

	ChickAppear();			// 

	ChaseAnnoyS();			// 強い減速ひよこがくっつく

	if (m_bSChick == true)
	{
		m_nCntSTime++;

		if (m_nCntSTime > (60 * SCHICK_TIME) * MAX_MEMBER)
		{
			m_bSChick = false;
			m_nCntSTime = 0;
		}
	}

							//マップとの当たり判定
	bool bGoal = false;

	CRoad_Pointer::RankPoint(this, bGoal);

	if (bGoal == true && m_bGoal == false)
	{
		m_bGoal = true;
		SetStateSpeed(STATE_SPEED_DOWN);
	}

	bool bLand = CCOL_MESH_MANAGER::Collision(this);
	if (!m_bJump) { m_nCntSky = 0; }
	else if (m_nCntSky > 300) { m_bDrop = true; }

	UpdateFEffect();

	if (!bLand && m_bJump)
	{//着地したなら
	 //着地モーション
		//CancelMotion(PLAYERANIM_LAND, false);

	}
	m_bJump = bLand;

	// プレイヤー番号
	if (m_pPlayerNum != NULL)
	{
		m_pPlayerNum->SetPosSize(m_pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR2(10.0f, 10.0f));

		if (m_PlayerType == CPlayer::PLAYERTYPE_PLAYER)
			m_pPlayerNum->SetTexture(m_nPlayerNum, 5, 1, 1);
		else
			m_pPlayerNum->SetTexture(4, 5, 1, 1);
	}

	// プレイヤー位置
	if (m_pPlayerpos != NULL)
	{
		m_pPlayerpos->SetPosSize(m_pos + D3DXVECTOR3(0.0f, 500.0f, 0.0f), D3DXVECTOR2(800.0f, 800.0f));
		m_pPlayerpos->SetTexture(m_nPlayerNum, 8, 1, 1);
	}

	//モーション更新
	UpdateMotion();
}


//=============================================================================
// 選択の更新処理
//=============================================================================
void CPlayer::UpdateSelect(void)
{
	if (m_nSelectNum == 0)
	{
		//待機モーション
		CancelMotion(PLAYERANIM_NEUTRAL, false);

		m_nSelectCounter = 0;
	}
	else if (m_nSelectNum == 1)
	{
		//ジャンプモーション
		CancelMotion(PLAYERANIM_JUMP, false);

		/*if (m_nSelectCounter < 20)
			m_nSelectCounter++;
		else
			m_nSelectNum = 0;*/
	}

	//モーション更新
	UpdateMotion();
}

//=============================================================================
// リザルトの更新処理
//=============================================================================
void CPlayer::UpdateResult(void)
{
	int nCounter = CResult::GetCounter();

	if (nCounter < RESULT_RANK_APP)
	{
		if (RESULT_CAMERA_ROLL <= nCounter)
		{// 指定カウントまで
			//走るモーション
			CancelMotion(PLAYERANIM_RUN, false);
			m_PlayerInfo.fCountTime = 7.0f;

			// 横移動
			m_pos = m_pos + D3DXVECTOR3(sinf(m_rot.y) * 2.5f, 0.0f, cosf(m_rot.y) * 2.5f);
			nCounter++;
		}
	}
	else
	{
		if (nCounter == RESULT_RANK_APP)
		{// 表彰
			m_pos.y -= 100.0f;

			if ((MAX_MEMBER - 1) - m_nPlayerNum == 0)
				m_pos = D3DXVECTOR3(-24585.0, -3231.0f, 1208.0f);
			else if ((MAX_MEMBER - 1) - m_nPlayerNum == 1)
				m_pos = D3DXVECTOR3(-24607.0, -3246.0f, 1238.0f);
			else if ((MAX_MEMBER - 1) - m_nPlayerNum == 2)
				m_pos = D3DXVECTOR3(-24562.0, -3257.0f, 1178.0f);

			m_rot.y = 0.95f;

			//待機モーション
			if ((MAX_MEMBER - 1) - m_nPlayerNum < 3)
			{
				if ((MAX_MEMBER - 1) - m_nPlayerNum == 0)
					CancelMotion(PLAYERANIM_WINNING, false);
				else
					CancelMotion(PLAYERANIM_APPLAUSE, false);
			}
		}
	}

	//モーション更新
	UpdateMotion();
}

//=============================================================================
// AIの更新処理
//=============================================================================
void CPlayer::UpdateAI(void)
{
	float fRot = AIInduction();
	float fRotY, fDifference;
	AIMovement(fRot);

	fDifference = fRot;
	if (fDifference < 0.0f) { fDifference *= -1.0f; }

	AICurve(fRot, fDifference);
	fRotY = fRot + m_rot.y;
	RemakeAngle(&fRotY);

	if (m_nStartCounter == m_nStartFrame)
	{
		if (m_fCntAho == 0.0f)
		{//アホになれる
			if (fDifference >= PLAYER_ADDROT * 0.65f)
			{//条件でアホになる
				if (CServer::Rand() % (m_nCharacterNum * 3 + 3) == 0)
				{
					m_fCntAho = 60.0f;
				}
				else { m_fCntAho = -120.0f * (1.0f + m_nCharacterNum * 0.25f); }
			}
		}

		if (m_fCntAho > 0.0f)
		{
			m_fCntAho--;
			if (m_fCntAho <= 0.0f) { m_fCntAho = -300.0f * (1.0f + m_nCharacterNum * 0.25f); }
		}
		else
		{
			m_fAddRot += fRot;
			SetStateSpeed(STATE_SPEED_ACCEL);
			if (fRot > PLAYER_ADDROT * (0.95f + (m_nCharacterNum * 0.05f))) 
			{ SetStateSpeed(STATE_SPEED_DRIFT); }
		}
	}
	else
		m_nStartCounter++;

	m_fAddRotOld = fDifference;
	if (m_fCntAho < 0.0f) { m_fCntAho++; }
	if (!m_bJump && m_bSJump)
	{//ジャンプ
		SetJump();
	}

	UseItem();
}
//=============================================================================
// AIの移動方向設定処理
//=============================================================================
float CPlayer::AIInduction(void)
{
	float fRot = CRoad_Pointer::NextRot(m_pos, m_pEnmPoint, m_fRoad, m_nMap, m_nNumRoad, m_FEffect);

	if (m_Induction != INDUCTION_MAX)
	{//誘導
	 //誘導のキャンセル
		float fWK = m_fFeedRot - fRot;
		if (fWK < 0.0f) { fWK *= -1.0f; }

		if (fWK > D3DX_PI * 0.5f)
		{//90度以上違うなら
			fRot += (m_fFeedRot - fRot) * 0.5f;
		}
		else { fRot = m_fFeedRot; }
	}

	fRot = fRot - (m_rot.y + m_fAddRot);
	RemakeAngle(&fRot);
	fRot *= 0.05f + (m_nCharacterNum * 0.0075f);
	RemakeAngle(&fRot);
	return fRot;
}
//=============================================================================
// AIのカーブ処理
//=============================================================================
void CPlayer::AICurve(float &fRot, float &fDifference)
{
	float fCurve = fRot;
	if (fDifference <= PLAYER_ADDROT * 0.65f)
	{//アクセル範囲内なら
		return;
	}
	//ドリフトに修正
	if (fRot < 0.0f) { fRot = -PLAYER_ADDROT; }
	else { fRot = PLAYER_ADDROT; }
	fRot += (fCurve - fRot) * (m_nCharacterNum * 0.1f);
	RemakeAngle(&fRot);
}
//=============================================================================
// AIの動き更新処理
//=============================================================================
void CPlayer::AIMovement(float &fRotY)
{
	if (m_nCharacterNum >= 4)
	{//ジャンプ
		if (m_State == PLAYERSTATE_SPEEDUP || m_State == PLAYERSTATE_SPEEDUP_S)
		{//スピードアップ中なら 終わりぎわジャンプ
			float fTime = (m_State == PLAYERSTATE_SPEEDUP ? SPEEDUP_TIME : 60.0f * KILLER_TIME);
			if (m_nCountSpeed > fTime - 1) { m_bSJump = true; }
		}
	}
	if (m_pNear != NULL &&
		m_PlayerInfo.fCountTime > PLAYER_COUNT * 0.7f &&
		m_pNear->m_PlayerInfo.fCountTime > PLAYER_COUNT * 0.7f)
	{//一定以上速いなら
		D3DXVECTOR3 &pos = m_pNear->Getpos();
		float fRot = atan2f(pos.x - m_pos.x, pos.z - m_pos.z);
		float fDis = sqrtf(powf(pos.x - m_pos.x, 2) + powf(pos.x - m_pos.x, 2));
		float fDifference = fRot - (m_rot.y + m_fAddRot);	RemakeAngle(&fRot);
		float fPlayRot = fDifference;

		if (fDifference < 0.0f) { fDifference *= -1.0f; }

		if (fDis < 60.0f && fDifference < D3DX_PI * 0.2f)
		{//真ん前なら 躱す
			if (fRotY >= 0.0f)
			{
				if (fRotY < PLAYER_ADDROT) { fRotY = PLAYER_ADDROT; }
			}
			else
			{
				if (fRotY > -PLAYER_ADDROT) { fRotY = -PLAYER_ADDROT; }
			}
		}
		if (m_fCntTackle <= 0.0f)
		{
			if (m_nCharacterNum % 3 == 1)
			{
				if (fDis < 40.0f && fDifference > D3DX_PI * 0.95f)
				{//真後ろなら ブレーキ
					m_move.x *= 0.85f;
					m_move.z *= 0.85f;
				}
			}
			if (m_nCharacterNum % 2 == 1)
			{
				if (fDis < 45.0f && fDifference > D3DX_PI * 0.6f && fDifference < D3DX_PI * 0.75f)
				{//横なら タックル
					fRotY += (fPlayRot - fRotY) * 0.3f;
					if (fRotY > PLAYER_ADDROT * 0.65f) { fRotY = PLAYER_ADDROT * 0.65f; }
					if (fRotY < -PLAYER_ADDROT * 0.65f) { fRotY = -PLAYER_ADDROT * 0.65f; }
				}
			}
		}
		else { m_fCntTackle--; }
	}
}
//=============================================================================
// アイテムの使用の更新処理
//=============================================================================
void CPlayer::UseItem(void)
{
	bool bUse = false;
	float fTime = 0.0f;

	//アイテムがないなら
	if (m_nNumItem <= 0) { return; }

	int nRank = CRoad_Manager::GetManager()->GetRank(m_nPlayerNum);
	BULLET Type = (BULLET)(m_bulletType[0] % BULLET_CHICK_ATTACK);
	switch (Type)
	{//アイテムごとに判断
	case BULLET_EGG_ATTACK:	if (UseATK(nRank)) { fTime = 90.0f;	bUse = true; }	break;
	case BULLET_EGG_ANNOY:	if (UseDEF(nRank)) { fTime = 120.0f;	bUse = true; }	break;
	case BULLET_EGG_SPEED:	if (UseSPD(nRank)) { fTime = 90.0f;	bUse = true; }	break;
	}

	if (m_fUseTime <= 0.0f)
	{//アイテムの使用
		if (bUse) { m_fUseTime = fTime; BulletEgg(); }
	}
	else { m_fUseTime--; }
}
//=============================================================================
// 妨害アイテムの攻撃処理
//=============================================================================
bool CPlayer::UseATK(int &nRank)
{
	if ((m_Induction == INDUCTION_ITEM || nRank > 6) && m_nNumItem > 2) { return true; }
	float fWk;

	switch (m_bulletType[0])
	{
	case BULLET_CHICK_ATTACK:	//近かったら
		if (nRank - 1 > 0)
		{//次の順位のやつ
			fWk = GetDistance(nRank - 1);
			if (fWk < 500.0f && fWk > 100.0f) { return true; }
		}
		break;
	case BULLET_EGG_ATTACK:		//使わない
		break;
	default:
		return true;
		break;
	}
	if (m_nNumItem > 2 && m_nMap == CCOL_MESH_MANAGER::TYPE_MAX - 1) 
	{
		if (CServer::Rand() % 240 == 0) { return true; }
	}
	return false;
}
//=============================================================================
// 妨害アイテムの使用処理
//=============================================================================
bool CPlayer::UseDEF(int &nRank)
{
	if ((m_nNumItem > 2 || nRank <= 0) && m_Induction == INDUCTION_ITEM) { return true; }
	float fWk = m_rot.y - m_fRotOld;

	switch (m_bulletType[0])
	{
	case BULLET_CHICK_ANNOY:	//近かったら
		if (nRank - 1 > 0)
		{//次の順位のやつ
			if (GetDistance(nRank - 1) < 80.0f) { return true; }
		}
		if (nRank + 1 < MAX_RACER - 1)
		{//前の順位のやつ
			if (GetDistance(nRank + 1) < 80.0f) { return true; }
		}
		break;
	case BULLET_EGG_ANNOY:		//曲がっていたら
		if (fWk != 0.0f && m_nNumItem > 1) { return true; }
		break;
	default:
		return true;
		break;
	}
	return false;
}
//=============================================================================
// 加速アイテムの使用処理
//=============================================================================
bool CPlayer::UseSPD(int &nRank)
{
	if (m_nNumItem > 2 && m_Induction == INDUCTION_ITEM) { return true; }
	if (m_PlayerInfo.fCountTime < 7.0f) { return true; }
	if (m_pEnmPoint->GetpNextPointer(0) == NULL && m_nMap == CCOL_MESH_MANAGER::TYPE_MAX - 1) { return true; }
	switch (m_bulletType[0])
	{
	case BULLET_CHICK_SPEED:
		break;
	case BULLET_EGG_SPEED:
		break;
	default:
		return true;
		break;
	}
	return false;
}
//=============================================================================
// 指定順位との距離の取得処理
//=============================================================================
float CPlayer::GetDistance(int nRank)
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
	D3DXVECTOR3 pos = pPlayer[nRank]->Getpos();

	return sqrtf(powf(pos.x - m_pos.x, 2) + powf(pos.z - m_pos.z, 2));
}
//=============================================================================
// キラーの設定処理
//=============================================================================
void CPlayer::SetKiller(void)
{
	//できるだけ近くのポイントを取得
	m_pEnmPoint = m_pEnmPoint->SetKiller(m_pos, m_nMap);
}
//=============================================================================
// キラーの更新処理
//=============================================================================
void CPlayer::UpdateKiller(void)
{
	m_fRoad = 0.0f;
	float fRot = CRoad_Pointer::NextRot(m_pos, m_pEnmPoint, m_fRoad, m_nMap, 1, m_FEffect);

	fRot = fRot - m_rot.y;
	RemakeAngle(&fRot);

	m_rot.y += fRot * 0.2f;
	RemakeAngle(&m_rot.y);
	SetStateSpeed(STATE_SPEED_ACCEL);
}
//=============================================================================
// 地面効果の更新処理
//=============================================================================
void CPlayer::UpdateFEffect(void)
{
	CDispEffect::EFFECT Effect = CDispEffect::EFFECT_MAX;

	if (m_FEffect == CCOL_MESH::EFFECT_SWAMP && !m_bJump)
	{//水溜まり
		if (m_State != PLAYERSTATE_SPEEDUP && m_State != PLAYERSTATE_SPEEDUP_S)
		{
			m_move *= 0.93f;
			if (m_PlayerInfo.fCountTime > PLAYER_COUNT * 0.6f)
			{
				m_PlayerInfo.fCountTime += (PLAYER_COUNT * 0.6f - m_PlayerInfo.fCountTime) * 0.05f;
			}
			Effect = CDispEffect::EFFECT_SWAMP;
			m_fPosY += (-10.0f - m_fPosY) * 0.1f;
		}
	}
	else { m_fPosY *= 0.9f; }

	if (m_FEffect == CCOL_MESH::EFFECT_GRASS && !m_bJump)
	{//ダート
		if (m_State != PLAYERSTATE_SPEEDUP && m_State != PLAYERSTATE_SPEEDUP_S)
		{
			m_move *= 0.93f;
			if (m_PlayerInfo.fCountTime > PLAYER_COUNT * 0.6f)
			{
				m_PlayerInfo.fCountTime += (PLAYER_COUNT * 0.6f - m_PlayerInfo.fCountTime) * 0.05f;
			}
		}
	}
	
	if (m_FEffect == CCOL_MESH::EFFECT_DROP)
	{//落下
		if (m_bDrop == false)
			m_bDrop = true;					// おちた
	}

	if (m_bDrop == true)
	{// おちた
		m_bControl = false;					// コントロールしない
		SetStateHandle(HANDLE_MAX);
		SetStateSpeed(STATE_SPEED_DOWN);

		if (m_nDropCounter == MAX_FALL_FADE + MAX_FALL_WAIT)
		{// もどる
			WarpNext();
		}
		else if (m_nDropCounter == ((MAX_FALL_FADE * 2.0f) + MAX_FALL_WAIT) - 1)
		{// フェード終了
			m_bControl = true;				// コントロールする
			m_nDropCounter = 0;				// 初期化
			m_nCntSky = 0;
			m_bDrop = false;				// おちていない

			return;
		}

		m_nDropCounter++;
	}
	
	if (m_bJump) { Effect = CDispEffect::EFFECT_MAX; }
	if (m_pDispEffect != NULL) { m_pDispEffect->SetEffect(Effect); }
}
//=============================================================================
// 落ちた時のワープ処理
//=============================================================================
void CPlayer::WarpNext(void)
{
	CRoad_Pointer *pNext = m_pPoint;
	if (pNext == NULL) { return; }

	SetState(PLAYERSTATE_NORMAL);
	m_rot.y = pNext->GetfRotY();
	m_pos = pNext->Getpos() + D3DXVECTOR3(sinf(m_rot.y), 0.0f, cosf(m_rot.y)) * 5.0f;
	m_OldPos = m_pos;
	m_move *= 0.0f;
	m_PlayerInfo.fCountTime = 0;
	m_FEffect = CCOL_MESH::EFFECT_NORMAL;
	SetKiller();
}
//=============================================================================
// 加速使用処理
//=============================================================================
void CPlayer::UseBoost(void)
{
	CGameCamera *pCamera = CGame::GetGameCamera(m_nPlayerNum);
	if (pCamera != NULL) { pCamera->UseBoost(); }
	if (m_pDispEffect != NULL) { m_pDispEffect->SetEffect(CDispEffect::EFFECT_BOOST); }

	m_bOrbit = false;
	CEfcOrbit *pOrbit;

	pOrbit = CEfcOrbit::Create()->Set(m_mtxWorld, D3DXVECTOR3(10.0f, 10.0f, 10.0f), D3DXVECTOR3(9.0f, 8.0f, 10.0f), m_bOrbit, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), 0, 0, 0);
	pOrbit->GetDrawType() = C2D::DRAW_TYPE_SUBTRACT;
	pOrbit = CEfcOrbit::Create()->Set(m_mtxWorld, D3DXVECTOR3(-10.0f, 10.0f, 10.0f), D3DXVECTOR3(-9.0f, 8.0f, 10.0f), m_bOrbit, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), 0, 0, 0);
	pOrbit->GetDrawType() = C2D::DRAW_TYPE_SUBTRACT;

	//羽から
	pOrbit = CEfcOrbit::Create()->Set(m_mtxWorld, D3DXVECTOR3(8.0f, 15.0f, 0.0f), D3DXVECTOR3(13.0f, 25.0f, 0.0f), m_bOrbit, D3DXCOLOR(1.0f, 0.98f, 0.02f, 0.0f), 0, 0, 0);
	pOrbit->GetType() = CEfcOrbit::TYPE_FADE;
	pOrbit = CEfcOrbit::Create()->Set(m_mtxWorld, D3DXVECTOR3(-8.0f, 15.0f, 0.0f), D3DXVECTOR3(-13.0f, 25.0f, 0.0f), m_bOrbit, D3DXCOLOR(1.0f, 0.98f, 0.02f, 0.0f), 0, 0, 0);
	pOrbit->GetType() = CEfcOrbit::TYPE_FADE;

	//わきから
	CEfcOrbit::Create()->Set(m_apModel[3]->GetMtxWorld(), D3DXVECTOR3(0.5f, 0.0f, 0.0f), D3DXVECTOR3(-0.5f, .0f, 0.0f), m_bOrbit, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), 0, 0, 0);
	CEfcOrbit::Create()->Set(m_apModel[5]->GetMtxWorld(), D3DXVECTOR3(-0.5f, 0.0f, 0.0f), D3DXVECTOR3(0.5f, .0f, 0.0f), m_bOrbit, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), 0, 0, 0);

	CCylinder::Create(m_pos, m_rot, CCylinder::TYPE_BOOST);
}
//=============================================================================
// 加速の終了処理
//=============================================================================
void CPlayer::EndBoost(void)
{
	m_bOrbit = true;
	CEfcOrbit::Delete(false);
	m_bOrbit = false;
}
//=============================================================================
// 状態の変更処理
//=============================================================================
void CPlayer::SetState(PLAYERSTATE state)
{
	switch (m_State)
	{
	case PLAYERSTATE_SPEEDUP:
	case PLAYERSTATE_SPEEDUP_S:
		EndBoost();
		break;
	}
	switch (state)
	{
	case PLAYERSTATE_SPEEDUP_S:	SetKiller();
	case PLAYERSTATE_SPEEDUP:
		UseBoost();
		break;
	case PLAYERSTATE_DAMAGE:	break;
	case PLAYERSTATE_SPEEDDOWN:
	case PLAYERSTATE_SPEEDDOWN_S:
		break;
	}
	m_State = state;
}
//=============================================================================
// エフェクトの更新処理
//=============================================================================
void CPlayer::EffectUp(void)
{
	//煙、足跡更新
	if (!m_bJump && m_PlayerAnim == PLAYERANIM_RUN)
	{//地面にいる && 歩きモーション
		D3DXVECTOR3 pos = m_pos + D3DXVECTOR3(sinf(m_rot.y + D3DX_PI * 0.5f), 0.0f, cosf(m_rot.y + D3DX_PI * 0.5f)) * (m_nKey == 0 ? -6.0f : 6.0f);

		switch (m_FEffect)
		{
		case CCOL_MESH::EFFECT_SWAMP:	EffectWater(pos);	break;
		case CCOL_MESH::EFFECT_GRASS:	EffectGrass(pos); 	break;
		case CCOL_MESH::EFFECT_NORMAL:
		case CCOL_MESH::EFFECT_BOOST:
			EffectNor(pos);
			break;

		}
	}
}
//=============================================================================
// 通常エフェクトの生成処理
//=============================================================================
void CPlayer::EffectNor(D3DXVECTOR3 &pos)
{//煙
	CModelEffect::Create(&pos, m_move, CModelEffect::TYPE_SMOKE);

	if (m_nKey % 2 == 0)
	{//足が付いたなら足跡
		C3DPolygon::Create(C3DPolygon::TYPE_FootSteps, pos
			, D3DXVECTOR3(-m_fCTiltV * D3DX_PI * 0.5f, m_rot.y, m_fCTiltW * D3DX_PI * 0.25f))->SetTexture(m_nKey / 2, 2, 1, 1);
	}
}
//=============================================================================
// アクセル（加速）処理
//=============================================================================
void CPlayer::Accelerator(bool bAccel)
{
	if (m_bJump) { return; }

	if (bAccel)
	{
		float fUp, fSpd;

		switch (m_State)
		{
		case PLAYERSTATE_SPEEDUP:
		case PLAYERSTATE_SPEEDUP_S:
			fUp = 1.0f;
			fSpd = PLAYER_SPDUP * 2.0f;
			break;
		default:
			fUp = 1.0f - (m_rot.y - m_fRotOld) / (D3DX_PI * 0.5f);
			fSpd = PLAYER_SPDUP;
			break;
		}
		if (m_PlayerInfo.fCountTime < 0.0f) { m_PlayerInfo.fCountTime = 0.0f; }
		if (fUp < 0.0f) { fUp = 0.0f; }
		float fWK = (fSpd * fUp) * (1.0f / powf(1.0f + (m_PlayerInfo.fCountTime * 0.1f), 10.0f));
		m_PlayerInfo.fCountTime += (fSpd * fUp) * (1.0f / powf(1.0f + (m_PlayerInfo.fCountTime), 2.0f));
		//m_fAccel += (fUp * PLAYER_SPDUP) / powf(m_fAccel + 1.0f, 3.0f);
		CDebugProc::Print("加速度 %.5f\n", m_PlayerInfo.fCountTime);

		if (m_PlayerInfo.fCountTime > 10.0f) { m_PlayerInfo.fCountTime = PLAYER_COUNT; }
	}
	else
	{
		float fMin = 0.0f;
		switch (m_StateSpeed)
		{
		case STATE_SPEED_BRAKS:	
			if (m_PlayerInfo.fCountTime > 0.0f)
			{ m_PlayerInfo.fCountTime *= 0.5f; }
			fMin = -PLAYER_COUNT * 0.35f; break;
		}
		m_PlayerInfo.fCountTime -= PLAYER_SPDDOWN * (1.0f / powf(1.0f + (m_PlayerInfo.fCountTime), 2.0f));
		if (m_PlayerInfo.fCountTime < fMin) { m_PlayerInfo.fCountTime = fMin; }
	}
}
//=============================================================================
// 水エフェクトの生成処理
//=============================================================================
void CPlayer::EffectWater(D3DXVECTOR3 &pos)
{//煙
	CModelEffect::Create(&pos, m_move, CModelEffect::TYPE_WATER);
	CModelEffect::Create(&pos, m_move, CModelEffect::TYPE_WATER);

	if (m_nKey % 2 == 0)
	{//足が付いたなら足跡
		CModelEffect::Create(&pos, m_move * 1.5f, CModelEffect::TYPE_WATER_S);
		CModelEffect::Create(&pos, m_move * 1.5f, CModelEffect::TYPE_WATER_S);
	}
}
//=============================================================================
// 草エフェクトの生成処理
//=============================================================================
void CPlayer::EffectGrass(D3DXVECTOR3 &pos)
{
	CModelEffect::Create(&pos, m_move, CModelEffect::TYPE_GRASS);

	if (m_nKey % 2 == 0)
	{//足が付いたなら足跡
		CModelEffect::Create(&pos, m_move * 1.5f, CModelEffect::TYPE_GRASS);
	}
}
//=============================================================================
// コントロールキー
//=============================================================================
void CPlayer::ControlKey(void)
{
	// 取得
	bool bOnline = CTitle::GetOnline();

	CSound *pSound = CManager::GetSound();

	CInputKeyBoard * pInputKeyboard = CManager::GetInput();		//キーボードの取得
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(m_nControllerNum);		//ジョイパットの取得

	if (m_bGoal == true)
	{
		m_bGoal = true;
		m_move *= 0.0f;
		SetStateHandle(HANDLE_MAX);
		SetStateSpeed(STATE_SPEED_DOWN);
		CancelMotion(PLAYERANIM_NEUTRAL, false);
		return;
	}

	//CDebugProc::Print("位置 : X %.2f, Y %.2f, Z %.2f\n", m_pos.x, m_pos.y, m_pos.z);

	//向きの設定
	if (m_StateSpeed != STATE_SPEED_STOP)
	{
		if ((bOnline == false &&
			(pInputKeyboard->GetKeyboardPress(DIK_A) == true ||
				pInputKeyboard->GetKeyboardPress(DIK_LEFT) == true)) ||
			pXpad->GetPress(INPUT_LS_L) == true ||
			pXpad->GetPress(INPUT_RS_L) == true ||
			pXpad->GetPress(INPUT_LEFT) == true)
		{ //左ハンドル状態
			SetStateHandle(HANDLE_LEFT);
			SetStick(pXpad, (pXpad->GetPress(INPUT_RS_L) == true ? 1 : 0));
		}
		else if ((bOnline == false &&
			(pInputKeyboard->GetKeyboardPress(DIK_D) == true ||
				pInputKeyboard->GetKeyboardPress(DIK_RIGHT) == true)) ||
			pXpad->GetPress(INPUT_LS_R) == true ||
			pXpad->GetPress(INPUT_RS_R) == true ||
			pXpad->GetPress(INPUT_RIGHT) == true)
		{//右ハンドル状態
			SetStateHandle(HANDLE_RIGHT);
			SetStick(pXpad, (pXpad->GetPress(INPUT_RS_R) == true ? 1 : 0));
		}
		else
		{//ハンドルを触っていない状態
			SetStateHandle(HANDLE_MAX);
		}
	}
	else
	{
		SetStateHandle(HANDLE_MAX);
	}

	// 操作
	if (
		((bOnline == false && pInputKeyboard->GetKeyboardPress(DIK_K) == true) ||
		(pXpad->GetPress(INPUT_L1) == true) ||
			(pXpad->GetPress(INPUT_L2) == true)) &&
			((bOnline == false && pInputKeyboard->GetKeyboardPress(DIK_L) == true) ||
		(pXpad->GetPress(INPUT_R1) == true) ||
				(pXpad->GetPress(INPUT_R2) == true))
		)
	{// ドリフト
		if (m_PlayerInfo.fCountTime < PLAYER_COUNT * 0.7f)
			SetStateSpeed(STATE_SPEED_ACCEL);
		else
			SetStateSpeed(STATE_SPEED_DRIFT);
	}
	else
	{
		//アクセル
		if ((bOnline == false && pInputKeyboard->GetKeyboardPress(DIK_K) == true) ||
			(pXpad->GetPress(INPUT_L1) == true) ||
			(pXpad->GetPress(INPUT_L2) == true))
		{//減速状態
			SetStateSpeed(STATE_SPEED_BRAKS);
		}
		else if
			((bOnline == false && pInputKeyboard->GetKeyboardPress(DIK_L) == true) ||
			(pXpad->GetPress(INPUT_R1) == true) ||
				(pXpad->GetPress(INPUT_R2) == true))
		{ //アクセルを状態
			SetStateSpeed(STATE_SPEED_ACCEL);
		}
		else
		{
			//停止判定
			D3DXVECTOR3 fDiffuse = m_pos - m_OldPos;

			if (fDiffuse.x < 0.10f && fDiffuse.x > -0.10f)
			{
				if (fDiffuse.z < 0.10f && fDiffuse.z > -0.10f)
				{
					SetStateSpeed(STATE_SPEED_STOP);

					if (m_State == PLAYERSTATE_SPEEDUP)
					{// スピードアイテムを使ったとき
						SetStateSpeed(STATE_SPEED_ACCEL);
					}
				}
				else
				{
					SetStateSpeed(STATE_SPEED_DOWN);
				}
			}
			else
			{
				SetStateSpeed(STATE_SPEED_DOWN);
			}
		}
	}

	//if (m_StateSpeed == STATE_SPEED_ACCEL)
	//{
	//	//走る音
	//	pSound->SetVolume(CSound::SOUND_LABEL_SE_RAN, 3.0f);
	//	pSound->PlaySound(CSound::SOUND_LABEL_SE_RAN);
	//}

	// ジャンプ


	if ((bOnline == false &&
		(pInputKeyboard->GetKeyboardTrigger(DIK_W) == true ||
			pInputKeyboard->GetKeyboardTrigger(DIK_UP) == true)) ||
		pXpad->GetTrigger(INPUT_A) == true ||
		pXpad->GetTrigger(INPUT_RS_U) == true)
	{// ジャンプキー
		SetJump();
	}

	if (pInputKeyboard->GetKeyboardTrigger(DIK_SPACE) == true ||
		pXpad->GetTrigger(INPUT_B) == true)
	{// 弾発射
		BulletEgg();
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_LSHIFT))
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_LSHIFT))
		{
			SetKiller();
		}
		UpdateKiller();
	}

	/*if ((pInputKeyboard->GetKeyboardPress(DIK_N) == true) || (pInputJoypad->GetPress(CXInput::XIJS_BUTTON_0) == true))
	{
	pSound->SetVolume(CSound::SOUND_LABEL_SE_KLAXON, 1.5f);
	pSound->PlaySound(CSound::SOUND_LABEL_SE_KLAXON);
	}*/
}

//=============================================================================
// 移動処理
//=============================================================================
void CPlayer::UpdateMove(void)
{
	CSound *pSound = CManager::GetSound();

	m_OldPos = m_pos;	//前回の位置を保存する

	if (m_bJump == false)
	{//傾斜処理
		D3DXVECTOR3 Vec2;
		Vec2 = D3DXVECTOR3(sinf(m_rot.y), 0.0f, cosf(m_rot.y));
		m_fTilt = (m_FNor.x * Vec2.x) + (m_FNor.y * Vec2.y) + (m_FNor.z * Vec2.z) /
			(sqrtf(powf(m_FNor.x, 2) + powf(m_FNor.y, 2) + powf(m_FNor.z, 2)) * sqrtf(powf(Vec2.x, 2) + powf(Vec2.y, 2) + powf(Vec2.z, 2)));
		acosf(m_fTilt);
		m_fCTiltV = (m_fTilt * -1.0f) * 0.5f;
		m_fTilt = m_fCTiltV + 0.1f;

		Vec2 = D3DXVECTOR3(sinf(m_rot.y + D3DX_PI * 0.5f), 0.0f, cosf(m_rot.y + D3DX_PI * 0.5f));
		m_fCTiltW = (m_FNor.x * Vec2.x) + (m_FNor.y * Vec2.y) + (m_FNor.z * Vec2.z) /
			(sqrtf(powf(m_FNor.x, 2) + powf(m_FNor.y, 2) + powf(m_FNor.z, 2)) * sqrtf(powf(Vec2.x, 2) + powf(Vec2.y, 2) + powf(Vec2.z, 2)));
		acosf(m_fCTiltW);
		m_fCTiltW = (m_fTilt * -1.0f) * 0.5f;
	}
	else { m_fTilt = 0.0f; }
	if (m_fTilt > 0.05f) { m_fTilt = 0.05f; }
	if (m_fTilt > -0.2f) { m_fTilt = -0.2f; }

	RemakeAngle(&m_rot.y);

	if (m_fCntWind > 0.0f)	
	{//風の更新
		m_fCntWind--;
		if (m_fCntWind <= 0.0f) { EndBoost(); }
	}
	else					{ m_WindMove *= 0.95f; }
	if (m_fPower > 0.0f)
	{//加速度プラス用　時間経過で減少
		m_fPower -= PLAYER_POWDOWN;
	}
	if (m_fTackle > 0.0f)
	{//タックル時強制アクセルF
		if (m_StateSpeed < STATE_SPEED_DRIFT)
		{
			m_StateSpeed = STATE_SPEED_ACCEL;
		}
		m_fTackle -= 1.0f;
	}
	if (m_pShadow != NULL)
	{
		m_pShadow->GetrotR() = D3DXVECTOR3(-m_fCTiltV * D3DX_PI * 0.5f, m_rot.y, m_fCTiltW * D3DX_PI * 0.25f);
	}

	if (m_State == PLAYERSTATE_SPEEDUP || m_State == PLAYERSTATE_SPEEDUP_S)
	{// スピードアイテムを使ったとき
		m_nCountSpeed++;

		float fTime = SPEEDUP_TIME;
		m_PlayerInfo.fCountTime = PLAYER_COUNT;
		if (m_State == PLAYERSTATE_SPEEDUP_S)
		{
			CancelMotion(PLAYERANIM_CHICKGETON, false);
			fTime = 60.0f * KILLER_TIME;
		}
		else if (m_State == PLAYERSTATE_SPEEDUP && m_bSpeedEgg == true)
		{
			CancelMotion(PLAYERANIM_EGGGETON, false);
		}

		if (m_nCountSpeed > fTime)
		{
			EndBoost();
			SetState(PLAYERSTATE_NORMAL);
			m_nCountSpeed = 0;
			CancelMotion(PLAYERANIM_NEUTRAL, false);
			if (m_bSpeedEgg == true)
			{
				m_bSpeedEgg = false;
			}
		}
	}
	if (m_fPower > PLAYER_POWMAX) { m_fPower = PLAYER_POWMAX; }


	float fAccel = m_PlayerInfo.fAccel;
	float fBraks = m_PlayerInfo.fBraks;
	float fAddRot = m_PlayerInfo.fAddRot;
	float fDown = m_PlayerInfo.fDown;
	float fPow = (m_PlayerInfo.fCountTime + m_fPower) / PLAYER_COUNT;
	
	fAddRot *= PLAYER_ADDROT_INIT;

	//状態ごとの更新処理
	switch (m_StateSpeed)
	{
	case STATE_SPEED_ACCEL:	//アクセル状態

		//走るモーション
		CancelMotion(PLAYERANIM_RUN, false);

		//ジャンプ状態なら
		if (m_bJump == true) { break; }

		if (m_State == PLAYERSTATE_NORMAL || m_State == PLAYERSTATE_SPEEDDOWN || m_State == PLAYERSTATE_SPEEDDOWN_S)
		{
			m_fSpeed = fAccel * fPow * (1.0f - m_fTilt);
		}

		//進行方向の設定
		m_move.x += sinf(m_rot.y) * (m_fSpeed);
		m_move.z += cosf(m_rot.y) * (m_fSpeed);

		Accelerator(true);
		break;
	case STATE_SPEED_BRAKS: //ブレーキ状態

		//走るモーション
		CancelMotion(PLAYERANIM_RUN, false);
		
		//ジャンプ状態なら
		if (m_bJump == true) { break; }

		m_fSpeed = fBraks * fPow * (1.0f - m_fTilt);

		//進行方向の設定
		m_move.x += sinf(m_rot.y) * m_fSpeed;
		m_move.z += cosf(m_rot.y) * m_fSpeed;

		//揺れを無効にする
		m_bShake = false;

		Accelerator(false);
		break;
	case STATE_SPEED_DRIFT:	//ドリフト状態

		fAccel *= PLAYER_DRIFT_ACCEL;
		fAddRot = m_PlayerInfo.fAddRot;
		fDown *= PLAYER_DRIFT_DOWN;

		if (m_State == PLAYERSTATE_SPEEDUP || m_State == PLAYERSTATE_SPEEDUP_S)
			fAccel *= 0.8f;

		//走るモーション
		CancelMotion(PLAYERANIM_RUN, false);

		//ジャンプ状態なら
		if (m_bJump == true) { break; }

		if (m_State == PLAYERSTATE_NORMAL || m_State == PLAYERSTATE_SPEEDDOWN || m_State == PLAYERSTATE_SPEEDDOWN_S)
		{
			m_fSpeed = fAccel * fPow * (1.0f - m_fTilt);
		}
		
		//進行方向の設定
		m_move.x += sinf(m_rot.y) * (m_fSpeed * PLAYER_DRIFT_ACCEL);
		m_move.z += cosf(m_rot.y) * (m_fSpeed * PLAYER_DRIFT_ACCEL);

		Accelerator(true);
		break;
	case STATE_SPEED_DOWN: //ダウン状態
		if (m_bJump == true) { break; }

		//CDebugProc::Print("DWON***\n");
		if (m_fSpeed < 0.15f) { m_fSpeed *= 0.1f; }
		m_fSpeed += (0.0f - m_fSpeed) * 0.015f;// ((1.0f - (m_PlayerInfo.fCountTime < 90 ? (m_PlayerInfo.fCountTime / 90) : 1.0f)) * (1.0f - m_fTilt * 1.5f));

		//進行方向の設定
		m_move.x += sinf(m_rot.y) * (m_fSpeed);
		m_move.z += cosf(m_rot.y) * (m_fSpeed);

		Accelerator(false);

		//停止判定
		if (m_move.x < 0.10f && m_move.x > -0.10f)
		{
			if (m_move.z < 0.10f && m_move.z > -0.10f)
			{
				SetStateSpeed(STATE_SPEED_STOP);
				m_move *= 0.0f;
			}
		}
		break;
	default:
		//走るモーション
		CancelMotion(PLAYERANIM_NEUTRAL, false);
		Accelerator(false);
		break;
	}

	switch (m_State)
	{// 弾を食らったとき
	case PLAYERSTATE_NORMAL:
		break;

	case PLAYERSTATE_SPEEDDOWN:
		//進行方向の設定
		if (m_PlayerInfo.fCountTime > SPEED_COUNT_ANNOY)
		{
			m_PlayerInfo.fCountTime = SPEED_COUNT_ANNOY;	// 減速
		}

		if (m_nCntParticle > ANNOY_PARTICLE)
		{
			D3DXVECTOR2 fPos;
			fPos.x = 30.0f - (float)(CServer::Rand() % 60);
			fPos.y = 30.0f - (float)(CServer::Rand() % 60);

			CParticle::Create(D3DXVECTOR3(m_pos.x + fPos.x, m_pos.y + 30.0f, m_pos.z + fPos.y),
				D3DXVECTOR3(-m_move.x, 1.0f, -m_move.z),
				D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f),
				D3DXVECTOR2(0.5f, 5.0f),
				20,
				CParticle::TEXTURE_SMOKE,
				CParticle::TYPE_DOWN,
				m_nPlayerNum);

			m_nCntParticle = 0;
		}

		//スピードダウンの音
		pSound->SetVolume(CSound::SOUND_LABEL_SE_SPEEDDOWN, 2.0f);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_SPEEDDOWN);

		m_nCntParticle++;

		break;

	case PLAYERSTATE_SPEEDDOWN_S:
		//進行方向の設定
		if (m_PlayerInfo.fCountTime > SPEED_COUNT_ANNOY)
		{ m_PlayerInfo.fCountTime = SPEED_COUNT_ANNOY; }	// 減速

		if (m_nCntParticle > ANNOY_PARTICLE)
		{
			D3DXVECTOR2 fPos;
			fPos.x = 20.0f - (float)(CServer::Rand() % 40);
			fPos.y = 20.0f - (float)(CServer::Rand() % 40);

			CParticle::Create(D3DXVECTOR3(m_pos.x + fPos.x, m_pos.y + 30.0f, m_pos.z + fPos.y),
				D3DXVECTOR3(-m_move.x, 1.0f, -m_move.z),
				D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f),
				D3DXVECTOR2(0.5f, 5.0f),
				20,
				CParticle::TEXTURE_SMOKE,
				CParticle::TYPE_DOWN,
				m_nPlayerNum);

			m_nCntParticle = 0;
		}

		//スピードダウンの音
		pSound->SetVolume(CSound::SOUND_LABEL_SE_SPEEDDOWN, 2.0f);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_SPEEDDOWN);

		m_nCntParticle++;
		break;

	case PLAYERSTATE_DAMAGE:
		//進行方向の設定
		m_PlayerInfo.fCountTime = SPEED_COUNT_DAMAGE;	// 減速
		m_fSpeed = 0.0f;
		break;
	}

	if (m_bDamage == true)
	{
		m_nCntDamage++;

		float fDamageTime = 0.0f;	// 状態が変わる時間の長さ

		if (m_State == PLAYERSTATE_DAMAGE)
		{// 攻撃を食らったとき
			fDamageTime = DAMAGE_TIME;
		}
		else if (m_State == PLAYERSTATE_SPEEDDOWN)
		{// スピードダウンを食らったとき
			fDamageTime = SPEEDDOWN_TIME;
		}
		else if (m_State == PLAYERSTATE_SPEEDDOWN_S)
		{// スピードダウンを食らったとき
			fDamageTime = HatchTime(SPEEDDOWN_TIME, 120.0f);
		}

		if (m_nCntDamage > fDamageTime)
		{
			if (m_State != PLAYERSTATE_SPEEDUP && m_State != PLAYERSTATE_SPEEDUP_S)
			{
				SetState(PLAYERSTATE_NORMAL);
				m_nCntDamage = 0;
				m_bDamage = false;
			}
		}
	}

	//if (m_PlayerType == PLAYERTYPE_PLAYER)
	{
		//CDebugProc::Print("m_bDamage : %d\n", m_bDamage);
		CDebugProc::Print("m_State : %s\n",
			(m_State == PLAYERSTATE_NORMAL ? "PLAYERSTATE_NORMAL" :
			(m_State == PLAYERSTATE_SPEEDUP ? "PLAYERSTATE_SPEEDUP" :
				(m_State == PLAYERSTATE_SPEEDUP_S ? "PLAYERSTATE_SPEEDUP_S" :
				(m_State == PLAYERSTATE_SPEEDDOWN ? "PLAYERSTATE_SPEEDDOWN" :
					(m_State == PLAYERSTATE_SPEEDDOWN_S ? "PLAYERSTATE_SPEEDDOWN_S" :
					(m_State == PLAYERSTATE_DAMAGE ? "PLAYERSTATE_DAMAGE" : "")))))));
		//CDebugProc::Print("m_nCntDamage : %d\n", m_nCntDamage);

		//CDebugProc::Print("m_StateSpeed : %s\n",
		//	(m_StateSpeed == STATE_SPEED_ACCEL ? "STATE_SPEED_ACCEL" :
		//	(m_StateSpeed == STATE_SPEED_BRAKS ? "STATE_SPEED_BRAKS" :
		//	(m_StateSpeed == STATE_SPEED_DRIFT ? "STATE_SPEED_DRIFT" :
		//	(m_StateSpeed == STATE_SPEED_DOWN ? "STATE_SPEED_DOWN" :
		//	(m_StateSpeed == STATE_SPEED_STOP ? "STATE_SPEED_STOP" : ""))))));
		//CDebugProc::Print("m_rot x : %.1f y : %.1f z : %.1f\n", m_rot.x, m_rot.y, m_rot.z);

		//CDebugProc::Print("fCountTime : %f\n", m_PlayerInfo.fCountTime);
		//CDebugProc::Print("m_fSpeed : %f\n", m_fSpeed);
		//CDebugProc::Print("fDown : %f\n", fDown);
	}

	//CDebugProc::Print("アクセル : %1f\n", m_PlayerInfo.fAccel);
	//CDebugProc::Print("スピード : %1f  %1f  %1f\n", m_move.x, m_move.y, m_move.z);

	//ハンドルの状態更新
	float fHandle = m_PlayerInfo.fCountTime / (PLAYER_COUNT * 0.5f);
	if (fHandle < 0.0f) { fHandle *= -1.0f; }
	//if (m_PlayerInfo.fCountTime < 0.0f && fHandle < -0.2f)
	//{ fHandle = -0.2f; }
	if (fHandle > 1.0f) { fHandle = 1.0f; }
	if (fHandle < -1.0f) { fHandle = -1.0f; }

	if (m_StateHandle == HANDLE_LEFT)
	{
		if (m_StateSpeed != STATE_SPEED_STOP)
		{
			m_fAddRot -= fAddRot * fHandle * m_fStick;
		}
	}
	else if (m_StateHandle == HANDLE_RIGHT)
	{
		if (m_StateSpeed != STATE_SPEED_STOP)
		{
			m_fAddRot += fAddRot * fHandle * m_fStick;
		}
	}

	// 重力
	//if (m_bShake == true)
	{
		m_move.y -= cosf(0) * PLAYER_GRAVITY;
	}

	//中心座標更新
	m_pos += m_move + m_WindMove;

	m_rot.y += m_fAddRot;

	//減速
	if (!m_bJump)
	{
		m_move.x += (0.0f - m_move.x) * fDown;
		m_move.z += (0.0f - m_move.z) * fDown;
	}

	EggJump();

	m_fAddRot += (0.0f - m_fAddRot) * PLAYER_DOWNROT;

	//CDebugProc::Print("addrot : %f\n", m_fAddRot);
	//CDebugProc::Print("fSpeed : %f\n", m_fSpeed);
}
//=============================================================================
// プレイヤーの高さ処理
//=============================================================================
void CPlayer::UpdateField(void)
{
	CScene *pScene = CScene::GetTop(MESH_PRIOTITY);

	//NULLチェック
	while (pScene != NULL)
	{
		//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetObjType() == CScene::OBJTYPE_GROUND)
		{//タイプが地面だったら
			CMeshField *pField = (CMeshField*)pScene;

			if (pField->OnField(m_pos, m_fvtxMaxY))
			{// 傾斜の計算
				float fHight = pField->GetHeightMesh(m_pos);

				//if (m_pos.y <= fHight)
				if (m_bJump == false || (m_bJump == true && m_pos.y < fHight))
				{
					float fEggHeight = 0.0f;

					if (m_State == PLAYERSTATE_SPEEDUP)
					{// 卵に乗ったように見える高さ
						fEggHeight = EGG_HEIGHT;
					}

					m_pos.y = fHight + fEggHeight;		//地面の高さを取得
					m_move.y = 0.0f;					//移動量を初期化する

														//ジャンプ中かどうか
														//bool bJumpFlag = pPlayer->GetJump();
														//if (bJumpFlag)
														//{
														//	pPlayer->GetPlayerInfoPoint()->nCountTime -= (int)std::round(pPlayer->GetPlayerInfoPoint()->nCountTime * 0.05f);
														//}

														//ジャンプの状態設定
					m_bJump = false;
					//m_pIncline->SetJumpFlag(false);

					break;
				}
			}
		}
		//Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

//=============================================================================
// スピードの状態設定
//=============================================================================
void CPlayer::SetStateSpeed(CPlayer::STATE_SPEED state)
{
	//数値の設定
	if (m_StateSpeed != state)
	{
		if ((m_StateSpeed == STATE_SPEED_ACCEL && state == STATE_SPEED_DRIFT) ||
			(state == STATE_SPEED_ACCEL && m_StateSpeed == STATE_SPEED_DRIFT))
		{
			m_nDriftCounter = 0;
		}
		/*else
		{
		m_PlayerInfo.fCountTime = 0;
		}*/

		//CDebugProc::Print("CHANGE***\n");

		CSound *pSound = CManager::GetSound();

		/*if (STATE_SPEED_ACCEL == state && STATE_DRIVE == m_MoveState)
		{// アクセル音
		pSound->SetVolume(CSound::SOUND_LABEL_SE_ACCEL, 0.7f);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_ACCEL);
		}
		else if (STATE_SPEED_BRAKS == state)
		{// ブレーキ音
		if (m_move.x >= 0.1f || m_move.x <= -0.1f || m_move.z >= 0.1f || m_move.z <= -0.1f)
		{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_BRAKING);
		pSound->StopSound(CSound::SOUND_LABEL_SE_ACCEL);
		}
		}*/
	}

	//状態の設定
	m_StateSpeed = state;
}
//=============================================================================
// ジャンプ処理
//=============================================================================
void CPlayer::SetJump(void)
{
	if (m_bJump == false)
	{// ジャンプしていない
		CancelMotion(PLAYERANIM_JUMP, false);
		m_bJump = true;
		m_bSJump = false;
		m_move.y += PLAYER_JUMP;
	}
}
//=============================================================================
// 設定処理
//=============================================================================
void CPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_pos = pos;
}

//=============================================================================
// オブジェクトとの当たり判定
//=============================================================================
void CPlayer::CollisionObject(void)
{
	CSound *pSound = CManager::GetSound();
	CScene *pScene;

	bool bType = false;	//タイプのフラグ

	for (int nCntPriority = OBJECT_PRIOTITY; nCntPriority <= OBJECT_PRIOTITY; nCntPriority++)
	{
		// プライオリティーチェック
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// プライオリティー内のリスト構造を最後まで見る
			CScene *pSceneNext = pScene->GetNext();		// 次のオブジェクトを保存

			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{// タイプが障害物だったら
				CObject *pObject = (CObject*)pScene;	// オブジェクトクラスのポインタ変数にする
				int nType = pObject->GetType();			// 障害物の種類を取得
														//当たり判定用のオブジェクトにのみ当たる
				if (nType == 2) { pObject->CollisionObject(&m_pos, &m_OldPos, &m_move); }
			}

			// Nextに次のSceneを入れる
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// 壁との当たり判定
//=============================================================================
void CPlayer::CollisitionWall(void)
{
	CScene *pScene;
	//プライオリティーチェック
	pScene = CScene::GetTop(WALL_PRIOTITY);

	//NULLチェック
	while (pScene != NULL)
	{
		//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{
			//タイプが壁だったら
			if (pScene->GetObjType() == OBJTYPE_WALL)
			{
				bool bTouch = ((CWall*)pScene)->CollisionWall(&m_pos, &m_OldPos, &m_move, 20.0f);

				if (bTouch)
				{ //当たっているなら減速する
					m_PlayerInfo.fAccel = m_PlayerInfo.fAccel * DECELERATION;
					m_PlayerInfo.fCountTime = 0;
					break;
				}
			}
		}

		//Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

//=============================================================================
// 角度修正
//=============================================================================
void CPlayer::RemakeAngle(float * pAngle)
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
// 餌との当たり判定
//=============================================================================
void CPlayer::CollisionFeed(void)
{
	CSound *pSound = CManager::GetSound();
	CScene *pScene;
	float fValue = 9999.9f;

	// プライオリティーチェック
	pScene = CScene::GetTop(FEED_PRIOTITY);
	while (pScene != NULL)
	{// プライオリティー内のリスト構造を最後まで見る
		CScene *pSceneNext = pScene->GetNext();		// 次のオブジェクトを保存

		if (pScene->GetObjType() == OBJTYPE_FEED)
		{// タイプが障害物だったら
			CFeed *pFeed = (CFeed*)pScene;	// オブジェクトクラスのポインタ変数にする

			if (pFeed->GetDeath() != true)
			{
				if (pFeed->TargetFeed(this, fValue) == true)
				{// 衝突した
					EggAppear(pFeed);	// 卵出現
					m_nNumEgg++;
					m_nNumItem++;

					break;
				}
			}
		}

		// Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

//=============================================================================
// 餌との当たり判定
//=============================================================================
void CPlayer::EggAppear(CFeed *pFeed)
{
	CSound *pSound = CManager::GetSound();

	if (m_pEgg[m_nNumEgg] == NULL)
	{
		if (pFeed->GetFeedType() == CFeed::FEEDTYPE_ATTACK)
		{// 攻撃の卵生成
			m_pEgg[m_nNumEgg] = CEgg::Create(m_pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(EGG_SCALE, EGG_SCALE, EGG_SCALE),
				CEgg::EGGTYPE_ATTACK,
				CEgg::BULLETTYPE_PLAYER,
				m_nPlayerNum);

			m_bulletType[m_nNumChick + m_nNumEgg] = BULLET_EGG_ATTACK;

			//卵が産まれる音
			pSound->PlaySound(CSound::SOUND_LABEL_SE_EGGLAY);
			pSound->SetVolume(CSound::SOUND_LABEL_SE_EGGLAY, 7.0f);
			pSound->SetFrequency(CSound::SOUND_LABEL_SE_EGGLAY, 0.7f);
		}
		else if (pFeed->GetFeedType() == CFeed::FEEDTYPE_ANNOY)
		{// 妨害の卵生成
			m_pEgg[m_nNumEgg] = CEgg::Create(m_pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(EGG_SCALE, EGG_SCALE, EGG_SCALE),
				CEgg::EGGTYPE_ANNOY,
				CEgg::BULLETTYPE_PLAYER,
				m_nPlayerNum);

			m_bulletType[m_nNumChick + m_nNumEgg] = BULLET_EGG_ANNOY;

			//卵が産まれる音
			pSound->PlaySound(CSound::SOUND_LABEL_SE_EGGLAY);
			pSound->SetVolume(CSound::SOUND_LABEL_SE_EGGLAY, 7.0f);
			pSound->SetFrequency(CSound::SOUND_LABEL_SE_EGGLAY, 0.7f);
		}
		else if (pFeed->GetFeedType() == CFeed::FEEDTYPE_SPEED)
		{// 加速の卵生成
			m_pEgg[m_nNumEgg] = CEgg::Create(m_pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(EGG_SCALE, EGG_SCALE, EGG_SCALE),
				CEgg::EGGTYPE_SPEED,
				CEgg::BULLETTYPE_PLAYER,
				m_nPlayerNum);

			m_bulletType[m_nNumChick + m_nNumEgg] = BULLET_EGG_SPEED;

			//卵が産まれる音
			pSound->PlaySound(CSound::SOUND_LABEL_SE_EGGLAY);
			pSound->SetVolume(CSound::SOUND_LABEL_SE_EGGLAY, 7.0f);
			pSound->SetFrequency(CSound::SOUND_LABEL_SE_EGGLAY, 0.7f);
		}
	}

	m_pEgg[m_nNumEgg]->SetState(CEgg::EGGSTATE_CHASE);
}


//=============================================================================
// 卵がついてくる処理
//=============================================================================
void CPlayer::ChaseEgg(void)
{
	m_nCntFrame++;

	if (m_nCntFrame >= MAX_FRAME)
	{
		m_nCntFrame = 0;
	}

	// 前のプレイヤーの角度を保存
	m_OldEggRot[m_nCntFrame] = m_rot;
	m_abJump[m_nCntFrame] = m_bJumpSave;

	if (m_nNumEgg + m_nNumChick <= 0) { return; }

	if (m_nNumEgg >= 1)
	{// 卵が一個の時
		if (m_pEgg[0]->GetState() == CEgg::EGGSTATE_CHASE)
		{
			// 前の向きを代入
			int nData = m_nCntFrame - EGG_POS * (1 + m_nNumChick);

			if (nData < 0)
			{
				nData += MAX_FRAME;
			}

			// 卵の位置設定
			m_pEgg[0]->SetPos(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (1 + m_nNumChick)) + m_pos.x,
				m_pEgg[0]->GetPos().y,
				(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (1 + m_nNumChick)) + m_pos.z));

			// 卵の角度設定
			m_pEgg[0]->SetRot(m_OldEggRot[nData]);

			if (m_abJump[nData] == true)
			{
				m_pEgg[0]->Jump(EGGJUMP);
			}
		}
	}
	if (m_nNumEgg >= 2)
	{// 卵が二個の時
	 // 二個目
		int nData = m_nCntFrame - EGG_POS * 2;

		if (nData < 0)
		{
			nData += MAX_FRAME;
		}

		m_pEgg[1]->SetPos(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (2 + m_nNumChick)) + m_pos.x,
			m_pEgg[1]->GetPos().y,
			(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (2 + m_nNumChick)) + m_pos.z));

		m_pEgg[1]->SetRot(m_OldEggRot[nData]);

		if (m_abJump[nData] == true)
		{
			m_pEgg[1]->Jump(EGGJUMP);
		}
	}
	if (m_nNumEgg >= 3)
	{
		// 三個目
		int nData = m_nCntFrame - EGG_POS * 3;

		if (nData < 0)
		{
			nData += MAX_FRAME;
		}

		m_pEgg[2]->SetPos(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (3 + m_nNumChick)) + m_pos.x,
			m_pEgg[2]->GetPos().y,
			(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (3 + m_nNumChick)) + m_pos.z));

		m_pEgg[2]->SetRot(m_OldEggRot[nData]);

		if (m_abJump[nData] == true)
		{
			m_pEgg[2]->Jump(EGGJUMP);
		}
	}

	if (m_nNumChick >= 1)
	{// 卵が一個の時
		if (m_pChick[0]->GetState() == CChick::STATE_CHASE)
		{
			// 前の向きを代入
			int nData = m_nCntFrame - EGG_POS;

			if (nData < 0)
			{
				nData += MAX_FRAME;
			}

			// 卵の位置設定
			m_pChick[0]->SetPos(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE) + m_pos.x,
				m_pChick[0]->GetPos().y,
				(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE) + m_pos.z));

			// 卵の角度設定
			m_pChick[0]->SetRot(m_OldEggRot[nData]);

			if (m_abJump[nData] == true)
			{
				m_pChick[0]->Jump(EGGJUMP);
			}
		}
	}
	if (m_nNumChick >= 2)
	{// ひよこが二匹の時
	 // 二匹目
		int nData = m_nCntFrame - EGG_POS * 2;

		if (nData < 0)
		{
			nData += MAX_FRAME;
		}

		m_pChick[1]->SetPos(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 2) + m_pos.x,
			m_pChick[1]->GetPos().y,
			(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 2) + m_pos.z));

		m_pChick[1]->SetRot(m_OldEggRot[nData]);

		if (m_abJump[nData] == true)
		{
			m_pChick[1]->Jump(EGGJUMP);
		}
	}
	if (m_nNumChick >= 3)
	{
		// 二匹目
		int nData = m_nCntFrame - EGG_POS * 3;

		if (nData < 0)
		{
			nData += MAX_FRAME;
		}

		m_pChick[2]->SetPos(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 3) + m_pos.x,
			m_pChick[2]->GetPos().y,
			(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 3) + m_pos.z));

		m_pChick[2]->SetRot(m_OldEggRot[nData]);

		if (m_abJump[nData] == true)
		{
			m_pChick[2]->Jump(EGGJUMP);
		}
	}
}

//=============================================================================
// 卵発射処理
//=============================================================================
void CPlayer::BulletEgg(void)
{
	CPlayer **pPlayer = NULL;

	if (m_State == PLAYERSTATE_DAMAGE) { return; }
	if (m_PlayerAnim == PLAYERANIM_DAMAGE) { return; }

	CSound *pSound = CManager::GetSound();

	if (m_nNumEgg + m_nNumChick > 0)
	{// 卵かひよこを持っているとき
		if (m_pChick[0] != NULL && m_pChick[0]->GetState() == CChick::STATE_CHASE && m_State != PLAYERSTATE_SPEEDUP_S && m_State != PLAYERSTATE_SPEEDDOWN_S && m_State != PLAYERSTATE_SPEEDDOWN)
		{
			m_pChick[0]->DeleteEfc();
			m_pChick[0]->SetState(CChick::STATE_BULLET);	// 状態を弾にする
			m_pChick[0]->SetRank(CGame::GetRanking(m_nPlayerNum));
			m_nPlayerRank = CGame::GetRanking(m_nPlayerNum);
			int nRank = CGame::GetRanking(m_nPlayerNum) - 1;
			D3DXVECTOR2 fSize = D3DXVECTOR2(0.0f, 0.0f);

			m_nNumChick--;	// 所持数を減らす

			m_nNumItem--;

			switch (m_pChick[0]->GetType())
			{
			case CChick::TYPE_ATTACK:
				CancelMotion(PLAYERANIM_INTIMIDATION, false);
				if (nRank >= 0)
				{
					for (int nCntChar = 0; nCntChar < MAX_MEMBER; nCntChar++)
					{// ひとつ前の順位のやつを見つける
						int nData = CGame::GetRanking(nCntChar);

						if (nRank == nData)
						{
							m_nDestRank = nCntChar;
							m_pChick[0]->SetDestRank(nCntChar);
							break;
						}
					}
				}
				else
				{
					m_pChick[0]->SetDis(false);
				}

				m_pChick[0]->SetRot(m_rot);
				break;

				// 加速
			case CChick::TYPE_SPEED:
				SetState(PLAYERSTATE_SPEEDUP);
				m_fSpeed += SPEED_CHICK;
				m_pChick[0]->SetDis(false);
				m_pChick[0]->Jump(CHICK_SPEEDJUMP);
				break;

				// 減速
			case CChick::TYPE_ANNOY:
				CancelMotion(PLAYERANIM_INTIMIDATION, false);
				CCylinder::Create(m_pos, m_rot, CCylinder::TYPE_HDEF);
				m_pChick[0]->SetDis(false);
				break;

				// 強い攻撃
			case CChick::TYPE_ATTACK_S:
				CancelMotion(PLAYERANIM_INTIMIDATION, false);
				CCylinder::Create(m_pos, m_rot, CCylinder::TYPE_HATK_SC);

				for (int nCntParticle = 0; nCntParticle < MAX_SMOKE; nCntParticle++)
				{
					fSize.x = SMOKE_SIZE + (float)(CServer::Rand() % 3);
					fSize.y = SMOKE_SIZE + (float)(CServer::Rand() % 3);

					CParticle::Create(D3DXVECTOR3((sinf(m_rot.y + D3DX_PI) * -30.0f) + m_pChick[0]->GetPos().x,
						m_pChick[0]->GetPos().y,
						(cosf(m_rot.y + D3DX_PI) * -30.0f) + m_pChick[0]->GetPos().z),
						D3DXVECTOR3(sinf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 3 + 1)), cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 1 + 1)), cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 3 + 1))),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
						fSize,
						30,
						CParticle::TEXTURE_SMOKE,
						CParticle::TYPE_TURN,
						m_nPlayerNum);
				}
				m_pChick[0]->SetAttackCol(false);
				m_pChick[0]->SetRot(m_rot);
				break;

				// 減速
			case CChick::TYPE_ANNOY_S:
				CancelMotion(PLAYERANIM_INTIMIDATION, false);
				m_pChick[0]->SetDis(false);
				break;

				// 加速
			case CChick::TYPE_SPEED_S:
				SetState(PLAYERSTATE_SPEEDUP_S);
				m_fSpeed += SPEED_CHICK;
				m_pChick[0]->SetDis(false);
				m_pChick[0]->SetSpeedS(true);
				break;
			}

			// 情報入れ替え
			m_pChick[0] = m_pChick[1];
			m_pChick[1] = m_pChick[2];
			m_pChick[2] = NULL;

			m_bulletType[0] = m_bulletType[1];
			m_bulletType[1] = m_bulletType[2];
			m_bulletType[2] = BULLET_EGG_ATTACK;
		}
		else if (m_pEgg[0] != NULL && m_pEgg[0]->GetState() == CEgg::EGGSTATE_CHASE && m_State != PLAYERSTATE_SPEEDUP_S && m_State != PLAYERSTATE_SPEEDDOWN_S && m_State != PLAYERSTATE_SPEEDDOWN)
		{// 一個目の卵に情報が入っていて、プレイヤーについてくる時
			m_pEgg[0]->SetState(CEgg::EGGSTATE_BULLET);	// 状態を弾にする

			switch (CManager::GetMode())
			{
			case CManager::MODE_TITLE:
				m_pEgg[0]->SetRank(CTitle::GetRanking(m_nPlayerNum));
				m_nPlayerRank = CTitle::GetRanking(m_nPlayerNum);
				break;
			case CManager::MODE_GAME:
				m_pEgg[0]->SetRank(CGame::GetRanking(m_nPlayerNum));
				m_nPlayerRank = CGame::GetRanking(m_nPlayerNum);
				break;
			}

			m_nNumEgg--;	// 所持数を減らす

			m_nNumItem--;

			switch (m_pEgg[0]->GetType())
			{
				// 攻撃
			case CEgg::EGGTYPE_ATTACK:
				CancelMotion(PLAYERANIM_INTIMIDATION, false);
				m_pEgg[0]->SetJump(false);
				m_pEgg[0]->Jump(THROW);
				m_pEgg[0]->SetRot(D3DXVECTOR3(0.0f, m_rot.y, 0.0f));
				m_pEgg[0]->SetThrow(TRUE);
				break;

				// 加速
			case CEgg::EGGTYPE_SPEED:
				// 卵の角度設定
				m_pEgg[0]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetState(PLAYERSTATE_SPEEDUP);
				m_fSpeed += SPEED_EGG;
				m_bSpeedEgg = true;
				break;

				// 減速
			case CEgg::EGGTYPE_ANNOY:
				m_pEgg[0]->SetDis(false);
				break;
			}

			// 情報入れ替え
			m_pEgg[0] = m_pEgg[1];
			m_pEgg[1] = m_pEgg[2];
			m_pEgg[2] = NULL;

			m_bulletType[0] = m_bulletType[1];
			m_bulletType[1] = m_bulletType[2];
			m_bulletType[2] = BULLET_EGG_ATTACK;
		}

		pSound->PlaySound(CSound::SOUND_LABEL_SE_THROW);
		pSound->SetVolume(CSound::SOUND_LABEL_SE_THROW, 3.0f);
		pSound->SetFrequency(CSound::SOUND_LABEL_SE_THROW, 0.5f);
	}
}

//=============================================================================
// 卵との当たり判定
//=============================================================================
void CPlayer::CollisionEgg(void)
{
	CSound *pSound = CManager::GetSound();
	CScene *pScene;

	// プライオリティーチェック
	pScene = CScene::GetTop(EGG_PRIOTITY);
	while (pScene != NULL)
	{// プライオリティー内のリスト構造を最後まで見る
		CScene *pSceneNext = pScene->GetNext();		// 次のオブジェクトを保存

		if (pScene->GetObjType() == OBJTYPE_EGG)
		{// タイプが障害物だったら
			CEgg *pEgg = (CEgg*)pScene;	// オブジェクトクラスのポインタ変数にする

			if (pEgg->GetNumPlayer() != m_nPlayerNum)
			{
				if (pEgg->CollisionEgg(&m_pos, &m_OldPos) == true)
				{// 衝突した
					switch (pEgg->GetType())
					{
						// 攻撃
					case CEgg::EGGTYPE_ATTACK:
						// ダメージ状態にする
						if (m_State != PLAYERSTATE_DAMAGE && m_State != PLAYERSTATE_SPEEDUP_S)
						{
							CancelMotion(PLAYERANIM_DAMAGE, false);

							m_bDamage = true;
							m_nCntDamage = 0;
							SetState(PLAYERSTATE_DAMAGE);

							D3DXVECTOR2 fSize;

							for (int nCntParticle = 0; nCntParticle < COL_PARTICLE; nCntParticle++)
							{
								fSize.x = 5.0f + (float)(CServer::Rand() % 5);
								fSize.y = 5.0f + (float)(CServer::Rand() % 5);

								CParticle::Create(m_pos,
									D3DXVECTOR3(sinf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 5 + 1)), cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 5 + 1)), cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 5 + 1))),
									D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
									fSize,
									20,
									CParticle::TEXTURE_STAR,
									CParticle::TYPE_NORMAL,
									m_nPlayerNum);
							}
						}
						pEgg->Uninit();	// 卵削除
						break;

						// 減速
					case CEgg::EGGTYPE_ANNOY:
						if (m_bDamage == false && m_State != PLAYERSTATE_SPEEDUP_S)
						{
							CancelMotion(PLAYERANIM_DAMAGE, false);

							m_bDamage = true;
							m_nCntDamage = 0;
							SetState(PLAYERSTATE_SPEEDDOWN);
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

//=============================================================================
// ひよことの当たり判定
//=============================================================================
void CPlayer::CollisionChick(void)
{
	CSound *pSound = CManager::GetSound();
	CScene *pScene;

	// プライオリティーチェック
	pScene = CScene::GetTop(EGG_PRIOTITY);

	while (pScene != NULL)
	{// プライオリティー内のリスト構造を最後まで見る
		CScene *pSceneNext = pScene->GetNext();		// 次のオブジェクトを保存

		if (pScene->GetObjType() == OBJTYPE_CHICK)
		{
			CChick *pChick = (CChick*)pScene;	// オブジェクトクラスのポインタ変数にする

			if (pChick->GetNumPlayer() != m_nPlayerNum && pChick->GetType() != CChick::TYPE_ANNOY_S && m_State != PLAYERSTATE_SPEEDUP_S)
			{
				if (pChick->CollisionChick(&m_pos, &m_OldPos) == true)
				{// 衝突した
					switch (pChick->GetType())
					{
						// 攻撃
					case CChick::TYPE_ATTACK:
						// ダメージ状態にする
						if (m_State != PLAYERSTATE_DAMAGE)
						{
							CCylinder::Create(m_pos, m_rot, CCylinder::TYPE_HATK);
							CancelMotion(PLAYERANIM_DAMAGE, false);
							m_bDamage = true;
							m_nCntDamage = 0;
							SetState(PLAYERSTATE_DAMAGE);

							D3DXVECTOR2 fSize;

							for (int nCntParticle = 0; nCntParticle < COL_PARTICLE; nCntParticle++)
							{
								fSize.x = 5.0f + (float)(CServer::Rand() % 5);
								fSize.y = 5.0f + (float)(CServer::Rand() % 5);

								CParticle::Create(m_pos,
									D3DXVECTOR3(sinf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 5 + 1)), cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 5 + 1)), cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 5 + 1))),
									D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
									fSize,
									20,
									CParticle::TEXTURE_STAR,
									CParticle::TYPE_NORMAL,
									m_nPlayerNum);
							}
						}
						pChick->Uninit();	// ひよこ削除
						break;

						// 強い攻撃
					case CChick::TYPE_ATTACK_S:
						// ダメージ状態にする
						if (m_State != PLAYERSTATE_DAMAGE && pChick->GetAttackS() == true && pChick->GetAttackCol() == true)
						{
							CCylinder::Create(m_pos, m_rot, CCylinder::TYPE_HATK);
							CancelMotion(PLAYERANIM_DAMAGE, false);
							m_bDamage = true;
							m_nCntDamage = 0;
							SetState(PLAYERSTATE_DAMAGE);

							D3DXVECTOR2 fSize;

							for (int nCntParticle = 0; nCntParticle < COL_PARTICLE_S; nCntParticle++)
							{
								fSize.x = 5.0f + (float)(CServer::Rand() % 5);
								fSize.y = 5.0f + (float)(CServer::Rand() % 5);

								CParticle::Create(m_pos,
									D3DXVECTOR3(sinf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 5 + 1)), cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 5 + 1)), cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 5 + 1))),
									D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
									fSize,
									20,
									CParticle::TEXTURE_STAR,
									CParticle::TYPE_NORMAL,
									m_nPlayerNum);
							}
						}
						break;

						// 減速
					case CChick::TYPE_ANNOY:
						if (m_bDamage == false)
						{
							CancelMotion(PLAYERANIM_DAMAGE, false);
							m_bDamage = true;
							m_nCntDamage = 0;
							SetState(PLAYERSTATE_SPEEDDOWN);
						}
						break;
					}
				}
			}
		}
		// Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

//=============================================================================
// ひよこの出現処理
//=============================================================================
void CPlayer::ChickAppear(void)
{
	if (m_pEgg[0] != NULL)
	{
		if (m_pEgg[0]->GetHatchingTimer() > HatchTime(HATCH_TIME, 400.0f))
		{// 孵化する時間が経過
		 // タイマーを0にもどす
			m_pEgg[0]->SetHatchingTimer(0);

			int nRank = CServer::Rand() % 101;
			int nGameTime = 0;
			int nRanking = 0;

			switch (CManager::GetMode())
			{
			case CManager::MODE_TITLE:
				nRanking = CTitle::GetRanking(m_nPlayerNum);
				break;
			case CManager::MODE_GAME:
				nGameTime = (CGame::GetGameCounter() - START_SET_TIME) / 60;
				nRanking = CGame::GetRanking(m_nPlayerNum);
				break;
			}

			CChick::TYPE type = CChick::TYPE_MAX;

			if (nGameTime < STRONG_TIME)
			{
				// タイプ設定
				type = SetChickType(type, false);
			}
			else if (nGameTime >= STRONG_TIME)
			{
				if (nRanking < CHICK_BORDER - 1)
				{// 4位より上の場合
				 // タイプ設定
					type = SetChickType(type, false);
				}
				else if (nRanking >= CHICK_BORDER - 1)
				{// 4位より下の場合
					for (int nCntRank = 0; nCntRank < (MAX_MEMBER / 2) + 1; nCntRank++)
					{
						if (nRanking == nCntRank + CHICK_BORDER - 1)
						{
							if (m_bSChick == false)
							{
								if (nRank <= 10 * (6 + nCntRank))
								{// 強いほう
								 // タイプ設定
									type = SetChickType(type, true);
									m_bSChick = true;
								}
								else if (nRank > 10 * (6 + nCntRank))
								{// 普通のほう
								 // タイプ設定
									type = SetChickType(type, false);
								}
							}
							else
							{
								// タイプ設定
								type = SetChickType(type, false);
							}

							break;
						}
					}
				}
			}

			// 種類設定
			m_bulletType[m_nNumChick] = (BULLET)(BULLET_CHICK_ATTACK + (type));

			if (type != CChick::TYPE_MAX)
			{
				m_pChick[m_nNumChick] = CChick::Create(m_pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					CHICK_SCALE,
					type,
					CChick::BULLETTYPE_PLAYER,
					CChick::STATE_CHASE,
					m_nPlayerNum);

				//m_pChick[m_nNumChick] = CChick::Create(m_pos,
				//	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	CHICK_SCALE,
				//	CChick::TYPE_ANNOY,
				//	CChick::TYPE_ANNOY_S,
				//	CChick::BULLETTYPE_PLAYER,
				//	CChick::STATE_CHASE,
				//	m_nPlayerNum);
				m_pChick[m_nNumChick]->NewEfc();
			}

			m_pEgg[0]->Uninit();
			m_pEgg[0] = m_pEgg[1];
			m_pEgg[1] = m_pEgg[2];
			m_pEgg[2] = NULL;
			m_nNumEgg--;

			m_nNumChick++;
		}
	}
}

//=============================================================================
// ひよこの成長速度設定
//=============================================================================
float CPlayer::HatchTime(float fTime, float fAddTime)
{
	float fHatchTime = fTime;

	int nRanking = 0;
	switch (CManager::GetMode())
	{
	case CManager::MODE_TITLE:
		nRanking = CTitle::GetRanking(m_nPlayerNum);
		break;
	case CManager::MODE_GAME:
		nRanking = CGame::GetRanking(m_nPlayerNum);
		break;
	}

	if (nRanking >= 3)
	{
		fHatchTime = fTime * (1.0f / (nRanking + 1)) + fAddTime;
	}

	return fHatchTime;
}

//=============================================================================
// ひよこの種類設定
//=============================================================================
CChick::TYPE CPlayer::SetChickType(CChick::TYPE type, bool bStrong)
{
	if (m_pEgg[0]->GetType() == CEgg::EGGTYPE_ATTACK)
	{// 攻撃卵
		if (bStrong == false)
		{// 普通
			type = CChick::TYPE_ATTACK;
		}
		else
		{// 強い
			type = CChick::TYPE_ATTACK_S;
		}
	}
	else if (m_pEgg[0]->GetType() == CEgg::EGGTYPE_ANNOY)
	{// 妨害卵
		if (bStrong == false)
		{// 普通
			type = CChick::TYPE_ANNOY;
		}
		else
		{// 強い
			type = CChick::TYPE_ANNOY_S;
		}
	}
	else if (m_pEgg[0]->GetType() == CEgg::EGGTYPE_SPEED)
	{// 加速卵
		if (bStrong == false)
		{// 普通
			type = CChick::TYPE_SPEED;
		}
		else
		{// 強い
			type = CChick::TYPE_SPEED_S;
		}
	}

	return type;
}

//=============================================================================
// 降らすひよこの出現処理
//=============================================================================
void CPlayer::FallChicks(D3DXVECTOR3 pos)
{
	CScene *pScene;

	for (int nCntChick = 0; nCntChick < CHICK_FALL_NUM; nCntChick++)
	{
		int fx = CServer::Rand() % FALL_CHICK_RANGE;
		int fz = CServer::Rand() % FALL_CHICK_RANGE;

		// ひよこ出現
		CChick::Create(D3DXVECTOR3(pos.x + ((FALL_CHICK_RANGE / 2) - fx), pos.y + (nCntChick * 100.0f), pos.z + ((FALL_CHICK_RANGE / 2) - fz)),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			CHICK_SCALE,
			CChick::TYPE_ATTACK_S,
			CChick::BULLETTYPE_PLAYER,
			CChick::STATE_BULLET,
			m_nPlayerNum);

		m_nCntChick++;
	}

	// プライオリティーチェック
	pScene = CScene::GetTop(EGG_PRIOTITY);
	while (pScene != NULL)
	{// プライオリティー内のリスト構造を最後まで見る
		CScene *pSceneNext = pScene->GetNext();		// 次のオブジェクトを保存

		if (pScene->GetObjType() == OBJTYPE_CHICK)
		{// タイプが障害物だったら
			CChick *pChick = (CChick*)pScene;	// オブジェクトクラスのポインタ変数にする

			if (pChick->GetState() == CChick::STATE_BULLET && pChick->GetType() == CChick::TYPE_ATTACK_S && pChick->GetAttackS() == false && pChick->GetDis() == true)
			{
				pChick->SetAttackS(true);
				pChick->SetDis(false);

				int nRanking = 0;
				switch (CManager::GetMode())
				{
				case CManager::MODE_TITLE:
					nRanking = CTitle::GetRanking(m_nPlayerNum);
					break;
				case CManager::MODE_GAME:
					nRanking = CGame::GetRanking(m_nPlayerNum);
					break;
				}
				pChick->SetRank(nRanking);
				pChick->SetDestRank(m_nDestRank);
			}
		}
		// Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

//=============================================================================
// 強い減速ひよこの出現処理
//=============================================================================
void CPlayer::AnnoyChicks(void)
{
	CPlayer **pPlayer = NULL;
	int nRanking = 0;
	switch (CManager::GetMode())
	{
	case CManager::MODE_TITLE:
		pPlayer = CTitle::GetPlayer();
		nRanking = CTitle::GetRanking(m_nPlayerNum);
		break;
	case CManager::MODE_GAME:
		pPlayer = CGame::GetPlayer();
		nRanking = CGame::GetRanking(m_nPlayerNum);
		break;
	}
	for (int nCntPlayer = 0; nCntPlayer < MAX_MEMBER; nCntPlayer++)
	{
		if (pPlayer[nCntPlayer] != NULL)
		{
			if (nCntPlayer != m_nPlayerNum)
			{
				if (m_pAnnoyChick[nCntPlayer] == NULL)
				{
					m_pAnnoyChick[nCntPlayer] = CChick::Create(
						D3DXVECTOR3(pPlayer[nCntPlayer]->GetPos().x, pPlayer[nCntPlayer]->GetPos().y + ANNOY_TO_PLAYER, pPlayer[nCntPlayer]->GetPos().z),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						CHICK_SCALE,
						CChick::TYPE_ANNOY_S,
						CChick::BULLETTYPE_PLAYER,
						CChick::STATE_BULLET,
						m_nPlayerNum);

					if (m_pAnnoyChick[nCntPlayer]->GetState() == CChick::STATE_BULLET
						&& m_pAnnoyChick[nCntPlayer]->GetType() == CChick::TYPE_ANNOY_S
						&& m_pAnnoyChick[nCntPlayer]->GetAttackS() == false
						&& m_pAnnoyChick[nCntPlayer]->GetDis() == true)
					{
						m_pAnnoyChick[nCntPlayer]->SetAttackS(true);
						m_pAnnoyChick[nCntPlayer]->SetRank(nRanking);
					}

					D3DXVECTOR2 fSize;

					for (int nCntParticle = 0; nCntParticle < MAX_SMOKE; nCntParticle++)
					{
						fSize.x = SMOKE_SIZE + (float)(CServer::Rand() % 3);
						fSize.y = SMOKE_SIZE + (float)(CServer::Rand() % 3);

						CParticle::Create(D3DXVECTOR3((sinf(m_rot.y + D3DX_PI) * -30.0f) + pPlayer[nCntPlayer]->GetPos().x,
							pPlayer[nCntPlayer]->GetPos().y + ANNOY_TO_PLAYER,
							(cosf(m_rot.y + D3DX_PI) * -30.0f) + pPlayer[nCntPlayer]->GetPos().z),
							D3DXVECTOR3(sinf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 3 + 1)), cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 1 + 1)), cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 3 + 1))),
							D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
							fSize,
							30,
							CParticle::TEXTURE_SMOKE,
							CParticle::TYPE_TURN,
							m_nPlayerNum);
					}

					pPlayer[nCntPlayer]->SetState(PLAYERSTATE_SPEEDDOWN_S);

					m_bAnnoyS = true;
				}
			}
		}
	}
}

//=============================================================================
// 強い減速ひよこがくっつく処理
//=============================================================================
void CPlayer::ChaseAnnoyS(void)
{
	if (m_pAnnoyChick[m_nPlayerNum] != NULL)
	{
		// 位置更新
		m_pAnnoyChick[m_nPlayerNum]->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + ANNOY_TO_PLAYER, m_pos.z));	// 少し上に位置更新
		m_pAnnoyChick[m_nPlayerNum]->SetRot(D3DXVECTOR3(m_rot.x, m_rot.y + -D3DX_PI, m_rot.z));		// 向きを画面に向ける

																									// 食らっている時間をカウント
		m_nAnnoySTimer++;

		if (m_nAnnoySTimer > HatchTime(SPEEDDOWN_TIME, 120.0f))
		{// 一定時間たったら
			m_nAnnoySTimer = 0;
			m_pAnnoyChick[m_nPlayerNum]->Uninit();
			m_pAnnoyChick[m_nPlayerNum] = NULL;
		}

		if (m_bDamage == false)
		{
			m_bDamage = true;
			m_nCntDamage = 0;
			SetState(PLAYERSTATE_SPEEDDOWN_S);

		}
	}
}

//=============================================================================
// 卵をジャンプさせる
//=============================================================================
void CPlayer::EggJump(void)
{
	if (m_bControl == true)
	{
		if (m_bJumpFlag == false)
		{
			if (m_bJump == true)
			{
				m_bJumpFlag = true;
				m_bJumpSave = true;
			}
			else
			{
				m_bJumpSave = false;
			}
		}
		else
		{
			m_bJumpSave = false;

			if (m_bJump == false)
			{
				m_bJumpFlag = false;
			}
		}
	}
}

//=============================================================================
// キャラ同士の衝突判定処理
//=============================================================================
void CPlayer::CollisionCharacter(void)
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

	if (pPlayer != NULL)
	{
		float fDis = 999999.9f;
		for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
		{// プレイヤーカウント
			if (pPlayer[nCntMember] != NULL)
			{// NULL以外
				if (pPlayer[nCntMember] != this)
				{// 自分以外
				 // 距離計算
					D3DXVECTOR3 pos = pPlayer[nCntMember]->GetPos();
					float fLenght = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));

					if (fDis > fLenght)
					{//より近いなら
						fDis = fLenght;
						m_pNear = pPlayer[nCntMember];
					}
					if (fLenght < PLAYER_LENGTH * 2.0f)
					{// 範囲内
					 // 角度計算
						float fAngle = atan2f(m_pos.x - pos.x, m_pos.z - pos.z);

						// 位置調整
						m_pos = D3DXVECTOR3(pos.x + sinf(fAngle) * PLAYER_LENGTH * 2.0f, m_pos.y, pos.z + cosf(fAngle) * PLAYER_LENGTH * 2.0f);

						//弾く
						Strike(pPlayer[nCntMember], pPlayer[nCntMember]->m_pos, pPlayer[nCntMember]->m_move);
					}
				}
			}
		}
	}
}

//=============================================================================
// 弾く処理
//=============================================================================
void CPlayer::Strike(CPlayer *pPlayer, D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	D3DXVECTOR3 Strike[2], Power[2];
	float fTargetRot[2], fMoveRot[2], fPower[2];

	//情報の作成
	Strike[0] = m_move;	Strike[1] = m_move;
	fTargetRot[0] = atan2f(m_pos.x - pos.x, m_pos.z - pos.z);
	fTargetRot[1] = fTargetRot[0] + D3DX_PI;
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		fMoveRot[nCnt] = atan2f(Strike[nCnt].x, Strike[nCnt].z);
		fPower[nCnt] = sqrtf(powf(Strike[nCnt].x, 2) + powf(Strike[nCnt].x, 2));
		//衝撃を保存
		Power[nCnt] = D3DXVECTOR3(sinf(fTargetRot[nCnt]), 0.0f, cosf(fTargetRot[nCnt])) * (sinf(fMoveRot[nCnt]) * Strike[nCnt].x);
		Power[nCnt] += D3DXVECTOR3(sinf(fTargetRot[nCnt]), 0.0f, cosf(fTargetRot[nCnt])) * (cosf(fMoveRot[nCnt]) * Strike[nCnt].z);
		Power[nCnt] *= PLAYER_STRIKE;
	}

	//衝撃の反映
	move -= Power[0];
	m_move -= Power[1];

	//自身に跳ね返す
	move += Power[1];
	m_move += Power[0];

	//跳ね返った衝撃に応じてカウント加算
	Tackle(ColMove(fTargetRot[0], fMoveRot[0], fPower[0], fPower[1]));

	if (pPlayer != NULL)
	{//プレイヤーなら
		pPlayer->m_move = move;
		pPlayer->Tackle(ColMove(fTargetRot[1], fMoveRot[1], fPower[0], fPower[1]));
	}
}
//=============================================================================
// 移動方向と衝撃方向から移動量を計算
//=============================================================================
float CPlayer::ColMove(float &fTargetRot, float &fMoveRot, float fPow0, float fPow1)
{
	float fDown;
	fTargetRot -= fMoveRot;
	fDown = (cosf(fTargetRot) * fPow0) / PLAYER_STRPLUS;
	fDown -= (cosf(fTargetRot + D3DX_PI) * fPow1) / PLAYER_STRPLUS;
	return fDown;
}
//=============================================================================
// タックル時の移動量加算
//=============================================================================
void CPlayer::Tackle(float fValue)
{
	m_fCntTackle = 60.0f;
	if (fValue >= 0.0f)
	{
		if (m_fPower < fValue) { m_fPower = fValue * (PLAYER_POWMAX / 100.0f); }
		m_PlayerInfo.fCountTime += fValue * (10.0f / 100.0f);
		if (m_fPower > PLAYER_POWMAX) { m_fPower = PLAYER_POWMAX; }
		if (m_PlayerInfo.fCountTime > 10.0f) { m_PlayerInfo.fCountTime = 10.0f; }
		if (fValue > 10.0f) { m_fTackle = fValue * 0.5f; }
	}
	else
	{
		//fValue *= 2.0f;
		m_fPower = 0.0f;
		if (fValue < -PLAYER_STRDOWN) { fValue = -PLAYER_STRDOWN; }

		m_PlayerInfo.fCountTime += fValue * (10.0f / 100.0f);
		if (m_PlayerInfo.fCountTime < 0.0f) { m_PlayerInfo.fCountTime = 0.0f; }
	}
}
//=============================================================================
// 風受け設定
//=============================================================================
void CPlayer::SetWind(float fRot)
{
	if (m_fCntWind > WIND_TIME - 10.0f) { return; }
	m_rot.y += (fRot - m_rot.y) * 1.0f;
	m_fCntWind = WIND_TIME;
	m_WindMove += D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * WIND_POW;
	EndBoost();
	UseBoost();
}
//=============================================================================
// CPUのコース取り変更
//=============================================================================
void CPlayer::UpVecUZ(void)
{
	bool		bVec = false;
	float &fRotY = m_rot.y;
	float fVecWK = fRotY - m_fRotOld;
	float fVecU = 0.0f;

	RemakeAngle(&fVecWK);
	if (fVecWK < -0.01f) { m_rot.z -= fVecWK * 0.5f;  bVec = true; if (m_rot.z > 0.5f) { m_rot.z = 0.5f; } }
	if (fVecWK > 0.01f) { m_rot.z -= fVecWK * 0.5f; bVec = true; if (m_rot.z < -0.5f) { m_rot.z = -0.5f; } }
	if (!bVec)
	{
		m_rot.z *= 0.985f;
	}
	m_rot.x = sqrtf(powf(m_rot.z, 2)) * 0.3f + (m_PlayerInfo.fCountTime / 360.0f) * D3DX_PI * 0.25f;
}
//=============================================================================
// スティック値代入
//=============================================================================
void CPlayer::SetStick(CInputJoyPad_0 *&pPad, int nNum)
{
	if (pPad != NULL)
	{
		if (pPad->GetStickDefeat(nNum))
		{//32767, -32768
			float fValue = (float)pPad->GetnStickX(nNum);
			if (fValue < 0.0f) { fValue = -fValue - 1.0f; }
			m_fStick = fValue / 32767.0f;
		}
		else { m_fStick = 1.0f; }

	}
}
//=============================================================================
// CPUのコース取り変更
//=============================================================================
void CPlayer::ChangeRoad(void)
{
	m_FeedType = (CFeed::FEEDTYPE)(CServer::Rand() % CFeed::FEEDTYPE_MAX);
	int nRank = CRoad_Manager::GetManager()->GetRank(m_nPlayerNum);
	if (nRank < 3) { m_nNumRoad = 1; }
	else
	{
		if (nRank < 6) { m_nNumRoad = CServer::CServer::Rand() % 2; }
		else { m_nNumRoad = 0; }
	}

	float fWKRoad = m_fRoad;
	float fPlus = (float)(CServer::CServer::Rand() % 4) * 7.5f;
	bool bPlus;
	if (m_nMap == 0) { bPlus = (m_nNumRoad == 0 ? false : true); }
	else { bPlus = (m_nNumRoad == 0 ? true : false); }

	if (bPlus)
	{
		if (m_fRoad + fPlus <= PLAYER_MAX_ROAD) { m_fRoad += fPlus; }
		else { m_fRoad -= fPlus; }
	}
	else
	{
		if (m_fRoad - fPlus >= -PLAYER_MAX_ROAD) { m_fRoad -= fPlus; }
		else { m_fRoad += fPlus; }
	}
}
//=============================================================================
// プレイヤーのモーション
//=============================================================================
void CPlayer::UpdateMotion(void)
{
	bool bSet = false;

	float fSpd = 1.0f;
	if (m_PlayerAnim == PLAYERANIM_RUN)
	{
		float fTime = m_PlayerInfo.fCountTime;
		if (fTime < 0.0f) { fTime *= -1.0f; }
		fSpd = 0.4f + (fTime / 12.0f);
	}
	m_fCountFlame += fSpd;	//F++

	while (m_fCountFlame > m_pKey->nFrame)
	{//キーの終了判定
		bSet = true;
		m_fCountFlame -= m_pKey->nFrame;
		if (m_aMotionInfo[m_PlayerAnim].nNumKey - 1 <= m_nKey)
		{//モーションの終了
			if (!m_aMotionInfo[m_PlayerAnim].bLoop) { m_PlayerAnim = PLAYERANIM_NEUTRAL; }
			m_nKey = 0;
			m_fCountFlame = 0.0f;
			m_pKey = &m_pKeyInfo[m_PlayerAnim][m_nKey];
		}
		else
		{//キーを進める
			m_nKey += 1;
			m_pKey = &m_pKeyInfo[m_PlayerAnim][m_nKey];

		}
	}
	if (bSet) { SettingParts(); EffectUp(); }

	UpMParts();	//パーツ更新
}
//=============================================================================
// モーション　パーツの更新
//=============================================================================
void CPlayer::UpMParts(void)
{
	if (m_apModel == NULL) { return; }

	float fRateMotion;
	D3DXVECTOR3 WKVec3;

	//現在のキーから次のキーへの再生フレーム数におけるモーションカウンターの相対値を算出
	fRateMotion = (float)m_fCountFlame / (float)m_pKey->nFrame;

	//モーション更新
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_apModel[nCntParts] != NULL)
		{
			WKVec3 = D3DXVECTOR3(
				m_pKey->aKey[nCntParts].fposX + m_aKayOffset[nCntParts].fposX,
				m_pKey->aKey[nCntParts].fposY + m_aKayOffset[nCntParts].fposY,
				m_pKey->aKey[nCntParts].fposZ + m_aKayOffset[nCntParts].fposZ);
			m_apModel[nCntParts]->SetPos(m_PartsPos[nCntParts] + (WKVec3 - m_PartsPos[nCntParts]) * fRateMotion);

			WKVec3 = D3DXVECTOR3(
				m_pKey->aKey[nCntParts].frotX + m_aKayOffset[nCntParts].frotX,
				m_pKey->aKey[nCntParts].frotY + m_aKayOffset[nCntParts].frotY,
				m_pKey->aKey[nCntParts].frotZ + m_aKayOffset[nCntParts].frotZ);
			RemakeAngle(&WKVec3.x);	RemakeAngle(&WKVec3.y);	RemakeAngle(&WKVec3.z);
			WKVec3 = m_PartsRot[nCntParts] + (WKVec3 - m_PartsRot[nCntParts]) * fRateMotion;
			RemakeAngle(&WKVec3.x);	RemakeAngle(&WKVec3.y);	RemakeAngle(&WKVec3.z);
			m_apModel[nCntParts]->SetRot(WKVec3);

		}
	}
}
//=============================================================================
// モーション　パーツの設定
//=============================================================================
void CPlayer::SettingParts(void)
{
	if (m_apModel == NULL) { return; }
	//モーション更新
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_apModel[nCntParts] != NULL)
		{
			m_PartsPos[nCntParts] = m_apModel[nCntParts]->GetPos();
			m_PartsRot[nCntParts] = m_apModel[nCntParts]->GetRot();
		}
	}
}
//=============================================================================
// モーション　パーツのオフセット設定
//=============================================================================
void CPlayer::OfSetParts(void)
{
	D3DXVECTOR3 WKVec3;

	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_apModel[nCntParts] != NULL)
		{
			WKVec3 = D3DXVECTOR3(
				m_aKayOffset[nCntParts].fposX,
				m_aKayOffset[nCntParts].fposY,
				m_aKayOffset[nCntParts].fposZ);
			m_apModel[nCntParts]->SetPos(WKVec3);

			WKVec3 = D3DXVECTOR3(
				m_aKayOffset[nCntParts].frotX,
				m_aKayOffset[nCntParts].frotY,
				m_aKayOffset[nCntParts].frotZ);
			RemakeAngle(&WKVec3.x);	RemakeAngle(&WKVec3.y);	RemakeAngle(&WKVec3.z);
			m_apModel[nCntParts]->SetRot(WKVec3);

		}
	}
	SettingParts();
}
//=============================================================================
// モーションの初期化
//=============================================================================
void CPlayer::ResetMotion(void)
{
	m_PlayerAnim = PLAYERANIM_NEUTRAL;
	m_pKey = &m_pKeyInfo[m_PlayerAnim][0];
	m_fCountFlame = 0.0f;
	m_nKey = 0;
	OfSetParts();
}
//=============================================================================
// モーションのキャンセル
//=============================================================================
void CPlayer::CancelMotion(PlayerAnim Anim, bool bRow)
{
	if (!bRow && m_PlayerAnim == Anim) { return; }
	if (m_PlayerAnim != PLAYERANIM_NEUTRAL && Anim == PLAYERANIM_RUN) { return; }
	m_PlayerAnim = Anim;
	m_pKey = &m_pKeyInfo[m_PlayerAnim][0];
	m_nKey = 0;
	m_fCountFlame = 0.0f;
	SettingParts();
}
//=============================================================================
// ファイル読み込み
//=============================================================================
void CPlayer::FileLoad(void)
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
	pFile = fopen(FILE_NAME_PRISONER, "r");
	//NULLチェック
	if (pFile != NULL)
	{
		for (int nCntMotion = 0; nCntMotion < MAX_MOTION; )
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
				g_nNumModel = atoi(pStrcur);

				for (int nCntModel = 0; nCntModel < g_nNumModel; nCntModel++)
				{
					int nNumber = nCntModel + CModel::PARTS_CHICKEN_BODY;
					LPD3DXBUFFER &m_pBuffMat = CModel::GetpBuffMat(nNumber);
					DWORD &m_nNumMat = CModel::GetnNumMat(nNumber);
					LPD3DXMESH &m_pMesh = CModel::GetpMesh(nNumber);

					//文字列の先頭を設定
					pStrcur = ReadLine(pFile, &aLine[0]);
					//文字列を取り戻す
					strcpy(aStr, pStrcur);
					if (memcmp(pStrcur, "MODEL_FILENAME = ", strlen("MODEL_FILENAME = ")) == 0)
					{
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
						strcpy(&g_aFileNameModel[nCntModel][0], aStr);

						// Xファイルの読み込み
						D3DXLoadMeshFromX(&g_aFileNameModel[nCntModel][0],
							D3DXMESH_SYSTEMMEM,
							pDevice,
							NULL,
							&m_pBuffMat,
							NULL,
							&m_nNumMat,
							&m_pMesh);
						int nNumber0 = nCntModel + CModel::PARTS_CHICKEN_BODY;

						//CModel::SetModelType(nCntModel);
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
char *CPlayer::ReadLine(FILE *pFile, char *pDst)
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
char * CPlayer::GetLineTop(char * pStr)
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
int CPlayer::PopString(char * pStr, char * pDest)
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