//=============================================================================
//
// ��ʉ��o���� [DispEffect.cpp]
// Author : Ryo Sugimoto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall�˔j
#include <string.h>
#include "manager.h"
#include "Server.h"
#include "DispEffect.h"
#include "2DPolygon.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CDispEffect::m_pTexAll[TEX_MAX] = {};			// �e�N�X�`���ԍ�

//==================================================================================================//
//    * �ǂݍ��݊֐� *
//==================================================================================================//
void CDispEffect::Load(void)
{
	LPDIRECT3DDEVICE9		pD3DDevice = CManager::GetRenderer()->GetDevice();
	char					aStr[64];
	for (int nCount = 0; nCount < TEX_MAX; nCount++)
	{
		strcpy(aStr, "data/TEXTURE/game/DispEffect/");
		if (m_pTexAll[nCount] == NULL)
		{//�ǂݍ���łȂ��Ȃ�
			switch (nCount)
			{//�ǂݍ��� 
			case TEX_SWAMP:		strcat(aStr, "DispEffect_Water.jpg");	break;
			case TEX_SWAMP_ONE:	strcat(aStr, "WaterDrop.png");			break;
			case TEX_BOOST:		strcat(aStr, "DispEffect_Boost.jpg");	break;
			case TEX_FootSteps:	strcat(aStr, "Footsteps.jpg");			break;
			case TEX_Shadow:	strcat(aStr, "shadow000.jpg");			break;
			case TEX_Pin:		strcat(aStr, "ItemEfc_Pin.jpg");		break;
			case TEX_Light:		strcat(aStr, "ItemEfc_Light.jpg");		break;
			case TEX_Warning:	strcat(aStr, "Warning000.jpg");			break;
			case TEX_White:		strcat(aStr, "White.jpg");				break;
			}
			D3DXCreateTextureFromFile(pD3DDevice, aStr, &m_pTexAll[nCount]);
		}
	}
}
//==================================================================================================//
//    * �j���֐� *
//==================================================================================================//
void CDispEffect::UnLoad(void)
{
	for (int nCount = 0; nCount < TEX_MAX; nCount++)
	{
		if (m_pTexAll[nCount] != NULL)
		{//�I��
			m_pTexAll[nCount]->Release();
			m_pTexAll[nCount] = NULL;
		}
	}
}
//==================================================================================================//
//    * �����֐� *
//==================================================================================================//
CDispEffect	*CDispEffect::Create(CPlayer *pPlayer)
{
	CDispEffect *pEffect = NULL;
	pEffect = new CDispEffect;
	if (pEffect != NULL)
	{
		pEffect->Init();
		pEffect->Set(pPlayer);
	}

	return pEffect;
}
//==================================================================================================//
//    * �������֐� *
//==================================================================================================//
HRESULT CDispEffect::Init(void)
{
	int nCount;

	m_Effect = EFFECT_MAX;
	m_fCntState = 0.0f;
	for (nCount = 0; nCount < EFFECT_MAX; nCount++)
	{//������
		m_pDispAnim[nCount] = NULL;
	}
	for (nCount = 0; nCount < DISP_ANIM_MAX; nCount++)
	{//������
		m_pAnim[nCount] = NULL;
	}

	return S_OK;
}
//==================================================================================================//
//    * �I���֐� *
//==================================================================================================//
void	CDispEffect::Uninit(void)
{
	int nCount;

	for (nCount = 0; nCount < EFFECT_MAX; nCount++)
	{
		if (m_pDispAnim[nCount] != NULL) { m_pDispAnim[nCount]->Uninit(); m_pDispAnim[nCount] = NULL; }
	}
	for (nCount = 0; nCount < DISP_ANIM_MAX; nCount++)
	{//��ʌʉ��o�̔j��
		if (m_pAnim[nCount] != NULL)
		{
			m_pAnim[nCount]->Uninit();
			m_pAnim[nCount] = NULL;
		}
	}

	//���S�t���O�𗧂Ă�
	Release();
}
//==================================================================================================//
//    * �����֐� *
//==================================================================================================//
void	CDispEffect::Update(void)
{
	m_fCntState += 1.0f;
	switch (m_Effect)
	{
	case EFFECT_SWAMP:
		if (m_fCntState > DISP_STIME)
		{//����
			m_fCntState = 0.0f;
			SetAnim();
		}
		break;
	case EFFECT_BOOST:
		if (m_fCntState > DISP_BTIME)
		{//�߂�
			m_Effect = EFFECT_SWAMP;
			SetEffect(EFFECT_MAX);
		}
		break;
	}

	AnimUp();
}
//==================================================================================================//
//    * �����֐� *
//==================================================================================================//
void	CDispEffect::Draw(void)
{
	bool bDraw = false;

	if (CClient::GetpmyClient() != NULL)
	{
		if (CGame::GetCameraNumber() == CClient::GetnID()) { bDraw = true; }
	}
	else
	{
		if (CGame::GetCameraNumber() == m_pPlayer->GetnPlayerNum()) { bDraw = true; }
	}
	if (!bDraw) { return; }

	int nCount;

	for (nCount = 0; nCount < DISP_ANIM_MAX; nCount++)
	{//��ʌʉ��o�̕`��
		if (m_pAnim[nCount] != NULL)
		{
			m_pAnim[nCount]->Draw();
		}
	}
	for (nCount = 0; nCount < EFFECT_MAX; nCount++)
	{//��ʑS�̂̍X�V
		if (m_pDispAnim[nCount] != NULL) { m_pDispAnim[nCount]->Draw(); }
	}
}
//==================================================================================================//
//    * �ݒ�֐� *
//==================================================================================================//
void	CDispEffect::Set(CPlayer *pPlayer)
{
	m_pPlayer = pPlayer;

	for (int nCount = 0; nCount < EFFECT_MAX; nCount++)
	{
		m_pDispAnim[nCount] = NULL;
		switch (nCount)
		{
		case EFFECT_SWAMP:	SetDispSwmp(); break;
		case EFFECT_BOOST:	SetDispBoost(); break;
		}
	}
}
//==================================================================================================//
//    * �G�t�F�N�g�X�V�֐� *
//==================================================================================================//
void	CDispEffect::AnimUp(void)
{
	int nCount;

	for (nCount = 0; nCount < DISP_ANIM_MAX; nCount++)
	{//��ʌʉ��o�̍X�V
		if (m_pAnim[nCount] != NULL)
		{//�ʉ��o�̍X�V
			m_pAnim[nCount]->Update();
			if (m_pAnim[nCount]->GetAnim() == C2DAnim::ANIMATION_END)
			{//�G�t�F�N�g�̏I��
				m_pAnim[nCount]->Uninit();
				m_pAnim[nCount] = NULL;
			}
		}
	}
	for (nCount = 0; nCount < EFFECT_MAX; nCount++)
	{//��ʑS�̂̍X�V

		if (m_pDispAnim[nCount] != NULL)
		{
			if (nCount == EFFECT_BOOST)
				m_pDispAnim[nCount]->Update();
		}
	}
}
//==================================================================================================//
//    * �T�C�Y�擾�֐� *
//==================================================================================================//
void	CDispEffect::GetSize(D3DXVECTOR3 &pos, float &fSizeX, float &fSizeY)
{
	int nPlayer = CGame::GetMaxPlayer();
	int nNumber = m_pPlayer->GetnPlayerNum();
	CClient *pClient = CClient::GetpmyClient();

	if (pClient != NULL || nPlayer <= 1)
	{//�I�����C�� || ��l �Ȃ�
		pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
		fSizeX = 1.0f;
		fSizeY = 1.0f;
	}
	else
	{
		if (nPlayer > 2)
		{//3�l�ȏ�Ȃ�
			fSizeX = 0.5f;
			fSizeY = 0.5f;
			pos = D3DXVECTOR3(
				(SCREEN_WIDTH * 0.25f) + (SCREEN_WIDTH * 0.5f * (nNumber % 2)),
				(SCREEN_HEIGHT * 0.25f) + (SCREEN_HEIGHT * 0.5f * (nNumber / 2)),
				0.0f);
		}
		else
		{//2�l
			fSizeX = 1.0f;
			fSizeY = 0.5f;
			pos = D3DXVECTOR3(
				SCREEN_WIDTH * 0.5f,
				(SCREEN_HEIGHT * 0.25f) + (SCREEN_HEIGHT * 0.5f * nNumber),
				0.0f);
		}
	}
}

//==================================================================================================//
//    * �G�t�F�N�g�^�C�v�̕ύX�֐� *
//==================================================================================================//
void CDispEffect::SetEffect(EFFECT Effect)
{
	int nCount;

	if (m_Effect == EFFECT_BOOST || m_Effect == Effect) { return; }
	m_Effect = Effect;
	m_fCntState = 0.0f;

	for (nCount = 0; nCount < EFFECT_MAX; nCount++)
	{
		if (m_Effect == nCount)
		{
			if (m_pDispAnim[nCount]->Getstate() == C2DAnim::STATE_FADEOUT)
			{//OUT��ԂȂ�IN��
				m_pDispAnim[nCount]->SetState(C2DAnim::STATE_FADEIN, 0.1f);
			}
		}
		else { m_pDispAnim[nCount]->SetState(C2DAnim::STATE_FADEOUT, 0.05f); }
	}
	for (nCount = 0; nCount < DISP_ANIM_MAX; nCount++)
	{//��ʌʉ��o�̕`��
		if (m_pAnim[nCount] != NULL)
		{
			if (m_Effect == nCount || m_Effect == EFFECT_MAX) { m_pAnim[nCount]->GetfStateSpd() = 1.0f; }
			else { m_pAnim[nCount]->GetfStateSpd() = 2.0f; }
		}
	}
	ChangeEffect();
}
//==================================================================================================//
//    * �G�t�F�N�g�ύX���ݒu�֐� *
//==================================================================================================//
void	CDispEffect::ChangeEffect(void)
{
	switch (m_Effect)
	{
	case EFFECT_SWAMP:
		float fRot = (CServer::Rand() % 628) * 0.01f;
		for (int nCount = 0; nCount < DISP_SSET; nCount++)
		{
			SetSwmp(SetAnim(false), fRot);
			fRot += (D3DX_PI * 2.0f) / DISP_SSET;
		}
		break;
	}
}
//==================================================================================================//
//    * �����܂�G�t�F�N�g(�S�̌���)�ݒu�֐� *
//==================================================================================================//
void	CDispEffect::SetDispSwmp(void)
{
	if (m_pDispAnim[EFFECT_SWAMP] != NULL) { return; }
	D3DXVECTOR3 pos;
	float fSizeX, fSizeY;

	GetSize(pos, fSizeX, fSizeY);
	m_pDispAnim[EFFECT_SWAMP] = C2DAnim::Create();
	m_pDispAnim[EFFECT_SWAMP]->Set(pos, SCREEN_WIDTH * 0.5f * fSizeX, SCREEN_HEIGHT * 0.5f * fSizeY, D3DX_PI,
		0, 1, 5, 6, C2DAnim::ANIMATION_LOOP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), C2D::DRAW_TYPE_SUBTRACT);
	m_pDispAnim[EFFECT_SWAMP]->SetState(C2DAnim::STATE_FADEOUT, 0.05f);
	m_pDispAnim[EFFECT_SWAMP]->BindTexture(m_pTexAll[TEX_SWAMP]);
}
//==================================================================================================//
//    * �����G�t�F�N�g(�S�̌���)�ݒu�֐� *
//==================================================================================================//
void	CDispEffect::SetDispBoost(void)
{
	if (m_pDispAnim[EFFECT_BOOST] != NULL) { return; }
	D3DXVECTOR3 pos;
	float fSizeX, fSizeY;

	GetSize(pos, fSizeX, fSizeY);
	m_pDispAnim[EFFECT_BOOST] = C2DAnim::Create();
	m_pDispAnim[EFFECT_BOOST]->Set(pos, SCREEN_WIDTH * 0.5f * fSizeX, SCREEN_HEIGHT * 0.5f * fSizeY, D3DX_PI,
		0, 1, 4, 4, C2DAnim::ANIMATION_LOOP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), C2D::DRAW_TYPE_ADD);
	m_pDispAnim[EFFECT_BOOST]->SetState(C2DAnim::STATE_FADEOUT, 0.05f);
	m_pDispAnim[EFFECT_BOOST]->BindTexture(m_pTexAll[TEX_BOOST]);
}
//==================================================================================================//
//    * �G�t�F�N�g(�ʌ���)�ݒu�֐� *
//==================================================================================================//
C2DAnim	*&CDispEffect::SetAnim(bool bSet)
{
	for (int nCount = 0; nCount < DISP_ANIM_MAX; nCount++)
	{
		if (m_pAnim[nCount] == NULL)
		{//�󂫂������Đ���

			if (!bSet) { return m_pAnim[nCount]; }
			switch (m_Effect)
			{
			case EFFECT_SWAMP:	SetSwmp(m_pAnim[nCount]);	break;
			case EFFECT_BOOST:	break;
			}
			break;
		}
	}
	return m_pAnim[0];
}
//==================================================================================================//
//    * �����܂�G�t�F�N�g(�ʌ���)�ݒu�֐� *
//==================================================================================================//
void	CDispEffect::SetSwmp(C2DAnim *&pAnim, float fRot)
{
	if (pAnim != NULL) { return; }
	D3DXVECTOR3 pos, plus;
	float fSizeX, fSizeY, fSize;
	float fDisX, fDisY, fCol;

	fCol = 0.025f;
	fSize = DISP_SSIZE * (1.0f + (CServer::Rand() % 100) * 0.005f);
	if (fRot <= -10.0f) { fRot = (CServer::Rand() % 628) * 0.01f; }
	else { fSize *= 1.15f; fCol *= 0.75f; }

	fDisX = 0.3f + (CServer::Rand() % 10) * 0.006f;
	fDisY = 0.3f + (CServer::Rand() % 10) * 0.006f;

	GetSize(pos, fSizeX, fSizeY);
	plus = D3DXVECTOR3(
		sinf(fRot) * SCREEN_WIDTH * fDisX * fSizeX,
		cosf(fRot) * SCREEN_HEIGHT * fDisY * fSizeY,
		0.0f);

	pos += plus;
	pAnim = C2DAnim::Create();
	pAnim->Set(pos, fSize * fSizeX, fSize * fSizeY, D3DX_PI,
		0, 1, 1, 1, C2DAnim::ANIMATION_LOOP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), C2D::DRAW_TYPE_NORMAL);
	pAnim->SetState(C2DAnim::STATE_FADEIN_D, fCol);
	pAnim->BindTexture(m_pTexAll[TEX_SWAMP_ONE]);
}