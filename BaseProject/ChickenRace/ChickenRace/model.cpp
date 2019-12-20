//=============================================================================
//
// モデル専用の処理 [model.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "toonshader.h"
#include "gamecamera.h"
#include "model3d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
D3DMATERIAL9	   *CModel::m_pMeshMaterials[PARTS_MAX] = {};					// メッシュマテリアルの情報
LPDIRECT3DTEXTURE9 *CModel::m_pShaderMeshTextures[PARTS_MAX] = {};				// シェーダー用のメッシュテクスチャ
LPDIRECT3DTEXTURE9 *CModel::m_pMeshTextures[MAX_MODEL_TEXTURE] = {};			// シェーダー用のメッシュテクスチャ
LPD3DXMESH			CModel::m_pMesh[PARTS_MAX] = {};							//メッシュ情報へのポインタ
LPD3DXBUFFER		CModel::m_pBuffMat[PARTS_MAX] = {};							//マテリアルの情報へのポインタ
DWORD				CModel::m_nNumMat[PARTS_MAX] = {};							//マテリアルの情報数
D3DXVECTOR3			CModel::m_VtxMax[PARTS_MAX];								//最大値
D3DXVECTOR3			CModel::m_VtxMin[PARTS_MAX];								//最小値
CModel::PARTS_TYPE	*CModel::m_partstype[TYPE_MAX] = {};
int					CModel::m_nModelMax[TYPE_MAX];								// モデルの種類
LPDIRECT3DTEXTURE9	CModel::m_pTexAll[TEX_MAX] = {};

//=============================================================================
// 生成処理
//=============================================================================
CModel * CModel::Create(const D3DXVECTOR3 pos, char FileName[40], D3DXVECTOR3 Scale)
{
	//インスタンスの生成
	CModel * pModel;
	pModel = new CModel;

	//初期化処理
	pModel->Init();

	//設定処理
	pModel->SetPos(pos);
	pModel->SetFirstPos(pos);
	//pModel->SetScale(Scale);

	return pModel;
}

//===============================================================================
//　コンストラクタ
//===============================================================================
void CModel::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int nNumVtx;				//頂点数
	DWORD sizeFVF;				//頂点フォーマットのサイズ
	BYTE *pVtxBuff;				//頂点バッファへのポインタ

	//===================================================
	//    　　　　　マテリアルとテクスチャの情報
	//===================================================
	for (int nCount = 0; nCount < PARTS_MAX; nCount++)
	{
		m_VtxMax[nCount] = D3DXVECTOR3(-10000, -10000, -10000);			//最大値
		m_VtxMin[nCount] = D3DXVECTOR3(10000, 10000, 10000);			//最小値

		//頂点数を取得
		nNumVtx = m_pMesh[nCount]->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh[nCount]->GetFVF());

		//頂点バッファのロック
		m_pMesh[nCount]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//頂点座標の代入

			//最大値
			if (vtx.x > m_VtxMax[nCount].x)
			{
				m_VtxMax[nCount].x = vtx.x;
			}
			if (vtx.y > m_VtxMax[nCount].y)
			{
				m_VtxMax[nCount].y = vtx.y;
			}
			if (vtx.z > m_VtxMax[nCount].z)
			{
				m_VtxMax[nCount].z = vtx.z;
			}
			//最小値
			if (vtx.x < m_VtxMin[nCount].x)
			{
				m_VtxMin[nCount].x = vtx.x;
			}
			if (vtx.y < m_VtxMin[nCount].y)
			{
				m_VtxMin[nCount].y = vtx.y;
			}
			if (vtx.z < m_VtxMin[nCount].z)
			{
				m_VtxMin[nCount].z = vtx.z;
			}

			//サイズ文のポインタを進める
			pVtxBuff += sizeFVF;
		}

		//頂点バッファのアンロック
		m_pMesh[nCount]->UnlockVertexBuffer();

		D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)m_pBuffMat[nCount]->GetBufferPointer();
		m_pMeshMaterials[nCount] = new D3DMATERIAL9[(int)m_nNumMat[nCount]];
		m_pShaderMeshTextures[nCount] = new LPDIRECT3DTEXTURE9[(int)m_nNumMat[nCount]];

		for (DWORD MatCount = 0; MatCount < (int)m_nNumMat[nCount]; MatCount++)
		{
			m_pMeshMaterials[nCount][MatCount] = d3dxMaterials[MatCount].MatD3D;
			m_pMeshMaterials[nCount][MatCount].Ambient = m_pMeshMaterials[nCount][MatCount].Diffuse;
			m_pShaderMeshTextures[nCount][MatCount] = NULL;

			if (d3dxMaterials[MatCount].pTextureFilename != NULL &&
				lstrlen(d3dxMaterials[MatCount].pTextureFilename) > 0)
			{
				D3DXCreateTextureFromFile(pDevice,
					d3dxMaterials[MatCount].pTextureFilename,
					&m_pShaderMeshTextures[nCount][MatCount]);
			}
		}
	}

	//使っているテクスチャ
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\costumetex\\happyboy.jpg", &m_pTexAll[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\costumetex\\hat.jpg", &m_pTexAll[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\costumetex\\featherbone.jpg", &m_pTexAll[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\costumetex\\pirate.jpg", &m_pTexAll[3]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\costumetex\\kabuto.jpg", &m_pTexAll[4]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\costumetex\\helmet.jpg", &m_pTexAll[5]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\costumetex\\beret.jpg", &m_pTexAll[6]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\costumetex\\clown.jpg", &m_pTexAll[7]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\modeltex\\ニワトリ00.jpg", &m_pTexAll[8]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\modeltex\\chick01.jpg", &m_pTexAll[9]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\modeltex\\chick02.jpg", &m_pTexAll[10]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\modeltex\\chick00.jpg", &m_pTexAll[11]);

}

//===============================================================================
//　コンストラクタ
//===============================================================================
CModel::CModel()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_FirstPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//初期位置
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	//拡大縮小率
	m_pTextures = NULL;
	m_bTexMat = false;
	m_pParent = NULL;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CModel::~CModel() { }

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//位置
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//向き達する
	m_Rot.y = 0.0f;
	m_pTextures = NULL;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModel::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CModel::Update(void) {}

//=============================================================================
// 描画処理
//=============================================================================
void CModel::Draw(float fAlpha)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxParent, mtxScale;	//計算
	D3DMATERIAL9 matDef;								//現在のマテリアルを保存
	D3DXMATERIAL *pMat;									//マテリアルデータへのポインタ
	LPD3DXEFFECT Shader = NULL;							//シェーダー
	CCamera *pCamera = NULL;							//カメラのポイント
	LPDIRECT3DTEXTURE9 ShaderTex = NULL;
	LPDIRECT3DTEXTURE9 LineTex = NULL;

	CModel3D *pModel3D = NULL;

	//カメラのポインタに情報を代入
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
					pCamera = (CCamera*)CGame::GetGameCamera(CGame::GetCameraNumber());
				break;
			}
			break;
		case CManager::MODE_RESULT:
			pCamera = (CCamera*)CResult::GetResultCamera();
			break;
		}
	}

	//シェーダーに情報を代入
	CToonShader *&pToonShader = CModel3D::GetToonShader();

	if (pToonShader != NULL)
	{
		Shader = pToonShader->GetShader();
	}

	//シェーダーの中身がある場合
	if (Shader != NULL)
	{
		if (pToonShader != NULL)
		{
			ShaderTex = pToonShader->GetTexture();
			LineTex = pToonShader->GetLineTexture();
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		if (m_pParent != NULL)
		{//親の場合
			mtxParent = m_pParent->GetMtxWorld();
		}
		else
		{//親ではない場合
			pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
		}

		//親のマトリックスと掛け合わせる
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

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
		Shader->SetMatrix("matWorld", &m_mtxWorld);

		//テクスチャの割り当て
		Shader->SetTexture("ShadeTexture", ShaderTex);
		Shader->SetTexture("LineTexture", LineTex);

		Shader->SetVector("EyePos", (D3DXVECTOR4*)&ViewPosV);

		//シェーダ開始
		Shader->Begin(NULL, 0);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//for (int nCount = 0; nCount < PARTS_MAX; nCount++)
		//{
			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)m_pBuffMat[m_Type]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)m_nNumMat[m_Type]; nCntMat++)
			{
				//シェーダのデータにモデルのテクスチャを入れる
				m_pShaderMeshTextures[m_Type][nCntMat] = m_pTextures;

				//マテリアルの設定
				pMat[nCntMat].MatD3D.Diffuse.a = fAlpha;
				m_pMeshMaterials[m_Type][nCntMat].Diffuse.a = fAlpha;

				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャをNULLにする
				pDevice->SetTexture(0, m_pShaderMeshTextures[m_Type][nCntMat]);

				//===================================================
				//    　　　　シェーダーの割り当て作業
				//===================================================
				//パスを指定して開始
				Shader->BeginPass(0);

				//シェーダーテクスチャの有無に応じてシェーダーをつける
				if (m_pShaderMeshTextures[m_Type][nCntMat] != NULL)
				{
					//テクスチャがある場合
					m_bTexMat = true;
					//シェーダーにboolの情報を渡す
					Shader->SetBool("TexMat", m_bTexMat);
				}
				else if (m_pShaderMeshTextures[m_Type][nCntMat] == NULL)
				{
					//テクスチャがない場合
					m_bTexMat = false;
					//シェーダーにboolの情報を渡す
					Shader->SetBool("TexMat", m_bTexMat);
				}

				//オブジェクトのテクスチャ
				Shader->SetTexture("DecalTexture", m_pShaderMeshTextures[m_Type][nCntMat]);
				//プロパティー名,NameID,設定する配列の値
				Shader->SetFloatArray("Diffuse", (FLOAT*)&m_pMeshMaterials[m_Type][nCntMat].Diffuse, 4);

				//変更を基になるディレクトリ ストアに保存
				Shader->CommitChanges();

				//オブジェクト(パーツ)の描画
				m_pMesh[m_Type]->DrawSubset(nCntMat);

				//パスの終了
				Shader->EndPass();
			}

		//}
		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);

		//シェーダの終了
		Shader->End();
	}
}
//=============================================================================
//　Xファイルの読み込み
//=============================================================================
void CModel::SetParts(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		if (m_partstype[nCount] != NULL) { return; }

		switch (nCount)
		{
		case TYPE_CHICKEN:
			m_nModelMax[nCount] = 12;
			m_partstype[nCount] = new PARTS_TYPE[m_nModelMax[nCount]];

			m_partstype[nCount][0] = PARTS_CHICKEN_BODY;
			m_partstype[nCount][1] = PARTS_CHICKEN_HEAD;
			m_partstype[nCount][2] = PARTS_CHICKEN_ASS;
			m_partstype[nCount][3] = PARTS_CHICKEN_WINGSR_0;
			m_partstype[nCount][4] = PARTS_CHICKEN_WINGSR_1;
			m_partstype[nCount][5] = PARTS_CHICKEN_WINGSL_0;
			m_partstype[nCount][6] = PARTS_CHICKEN_WINGSL_1;
			m_partstype[nCount][7] = PARTS_CHICKEN_LEGR;
			m_partstype[nCount][8] = PARTS_CHICKEN_FOOTR;
			m_partstype[nCount][9] = PARTS_CHICKEN_LEGL;
			m_partstype[nCount][10] = PARTS_CHICKEN_FOOTL;
			m_partstype[nCount][11] = PARTS_MAX;

			break;
		case TYPE_CHICK:

			m_nModelMax[nCount] = 6;
			m_partstype[nCount] = new PARTS_TYPE[m_nModelMax[nCount]];

			m_partstype[nCount][0] = PARTS_CHICK_BODY;
			m_partstype[nCount][1] = PARTS_CHICK_HEAD;
			m_partstype[nCount][2] = PARTS_CHICK_LEGR;
			m_partstype[nCount][3] = PARTS_CHICK_FOOTR;
			m_partstype[nCount][4] = PARTS_CHICK_LEGL;
			m_partstype[nCount][5] = PARTS_CHICK_FOOTL;
			break;
		case TYPE_WOOD:
			m_nModelMax[nCount] = 9;
			m_partstype[nCount] = new PARTS_TYPE[m_nModelMax[nCount]];

			m_partstype[nCount][0] = PARTS_WOOD_LEG;
			m_partstype[nCount][1] = PARTS_WOOD_BODY_R;
			m_partstype[nCount][2] = PARTS_WOOD_LONG_R;
			m_partstype[nCount][3] = PARTS_WOOD_SHORT_R;
			m_partstype[nCount][4] = PARTS_WOOD_LEAF_R;
			m_partstype[nCount][5] = PARTS_WOOD_BODY_L;
			m_partstype[nCount][6] = PARTS_WOOD_LONG_L;
			m_partstype[nCount][7] = PARTS_WOOD_SHORT_L;
			m_partstype[nCount][8] = PARTS_WOOD_LEAF_L;

			break;
		}
	}
}
//=============================================================================
//　Xファイルの読み込み
//=============================================================================
void CModel::ParentModel(CModel **&apModel, TYPE type, int nSelectParts)
{
	if (apModel != NULL) { return; }

	apModel = new CModel*[m_nModelMax[type]];
	for (int nCount = 0; nCount < m_nModelMax[type]; nCount++)
	{// 減速鶏のポインタを初期化
		apModel[nCount] = NULL;

		//NULLチェック
		if (apModel[nCount] == NULL)
		{//動的確保
			apModel[nCount] = new CModel;
			//NULLチェック
			if (apModel[nCount] != NULL)
			{
				//モデルの生成
				apModel[nCount]->m_Type = m_partstype[type][nCount];

				if (apModel[nCount]->m_Type == PARTS_MAX)
				{//キャラごとに設定
					apModel[nCount]->m_Type = (CModel::PARTS_TYPE)(PARTS_CHICKEN_11 + nSelectParts);
				}

				apModel[nCount]->Init();
			}
		}
	}
}
//=============================================================================
//　Xファイルの読み込み
//=============================================================================
void CModel::PartsTypeUnLoad(void)
{
	for (int nCount = 0; nCount < TEX_MAX; nCount++)
	{
		if (m_pTexAll[nCount] != NULL)
		{
			m_pTexAll[nCount]->Release();
			m_pTexAll[nCount] = NULL;
		}
	}

	for (int nCount = 0; nCount < PARTS_MAX; nCount++)
	{
		// メッシュの開放
		if (m_pMesh[nCount] != NULL)
		{
			m_pMesh[nCount]->Release();
			m_pMesh[nCount] = NULL;
		}

		// マテリアルの開放
		if (m_pBuffMat[nCount] != NULL)
		{
			m_pBuffMat[nCount]->Release();
			m_pBuffMat[nCount] = NULL;
		}
	}

	for (int nCount = 0; nCount < PARTS_MAX; nCount++)
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

	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		delete[] m_partstype[nCount];
		m_partstype[nCount] = NULL;
	}
}