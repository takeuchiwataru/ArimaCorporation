//=============================================================================
//
// オブジェクトの処理 [model3D.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "scene.h"
#include "model3D.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include <math.h>
#include "billboord.h"
#include "gamecamera.h"
#include "player.h"
#include "resultcamera.h"
#include "tutorial.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LENGTH_BLOW		(1000.0f)		//吹っ飛び判定を有効にする距離
#define LENGTH_SMALLOBJ	(6000.0f)		//小さいオブジェクトを描画する範囲
#define LENGTH_HIGHTOBJ	(8000.0f)		//高いオブジェクトを描画する範囲
#define STOP_MOVE		(0.05f)			//移動を停止する範囲
#define DELETE_TIME		(60 * 5)		//消す時間

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//===============================================================================
//　オーバーロードコンストラクタ
//===============================================================================
CModel3D::CModel3D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pMeshObject = NULL;					//メッシュ情報へのポインタ
	m_pBuffMatObject = NULL;				//マテリアルの情報へのポインタ
	m_nNumMatObject = 0;					//マテリアルの情報数
	m_mtxWorldObject;						//ワールドマトリックス
	m_VtxMinModel, m_VtxMaxModel;			//モデルの最小値・最大値
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//動き
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);//大きさ
	m_VecAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fValueRot = 0.0f;
	m_fMass = 2.0f;
	m_fRotY = 0.0f;
	m_blowOff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bcolChange = false;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// カラー
	m_bRoll = true;
	m_bStateRoll = false;
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	for (int nCntCorner = 0; nCntCorner < VTX_CORNER_MAX; nCntCorner++)
	{// 当たり判定用ボックスの角８つの頂点座標
		m_aVtxCornerPos[nCntCorner] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}
//===============================================================================
//　デストラクタ
//===============================================================================
CModel3D::~CModel3D() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//位置
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//向き達する

	m_VtxMaxModel = D3DXVECTOR3(m_VtxMaxModel.x * m_Scale.x, m_VtxMaxModel.y * m_Scale.y, m_VtxMaxModel.z * m_Scale.z);
	m_VtxMinModel = D3DXVECTOR3(m_VtxMinModel.x * m_Scale.x, m_VtxMinModel.y * m_Scale.y, m_VtxMinModel.z * m_Scale.z);

	// ボックスの角８つの頂点座標
	m_aVtxCornerPos[VTX_CORNER_LDF] = D3DXVECTOR3(m_VtxMinModel.x, m_VtxMinModel.y, m_VtxMinModel.z);
	m_aVtxCornerPos[VTX_CORNER_LUF] = D3DXVECTOR3(m_VtxMinModel.x, m_VtxMaxModel.y, m_VtxMinModel.z);
	m_aVtxCornerPos[VTX_CORNER_RDF] = D3DXVECTOR3(m_VtxMaxModel.x, m_VtxMinModel.y, m_VtxMinModel.z);
	m_aVtxCornerPos[VTX_CORNER_RUF] = D3DXVECTOR3(m_VtxMaxModel.x, m_VtxMaxModel.y, m_VtxMinModel.z);
	m_aVtxCornerPos[VTX_CORNER_RDB] = D3DXVECTOR3(m_VtxMaxModel.x, m_VtxMinModel.y, m_VtxMaxModel.z);
	m_aVtxCornerPos[VTX_CORNER_RUB] = D3DXVECTOR3(m_VtxMaxModel.x, m_VtxMaxModel.y, m_VtxMaxModel.z);
	m_aVtxCornerPos[VTX_CORNER_LDB] = D3DXVECTOR3(m_VtxMinModel.x, m_VtxMinModel.y, m_VtxMaxModel.z);
	m_aVtxCornerPos[VTX_CORNER_LUB] = D3DXVECTOR3(m_VtxMinModel.x, m_VtxMaxModel.y, m_VtxMaxModel.z);

	m_nIdxCornerPosMinY = VTX_CORNER_LDF;

	m_Rot.y = 0.0f;

	for (int nCntVtx = 0; nCntVtx < VTX_CORNER_MAX; nCntVtx++)
	{
		m_aVtxPosStorage[nCntVtx] = m_aVtxCornerPos[nCntVtx];
	}

	//変数の初期化
	m_bUpdate = true;			// アップデートを通すかどうか
	m_bUpdateBlow = true;		// 吹っ飛ばす処理をするかどうか
	m_bBlow = false;			// 吹っ飛ばしたかどうか
	m_bDelete = false;			// 破棄するかどうか
	m_nCountTime = 0;			// カウンター
	m_bSmallObjDraw = false;	// 小さいオブジェクトの描画フラグ
	m_bHightObjDraw = false;	// 高いオブジェクトの描画フラグ
	m_bOnlyLengthDraw = false;	// 描画距離だけを求めるオブジェクトフラグ
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModel3D::Uninit(void)
{
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CModel3D::Update(void)
{
	//現在のモードを取得
	CManager::MODE mode = CManager::GetMode();

	//ゲームモードならクリッピングの処理
	if (CManager::MODE_GAME == mode)
	{
		//SetDraw(CGame::GetGameCamera()->Clipping(m_VtxMinModel + m_Pos, m_VtxMaxModel + m_Pos));
	}
	else if (CManager::MODE_TUTORIAL == mode)
	{
		//SetDraw(CTutorial::GetCamera()->Clipping(m_VtxMinModel + m_Pos, m_VtxMaxModel + m_Pos));
	}

	if (m_bBlow)
	{//吹っ飛ばした状態なら
		m_nCountTime++;

		//一定時間たったら破棄状態にする
		if ((m_nCountTime % DELETE_TIME) == 0) { m_bDelete = true; }
	}

	if ((false == GetDraw()) || (!m_bUpdate)) { return; }		//描画しない状態だったら

	if ((CManager::MODE_GAME == mode) || (CManager::MODE_TUTORIAL == mode))
	{
		//距離判定をする
		if (LengthJudgment() == true) { return; }

		//街灯等は処理を通さない
		if (m_bHightObjDraw == true) { return; }

		//判定を通ったら
		if ((!m_bUpdateBlow) &&											//距離が一定以上
			(m_Move.x < STOP_MOVE) && (m_Move.x > -STOP_MOVE) &&		// Xの移動量が一定値以下
			(m_Move.y < STOP_MOVE) && (m_Move.y > -STOP_MOVE) &&		// Yの移動値が一定値以下
			(m_Move.z < STOP_MOVE) && (m_Move.z > -STOP_MOVE) &&		// Zの移動値が一定値以下
			(m_blowOff.x < STOP_MOVE) && (m_blowOff.x > -STOP_MOVE) &&	// Xの吹っ飛ぶ量が一定値以下
			(m_blowOff.z < STOP_MOVE) && (m_blowOff.z > -STOP_MOVE))	// Zの吹っ飛ぶ量が一定値以下
		{
			return;
		}

		// 位置の保存
		D3DXVECTOR3 posOld = m_Pos;

		// 重力
		m_Move.y -= cosf(0) * 0.5f;

		if (m_bRoll && m_bBlow)
		{// 転がるかどうか
			if (D3DXVec3Length(&m_blowOff) > m_fMass && m_bStateRoll)
			{
				// 衝突時の角度計算
				RotCalculation();

				// 頂点座標の再計算
				RecalculationVtx();
			}

			// 地面についたとき
			Landing(posOld);

			m_Pos += m_Move;
			m_Pos += m_blowOff;
		}

		// 移動量の慣性
		m_Move.x += (0 - m_Move.x) * 0.04f;
		m_Move.z += (0 - m_Move.z) * 0.04f;

		// 吹っ飛ぶ移動量の慣性
		m_blowOff.x += (0 - m_blowOff.x) * 0.04f;
		m_blowOff.z += (0 - m_blowOff.z) * 0.04f;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CModel3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxSpin, mtxScale;	//計算用マトリックス
	D3DMATERIAL9 matDef;							//現在のマテリアルを保存
	D3DXMATERIAL *pMat;								//マテリアルデータへのポインタ

													// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorldObject);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorldObject, &m_mtxWorldObject, &mtxRot);

	// クォータニオン
	D3DXQuaternionRotationAxis(&m_quat, &m_VecAxis, m_fValueRot);

	// クォータニオンから回転行列
	D3DXMatrixRotationQuaternion(&mtxSpin, &m_quat);

	// ローカルで出た値を保存
	m_mtxRot = mtxSpin;

	// 掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorldObject, &m_mtxWorldObject, &m_mtxRot);

	// 拡大縮小行列の作成
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorldObject, &m_mtxWorldObject, &mtxScale);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorldObject, &m_mtxWorldObject, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldObject);

	if (m_pBuffMatObject != NULL)
	{
		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_pBuffMatObject->GetBufferPointer();
		float fcol_a;
		float fcol_r;
		float fcol_g;
		float fcol_b;

		for (int nCntMat = 0; nCntMat < (int)m_nNumMatObject; nCntMat++)
		{
			if (m_bcolChange == true)
			{	// カラー変更(透明度)
				fcol_a = pMat[nCntMat].MatD3D.Diffuse.a;
				pMat[nCntMat].MatD3D.Diffuse.a = m_col.a;
				fcol_r = pMat[nCntMat].MatD3D.Diffuse.r;
				pMat[nCntMat].MatD3D.Diffuse.r = m_col.r;
				fcol_g = pMat[nCntMat].MatD3D.Diffuse.g;
				pMat[nCntMat].MatD3D.Diffuse.g = m_col.g;
				fcol_b = pMat[nCntMat].MatD3D.Diffuse.b;
				pMat[nCntMat].MatD3D.Diffuse.b = m_col.b;
			}

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//テクスチャがある場合
				pDevice->SetTexture(0, m_pMeshTextures);
			}
			else
			{	// テクスチャを使っていない
				pDevice->SetTexture(0, NULL);
			}
			//オブジェクト(パーツ)の描画
			m_pMeshObject->DrawSubset(nCntMat);

			// カラー変更
			if (m_bcolChange == true)
			{
				pMat[nCntMat].MatD3D.Diffuse.a = fcol_a;
				pMat[nCntMat].MatD3D.Diffuse.r = fcol_r;
				pMat[nCntMat].MatD3D.Diffuse.g = fcol_g;
				pMat[nCntMat].MatD3D.Diffuse.b = fcol_b;
			}
		}

		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);

		m_bcolChange = false;	// 変更終了
	}
}
//===============================================================================
//　値を渡す
//===============================================================================
void CModel3D::BindModel(LPD3DXMESH pMeshObject, LPD3DXBUFFER pBuffMatObject, DWORD nNumMatObject, LPDIRECT3DTEXTURE9 pMeshTextures, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	m_pMeshObject = pMeshObject;
	m_pBuffMatObject = pBuffMatObject;
	m_nNumMatObject = nNumMatObject;
	m_pMeshTextures = pMeshTextures;
	m_VtxMaxModel = VtxMax;
	m_VtxMinModel = VtxMin;
}
//===============================================================================
//　クリエイト
//===============================================================================
CModel3D * CModel3D::Create(void)
{
	CModel3D *pModel3D = NULL;

	//NULLチェック
	if (pModel3D == NULL)
	{//メモリの動的確保

		pModel3D = new CModel3D;

		if (pModel3D != NULL)
		{
			//オブジェクトクラスの生成
			pModel3D->Init();
		}
	}

	return pModel3D;
}

//===============================================================================
// 角度の初期化
//===============================================================================
void CModel3D::InitRot(void)
{
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// クォータニオンの数値をリセット
	m_VecAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fValueRot = 0.0f;

	D3DXMatrixIdentity(&m_mtxRot);
	m_quat.x = 0.0f;
	m_quat.z = 0.0f;
}

//===============================================================================
// 頂点の最大値と最小値を再計算
//===============================================================================
void CModel3D::RecalculationVtx(void)
{
	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxWorld, mtx;	// ワールドマトリックスの保存用
	m_VtxMaxModel = D3DXVECTOR3(-10000, -10000, -10000);	// 最大値
	m_VtxMinModel = D3DXVECTOR3(10000, 10000, 10000);		// 最小値

	D3DXMatrixIdentity(&mtx);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

	// クォータニオンと掛け合わせる
	D3DXMatrixMultiply(&mtx, &mtx, &m_mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	// 回転用マトリクス
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);

	for (int nCntVtxCorner = 0; nCntVtxCorner < VTX_CORNER_MAX; nCntVtxCorner++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorld);

		// 回転を反映
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_aVtxPosStorage[nCntVtxCorner].x, m_aVtxPosStorage[nCntVtxCorner].y, m_aVtxPosStorage[nCntVtxCorner].z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtx);

		// ボックスの角８つの頂点座標
		m_aVtxCornerPos[nCntVtxCorner] = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

		/*		最大値		*/
		if (m_aVtxCornerPos[nCntVtxCorner].x > m_VtxMaxModel.x)
		{// X座標の比較
			m_VtxMaxModel.x = m_aVtxCornerPos[nCntVtxCorner].x;
		}
		if (m_aVtxCornerPos[nCntVtxCorner].y > m_VtxMaxModel.y)
		{// Y座標の比較
			m_VtxMaxModel.y = m_aVtxCornerPos[nCntVtxCorner].y;
		}
		if (m_aVtxCornerPos[nCntVtxCorner].z > m_VtxMaxModel.z)
		{// Z座標の比較
			m_VtxMaxModel.z = m_aVtxCornerPos[nCntVtxCorner].z;
		}

		/*		最小値		*/
		if (m_aVtxCornerPos[nCntVtxCorner].x < m_VtxMinModel.x)
		{// X座標の比較
			m_VtxMinModel.x = m_aVtxCornerPos[nCntVtxCorner].x;
		}
		if (m_aVtxCornerPos[nCntVtxCorner].y < m_VtxMinModel.y)
		{// Y座標の比較
			m_VtxMinModel.y = m_aVtxCornerPos[nCntVtxCorner].y;
			m_nIdxCornerPosMinY = nCntVtxCorner;
		}
		if (m_aVtxCornerPos[nCntVtxCorner].z < m_VtxMinModel.z)
		{// Z座標の比較
			m_VtxMinModel.z = m_aVtxCornerPos[nCntVtxCorner].z;
		}
	}
}

//===============================================================================
// 吹き飛ばし処理
//===============================================================================
D3DXVECTOR3 CModel3D::BlowOff(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fHeavy)
{
	if (m_bUpdate)
	{
		D3DXVECTOR3 newV1, newV2;		// 衝突後の速度
		float fElast = 1.0f;			// 弾性係数

		// 相手の速度
		newV1.x = (-move.x + m_Move.x) * (1.0f + fElast) / (fHeavy / m_fMass + 1.0f) + move.x;
		newV1.z = (-move.z + m_Move.z) * (1.0f + fElast) / (fHeavy / m_fMass + 1.0f) + move.z;

		// 自分の速度
		newV2.x = (-m_Move.x + move.x) * (1.0f + fElast) / (m_fMass / fHeavy + 1.0f) + m_Move.x;
		newV2.z = (-m_Move.z + move.z) * (1.0f + fElast) / (m_fMass / fHeavy + 1.0f) + m_Move.z;
		if (m_bRoll == true)
		{
			// XZ軸への移動量
			m_blowOff.x = newV2.x;
			m_blowOff.z = newV2.z;
		}
		else
		{
			// XZ軸への移動量
			m_blowOff.x = -newV2.x;
			m_blowOff.z = -newV2.z;
		}

		// 移動量を座標に加算
		m_Pos += m_blowOff;

		m_bBlow = true;			//吹っ飛ばした状態にする
		m_bStateRoll = true;	// 転がっている状態にする

		return D3DXVECTOR3(newV1.x, move.y, newV1.z);
	}

	return move;
}

//===============================================================================
// 外積の当たり判定
//===============================================================================
bool CModel3D::Collision(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin, D3DXVECTOR3 move)
{
	bool bHit = false;
	bool bHitX = false, bHitY = false, bHitZ = false;
	D3DXVECTOR3 vtxMaxObject = D3DXVECTOR3(m_mtxWorldObject._41, m_mtxWorldObject._42, m_mtxWorldObject._43);	// ワールド座標の取得
	D3DXVECTOR3 vtxMinObject = D3DXVECTOR3(m_mtxWorldObject._41, m_mtxWorldObject._42, m_mtxWorldObject._43);	// ワールド座標の取得

	// 頂点座標の最大値を求める
	vtxMaxObject.x += m_VtxMaxModel.x * m_scale.x;
	vtxMaxObject.y += m_VtxMaxModel.y * m_scale.y;
	vtxMaxObject.z += m_VtxMaxModel.z * m_scale.z;

	// 頂点座標の最小値を求める
	vtxMinObject.x += m_VtxMinModel.x * m_scale.x;
	vtxMinObject.y += m_VtxMinModel.y * m_scale.y;
	vtxMinObject.z += m_VtxMinModel.z * m_scale.z;

	D3DXVECTOR3 vecA, vecB, vecC;

	/********************************/
	/*			Xの外積計算			*/
	/********************************/
	// 最小値側の判定
	vecA = D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMaxObject.z) - D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMinObject.z);
	vecB = (pos + vtxMax) - D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMinObject.z);
	vecC = (pos + vtxMin) - D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMinObject.z);

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0 || (vecA.z * vecC.x) - (vecA.x * vecC.z) > 0)
	{// 範囲内
	 // 最大数側の判定
		vecA = D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMinObject.z) - D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMaxObject.z);
		vecB = (pos + vtxMin) - D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMaxObject.z);
		vecC = (pos + vtxMax) - D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMaxObject.z);

		if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0 || (vecA.z * vecC.x) - (vecA.x * vecC.z) > 0)
		{// 範囲内
			bHitX = true;
		}
		else { return false; }
	}
	else { return false; }

	/********************************/
	/*			Yの外積計算			*/
	/********************************/
	// 最小値側の判定
	vecA = D3DXVECTOR3(vtxMinObject.x, vtxMinObject.y, 0.0f) - D3DXVECTOR3(vtxMaxObject.x, vtxMinObject.y, 0.0f);
	vecB = (pos + vtxMax) - D3DXVECTOR3(vtxMaxObject.x, vtxMinObject.y, 0.0f);
	vecC = (pos + vtxMin) - D3DXVECTOR3(vtxMaxObject.x, vtxMinObject.y, 0.0f);

	if ((vecA.y * vecB.x) - (vecA.x * vecB.y) > 0 || (vecA.y * vecC.x) - (vecA.x * vecC.y) > 0)
	{// 範囲内
	 // 最大数側の判定
		vecA = D3DXVECTOR3(vtxMaxObject.x, vtxMaxObject.y, 0.0f) - D3DXVECTOR3(vtxMinObject.x, vtxMaxObject.y, 0.0f);
		vecB = (pos + vtxMin) - D3DXVECTOR3(vtxMinObject.x, vtxMaxObject.y, 0.0f);
		vecC = (pos + vtxMax) - D3DXVECTOR3(vtxMinObject.x, vtxMaxObject.y, 0.0f);

		if ((vecA.y * vecB.x) - (vecA.x * vecB.y) > 0 || (vecA.y * vecC.x) - (vecA.x * vecC.y) > 0)
		{// 範囲内
			bHitY = true;
		}
		else { return false; }
	}
	else { return false; }

	/********************************/
	/*			Zの外積計算			*/
	/********************************/
	// 最小値側の判定
	vecA = D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMinObject.z) - D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMinObject.z);
	vecB = (pos + vtxMax) - D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMinObject.z);
	vecC = (pos + vtxMin) - D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMinObject.z);

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0 || (vecA.z * vecC.x) - (vecA.x * vecC.z) > 0)
	{// 範囲内
	 // 最大数側の判定
		vecA = D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMaxObject.z) - D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMaxObject.z);
		vecB = (pos + vtxMin) - D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMaxObject.z);
		vecC = (pos + vtxMax) - D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMaxObject.z);

		if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0 || (vecA.z * vecC.x) - (vecA.x * vecC.z) > 0)
		{// 範囲内
			bHitZ = true;
		}
		else { return false; }
	}
	else { return false; }

	if (bHitX && bHitY && bHitZ)
	{// 範囲内に入っている
		bHit = true;

		D3DXVECTOR3 middle = (vtxMinObject + vtxMaxObject) * 0.5f;		// 中心の座標
		D3DXVECTOR3 vec0(0.0f, 0.0f, 0.0f);

		if (pos.x < middle.x)
		{// 相手のX座標が自分より小さい
			if (pos.z < middle.z)
			{// 相手のZ座標が自分より小さい
				vec0 = D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMaxObject.z) - D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMinObject.z);
			}
			else if (pos.z > middle.z)
			{// 相手のZ座標が自分より大きい
				vec0 = D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMaxObject.z) - D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMinObject.z);
			}
		}
		else if (pos.x > middle.x)
		{// 相手のX座標が自分より大きい
			if (pos.z < middle.z)
			{// 相手のZ座標が自分より小さい
				vec0 = D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMinObject.z) - D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMaxObject.z);
			}
			else if (pos.z > middle.z)
			{// 相手のZ座標が自分より大きい
				vec0 = D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMinObject.z) - D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMaxObject.z);
			}
		}

		// Y軸の回転方向を求める
		m_fRotY = CrossingAngle(vec0, move);
	}

	return bHit;
}

//===============================================================================
// 着陸時の処理
//===============================================================================
void CModel3D::Landing(D3DXVECTOR3 posOld)
{
	if (m_bBlow)
	{// 動いたとき
	//プライオリティーチェック
		CScene *pScene = CScene::GetTop(MESH_PRIOTITY);

		//NULLチェック
		while (pScene != NULL)
		{
			//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
			CScene *pSceneNext = pScene->GetNext();

			if (pScene->GetObjType() == OBJTYPE_GROUND)
			{//タイプが地面だったら
				CMeshField *pField = (CMeshField*)pScene;
				if (pField->OnField(m_Pos, CModel3D::GetVtxMax().y))
				{// 地面に乗せる
					float fHeight = pField->GetHeightMesh(m_Pos);

					if (fHeight > m_Pos.y + CModel3D::GetVtxMin().y && fHeight < posOld.y + CModel3D::GetVtxMin().y)
					{// 空中から地面に着いたとき
						m_Move.y = fHeight - (m_Pos.y + CModel3D::GetVtxMin().y);
						m_Pos.y = fHeight - CModel3D::GetVtxMin().y;
					}
					else if (fHeight > m_Pos.y + CModel3D::GetVtxMin().y && fHeight >= posOld.y + CModel3D::GetVtxMin().y)
					{// 地面に埋まっているとき
						m_Move.y = 0.0f;
						m_Pos.y = fHeight - CModel3D::GetVtxMin().y;
					}

					break;
				}
			}

			//Nextに次のSceneを入れる
			pScene = pSceneNext;
		}
	}
}

//===============================================================================
// ２つのベクトルのなす角度
//===============================================================================
float CModel3D::CrossingAngle(D3DXVECTOR3 vec0, D3DXVECTOR3 vec1)
{
	float fAngle;

	// ２つのベクトルのなす角を求める
	fAngle = (vec0.x * vec1.x) + (vec0.y * vec1.y) + (vec0.z * vec1.z);
	fAngle = fAngle / (sqrtf(powf(vec0.x, 2) + powf(vec0.y, 2) + powf(vec0.z, 2)) * (sqrtf(powf(vec1.x, 2) + powf(vec1.y, 2) + powf(vec1.z, 2))));

	return acosf(fAngle);
}

//===============================================================================
// 衝突時の角度計算
//===============================================================================
void CModel3D::RotCalculation(void)
{
	if (m_blowOff.x > m_fMass || m_blowOff.x < -m_fMass || m_blowOff.z > m_fMass || m_blowOff.z < -m_fMass)
	{// 移動量を回転軸にする
		m_VecAxis = D3DXVECTOR3(m_blowOff.z, m_fRotY, -m_blowOff.x);

		// 移動量ベクトルの大きさの取得
		float fVecLength = D3DXVec3Length(&m_blowOff);

		// 回転角を上げる
		m_fValueRot += fVecLength * 0.01f;
	}
	else if (m_bBlow)
	{// 一定以下で角度を補正する
		D3DXVECTOR3 length0 = m_aVtxCornerPos[(m_nIdxCornerPosMinY + 2) % VTX_CORNER_MAX] - m_aVtxCornerPos[m_nIdxCornerPosMinY];
		D3DXVECTOR3 length1 = m_aVtxCornerPos[(m_nIdxCornerPosMinY + (VTX_CORNER_MAX - 2)) % VTX_CORNER_MAX] - m_aVtxCornerPos[m_nIdxCornerPosMinY];

		float fLength0 = sqrtf(powf(length0.x, 2.0f) + powf(length0.z, 2.0f));
		float fLength1 = sqrtf(powf(length1.x, 2.0f) + powf(length1.z, 2.0f));

		D3DXVECTOR3 opposition;
		if (fLength0 > fLength1 && 0 == m_nIdxCornerPosMinY % 2)
		{// 右側下部
			opposition = m_aVtxCornerPos[(m_nIdxCornerPosMinY + 3) % VTX_CORNER_MAX];
		}
		else if (fLength0 > fLength1 && 1 == m_nIdxCornerPosMinY % 2)
		{// 右側上部
			opposition = m_aVtxCornerPos[(m_nIdxCornerPosMinY + 1) % VTX_CORNER_MAX];
		}
		else if (fLength0 < fLength1 && 0 == m_nIdxCornerPosMinY % 2)
		{// 左側下部
			opposition = m_aVtxCornerPos[(m_nIdxCornerPosMinY + (VTX_CORNER_MAX - 1)) % VTX_CORNER_MAX];
		}
		else if (fLength0 < fLength1 && 1 == m_nIdxCornerPosMinY % 2)
		{// 左側上部
			opposition = m_aVtxCornerPos[(m_nIdxCornerPosMinY + (VTX_CORNER_MAX - 3)) % VTX_CORNER_MAX];
		}
		else
		{// 例外
			opposition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		if (opposition.y > m_aVtxCornerPos[m_nIdxCornerPosMinY].y)
		{
			// 高さを求める
			float fHeight = opposition.y - m_aVtxCornerPos[m_nIdxCornerPosMinY].y;

			// 底辺の長さを求める
			float fBottomX = opposition.x - m_aVtxCornerPos[m_nIdxCornerPosMinY].x;
			float fBottomZ = opposition.z - m_aVtxCornerPos[m_nIdxCornerPosMinY].z;

			// 角度を求める（原点は真ん中なので半分の値にする）
			float fAngleX = atanf(fHeight / fBottomX) * 0.5f;
			float fAngleZ = atanf(fHeight / fBottomZ) * 0.5f;

			/*					  */
			/*			X軸		  */
			/*					  */
			// 差分を求める
			float fDiff = fAngleX - m_Rot.x;

			if (fDiff > D3DX_PI) { fDiff -= D3DX_PI * 2.0f; }
			if (fDiff < -D3DX_PI) { fDiff += D3DX_PI * 2.0f; }

			// 角度を目的に近づける
			m_Rot.x += fDiff * 0.1f;

			if (m_Rot.x > D3DX_PI) { m_Rot.x -= D3DX_PI * 2.0f; }
			if (m_Rot.x < -D3DX_PI) { m_Rot.x += D3DX_PI * 2.0f; }

			/*					  */
			/*			Z軸		  */
			/*					  */
			// 差分を求める
			fDiff = fAngleZ - m_Rot.z;

			if (fDiff > D3DX_PI) { fDiff -= D3DX_PI * 2.0f; }
			if (fDiff < -D3DX_PI) { fDiff += D3DX_PI * 2.0f; }

			// 角度を目的に近づける
			m_Rot.z += fDiff * 0.1f;

			if (m_Rot.z > D3DX_PI) { m_Rot.z -= D3DX_PI * 2.0f; }
			if (m_Rot.z < -D3DX_PI) { m_Rot.z += D3DX_PI * 2.0f; }
		}

		if (m_bStateRoll)
		{// 転がっている状態のフラグを下げる
			m_bStateRoll = false;
		}
	}
}

//=============================================================================
// プレイヤーとの距離判定
//=============================================================================
bool CModel3D::LengthJudgment(void)
{
	bool bReturn = false;	//処理続行フラグ

	//距離を求める
	D3DXVECTOR3 PlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// CGame::GetPlayer()->GetPos();
	D3DXVECTOR3 Distance = PlayerPos - m_Pos;
	m_fLength = sqrtf((Distance.x * Distance.x) + (Distance.z * Distance.z));

	//吹っ飛ばす処理を更新状態にする
	m_bUpdateBlow = true;

	//判定距離以上だったら更新しない状態にする
	if (m_fLength > LENGTH_BLOW) { m_bUpdateBlow = false; }

	if ((m_fLength > LENGTH_SMALLOBJ) && (m_bSmallObjDraw == true))
	{//一定以上の距離だったら描画しない状態にする
		SetDraw(false);		//描画しない状態にする
		bReturn = true;
	}
	else if ((m_fLength > LENGTH_HIGHTOBJ) && (m_bHightObjDraw == true))
	{
		SetDraw(false);
		bReturn = true;
	}
	else if (m_bOnlyLengthDraw)
	{
		bReturn = true;
	}

	return bReturn;
}

//=============================================================================
// 当たり判定ボックスの拡縮
//=============================================================================
void CModel3D::ScaleVtxCornerPos(D3DXVECTOR3 scale)
{
	// 値を保存
	m_scale = scale;
}