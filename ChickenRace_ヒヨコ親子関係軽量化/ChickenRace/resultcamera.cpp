//==============================================================================
//
// リザルトカメラ処理 [resultcamera.cpp]
// Auther : 有馬 武志
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
	m_posR = D3DXVECTOR3(-450.0f, -30.0f, 0.0f);	//視点の初期値
	m_posV = D3DXVECTOR3(100.0f, 80.0f, -1.0f);		//注視点の初期値
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