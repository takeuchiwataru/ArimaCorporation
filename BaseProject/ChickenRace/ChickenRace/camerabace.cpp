//=============================================================================
//
// カメラベース処理 [camerabace.cpp]
// Author : 佐藤安純
//
//=============================================================================
#include "camerabace.h"
#include "manager.h"
#include "input.h"
#include "scene.h"
#include "player.h"
#include "game.h"
#include "title.h"

//*****************************************************************************
// マクロ定義
//****************************************************************************
#define VECTOL			(2)									//ベクトル
#define MAX_POS			(4)									//参照位置の最大数

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CCamera::CCamera()
{
	m_posV = VECTOR_ZERO;					//視点の初期値
	m_posR = VECTOR_ZERO;					//注視点の初期値
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//法線の向き
	m_rot = VECTOR_ZERO;					//向き
}

//===============================================================================
//　デストラクタ
//===============================================================================
CCamera::~CCamera() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCamera::Init(void)
{
	m_posV = VECTOR_ZERO;						//視点の初期値
	m_posR = VECTOR_ZERO;						//注視点の初期値
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//法線の向き
	m_rot = VECTOR_ZERO;						//向き
	m_fRotDest = 0;
	m_fCameraAngle = 25.0f;						//角度

	m_viewport.X		= 0;								// X開始位置
	m_viewport.Y		= 0;								// Y開始位置
	m_viewport.Width	= SCREEN_WIDTH;						// X開始位置からの大きさ
	m_viewport.Height	= SCREEN_HEIGHT;					// Y開始位置からの大きさ
	m_viewport.MinZ		= 0.0f;								// 最小
	m_viewport.MaxZ		= 1.0f;								// 最大
	
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CCamera::Uninit(void) {}

//=============================================================================
// 更新処理
//=============================================================================
void CCamera::Update(void) {}

//=============================================================================
//　カメラの生成
//=============================================================================
void CCamera::SetCamera()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ビューポート設定
	pDevice->SetViewport(&m_viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH
	(
		&m_mtxProjection,
		D3DXToRadian(m_fCameraAngle),
		(float)m_viewport.Width / (float)m_viewport.Height,
		10.0f,
		30000.0f
	);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// クリッピング処理
//=============================================================================
bool CCamera::Clipping(D3DXVECTOR3 VtxMin, D3DXVECTOR3 VtxMax)
{
	//変数宣言
	D3DXVECTOR3 Reference[MAX_POS];
	D3DXVECTOR3 Pos[VECTOL] = { VECTOR_ZERO, VECTOR_ZERO };
	D3DXVECTOR3 VecA;
	D3DXVECTOR3 VecB;
	bool		bDraw = false;
	float		fAngle = 0.0f;

	//４頂点の設定
	Reference[0] = D3DXVECTOR3(VtxMin.x, 0.0f, VtxMax.z);
	Reference[1] = D3DXVECTOR3(VtxMax.x, 0.0f, VtxMax.z);
	Reference[2] = D3DXVECTOR3(VtxMin.x, 0.0f, VtxMin.z);
	Reference[3] = D3DXVECTOR3(VtxMax.x, 0.0f, VtxMin.z);

	//角度設定
	fAngle = (D3DX_PI * 0.5f) + (m_rot.y - D3DX_PI);
	RemakeAngle(&fAngle);

	//右側の画角の位置を求める
	Pos[0].x = sinf(fAngle) * -1500.0f + m_posV.x;
	Pos[0].z = cosf(fAngle) * -1500.0f + m_posV.z;

	//角度設定
	fAngle = (D3DX_PI * -0.5f) + (m_rot.y - D3DX_PI);
	RemakeAngle(&fAngle);

	//左側の画角の位置を求める
	Pos[1].x = sinf(fAngle) * 1500.0f + m_posV.x;
	Pos[1].z = cosf(fAngle) * 1500.0f + m_posV.z;

	//範囲チェック
	for (int nCntPoint = 0; nCntPoint < MAX_POS; nCntPoint++)
	{
		for (int nCntVec = 0; nCntVec < VECTOL; nCntVec++)
		{
			VecA = Pos[nCntVec] - m_posV;							//カメラの位置から画角の終点のベクトルを求める
			VecB = Reference[nCntPoint] - Pos[nCntVec];				//画角の終点と位置のベクトルを求める			
			float fCross = (VecA.z * VecB.x) - (VecA.x * VecB.z);	//外積を求める

																	//ベクトルより小さいかどうか
			if (fCross <= 0)
			{
				bDraw = true;		//描画状態にする
				return bDraw;
			}
			else
			{
				bDraw = false;		//描画しない状態にする
			}
		}
	}

	return bDraw;
}

//=============================================================================
// 中心点でのクリッピング処理
//=============================================================================
bool CCamera::ClippingPos(D3DXVECTOR3 ObjPos)
{
	//変数宣言
	D3DXVECTOR3 Pos[2];
	D3DXVECTOR3 VecA;
	D3DXVECTOR3 VecB;
	bool		bDraw = false;
	float		fAngle = 0.0f;

	//角度設定
	fAngle = (D3DX_PI * 0.5f) + (m_rot.y - D3DX_PI);
	RemakeAngle(&fAngle);

	//右側の画角の位置を求める
	Pos[0].x = sinf(fAngle) * -1500.0f + m_posV.x;
	Pos[0].z = cosf(fAngle) * -1500.0f + m_posV.z;

	//角度設定
	fAngle = (D3DX_PI * -0.5f) + (m_rot.y - D3DX_PI);
	RemakeAngle(&fAngle);

	//左側の画角の位置を求める
	Pos[1].x = sinf(fAngle) * 1500.0f + m_posV.x;
	Pos[1].z = cosf(fAngle) * 1500.0f + m_posV.z;

	//範囲チェック
	for (int nCntVec = 0; nCntVec < VECTOL; nCntVec++)
	{
		VecA = Pos[nCntVec] - m_posV;							//カメラの位置から画角の終点のベクトルを求める
		VecB = ObjPos - Pos[nCntVec];							//画角の終点と位置のベクトルを求める			
		float fCross = (VecA.z * VecB.x) - (VecA.x * VecB.z);	//外積を求める

																//ベクトルより小さいかどうか
		if (fCross <= 0)
		{
			bDraw = true;		//描画しない状態にする
			break;
		}
		else
		{
			bDraw = false;		//描画する状態にする
		}
	}

	return bDraw;
}

//=============================================================================
// 角度修正
//=============================================================================
void CCamera::RemakeAngle(float * pAngle)
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
// ビューポート処理									(public)	*** CCamera ***
//=============================================================================
void CCamera::SetViewPort(DWORD X, DWORD Y, DWORD Width, DWORD Height)
{
	m_viewport.X = X;		// 位置X
	m_viewport.Y = Y;		// 位置Y
	m_viewport.Width = Width;	// サイズX
	m_viewport.Height = Height;	// サイズY
	m_viewport.MinZ = 0.0f;		// 最小
	m_viewport.MaxZ = 1.0f;		// 最大
}