//=============================================================================
//
// ���f�����o���� [ModelEffect.cpp]
// Author : Ryo Sugimoto
//
//=============================================================================
#include "ModelEffect.h"
#include "model3D.h"
#include "object.h"
#include "manager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//==================================================================================================//
//    * ���f�����o�̐����֐� *
//==================================================================================================//
CModelEffect	*CModelEffect::Create(D3DXVECTOR3 *pos, D3DXVECTOR3 &move, TYPE type, STATE state)
{
	CModelEffect *pEffect = NULL;
	pEffect = new CModelEffect;

	if (pEffect != NULL)
	{
		pEffect->Init();
		pEffect->Set(pos, move, type, state);
	}

	return pEffect;
}
//==================================================================================================//
//    * ���f�����o�̐����֐� *
//==================================================================================================//
void	CModelEffect::Set(D3DXVECTOR3 *&pos, D3DXVECTOR3 &move, TYPE &type, STATE &state)
{
	D3DXVECTOR3 &m_pos = GetposR();
	float fRot = (CServer::Rand() % 628) * 0.01f;
	float fSize = 10.0f * (1.0f + (CServer::Rand() % 100) * 0.005f);
	float fWK = 1.0f;
	float fRotMove = 0.0f;
	Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_Type = type;
	m_State = state;

	switch (m_Type)
	{
	case TYPE_SMOKE:
		GetScaleR().x = 0.125f;
		SetModelType(MODEL_TYPE_DRIT);
		SetTextureType(TEXTURE_TYPE_DRIT);
		CModel3D::Init();
		m_pos = *pos;
		m_pos += D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * fSize;
		m_move = move;
		m_move.y += 1.0f;
		fRotMove = 0.025f;
		break;
	case TYPE_GRASS:
		GetScaleR().x = 0.085f;
		SetModelType(MODEL_TYPE_GRASS);
		SetTextureType(TEXTURE_TYPE_GRASS);
		CModel3D::Init();
		m_pos = *pos;
		m_pos += D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * fSize;
		m_move = move;
		m_move.y += 1.5f;
		fRotMove = 0.085f;
		break;
	case TYPE_WATER:
	case TYPE_WATER_S:
		m_DrawType = C2D::DRAW_TYPE_ADD;
		fWK = 1.0f + (float)(TYPE_WATER - m_Type);
		GetScaleR().x = fSize * 0.02f * (1.0f + (fWK - 1.0f) * 0.5f);
		SetModelType(MODEL_TYPE_DRIT);
		SetTextureType(TEXTURE_TYPE_WATER);
		CModel3D::Init();
		m_pos = *pos;
		m_pos += D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * fSize;
		m_move = D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * (2.0f / (fWK * 2.0f));
		m_move += move * (1.0f + ((fWK - 1.0f) * 0.5f));
		fRotMove = 0.15f;
		m_move.y += 2.0f * fWK;
		break;
	}
	m_MoveRot = D3DXVECTOR3(
		(CServer::Rand() % 201 - 100) * 0.01f * fRotMove,
		(CServer::Rand() % 201 - 100) * 0.01f * fRotMove,
		(CServer::Rand() % 201 - 100) * 0.01f * fRotMove) * D3DX_PI;
}
//==================================================================================================//
//    * ���f�����o�̏������֐� *
//==================================================================================================//
HRESULT CModelEffect::Init(void)
{
	m_pPos = NULL;
	m_move = INIT_VECTOR;
	m_MoveRot = INIT_VECTOR;
	m_fCntState = 0.0f;
	m_Type = TYPE_MAX;
	m_DrawType = C2D::DRAW_TYPE_NORMAL;
	return S_OK;
}
//==================================================================================================//
//    * ���f�����o�̍X�V�֐� *
//==================================================================================================//
void	CModelEffect::Update(void)
{
	D3DXVECTOR3 &m_pos = GetposR();
	D3DXVECTOR3 &m_rot = GetrotR();
	m_pos += m_move;

	switch (m_Type)
	{
	case TYPE_SMOKE:
		if (m_fCntState > SMOKE_TIME) { Uninit(); return; }
		Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_fCntState / SMOKE_TIME)));
		m_move.x *= 0.99f;
		m_move.z *= 0.99f;
		m_move.y *= 0.95f;
		GetScaleR().x += (1.0f - (m_fCntState / SMOKE_TIME)) * 0.02f;
		m_MoveRot *= 0.975f;
		break;
	case TYPE_GRASS:
		if (m_fCntState > SMOKE_TIME) { Uninit(); return; }
		Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_fCntState / SMOKE_TIME)));
		m_move.x *= 0.99f;
		m_move.z *= 0.99f;
		m_move.y *= 0.95f;
		GetScaleR().x += (1.0f - (m_fCntState / SMOKE_TIME)) * 0.02f;
		m_MoveRot *= 0.95f;
		break;
	case TYPE_WATER:
	case TYPE_WATER_S:
		if (m_fCntState > WATER_TIME) { Uninit(); return; }
		Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_fCntState / WATER_TIME)));
		m_move.x *= 0.99f;
		m_move.z *= 0.99f;
		m_move.y -= WATER_G;
		m_MoveRot *= 0.94f;
		break;
	}
	m_rot += m_MoveRot;
	m_fCntState += 1.0f;
}
//==================================================================================================//
//    * ���f�����o�̕`��֐� *
//==================================================================================================//
void	CModelEffect::Draw(void)
{
	LPDIRECT3DDEVICE9	pD3DDevice = CManager::GetRenderer()->GetDevice();

	C2D::DrawPrepare(m_DrawType, pD3DDevice);

	GetbColR() = true;
	CModel3D::Draw();

	//������߂�
	C2D::DrawPrepare(C2D::DRAW_TYPE_NORMAL, pD3DDevice);
}
