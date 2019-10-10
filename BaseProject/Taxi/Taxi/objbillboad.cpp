//=============================================================================
//
// オブジェクトビルボード処理 [objbillboad.cpp]
// Author : 佐藤 安純
//
//=============================================================================
#include "objbillboad.h"
#include "manager.h"
#include "renderer.h"
#include "gamecamera.h"
#include "tutorial.h"
#include "player.h"
#include "object.h"

//==================================
// マクロ定義
//==================================
#define PRIORITY	(3)
#define VECTOR_ZERO	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))

//テクスチャ名
#define TEX_NAME00 ("data\\TEXTURE\\game\\ObjBillboad\\AroundKone.png")
#define TEX_NAME01 ("data\\TEXTURE\\game\\ObjBillboad\\wood.png")

//サイズ
#define KONE_SIZE (D3DXVECTOR2(25.0f, 25.0f))
#define WOOD_SIZE (D3DXVECTOR2(150.0f, 350.0f))

//描画範囲
#define KONE_LENGTH (6000.0f)	//コーンの描画範囲
#define WOOD_LENGTH (20000.0f)	//木の描画範囲

#define WOOD_MODEL_LENGTH (8000.0f)	//木のモデルに変えるタイミング

//==================================
// 静的メンバ変数宣言
//==================================
LPDIRECT3DTEXTURE9 CObjBillboad::m_pTexture[OBJBILL_TEX] = {};

//=============================================================================
// 生成処理
//=============================================================================
CObjBillboad * CObjBillboad::Create(D3DXVECTOR3 pos, int nObjType)
{
	CObjBillboad * pObjBillboad = new CObjBillboad;
	pObjBillboad->SetPos(pos);
	pObjBillboad->SetTexNum(nObjType);
	pObjBillboad->Init();

	return pObjBillboad;
}

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CObjBillboad::CObjBillboad(): CScene(PRIORITY, OBJTYPE_BILLBOORD) 
{
	m_pos = VECTOR_ZERO;
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_nTexNum = -1;
	m_fDrawLength = 0.0f;
	m_fModelLength = 0.0f;
	m_bSetModel = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CObjBillboad::~CObjBillboad(){}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CObjBillboad::LoadTexture()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateTextureFromFile(pDevice, TEX_NAME00, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, TEX_NAME01, &m_pTexture[1]);
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CObjBillboad::UnloadTexture()
{
	for (int nCntTex = 0; nCntTex < OBJBILL_TEX; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObjBillboad::Init()
{
	//変数の初期化
	m_nCollision = 0;
	m_nModelNumTex = 0;
	m_ModelSize = VECTOR_ZERO;

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f);

	//法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファのアンロックする
	m_pVtxBuff->Unlock();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CObjBillboad::Uninit()
{
	if (m_pVtxBuff != NULL)
	{// 頂点情報の破棄
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//自身の破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CObjBillboad::Update()
{
	//クリッピング処理の追加
	CManager::MODE mode = CManager::GetMode();
	CGameCamera * pGameCamera = NULL;
	D3DXVECTOR3 PlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (mode == CManager::MODE_GAME)
	{//ゲーム
		pGameCamera = CGame::GetGameCamera();
		PlayerPos = CGame::GetPlayer()->GetPos();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//チュートリアル
		pGameCamera = CTutorial::GetCamera();
		PlayerPos = CTutorial::GetPlayer()->GetPos();
	}

	//プレイヤーとの距離を求める
	D3DXVECTOR3 Distance = m_pos - PlayerPos;
	float fLength = sqrtf((Distance.x * Distance.x) + (Distance.z * Distance.z));

	//一定距離以上だったら
	if (fLength > m_fDrawLength)
	{
		SetDraw(false);
		return;
	}
	else
	{
		SetDraw(true);
	}

	//モデルに切り替えないならリターン
	if (!m_bSetModel) { return; }

	//モデルの表示範囲判定
	if (fLength < m_fModelLength)
	{
		////オブジェクトの生成
		//CObject::Create(m_ModelPos, VECTOR_ZERO, m_ModelSize, 0.0f, m_nModelNumTex, m_nObjType,
		//				CModel3D::MOVETYPE_NOT, m_nCollision);

		////ビルボードを破棄
		//Uninit();

		SetDraw(false);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CObjBillboad::Draw()
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxView;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//逆行列を設定
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_pVtxBuff != NULL)
	{
		//頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture[m_nTexNum]);

		// ライティングを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// ライティングを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//=============================================================================
// テクスチャの番号設定
//=============================================================================
void CObjBillboad::SetTexNum(int nObjType)
{
	m_nObjType = nObjType;

	if (m_nObjType == TYPE_KONE)
	{//コーン
		m_size = KONE_SIZE;
		m_nTexNum = TYPE_KONE;
		m_fDrawLength = KONE_LENGTH;
	}
	else if (m_nObjType == TYPE_TREE02)
	{//木
		m_size = WOOD_SIZE;
		m_nTexNum = TYPE_WOOD;
		m_fDrawLength = WOOD_LENGTH;
		m_fModelLength = WOOD_MODEL_LENGTH;
		m_ModelPos = m_pos;
		m_pos = D3DXVECTOR3(m_pos.x, m_pos.y + 150.0f, m_pos.z);
		m_bSetModel = true;
	}
}

//=============================================================================
// モデルの情報設定
//=============================================================================
void CObjBillboad::SetModelInfo(D3DXVECTOR3 Scale, int ModelTex, int nCollision)
{
	m_ModelSize = Scale;		//モデルの大きさ
	m_nModelNumTex = ModelTex;	//テクスチャ情報
	m_nCollision = nCollision;	//当たり判定
}