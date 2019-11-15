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

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

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
	pModel->SetScale(Scale);

	return pModel;
}

//===============================================================================
//　コンストラクタ
//===============================================================================
CModel::CModel()
{
	m_pMesh = NULL;								//メッシュ情報へのポインタ
	m_pBuffMat = NULL;							//マテリアルの情報へのポインタ
	m_nNumMat = 0;								//マテリアルの情報数
	m_VtxMin, m_VtxMax;							//モデルの最小値・最大値
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_FirstPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//初期位置
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	//拡大縮小率
	m_pTextures = NULL;
	m_bTexMat = false;
	m_pMeshMaterials = NULL;
	m_pShaderMeshTextures = NULL;
	m_pToonShader = NULL;

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

	int nNumVtx;				//頂点数
	DWORD sizeFVF;				//頂点フォーマットのサイズ
	BYTE *pVtxBuff;				//頂点バッファへのポインタ

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//位置
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//向き達する
	m_VtxMax = D3DXVECTOR3(-10000, -10000, -10000);			//最大値
	m_VtxMin = D3DXVECTOR3(10000, 10000, 10000);			//最小値

	//頂点数を取得
	nNumVtx = m_pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//頂点バッファのロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//頂点座標の代入

														//最大値
		if (vtx.x > m_VtxMax.x)
		{
			m_VtxMax.x = vtx.x;
		}
		if (vtx.y > m_VtxMax.y)
		{
			m_VtxMax.y = vtx.y;
		}
		if (vtx.z > m_VtxMax.z)
		{
			m_VtxMax.z = vtx.z;
		}
		//最小値
		if (vtx.x < m_VtxMin.x)
		{
			m_VtxMin.x = vtx.x;
		}
		if (vtx.y < m_VtxMin.y)
		{
			m_VtxMin.y = vtx.y;
		}
		if (vtx.z < m_VtxMin.z)
		{
			m_VtxMin.z = vtx.z;
		}

		//サイズ文のポインタを進める
		pVtxBuff += sizeFVF;
	}

	//頂点バッファのアンロック
	m_pMesh->UnlockVertexBuffer();

	m_Rot.y = 0.0f;

	m_pToonShader = NULL;			// シェーダーのポインタの初期化
	m_pMeshMaterials = NULL;
	m_pShaderMeshTextures = NULL;

	//===================================================
	//    　　　　　マテリアルとテクスチャの情報
	//===================================================
	D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	m_pMeshMaterials = new D3DMATERIAL9[(int)m_nNumMat];
	m_pShaderMeshTextures = new LPDIRECT3DTEXTURE9[(int)m_nNumMat];

	for (DWORD MatCount = 0; MatCount < (int)m_nNumMat; MatCount++)
	{
		m_pMeshMaterials[MatCount] = d3dxMaterials[MatCount].MatD3D;
		m_pMeshMaterials[MatCount].Ambient = m_pMeshMaterials[MatCount].Diffuse;
		m_pShaderMeshTextures[MatCount] = NULL;

		if (d3dxMaterials[MatCount].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[MatCount].pTextureFilename) > 0)
		{
			D3DXCreateTextureFromFile(pDevice,
				d3dxMaterials[MatCount].pTextureFilename,
				&m_pShaderMeshTextures[MatCount]);
		}
	}

	//シェーダーの読み込むファイル
	if (m_pToonShader == NULL)
	{
		m_pToonShader = new CToonShader;
		m_pToonShader->Init("data\\SHADER\\ToonShader.fx");
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModel::Uninit(void)
{
	//シェーダー残骸消すもの
	if (m_pMeshMaterials != NULL)
	{
		delete[] m_pMeshMaterials;
		m_pMeshMaterials = NULL;
	}
	if (m_pShaderMeshTextures != NULL)
	{
		delete[] m_pShaderMeshTextures;
		m_pShaderMeshTextures = NULL;
	}

	//シェーダーのポインタの破棄
	if (m_pToonShader != NULL)
	{
		m_pToonShader->Uninit();
		delete m_pToonShader;
		m_pToonShader = NULL;
	}

	// メッシュの開放
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// マテリアルの開放
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//テクスチャの破棄
	if (m_pTextures != NULL)
	{
		//m_pTextures->Release();
		m_pTextures = NULL;
	}
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
		if (m_pToonShader != NULL)
		{
			ShaderTex = m_pToonShader->GetTexture();
			LineTex = m_pToonShader->GetLineTexture();
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//拡大縮小行列の作成
		D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

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

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
		{
			//シェーダのデータにモデルのテクスチャを入れる
			m_pShaderMeshTextures[nCntMat] = m_pTextures;

			//マテリアルの設定
			pMat[nCntMat].MatD3D.Diffuse.a = fAlpha;
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャをNULLにする
			pDevice->SetTexture(0, m_pShaderMeshTextures[nCntMat]);

			//===================================================
			//    　　　　シェーダーの割り当て作業
			//===================================================
			//パスを指定して開始
			Shader->BeginPass(0);

			//シェーダーテクスチャの有無に応じてシェーダーをつける
			if (m_pShaderMeshTextures[nCntMat] != NULL)
			{
				//テクスチャがある場合
				m_bTexMat = true;
				//シェーダーにboolの情報を渡す
				Shader->SetBool("TexMat", m_bTexMat);
			}
			else if (m_pShaderMeshTextures[nCntMat] == NULL)
			{
				//テクスチャがない場合
				m_bTexMat = false;
				//シェーダーにboolの情報を渡す
				Shader->SetBool("TexMat", m_bTexMat);
			}

			//オブジェクトのテクスチャ
			Shader->SetTexture("DecalTexture", m_pShaderMeshTextures[nCntMat]);
			//プロパティー名,NameID,設定する配列の値
			Shader->SetFloatArray("Diffuse", (FLOAT*)&m_pMeshMaterials[nCntMat].Diffuse, 4);

			//変更を基になるディレクトリ ストアに保存
			Shader->CommitChanges();

			//オブジェクト(パーツ)の描画
			m_pMesh->DrawSubset(nCntMat);

			//パスの終了
			Shader->EndPass();
		}

		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);

		//シェーダの終了
		Shader->End();
	}
}

//=============================================================================
//　マテリアルカラーの設定
//=============================================================================
void CModel::SetColor(D3DXCOLOR col)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DMATERIAL9 matDef;	// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;		// マテリアルデータへのポインタ

							// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// 色を付ける
	pMat->MatD3D.Diffuse = col;

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//　Xファイルの読み込み
//=============================================================================
void CModel::CreateXFile(char FileName[40])
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(&FileName[0],
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);
}