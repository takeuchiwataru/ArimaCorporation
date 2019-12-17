//=============================================================================
//
// ���U���g�̏��� [result.cpp]
// Author : �L�n ���u
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall�˔j
#include <string.h>				//�X�g�����O�g�p�̂���
#include "result.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "resultcamera.h"
#include "object.h"

#include "resultui.h"
#include "player.h"
#include "model.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define EFFECT_FRAME	(300)		// �G�߂̃G�t�F�N�g�t���[��
#define SEASON_MAX		(4)			// �G�߂̃J�E���g
#define EFFECT_CREATE	(10)		// �G�t�F�N�g��������
#define RESULT_POS		(D3DXVECTOR3(SCREEN_WIDTH /2, SCREEN_HEIGHT / 2 - 80.0f, 0.0f))	// �ʒu(�^�C�g�����S000)
#define RESULT_SIZE		(D3DXVECTOR2(480.0f, 100.0f))										// �T�C�Y(�^�C�g�����S000)
#define LOGO_POS		(D3DXVECTOR3(RESULT_POS.x + 130.0f, RESULT_POS.y + 100.0f, 0.0f))		// �ʒu(�^�C�g�����S001)
#define LOGO_SIZE		(D3DXVECTOR2(125.0f, 35.0f))										// �T�C�Y(�^�C�g�����S001)
#define PRESS_POS		(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT/ 2) + 200.0f, 0.0f))// �ʒu(Press Start Enter)
#define PRESS_SIZE		(D3DXVECTOR2(250.0f, 20.0f))										// �T�C�Y(Press Start Enter)

#define TEXT_OBJECTNAME1		"data\\TEXT\\�Q�[���}�b�v\\objecy.txt"			// �ǂݍ��ރe�L�X�g�t�@�C��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int				CResult::m_nResultCounter = 0;		// �^�C�g���̃J�E���^�[

CResultCamera	*CResult::m_pResultCamera = NULL;

CPlayer			*CResult::m_pPlayer[MAX_MEMBER] = { NULL };

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CResult::CResult()
{
	for (int nCount = 0; nCount < MAX_MAP_OBJECT; nCount++)
		m_Map[nCount] = {};

	m_nResultCounter = 0;

	m_pResultUI = NULL;			// UI���j���[
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CResult::Init()
{
	//===================================
	//		 Load�̓ǂݍ��ݏꏊ
	//===================================
	CFade::Load();		//�t�F�[�h�̃e�N�X�`���̓ǂݍ���
	CResultUI::Load();
	CObject::Load();			//�I�u�W�F�N�g�̃e�N�X�`���̓ǂݍ���
	CToonShader::Load();

	CChick::Load();				//�Ђ悱�̃e�N�X�`���̓ǂݍ���
	CPlayer::Load();			//���f���̓ǂݍ���
	CModel::Load();				//�L�������f���̓ǂݍ���

								//�}�b�v��ǂݍ���
	TextLoad(6);

	//	�ϐ��̏�����
	m_nResultCounter = 0;

	if (m_pResultCamera == NULL)
	{
		m_pResultCamera = new CResultCamera;
		if (m_pResultCamera != NULL) { m_pResultCamera->Init(); }
	}

	// ���U���g
	if (m_pResultUI == NULL)
	{
		m_pResultUI = CResultUI::Create();
	}

	for (int nCount = 0; nCount < m_nSetObjectNum; nCount++)
	{
		//�I�u�W�F�N�g�̐���
		CObject::Create(m_Map[nCount].m_pos, m_Map[nCount].m_rot, m_Map[nCount].m_scale, 0.0f, m_Map[nCount].m_nTexType, m_Map[nCount].m_nType, m_Map[nCount].m_nCollision);
	}

	//D3DXVECTOR3 pos1 = D3DXVECTOR3(-24585.0, -3231.0f, 1208.0f);
	//D3DXVECTOR3 pos2 = D3DXVECTOR3(-24607.0, -3246.0f, 1238.0f);
	//D3DXVECTOR3 pos3 = D3DXVECTOR3(-24562.0, -3257.0f, 1178.0f);

	int *pnCharSelectNum = CGame::GetCharSelectNum();
	int *pnRankingSort = CGame::GetRankingSort();

	D3DXVECTOR3 pos = D3DXVECTOR3(-24409.0, -3290.0f, 1049.0f);

	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// �����o�[�J�E���g
		pos = pos + D3DXVECTOR3(sinf(0.95f + (D3DX_PI * 0.5f)) * 20.0f, 0.0f, cosf(0.95f + (D3DX_PI * 0.5f)) * 20.0f);

		D3DXVECTOR3 posEdit = pos;
		if (nCntMember % 2 == 0)
			posEdit = pos + D3DXVECTOR3(sinf(0.95f) * 20.0f, 0.0f, cosf(0.95f) * 20.0f);

		//�v���C���[�̐���
		if (m_pPlayer[nCntMember] == NULL)
			m_pPlayer[nCntMember] = CPlayer::Create(
				posEdit,
				D3DXVECTOR3(0.0f, (0.95f - (D3DX_PI * 0.5f)), 0.0f),
				(MAX_MEMBER - 1) - nCntMember, 0, pnCharSelectNum[pnRankingSort[nCntMember]], CPlayer::PLAYERTYPE_RESULT);
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CResult::Uninit(void)
{
	//===================================
	//	�@�@UnLoad�̔j������ꏊ
	//===================================
	CFade::UnLoad();	//�t�F�[�h�̃e�N�X�`���̔j��
	CResultUI::Unload();
	CObject::UnLoad();				//�I�u�W�F�N�g�̃e�N�X�`���̔j��

	CChick::UnLoad();				//�Ђ悱�̃e�N�X�`���̔j��
	CPlayer::Unload();			//���f���̓ǂݍ���

	CModel3D::UnLoad();
	CModel3D::ModelShaderDeleter();

	// �^�C�g���J�����̔j��
	if (m_pResultCamera != NULL)
	{
		m_pResultCamera->Uninit();
		delete m_pResultCamera;
		m_pResultCamera = NULL;
	}

	// ���U���g
	if (m_pResultUI != NULL)
	{
		m_pResultUI->Uninit();
		m_pResultUI = NULL;
	}

	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// �����o�[�J�E���g
		if (m_pPlayer[nCntMember] != NULL)
		{
			m_pPlayer[nCntMember]->Uninit();
			m_pPlayer[nCntMember] = NULL;
		}
	}

	//�t�F�[�h�ȊO�폜
	CScene::NotFadeReleseAll();
}
//=============================================================================
// �X�V����
//=============================================================================
void CResult::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(0);					//�W���C�p�b�g�̎擾
	CInputMouse *pCInputMouse = CManager::GetInputMouse();

	//�T�E���h�̏��
	CSound *pSound = CManager::GetSound();

	//�t�F�[�h�̃|�C���g
	CFade::FADE pFade = CFade::GetFade();

	//	�^�C�g���̃J�E���^�[���Z
	m_nResultCounter++;

	if (m_pResultCamera != NULL) { m_pResultCamera->Updata(); }

	if (RESULT_WAIT <= m_nResultCounter)
	{
		if (pFade == CFade::FADE_NONE)
		{
			if (pCInputKeyBoard->GetKeyboardAny(1) == true || pXpad->GetAllTrigger() == true ||
				RESULT_END <= m_nResultCounter)
				CFade::Create(CManager::MODE_TITLE);
		}
	}
	else if (m_nResultCounter < (RESULT_RANK_APP - 15))
	{
		if (pCInputKeyBoard->GetKeyboardAny(1) == true || pXpad->GetAllTrigger() == true)
			m_nResultCounter = (RESULT_RANK_APP - 15);
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CResult::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(157, 184, 224, 255),
		1.0f,
		0);

	//���U���g�J�����̐���
	if (m_pResultCamera != NULL) { m_pResultCamera->SetCamera(); }

	//�S�Ă̕`��
	CScene::DrawAll();
}

//===============================================================================
// �t�@�C�����烍�[�h
//===============================================================================
void CResult::TextLoad(int nLoadNumber)
{
	//�t�@�C���p�ϐ�
	FILE *pFile;		//�t�@�C���|�C���^
	char *pStrcur;		//���݂̐擪�̕�����
	char aLine[256];	//������
	char aStr[256];		//�ꎞ�ۑ�������
	int nIndex = 0;		//���݂̃C���f�b�N�X
	int nWord = 0;		//�|�b�v�ŕԂ��ꂽ�l��ێ�

						//�t�@�C���|�C���^�̏���������
	pFile = NULL;

	//�t�@�C�����J�� �ǂݍ���
	pFile = fopen(TEXT_OBJECTNAME1, "r");

	//NULL�`�F�b�N
	if (pFile != NULL)
	{
		//������̐擪��ݒ�
		pStrcur = ReadLine(pFile, &aLine[0]);
		//����������o��
		strcpy(aStr, pStrcur);

		//������̃f�[�^ ��r���镶���� ��r���镶����
		if (memcmp(pStrcur, "OBJECT_SETNUM = ", strlen("OBJECT_SETNUM = ")) == 0)
		{
			//���o��
			pStrcur += strlen("OBJECT_SETNUM = ");
			//������̐擪��ݒ�
			strcpy(aStr, pStrcur);
			//�����񔲂��o��
			m_nSetObjectNum = atoi(pStrcur);
		}

		//�I�u�W�F�N�g�̐�����
		for (int nCntObject = 0; nCntObject < m_nSetObjectNum; nCntObject++)
		{
			//������̐擪��ݒ�
			pStrcur = ReadLine(pFile, &aLine[0]);
			//����������o��
			strcpy(aStr, pStrcur);

			if (memcmp(pStrcur, "OBJECT_START", strlen("OBJECT_START")) == 0)
			{
				while (1)
				{
					//������̐擪��ݒ�
					pStrcur = ReadLine(pFile, &aLine[0]);

					//��ނ̂�ǂݍ���
					if (memcmp(pStrcur, "TYPE = ", strlen("TYPE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("TYPE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Map[nCntObject].m_nType = atoi(pStrcur);
					}
					//��ނ̂�ǂݍ���
					if (memcmp(pStrcur, "TEXTURETYPE = ", strlen("TEXTURETYPE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("TEXTURETYPE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Map[nCntObject].m_nTexType = atoi(pStrcur);
					}
					//�����蔻���ǂݍ���
					if (memcmp(pStrcur, "COLLISION = ", strlen("COLLISION = ")) == 0)
					{
						//���o��
						pStrcur += strlen("COLLISION = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Map[nCntObject].m_nCollision = atoi(pStrcur);
					}
					//POS��ǂݍ���
					if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
					{
						//���o��
						pStrcur += strlen("POS = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Map[nCntObject].m_pos.x = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Map[nCntObject].m_pos.y = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Map[nCntObject].m_pos.z = (float)atof(pStrcur);

					}
					//ROT��ǂݍ���
					if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
					{
						//���o��
						pStrcur += strlen("ROT = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Map[nCntObject].m_rot.x = (float)atof(pStrcur);

						//���������i�߂�
						pStrcur += nWord;
						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Map[nCntObject].m_rot.y = (float)atof(pStrcur);

						//���������i�߂�
						pStrcur += nWord;
						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Map[nCntObject].m_rot.z = (float)atof(pStrcur);
					}
					//SIZ��ǂݍ���
					if (memcmp(pStrcur, "SIZ = ", strlen("SIZ = ")) == 0)
					{
						//���o��
						pStrcur += strlen("SIZ = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Map[nCntObject].m_scale.x = (float)atof(pStrcur);

						//���������i�߂�
						pStrcur += nWord;
						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Map[nCntObject].m_scale.y = (float)atof(pStrcur);

						//���������i�߂�
						pStrcur += nWord;
						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Map[nCntObject].m_scale.z = (float)atof(pStrcur);
					}
					else if (memcmp(pStrcur, "OBJECT_END", strlen("OBJECT_END")) == 0)
					{
						break;
					}
				}
			}
		}
	}
}

//=============================================================================
//�@�t�@�C���ǂݍ��ݖ�������r��
//=============================================================================
char *CResult::ReadLine(FILE *pFile, char *pDst)
{
	while (1)
	{
		//�P�s���ǂݍ���
		fgets(&pDst[0], 256, pFile);

		//������̃f�[�^ ��r���镶���� ��r���镶����
		if (memcmp(pDst, "#", strlen("#")) == 0)
		{
			pDst += strlen("\n");
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pDst, "\t", strlen("\t")) == 0)
		{
			pDst += strlen("\t");
			while (1)
			{
				if (memcmp(pDst, "\t", strlen("\t")) == 0)
				{
					pDst += strlen("\t");
				}
				else
				{
					break;
				}
			}
			break;
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pDst, " ", strlen(" ")) == 0)
		{
			pDst += strlen(" ");
			while (1)
			{
				if (memcmp(pDst, " ", strlen(" ")) == 0)
				{
					pDst += strlen(" ");
				}
				else
				{
					break;
				}
				break;
			}
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pDst, "\n", strlen("\n")) == 0)
		{
			pDst += strlen("\n");
		}
		else
		{
			break;
		}
	}
	return pDst;
}

//=============================================================================
//�@�t�@�C���ǂݍ��ݐ擪��r��
//=============================================================================
char *CResult::GetLineTop(char * pStr)
{
	while (1)
	{
		//������̃f�[�^ ��r���镶���� ��r���镶����
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{
			pStr += strlen(" ");
			while (1)
			{
				if (memcmp(pStr, " ", strlen(" ")) == 0)
				{
					pStr += strlen(" ");
				}
				else
				{
					break;
				}
				break;
			}
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{
			pStr += strlen("\t");
			while (1)
			{
				if (memcmp(pStr, "\t", strlen("\t")) == 0)
				{
					pStr += strlen("\t");
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}
	return pStr;
}

//=============================================================================
//�@��������Ԃ�
//=============================================================================
int CResult::PopString(char * pStr, char * pDest)
{
	int nWord = 0;

	while (1)
	{	//���o��
		pStr += 1;
		nWord += 1;
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{	//���o��
			pStr = "\0";
			nWord += 1;
			break;
		}
		if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{	//���o��
			pStr = "\0";
			nWord += (int)strlen("\t");
			break;
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pStr, "\n", strlen("\n")) == 0)
		{
			//���o��
			nWord += (int)strlen("\n");
			break;
		}
	}
	strcpy(pDest, pStr);
	//������̐���Ԃ�
	return nWord;
}