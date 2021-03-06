//=============================================================================
//
// トゥーンシェーダー基本処理 [ToonShader.cpp]
// Author : 有馬　武志
//
//=============================================================================
#include "toonshader.h"
#include "manager.h"
#include "renderer.h"

LPDIRECT3DTEXTURE9	CToonShader::m_pTexture = NULL;				//トゥーンテクスチャの情報
LPDIRECT3DTEXTURE9	CToonShader::m_pTextureMap = NULL;			//マップテクスチャの情報
LPDIRECT3DTEXTURE9	CToonShader::m_pTextureLine = NULL;			//ラインテクスチャの情報
LPDIRECT3DTEXTURE9	CToonShader::m_pTextureMapLine = NULL;		//マップテクスチャの情報

//=============================================================================
// コンストラクタ
//=============================================================================
CToonShader::CToonShader()
{
	//m_pTexture = NULL;
	m_pShader = NULL;
	//m_pTextureLine = NULL;
	//m_pTextureMapLine = NULL;
	//m_pTextureMap = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CToonShader::~CToonShader() {}

//=============================================================================
// 初期化処理
//=============================================================================
void CToonShader::Init(char * FileName)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//シェーダーの読み込み
	D3DXCreateEffectFromFile(pDevice,
		FileName,
		NULL,						//プリプロセッサーを行う構造体
		NULL,						//インクルード操作を行うインターフェイス
		0,							//読み込み時のオプション
		0,							//シェーダ間でやり取りする変数
		&m_pShader,					//シェーダのポインタ
		NULL);						//エラー情報

	//デバック用ライト位置
	//0.22f, -0.87f, 0.44f, 0.0f
	//-0.18f, -0.88f, -0.44f, 0.0f
	//0.89f, -0.11f, 0.44f, 0.0f
	//0.72f, -0.80f, 0.54f, 0.0f

	//ライトの設定
	m_pShader->SetVector("LightPos", &D3DXVECTOR4(0.89f, -0.11f, 0.44f, 0.0f));
}

//=============================================================================
// 終了処理
//=============================================================================
void CToonShader::Uninit(void)
{
	if (m_pTexture != NULL) { m_pTexture = NULL; }
	if (m_pTextureLine != NULL) { m_pTextureLine = NULL; }
	if (m_pTextureMapLine != NULL) { m_pTextureMapLine = NULL; }
	if (m_pTextureMap != NULL) { m_pTextureMap = NULL; }

	//シェーダー情報の破棄
	if (m_pShader != NULL)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}
}
//=============================================================================
// 終了処理
//=============================================================================
void CToonShader::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//シェーダのテクスチャ
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Shader\\Shade.bmp", &m_pTexture)))
	{
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
	}
	//シェーダのラインテクスチャ
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Shader\\outline01.bmp", &m_pTextureLine)))
	{
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
	}
	//シェーダのマップラインテクスチャ
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Shader\\outline05.bmp", &m_pTextureMapLine)))
	{
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
	}
	//シェーダのマップラインテクスチャ
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Shader\\Shade05.bmp", &m_pTextureMap)))
	{
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
	}
}

//=============================================================================
// シェーダーの情報を渡す窓口
//=============================================================================
LPD3DXEFFECT CToonShader::GetShader(void)
{
	return m_pShader;
}

//=============================================================================
// トゥーンテクスチャの情報を渡す窓口
//=============================================================================
LPDIRECT3DTEXTURE9 CToonShader::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
// アウトラインの情報を渡す窓口
//=============================================================================
LPDIRECT3DTEXTURE9 CToonShader::GetLineTexture(void)
{
	return m_pTextureLine;
}

//=============================================================================
// アウトラインの情報を渡す窓口
//=============================================================================
LPDIRECT3DTEXTURE9 CToonShader::GetLineMapTexture(void)
{
	return m_pTextureMapLine;
}

//=============================================================================
// トゥーンテクスチャの情報を渡す窓口
//=============================================================================
LPDIRECT3DTEXTURE9 CToonShader::GetMapTexture(void)
{
	return m_pTextureMap;
}