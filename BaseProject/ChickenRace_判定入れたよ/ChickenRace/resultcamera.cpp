//==============================================================================
//
// リザルトカメラ処理 [resultcamera.cpp]
// Auther : shun yokomichi
//
//==============================================================================
#include "resultcamera.h"
#include "game.h"

//===============================================================================
//　マクロ定義
//===============================================================================

//===============================================================================
//　コンストラクタ
//===============================================================================
CResultCamera::CResultCamera() {}

//===============================================================================
//　デストラクタ
//===============================================================================
CResultCamera::~CResultCamera() {}

//===============================================================================
//　初期化
//===============================================================================
HRESULT CResultCamera::Init(void)
{
	CCamera::Init();
	m_posV = D3DXVECTOR3(650.0f, 50.0f, 0.0f);	//視点の初期値
	m_posR = D3DXVECTOR3(300.0f, 0.0f, -400.0f);		//注視点の初期値
	m_fRotDest = 0;
	return S_OK;
}

//===============================================================================
//　終了処理
//===============================================================================
void CResultCamera::Uninit(void)
{
	CCamera::Uninit();
}

//===============================================================================
//　更新処理
//===============================================================================
void CResultCamera::Updata(void)
{

}
//===============================================================================
//　カメラの設定
//===============================================================================
void CResultCamera::SetCamera(void)
{
	CCamera::SetCamera();
}