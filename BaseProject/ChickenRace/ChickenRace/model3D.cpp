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
#include "toonshader.h"
#include "camerabace.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPD3DXMESH			CModel3D::m_pMeshModel[MODEL_TYPE_MAX] = {};			// メッシュ情報へのポインタ
LPD3DXBUFFER		CModel3D::m_pBuffMatModel[MODEL_TYPE_MAX] = {};			// マテリアルの情報へのポインタ
DWORD				CModel3D::m_nNumMatModel[MODEL_TYPE_MAX] = {};			// マテリアルの情報数
LPDIRECT3DTEXTURE9  CModel3D::m_pMeshTextures[TEXTURE_TYPE_MAX] = {};		// テクスチャの情報
CToonShader		   *CModel3D::m_pToonShader = NULL;							// シェーダーのポインタ
D3DMATERIAL9	   *CModel3D::m_pMeshMaterials[MODEL_TYPE_MAX] = {};		// メッシュマテリアルの情報
LPDIRECT3DTEXTURE9 *CModel3D::m_pShaderMeshTextures[MODEL_TYPE_MAX] = {};	// シェーダー用のメッシュテクスチャ
D3DXVECTOR3			CModel3D::m_VtxMinModel[MODEL_TYPE_MAX];
D3DXVECTOR3			CModel3D::m_VtxMaxModel[MODEL_TYPE_MAX];					// モデルの最小値・最大値

//*****************************************************************************
//					 モデルの読み込みファイル名
//*****************************************************************************
const char *CModel3D::m_apModelFile[MODEL_TYPE_MAX] =
{
	//=============================
	//　         マップ
	//=============================
	"data\\MODEL\\Object\\tree4.x",
	"data\\MODEL\\Object\\柵.x",
	"data\\MODEL\\Collision\\box.x",
	"data\\MODEL\\Map\\Map_First.x",
	"data\\MODEL\\Map\\Map_Second.x",
	"data\\MODEL\\Map\\Map_Mountain.x",
	"data\\MODEL\\Map\\Map_Sky_1.x",
	"data\\MODEL\\Object\\ニワトリ小屋.x",
	"data\\MODEL\\Object\\L_看板.x",
	"data\\MODEL\\Object\\R_看板.x",
	"data\\MODEL\\Object\\LR_看板.x",
	"data\\MODEL\\Object\\橋.x",
	"data\\MODEL\\Object\\石像.x",
	"data\\MODEL\\Object\\表彰台.x",
	"data\\MODEL\\Object\\bigkanban.x",
	"data\\MODEL\\Object\\岩.x",
	"data\\MODEL\\Object\\ゴール.x",
	"data\\MODEL\\Object\\崖_看板.x",
	"data\\MODEL\\Object\\餌_看板.x",
	"data\\MODEL\\Object\\水_看板.x",
	"data\\MODEL\\Object\\門.x",

	//=============================
	//　          餌
	//=============================
	"data\\MODEL\\Item\\肉.x",
	"data\\MODEL\\Item\\ブドウ.x",
	"data\\MODEL\\Item\\トウモロコシ.x",

	//=============================
	//　          卵
	//=============================
	"data\\MODEL\\Weapon\\egg.x",

	//=============================
	//　          ヒヨコ
	//=============================
	"data\\MODEL\\Weapon\\chick.x",

	//=============================
	//　          エフェクト
	//=============================
	"data\\MODEL\\Effect\\Dirt.x",
	"data\\MODEL\\Effect\\Grass.x"
};
//*****************************************************************************
//						 テクスチャの読み込みファイル名
//*****************************************************************************
const char *CModel3D::m_apTextureFile[TEXTURE_TYPE_MAX] =
{
	//=============================
	//　         マップ
	//=============================
	"data\\TEXTURE\\modeltex\\tree000.jpg",
	"data\\TEXTURE\\modeltex\\柵.jpg",
	"data\\TEXTURE\\modeltex\\bender.jpg",
	"data\\TEXTURE\\modeltex\\meat.jpg",
	"data\\TEXTURE\\modeltex\\meat.jpg",
	"data\\TEXTURE\\modeltex\\Mountain.jpg",
	"data\\TEXTURE\\modeltex\\Map_Sky.jpg",
	"data\\TEXTURE\\modeltex\\ニワトリ小屋.png",
	"data\\TEXTURE\\modeltex\\看板.jpg",
	"data\\TEXTURE\\modeltex\\看板.jpg",
	"data\\TEXTURE\\modeltex\\看板.jpg",
	"data\\TEXTURE\\modeltex\\橋.jpg",
	"data\\TEXTURE\\modeltex\\石像金.jpg",
	"data\\TEXTURE\\modeltex\\表彰台.jpg",
	"data\\TEXTURE\\modeltex\\bigkanban.jpg",
	"data\\TEXTURE\\modeltex\\岩.jpg",
	"data\\TEXTURE\\modeltex\\Snap000.jpg",
	"data\\TEXTURE\\modeltex\\看板(崖).jpg",
	"data\\TEXTURE\\modeltex\\看板(餌).jpg",
	"data\\TEXTURE\\modeltex\\看板(水たまり).jpg",
	"data\\TEXTURE\\modeltex\\柵.jpg",

	//=============================
	//　          餌
	//=============================
	"data\\TEXTURE\\modeltex\\肉.jpg",
	"data\\TEXTURE\\modeltex\\ブドウ.jpg",
	"data\\TEXTURE\\modeltex\\トウモロコシ.jpg",

	//============================
	//　          卵
	//=============================
	"data\\TEXTURE\\modeltex\\egg02.jpg",
	"data\\TEXTURE\\modeltex\\egg00.jpg",
	"data\\TEXTURE\\modeltex\\egg01.jpg",

	//=============================
	//　          ヒヨコ
	//=============================
	"data\\TEXTURE\\modeltex\\egg02.jpg",
	"data\\TEXTURE\\modeltex\\egg00.jpg",
	"data\\TEXTURE\\modeltex\\egg01.jpg",
	"data\\TEXTURE\\modeltex\\egg02.jpg",
	"data\\TEXTURE\\modeltex\\egg00.jpg",
	"data\\TEXTURE\\modeltex\\egg01.jpg",

	//=============================
	//　          エフェクト
	//=============================
	"data\\TEXTURE\\modeltex\\ModelEffect.jpg",
	"data\\TEXTURE\\modeltex\\water.jpg",
	"data\\TEXTURE\\modeltex\\Grass.jpg"
};
//===============================================================================
//　オーバーロードコンストラクタ
//===============================================================================
CModel3D::CModel3D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_mtxWorldObject;							//ワールドマトリックス
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//動き
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	//大きさ
	m_VecAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fValueRot = 0.0f;
	m_bcolChange = false;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// カラー
	m_bTexMat = false;
	m_MapView = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_modeltype = MODEL_TYPE_MAX;
	for (int nCnt = 0; nCnt < MAX_PLAYCOL; nCnt++) { m_fCola[nCnt] = 0.0f; }
}
//===============================================================================
//　デストラクタ
//===============================================================================
CModel3D::~CModel3D() {}

//===============================================================================
//　個々読み込み処理
//===============================================================================
LPD3DXMESH	&CModel3D::MeshLoad(MODEL_TYPE modeltype)
{
	if (modeltype >= MODEL_TYPE_MAX) { return m_pMeshModel[0]; }
	if (m_pMeshMaterials[modeltype] == NULL)
	{
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		//モデルの読み込み
		D3DXLoadMeshFromX(m_apModelFile[modeltype], D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[modeltype], NULL, &m_nNumMatModel[modeltype], &m_pMeshModel[modeltype]);
		D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)m_pBuffMatModel[modeltype]->GetBufferPointer();

		//マテリアル情報とシェーダのテクスチャを動的確保
		m_pMeshMaterials[modeltype] = new D3DMATERIAL9[(int)m_nNumMatModel[modeltype]];
		m_pShaderMeshTextures[modeltype] = new LPDIRECT3DTEXTURE9[(int)m_nNumMatModel[modeltype]];

		for (DWORD MatCount = 0; MatCount < (int)m_nNumMatModel[modeltype]; MatCount++)
		{
			m_pMeshMaterials[modeltype][MatCount] = d3dxMaterials[MatCount].MatD3D;
			m_pMeshMaterials[modeltype][MatCount].Ambient = m_pMeshMaterials[modeltype][MatCount].Diffuse;
			m_pShaderMeshTextures[modeltype][MatCount] = NULL;

			if (d3dxMaterials[MatCount].pTextureFilename != NULL &&
				lstrlen(d3dxMaterials[MatCount].pTextureFilename) > 0)
			{
				D3DXCreateTextureFromFile(pDevice,
					d3dxMaterials[MatCount].pTextureFilename,
					&m_pShaderMeshTextures[modeltype][MatCount]);
			}
		}
		m_VtxMaxModel[modeltype] = INIT_VECTOR;
		m_VtxMinModel[modeltype] = INIT_VECTOR;

		int nNumVtx;				//頂点数
		DWORD sizeFVF;				//頂点フォーマットのサイズ
		BYTE *pVtxBuff;				//頂点バッファへのポインタ

		nNumVtx = m_pMeshModel[modeltype]->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(m_pMeshModel[modeltype]->GetFVF());

		//頂点バッファのロック
		m_pMeshModel[modeltype]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//頂点座標の代入

															//最大値
			if (vtx.x > m_VtxMaxModel[modeltype].x)
			{
				m_VtxMaxModel[modeltype].x = vtx.x;
			}
			if (vtx.y > m_VtxMaxModel[modeltype].y)
			{
				m_VtxMaxModel[modeltype].y = vtx.y;
			}
			if (vtx.z > m_VtxMaxModel[modeltype].z)
			{
				m_VtxMaxModel[modeltype].z = vtx.z;
			}
			//最小値
			if (vtx.x < m_VtxMinModel[modeltype].x)
			{
				m_VtxMinModel[modeltype].x = vtx.x;
			}
			if (vtx.y < m_VtxMinModel[modeltype].y)
			{
				m_VtxMinModel[modeltype].y = vtx.y;
			}
			if (vtx.z < m_VtxMinModel[modeltype].z)
			{
				m_VtxMinModel[modeltype].z = vtx.z;
			}

			//サイズ文のポインタを進める
			pVtxBuff += sizeFVF;
		}

		//頂点バッファのアンロック
		m_pMeshModel[modeltype]->UnlockVertexBuffer();
	}

	//シェーダーの読み込むファイル
	if (m_pToonShader == NULL)
	{
		m_pToonShader = new CToonShader;
		m_pToonShader->Init("data\\SHADER\\ToonShader.fx");
	}

	return m_pMeshModel[modeltype];
}
//===============================================================================
//　開放の処理
//===============================================================================
void CModel3D::UnLoad(void)
{
	for (int nCount = 0; nCount < MODEL_TYPE_MAX; nCount++)
	{
		// メッシュの開放
		if (m_pMeshModel[nCount] != NULL)
		{
			m_pMeshModel[nCount]->Release();
			m_pMeshModel[nCount] = NULL;
		}
		// マテリアルの開放
		if (m_pBuffMatModel[nCount] != NULL)
		{
			m_pBuffMatModel[nCount]->Release();
			m_pBuffMatModel[nCount] = NULL;
		}
	}

	for (int nCount = 0; nCount < TEXTURE_TYPE_MAX; nCount++)
	{
		//テクスチャの破棄
		if (m_pMeshTextures[nCount] != NULL)
		{
			m_pMeshTextures[nCount]->Release();
			m_pMeshTextures[nCount] = NULL;
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//位置
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//向き達する

	m_Rot.y = 0.0f;

	//種類を入れる
	m_modeltype = (MODEL_TYPE)m_nModelType;

	MeshLoad(m_modeltype);

	if (m_pMeshTextures[m_nTextureType] == NULL)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, m_apTextureFile[m_nTextureType], &m_pMeshTextures[m_nTextureType]);
	}
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

	// 位置の保存
	D3DXVECTOR3 posOld = m_Pos;

	// 重力
	m_Move.y -= cosf(0) * 0.5f;

	// 移動量の慣性
	m_Move.x += (0 - m_Move.x) * 0.04f;
	m_Move.z += (0 - m_Move.z) * 0.04f;

	m_bcolChange = true;
}

//=============================================================================
// 描画処理
//=============================================================================
void CModel3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX		mtxRot, mtxTrans, mtxSpin, mtxScale;	//計算用マトリックス
	D3DMATERIAL9	matDef;									//現在のマテリアルを保存
	D3DXMATERIAL	*pMat;									//マテリアルデータへのポインタ
	LPD3DXEFFECT	Shader = NULL;							//シェーダー
	CCamera			*pCamera = NULL;						//カメラのポイント

	int nNumber = -1;
	D3DXVECTOR3 pos0;
	if (pCamera == NULL)
	{
		switch (CManager::GetMode())
		{// モード
		case CManager::MODE_TITLE:
			pCamera = (CCamera*)CTitle::GetTitleCamera();
			break;
		case CManager::MODE_GAME:
			switch (CGame::GetGameMode())
			{// ゲームモード
			case CGame::GAMEMODE_CHARSELECT:
				if (CGame::GetCameraNumber() == -1)
					pCamera = (CCamera*)CGame::GetCourseCamera();
				else
					pCamera = (CCamera*)CGame::GetGameCamera(CGame::GetCameraNumber());
				break;
			case CGame::GAMEMODE_COURSESELECT:
			case CGame::GAMEMODE_COURSE_VIEW:
				pCamera = (CCamera*)CGame::GetCourseCamera();
				break;
			case CGame::GAMEMODE_PLAY:
				if (CGame::GetCameraNumber() == -1)
					pCamera = (CCamera*)CGame::GetCourseCamera();
				else
				{
					nNumber = CGame::GetCameraNumber();
					pCamera = (CCamera*)CGame::GetGameCamera(CGame::GetCameraNumber());
					pos0 = pCamera->GetCameraPosV();
				}
				break;
			}
			break;
		case CManager::MODE_RESULT:
			pCamera = (CCamera*)CResult::GetResultCamera();
			break;
		}
	}

	if (pCamera != NULL && nNumber >= 0 && (GetObjType() == OBJTYPE_OBJECT || GetObjType() == OBJTYPE_EGG))
	{
		m_bcolChange = true;

		if (m_modeltype != MODEL_TYPE_BRIDGE && m_modeltype != MODEL_TYPE_BIGSIGN)
		{
			if (pCamera->Clipping(m_Pos, m_VtxMinModel[m_modeltype] + m_Pos, m_VtxMaxModel[m_modeltype] + m_Pos))
			{
				m_fCola[nNumber] -= 0.05f;
				if (m_fCola[nNumber] < 0.0f) { m_fCola[nNumber] = 0.0f; return; }
			}
			else
			{
				m_fCola[nNumber] += 0.05f;
				if (m_fCola[nNumber] > 1.0f) { m_fCola[nNumber] = 1.0f; }
			}
			m_col.a = m_fCola[nNumber];
		}
	}

	//シェーダーに情報を代入
	if (m_pToonShader != NULL)
	{
		Shader = m_pToonShader->GetShader();
	}

	//シェーダーの中身がある場合
	if (Shader != NULL)
	{
		LPDIRECT3DTEXTURE9 ShaderTex = m_pToonShader->GetTexture();
		LPDIRECT3DTEXTURE9 LineTex = m_pToonShader->GetLineTexture();

		LPDIRECT3DTEXTURE9 ShaderMapTex = m_pToonShader->GetMapTexture();
		LPDIRECT3DTEXTURE9 LineMapTex = m_pToonShader->GetLineMapTexture();

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

		//// 拡大縮小行列の作成
		//D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
		//D3DXMatrixMultiply(&m_mtxWorldObject, &m_mtxWorldObject, &mtxScale);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
		D3DXMatrixMultiply(&m_mtxWorldObject, &m_mtxWorldObject, &mtxTrans);

		//拡大処理
		m_mtxWorldObject._44 = (1.0f / m_Scale.x);
		m_mtxWorldObject._41 *= m_mtxWorldObject._44;
		m_mtxWorldObject._42 *= m_mtxWorldObject._44;
		m_mtxWorldObject._43 *= m_mtxWorldObject._44;

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldObject);

		//===================================================
		//    　　　　　　シェーダーの情報設定
		//===================================================
		//テクニックの設定
		Shader->SetTechnique("ToonShading");

		//マトリックスを渡す
		D3DXMATRIX mtxView = pCamera->GetViewMatrix();
		D3DXMATRIX mtxProjection = pCamera->GetProjectionMatrix();
		D3DXVECTOR3 ViewPosV = pCamera->GetCameraPosV();

		//シェーダーにマトリックスを渡す
		Shader->SetMatrix("matProj", &mtxProjection);
		Shader->SetMatrix("matView", &mtxView);
		Shader->SetMatrix("matWorld", &m_mtxWorldObject);

		if (m_nModelType == 3 || m_nModelType == 4 || m_nModelType == 5)
		{//マップ
			Shader->SetTexture("ShadeTexture", ShaderMapTex);
			Shader->SetTexture("LineTexture", LineMapTex);
		}
		else if (m_nModelType != 3 && m_nModelType != 4 && m_nModelType != 5)
		{//それ以外
			Shader->SetTexture("ShadeTexture", ShaderTex);
			Shader->SetTexture("LineTexture", LineTex);
		}

		Shader->SetVector("EyePos", (D3DXVECTOR4*)&ViewPosV);

		//シェーダ開始
		Shader->Begin(NULL, 0);

		if (m_pBuffMatModel[m_modeltype] != NULL)
		{
			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)m_pBuffMatModel[m_modeltype]->GetBufferPointer();
			D3DCOLORVALUE col;

			for (int nCntMat = 0; nCntMat < (int)m_nNumMatModel[m_modeltype]; nCntMat++)
			{
				//シェーダのデータにモデルのテクスチャを入れる
				m_pShaderMeshTextures[m_modeltype][nCntMat] = m_pMeshTextures[m_nTextureType];

				if (m_bcolChange == true)
				{// カラー変更(透明度)
					col = m_pMeshMaterials[m_modeltype][nCntMat].Diffuse;

					m_pMeshMaterials[m_modeltype][nCntMat].Diffuse.a = m_col.a;
					m_pMeshMaterials[m_modeltype][nCntMat].Diffuse.r = m_col.r;
					m_pMeshMaterials[m_modeltype][nCntMat].Diffuse.g = m_col.g;
					m_pMeshMaterials[m_modeltype][nCntMat].Diffuse.b = m_col.b;
				}

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャがある場合
				pDevice->SetTexture(0, m_pShaderMeshTextures[m_modeltype][nCntMat]);

				// カラー変更
				//if (m_bcolChange == true)
				//{
				//	pMat[nCntMat].MatD3D.Diffuse.a = fcol_a;
				//	pMat[nCntMat].MatD3D.Diffuse.r = fcol_r;
				//	pMat[nCntMat].MatD3D.Diffuse.g = fcol_g;
				//	pMat[nCntMat].MatD3D.Diffuse.b = fcol_b;
				//}

				//===================================================
				//    　　　　シェーダーの割り当て作業
				//===================================================
				if (m_nModelType != MODEL_TYPE_BRIDGE)
				{
					//パスを指定して開始
					Shader->BeginPass(0);

					//シェーダーテクスチャの有無に応じてシェーダーをつける
					if (m_pShaderMeshTextures[m_modeltype][nCntMat] != NULL)
					{
						//テクスチャがある場合
						m_bTexMat = true;
						//シェーダーにboolの情報を渡す
						Shader->SetBool("TexMat", m_bTexMat);
					}
					else if (m_pShaderMeshTextures[m_modeltype][nCntMat] == NULL)
					{
						//テクスチャがない場合
						m_bTexMat = false;
						//シェーダーにboolの情報を渡す11111
						Shader->SetBool("TexMat", m_bTexMat);
					}

					//オブジェクトのテクスチャ
					Shader->SetTexture("DecalTexture", m_pShaderMeshTextures[m_modeltype][nCntMat]);

					//プロパティー名,NameID,設定する配列の値
					Shader->SetFloatArray("Diffuse", (FLOAT*)&m_pMeshMaterials[m_modeltype][nCntMat].Diffuse, 4);

					//変更を基になるディレクトリ ストアに保存
					Shader->CommitChanges();
				}

				//オブジェクト(パーツ)の描画
				m_pMeshModel[m_modeltype]->DrawSubset(nCntMat);

				//パスの終了
				Shader->EndPass();
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
			m_mtxWorldObject._44 = 1.0f;

			m_bcolChange = false;	// 変更終了

		}
		//シェーダの終了
		Shader->End();
	}
}
//===============================================================================
//　シェーダの情報の削除
//===============================================================================
void CModel3D::ModelShaderDeleter(void)
{
	for (int nCount = 0; nCount < MODEL_TYPE_MAX; nCount++)
	{
		//シェーダー残骸消すもの
		if (m_pMeshMaterials[nCount] != NULL)
		{
			delete[] m_pMeshMaterials[nCount];
			m_pMeshMaterials[nCount] = NULL;
		}
		if (m_pShaderMeshTextures[nCount] != NULL)
		{
			delete[] m_pShaderMeshTextures[nCount];
			m_pShaderMeshTextures[nCount] = NULL;
		}
	}

	//シェーダーのポインタの破棄
	if (m_pToonShader != NULL)
	{
		m_pToonShader->Uninit();
		delete m_pToonShader;
		m_pToonShader = NULL;
	}
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
// 外積の当たり判定
//===============================================================================
bool CModel3D::Collision(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin, D3DXVECTOR3 move)
{
	bool bHit = false;
	bool bHitX = false, bHitY = false, bHitZ = false;
	D3DXVECTOR3 vtxMaxObject = D3DXVECTOR3(m_mtxWorldObject._41, m_mtxWorldObject._42, m_mtxWorldObject._43);	// ワールド座標の取得
	D3DXVECTOR3 vtxMinObject = D3DXVECTOR3(m_mtxWorldObject._41, m_mtxWorldObject._42, m_mtxWorldObject._43);	// ワールド座標の取得

																												// 頂点座標の最大値を求める
	vtxMaxObject.x += m_VtxMaxModel[m_modeltype].x * m_Scale.x;
	vtxMaxObject.y += m_VtxMaxModel[m_modeltype].y * m_Scale.y;
	vtxMaxObject.z += m_VtxMaxModel[m_modeltype].z * m_Scale.z;

	// 頂点座標の最小値を求める
	vtxMinObject.x += m_VtxMinModel[m_modeltype].x * m_Scale.x;
	vtxMinObject.y += m_VtxMinModel[m_modeltype].y * m_Scale.y;
	vtxMinObject.z += m_VtxMinModel[m_modeltype].z * m_Scale.z;

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
	}

	return bHit;
}