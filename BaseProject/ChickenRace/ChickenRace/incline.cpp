//=============================================================================
//
// 傾斜処理 [incline.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "incline.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "meshField.h"

//==================================
// マクロ定義
//==================================

//==================================
// 静的メンバ変数宣言
//==================================

//==================================
// 生成処理
//==================================
CIncline* CIncline::Create(void)
{
	CIncline *pIncline;

	pIncline = new CIncline;	// メモリを確保

	if (pIncline != NULL)
	{// メモリ確保成功
		pIncline->Init();
	}

	return pIncline;
}

//=========================================
// コンストラクタ
//=========================================
CIncline::CIncline()
{
	m_pField = NULL;
	m_fJumpHeight = 0.0f;
	m_fSpeed = 0.0f;
	m_bJump = false;
	m_fClimbSpeed = 0.0f;
	m_fAngle = 0.0f;
	m_norField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=========================================
// デストラクタ
//=========================================
CIncline::~CIncline()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CIncline::Init(void)
{
	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CIncline::Uninit(void)
{
	// 自分を破棄
	delete this;
}

//=========================================
// 更新処理
//=========================================
void CIncline::Update(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSpeed)
{
	m_fSpeed = fSpeed;
	m_fJumpHeight = 0.0f;

	if (NULL != m_pField)
	{// 地面のポインタがNULLでないとき
		const float fAngleOld = m_fAngle;					// 自分の向いている方向に対しての傾斜の角度
		const float fSpeedOld = m_fSpeed;

		m_fAngle = m_pField->GetInclineAngle(pos, rot.y);	// 自分の向いている方向に対しての傾斜の角度の取得

		if (m_fAngle > 0.0f)
		{// 下りるときに速度を上げる
			m_fSpeed += fSpeed * m_fAngle * m_fClimbSpeed;
		}
		else if (m_fAngle < 0.0f)
		{// 上るときに速度を下げる
			m_fSpeed -= fSpeed * fabsf(m_fAngle) * m_fClimbSpeed;
			if (1.0f > m_fSpeed) { m_fSpeed = 1.0f; }
		}

		if (m_fAngle > fAngleOld && fAngleOld <= 0.0f)
		{// ジャンプするとき
			m_fJumpHeight = (fabsf(fAngleOld) + 1.0f) * m_fSpeed * 5.0f;
			if (m_fJumpHeight > 0.0f)
			{// 上に飛んだ時ジャンプ状態にする
				m_bJump = true;
				m_fSpeed = fSpeedOld;
			}
		}
	}
}

//=========================================
// 傾斜に沿った角度の取得
//=========================================
D3DXMATRIX CIncline::GetRot(D3DXVECTOR3 pos, D3DXMATRIX mtxRot)
{
	D3DXMATRIX mtxRot1, mtxRot2;
	if (NULL != m_pField)
	{// 地面のポインタがNULLでないとき
		if (!m_bJump) { m_norField = m_pField->GetNor(pos); }
		if (isnan(m_norField.x) || isnan(m_norField.z) || isnan(m_norField.y))
		{
			m_norField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		D3DXMatrixRotationYawPitchRoll(&mtxRot1, 0.0f, m_norField.z, -m_norField.x);

		D3DXMatrixMultiply(&mtxRot2, &mtxRot, &mtxRot1);
	}

	return mtxRot2;
}