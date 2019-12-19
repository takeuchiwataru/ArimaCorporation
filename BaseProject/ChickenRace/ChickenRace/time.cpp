//=============================================================================
//
// �^�C�}�[���� [time.cpp]
// Author : keisuke komatsu
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall�˔j
#include <string.h>				//�X�g�����O�g�p�̂���
#include "manager.h"
#include "renderer.h"
#include "time.h"
#include "number.h"
#include "game.h"
#include "fade.h"
#include "gamecamera.h"
#include "tutorial.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define CENTER_WIDTH		(SCREEN_WIDTH / 2)				// X�̒��S���W
#define TIMER_SPACE			(50.0f)							// �����Ɛ����̊Ԃ̃T�C�Y(�Q�[������)
#define TIMER_POSITION_Y	(70.0f)							// �^�C�}�[��Y���W(�Q�[������)
#define TIME_SPACE			(40.0f)							// �����Ɛ����̊Ԃ̃T�C�Y(���q�l�̎���)
#define TIME_POSITION_Y		((SCREEN_HEIGHT / 2) - 80.0f)	// �^�C�}�[��Y���W(���q�l�̎���)
#define TIME_POSITION_X		((SCREEN_WIDTH / 2) + 180.0f)	// �^�C�}�[��X���W(���q�l�̎���)
#define GAME_TIME			(25)							// �Q�[�����̎���
#define POWER_X				(10)
#define DIRECTING_FRAME		(2)							// Time Up �\���̃t���[������
//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	CTime::m_pTexture[CTime::TEXTURE_MAX] = { NULL };

int						CTime::m_nTime = 0;
int						CTime::m_nCutomerTime = 0;
bool					CTime::m_bRideEnd = false;
CTime::STATETIME		CTime::m_stateTime = STATE_NONE;
CTime::STATECUTOMERADD	CTime::m_stateCutomer = STATECUTOMERADD_NONE;

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CTime::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// �e�N�X�`���J�E���g
		char cName[128] = {};

		switch (nCntTex)
		{
		case TEXTURE_COLON:
			strcpy(cName, "data/TEXTURE/number/colon.png");
			break;
		}

		// �e�N�X�`���̐���
		if (m_pTexture[nCntTex] == NULL)
			D3DXCreateTextureFromFile(pDevice, cName, &m_pTexture[nCntTex]);
	}

	CNumber::Load();

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CTime::Unload(void)
{
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// �e�N�X�`���J�E���g
	 //�e�N�X�`���[�̔j��
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}

	CNumber::Unload();
}

//=============================================================================
// ��������
//=============================================================================
CTime *CTime::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CTime *pTime = NULL;

	//NULL�`�F�b�N
	if (pTime == NULL)
	{//���I�m��
		pTime = new CTime;

		//NULL�`�F�b�N
		if (pTime != NULL)
		{
			//�ʒu�̐ݒ�
			pTime->m_pos = pos;
			pTime->m_size = size;
			//����������
			pTime->Init();
		}
	}

	return pTime;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTime::CTime(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	for (int nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
		m_apNumber[nCntTime] = NULL;

	for (int nCntColon = 0; nCntColon < MAX_COLON; nCntColon++)
		m_pColon[nCntColon] = NULL;

	m_nTimeCount = 0;
	m_nCutomerTime = 0;
	m_nCutmoerCount = -1;
	m_nCutmoerNum = 0;
	m_nCntDirecting = 0;
	m_bUse = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTime::~CTime() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTime::Init(void)
{
	m_bUse = false;
	int nTexData = 0;

	for (int nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
	{	// �^�C�}�[�����ݒ�
		m_apNumber[nCntTime] = new CNumber;
		m_apNumber[nCntTime]->Init(D3DXVECTOR3(m_pos.x - ((m_size.x * 2.0f) * nCntTime) - ((m_size.x * 2.0f) * (nCntTime / 2)), m_pos.y, m_pos.z), m_size, 0);
	}

	for (int nCntColon = 0; nCntColon < MAX_COLON; nCntColon++)
	{
		m_pColon[nCntColon] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
		m_pColon[nCntColon]->Init();
		m_pColon[nCntColon]->SetPosSize(
			D3DXVECTOR3(m_pos.x - ((m_size.x * 2.0f) * ((nCntColon + 1) * 2.0f)) - ((m_size.x * 2.0f) * (nCntColon)), m_pos.y, m_pos.z),
			D3DXVECTOR2(m_size.x * 0.8f, m_size.y * 0.8f));
		m_pColon[nCntColon]->BindTexture(m_pTexture[TEXTURE_COLON]);
	}

	// �����̃e�N�X�`���ݒ�
	TexTime(0);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTime::Uninit(void)
{
	for (int nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
	{	// �^�C�}�[�̔j��
		if (m_apNumber[nCntTime] != NULL)
		{
			m_apNumber[nCntTime]->Uninit();
			m_apNumber[nCntTime] = NULL;
		}
	}

	for (int nCntColon = 0; nCntColon < MAX_COLON; nCntColon++)
	{
		if (m_pColon[nCntColon] != NULL)
		{
			m_pColon[nCntColon]->Uninit();
			m_pColon[nCntColon] = NULL;
		}
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTime::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CTime::Draw(void)
{
	for (int nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
	{
		if (m_apNumber[nCntTime] != NULL)
		{
			m_apNumber[nCntTime]->Draw();
		}
	}
}

//=============================================================================
// �^�C�}�[��Texture�Ǘ�
//=============================================================================
void CTime::TexTime(int nTexData, bool bEdit)
{
	int nNum = nTexData;

	if (bEdit == true)
	{
		char cNum[5] = {};
		char cSet[5] = {};
		int nEdit = 0;
		nEdit = (int)((float)99 * (float)((float)((nTexData) % 60) / (float)60));
		nEdit %= 10;
		sprintf(cSet, "%d", nEdit);
		cNum[4] = cSet[0];

		nEdit = (int)((float)99 * (float)((float)((nTexData) % 60) / (float)60));
		nEdit /= 10;
		sprintf(cSet, "%d", nEdit);
		cNum[3] = cSet[0];

		nEdit = (int)((nTexData) / 60) % 60;
		nEdit %= 10;
		sprintf(cSet, "%d", nEdit);
		cNum[2] = cSet[0];

		nEdit = (int)((nTexData) / 60) % 60;
		nEdit /= 10;
		sprintf(cSet, "%d", nEdit);
		cNum[1] = cSet[0];

		nEdit = (int)((nTexData) / 60) / 60;
		sprintf(cSet, "%d", nEdit);
		cNum[0] = cSet[0];

		nNum = atoi(cNum);
	}

	for (int nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
	{// �e�N�X�`���ɔ��f
		if (m_apNumber[nCntTime] != NULL)
		{
			m_apNumber[nCntTime]->SetNumber(nNum);

			nNum /= 10;
		}
	}
}

//=============================================================================
// �^�C�����Z����
//=============================================================================
void CTime::AddTime(int nTime)
{
	if (m_nTime > 0)
	{
		m_nTime += nTime;
	}
	m_stateTime = STATE_TIMEADD;
}

//=============================================================================
// �ʒu
//=============================================================================
void CTime::Setpos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	for (int nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
	{	// �^�C�}�[�̔j��
		if (m_apNumber[nCntTime] != NULL)
		{
			m_apNumber[nCntTime]->SetSize(D3DXVECTOR2(m_size.x, m_size.y), D3DXVECTOR2(m_pos.x - ((m_size.x * 2.0f) * nCntTime) - ((m_size.x * 2.0f) * (nCntTime / 2)), m_pos.y));
		}
	}

	for (int nCntColon = 0; nCntColon < MAX_COLON; nCntColon++)
	{
		if (m_pColon[nCntColon] != NULL)
		{
			m_pColon[nCntColon]->SetPosSize(
				D3DXVECTOR3(m_pos.x - ((m_size.x * 2.0f) * ((nCntColon + 1) * 2.0f)) - ((m_size.x * 2.0f) * (nCntColon)), m_pos.y, m_pos.z),
				D3DXVECTOR2(m_size.x * 0.8f, m_size.y * 0.8f));
		}
	}
}

//=============================================================================
// �F
//=============================================================================
void CTime::Setcol(D3DXCOLOR col)
{
	for (int nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
	{	// �^�C�}�[�̔j��
		if (m_apNumber[nCntTime] != NULL)
		{
			m_apNumber[nCntTime]->SetCol(col);
		}
	}

	for (int nCntColon = 0; nCntColon < MAX_COLON; nCntColon++)
	{
		if (m_pColon[nCntColon] != NULL)
		{
			m_pColon[nCntColon]->SetColor(&col);
		}
	}
}