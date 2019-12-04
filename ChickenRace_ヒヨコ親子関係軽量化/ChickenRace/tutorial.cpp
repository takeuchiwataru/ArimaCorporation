//=============================================================================
//
// �`���[�g���A���̏��� [tutorial.cpp]
// Author : ���� ����
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall�˔j
#include <stdio.h>				//�C���N���h�t�@�C��

#include "tutorial.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "meshfield.h"
#include "gamecamera.h"
#include "player.h"
#include "loadText.h"
#include "object.h"
#include "loadText.h"
#include "time.h"
#include "wall.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define VECTOR_ZERO				(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define TEXT_MESHFIELDNAME1		"data\\TEXT\\�X�e�[�W6\\meshfield.txt"	// ���b�V���t�B�[���h�e�L�X�g�t�@�C��
#define TEXT_OBJECTNAME1		"data\\TEXT\\�X�e�[�W6\\objecy.txt"		// �I�u�W�F�N�g�̃e�L�X�g�t�@�C��
#define TEXT_PLAYER_MOTION		"data\\TEXT\\Player\\Player.txt"		// �v���C���[�̃��[�V�����t�@�C��
#define TEX_LINE				"data\\TEXTURE\\Tutorial\\Line.png"		// ���̓e�N�X�`���t�@�C����
#define TEXT_EFFECT_TOOL		"data\\TEXT\\Particle.txt"				// �G�t�F�N�g���
#define TEXT_BOY_MOTION			"data\\TEXT\\���q����\\motion_boy.txt"	// �j�����[�V�����t�@�C��
#define TEXT_GIRL_MOTION		"data\\TEXT\\���q����\\motion_girl.txt"	// �������[�V�����t�@�C��
#define TEXT_OBJBILL			"data\\TEXT\\�X�e�[�W6\\objbillboad.txt"	// �I�u�W�F�N�g�r���{�[�h���
#define TEXT_WALLNAME1			"data\\TEXT\\�X�e�[�W6\\wall.txt"			// �ǂݍ��ރe�L�X�g�t�@�C��
#define TEXT_CUSTOMER_SUMMER	"data\\TEXT\\���q����\\Summer.txt"			// ���q����̏��i�āj
#define TIME					(60 * 5)								// �\������
#define ACCLE_TIME				(100)									// �A�N�Z������
#define BREAK_TIME				(50)									// �u���[�L����
#define HANDLE_TIME				(50)									// �\������
#define END_TIME				(60 * 2)								// �\������
#define KEY_TIME				(30)									// �L�[�̐؂�ւ�����									

//�t���[��
#define FLAM_POS	(D3DXVECTOR3(640.0f, 610.0f, 0.0f))
#define FLAM_SIZE	(D3DXVECTOR2(560.0f, 120.0f))

//���S
#define LOGO_POS	(D3DXVECTOR3(240.0f, 520.0f, 0.0f))
#define LOGO_SIZE	(D3DXVECTOR2(135.0f, 67.5f))

//�A�C�R��
#define ICON_POS	(D3DXVECTOR3(260.0f, 630.0f, 0.0f))
#define ICON_SIZE	(D3DXVECTOR2(48.0f, 48.0f))

//�X�y�[�X�L�[
#define SPACE_POS	(D3DXVECTOR3(1020.0f, 660.0f, 0.0f))
#define SPACE_SIZE	(D3DXVECTOR2(90.0f, 15.0f))

//X�{�^��
#define X_POS	(D3DXVECTOR3(1050.0f, 660.0f, 0.0f))
#define X_SIZE	(D3DXVECTOR2(30.0f, 20.0f))

//�R���g���[���[�{�^��
#define CONTROL_POS	(D3DXVECTOR3(640.0f, 650.0f, 0.0f))
#define CONTROL_SIZE	(D3DXVECTOR2(25.0f, 25.0f))

//�R���g���[���[�{�^��
#define STICK_POS	(D3DXVECTOR3(640.0f, 650.0f, 0.0f))
#define STICK_SIZE	(D3DXVECTOR2(40.0f, 40.0f))

//�M�A
#define GEAR_POS	(D3DXVECTOR3(1200.0f, 400.0f, 0.0f))
#define GEAR_SIZE	(D3DXVECTOR2(60.0f, 60.0f))

//�X�L�b�v�{�^��
#define SKIP_POS	(D3DXVECTOR3(635.0f, 35.0f, 0.0f))
#define SKIP_SIZE	(D3DXVECTOR2(140.0f, 30.0f))

//����
#define LINE_POS	(D3DXVECTOR3(640.0f, 600.0f, 0.0f))
#define LINE_SIZE	(D3DXVECTOR2(300.0f, 100.0f))
#define UV			(0.04545f)

//�C���[�W�摜
#define IMAGE_POS	(D3DXVECTOR3(200.0f, 315.0f, 0.0f))
#define IMAGE_SIZE	(D3DXVECTOR2(195.0f, 145.0f))
#define IMAGE_ADDSIZE	(D3DXVECTOR2(4.0f, 3.0f))

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CMeshField * CTutorial::m_pMeshField = NULL;
CGameCamera * CTutorial::m_pCamera = NULL;
CPlayer * CTutorial::m_pPlayer = NULL;
CLoadTextMotion * CTutorial::m_pPlayerMotion = NULL;
CGround * CTutorial::m_pGround = NULL;
CLoadTextEffect * CTutorial::m_pLoadEffect = NULL;
CLoadTextMotion * CTutorial::m_pBoyMotion = NULL;
CLoadTextMotion * CTutorial::m_pGirlMotion = NULL;
CLogo * CTutorial::m_pScoreUI[MAX_SCORE_UI_TUTORIAL] = {};
bool CTutorial::m_bCustomer = false;
CTutorial::TYPE CTutorial::m_type = TYPE_START;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CTutorial::CTutorial(){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTutorial::~CTutorial(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTutorial::Init()
{
	//�ϐ��̏�����
	m_pSpace = NULL;
	m_pImage = NULL;
	m_pSkip = NULL;
	m_pBotunn = NULL;
	m_bInput = CManager::GetInputFlag();
	m_nCountTimeKey = 0;
	m_nAinNum = 0;
	
	//--------------------
	// Load
	//--------------------
	CFade::Load();					//�t�F�[�h�e�N�X�`��
	CMeshField::Load();				//���b�V���t�B�[���h�e�N�X�`��
	CObject::Load();				//�I�u�W�F�N�g�e�N�X�`��
	CShadow::Load();				//�e�̃e�N�X�`��
	CWall::Load();					//�ǂ̓ǂݍ���

	if (m_pPlayerMotion == NULL) { m_pPlayerMotion = CLoadTextMotion::Create(TEXT_PLAYER_MOTION); }	//�v���C���[�̃��[�V�����ǂݍ���
	//CPlayer::LoadModel();		//���f���̓ǂݍ���

	if (m_pBoyMotion == NULL) { m_pBoyMotion = CLoadTextMotion::Create(TEXT_BOY_MOTION); }		//�j�����[�V�����ǂݍ���
	if (m_pGirlMotion == NULL) { m_pGirlMotion = CLoadTextMotion::Create(TEXT_GIRL_MOTION); }	//�������[�V�����ǂݍ���

	//--------------------
	// Create
	//--------------------
	ObjectCreate();		//�I�u�W�F�N�g�̐���

	//�ǂ̓ǂݍ���
	WallTexLoad();

	MeshFildCreate();	//�n�ʂ̐���

	//�v���C���[�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�J�����̐���
	if (m_pCamera == NULL)
	{
		m_pCamera = new CGameCamera;
		if (m_pCamera != NULL) { m_pCamera->Init(); }
	}

	//�ϐ��̏�����
	m_type = TYPE_START;
	m_nCountTime = 0;
	m_bCustomer = false;

	//���̓|���S���̐���
	CreateLine();

	//�O�i�A��ރ{�^���𑀍�ł��Ȃ��悤�ɂ���
	m_pPlayer->SetDrive(false);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTutorial::Uninit(void)
{
	//-------------------
	// Unload
	//-------------------
	CFade::UnLoad();					//�t�F�[�h�e�N�X�`��
	CMeshField::UnLoad();				//���b�V���t�B�[���h�e�N�X�`��
	//CPlayer::UnloadModel();				//�v���C���[���f��
	CObject::UnLoad();					//�I�u�W�F�N�g�̃e�N�X�`���̔j��
	CShadow::UnLoad();					//�e�e�N�X�`��
	CWall::UnLoad();					//�ǂ̃e�N�X�`���j��
	//-------------------
	// Uninit
	//-------------------
	//�v���C���[�̃��[�V�����̔j��
	if (m_pPlayerMotion != NULL)
	{
		m_pPlayerMotion->Uninit();
		delete m_pPlayerMotion;
		m_pPlayerMotion = NULL;
	}

	//�j���̃��[�V�����j��
	if (m_pBoyMotion != NULL)
	{
		m_pBoyMotion->Uninit();
		delete m_pBoyMotion;
		m_pBoyMotion = NULL;
	}

	//�������[�V�����̔j��
	if (m_pGirlMotion != NULL)
	{
		m_pGirlMotion->Uninit();
		delete m_pGirlMotion;
		m_pGirlMotion = NULL;
	}

	if (m_pPlayer != NULL) { m_pPlayer = NULL; }	//�v���C���[�̔j��
	if (m_pGround != NULL) { m_pGround = NULL; }	//��̔j��

	//�J����
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	UninitBottun();
	UninitControlBottun();

	//�t�F�[�h�ȊO�폜
	CScene::NotFadeReleseAll();

	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	if (m_pLine != NULL) { m_pLine = NULL; }					//���̓|���S���̔j��
}

//=============================================================================
// �X�V����
//=============================================================================
void CTutorial::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputXPad *pXpad = CManager::GetXInput();

	if (pXpad->GetTrigger(XINPUT_GAMEPAD_START, 0) == true ||
		pCInputKeyBoard->GetKeyboardTrigger(DIK_SPACE) == true)
	{//�^�C�g������Q�[����
	 //�t�F�[�h���n�܂�����
		UpdateFade();
	}

	//�J�����̍X�V
	if (m_pCamera != NULL) { m_pCamera->Update(); }
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTutorial::Draw(void)
{
	//�J�����̐ݒ�
	if (m_pCamera != NULL) { m_pCamera->SetCamera(); }
}

//=============================================================================
// �t�F�[�h�̍X�V����
//=============================================================================
void CTutorial::UpdateFade(void)
{
	//�T�E���h�̏��
	CSound *pSound = CManager::GetSound();

	//�t�F�[�h�̃|�C���g
	CFade::FADE pFade = CFade::GetFade();

	if (pFade == CFade::FADE_NONE)
	{
		//�|�[�Y�̑I���̌��艹
		//pSound->PlaySound(CSound::SOUND_LABEL_SE_TITLE_ENTER);
		CFade::Create(CManager::MODE_GAME);
	}
}

//=============================================================================
// �{�^���̔j��
//=============================================================================
void CTutorial::UninitBottun(void)
{

}

//=============================================================================
// �R���g���[���[�{�^���̔j��
//=============================================================================
void CTutorial::UninitControlBottun(void)
{

}

//=============================================================================
// �{�^���̐���
//=============================================================================
void CTutorial::CreateBottun(void)
{

}

//=============================================================================
// �X�L�b�v�L�[�̐���
//=============================================================================
void CTutorial::CreateSkip(bool bInput)
{

}

//=============================================================================
// �A�j���[�V�����X�V����
//=============================================================================
void CTutorial::UpdateAnim(void)
{
	
}

//=============================================================================
//  ���͂̐؂�ւ�����
//=============================================================================
void CTutorial::ChangeLine(void)
{
	//��Ԃ̉��Z
	m_type = (TYPE)(m_type + 1);

	//UV�̐ݒ�
	if (m_pLine != NULL) { m_pLine->SetUV(UV, m_type); }
}

//=============================================================================
// ���͂̐���
//=============================================================================
void CTutorial::CreateLine(void)
{
	//�ϐ��̏�����
	m_pTexture = NULL;
	m_pLine = NULL;

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{//	�t���O�̏�����
		m_bLine[nCnt] = false;
	}

	//�e�N�X�`���̐���
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), TEX_LINE, &m_pTexture);

	//�|���S���̐���
	if (m_pLine == NULL)
	{
		m_pLine = new CScene2D(6, CScene::OBJTYPE_LOGO);

		if (m_pLine != NULL)
		{
			m_pLine->Init();							//����������
			m_pLine->SetPosSize(LINE_POS, LINE_SIZE);	//�ʒu�ƃT�C�Y�̐ݒ�
			m_pLine->BindTexture(m_pTexture);			//�e�N�X�`���̐ݒ�
			m_pLine->SetUV(UV, m_type);					//UV�̐ݒ�
		}
	}
}

//===============================================================================
// ���b�V���t�B�[���h���t�@�C�����烍�[�h
//===============================================================================
void CTutorial::MeshFildCreate(void)
{
	//�t�@�C���p�ϐ�
	FILE *pFile;		//�t�@�C���|�C���^
	char *pStrcur;		//���݂̐擪�̕�����
	char aLine[256];	//������
	char aStr[256];		//�ꎞ�ۑ�������
	int nIndex = 0;		//���݂̃C���f�b�N�X
	int nWord = 0;		//�|�b�v�ŕԂ��ꂽ�l��ێ�

	//�ǂݍ��񂾃f�[�^�̕ۊǕϐ�
	int nSetMeshFieldNum = 0;		//�t�B�[���h�̍ő吔
	int nTexType = -1;				//�e�N�X�`���̃^�C�v�ԍ�
	int nWidthDivide = 0;			//������
	int nDepthDivide = 0;			//������
	float fWidthTexUV = 0.0f;		//�e�N�X�`���̉�UV
	float fHightTexUV = 0.0f;		//�e�N�X�`���̏cUV
	float fWidrhLength = 0.0f;		//�����̒���
	float fDepthLength = 0.0f;		//���s���̒���
	float fVtxHeight_No0 = 0.0f;
	float fVtxHeight_No1 = 0.0f;
	float fVtxHeight_No2 = 0.0f;
	float fVtxHeight_No3 = 0.0f;
	D3DXVECTOR3 Pos = VECTOR_ZERO;

	//�t�@�C���|�C���^�̏���������
	pFile = NULL;

	//�t�@�C�����J�� �ǂݍ���
	pFile = fopen(TEXT_MESHFIELDNAME1, "r");

	//NULL�`�F�b�N
	if (pFile != NULL)
	{
		//������̐擪��ݒ�
		pStrcur = ReadLine(pFile, &aLine[0]);
		//����������o��
		strcpy(aStr, pStrcur);

		//������̃f�[�^ ��r���镶���� ��r���镶����
		if (memcmp(pStrcur, "MESHFIELD_SETNUM = ", strlen("MESHFIELD_SETNUM = ")) == 0)
		{
			//���o��
			pStrcur += strlen("MESHFIELD_SETNUM = ");
			//������̐擪��ݒ�
			strcpy(aStr, pStrcur);
			//�����񔲂��o��
			nSetMeshFieldNum = atoi(pStrcur);
		}

		//�I�u�W�F�N�g�̐�����
		for (int nCntObject = 0; nCntObject < nSetMeshFieldNum; nCntObject++)
		{
			//������̐擪��ݒ�
			pStrcur = ReadLine(pFile, &aLine[0]);
			//����������o��
			strcpy(aStr, pStrcur);

			if (memcmp(pStrcur, "MESHFIELD_START", strlen("MESHFIELD_START")) == 0)
			{
				while (1)
				{
					//������̐擪��ݒ�
					pStrcur = ReadLine(pFile, &aLine[0]);

					//��ނ̂�ǂݍ���
					if (memcmp(pStrcur, "TEXTURETYPE = ", strlen("TEXTURETYPE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("TEXTURETYPE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						nTexType = atoi(pStrcur);
					}
					//���̕�������ǂݍ���
					if (memcmp(pStrcur, "X_DIVIDE = ", strlen("X_DIVIDE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("X_DIVIDE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						nWidthDivide = atoi(pStrcur);
					}
					//�c�̕�������ǂݍ���
					if (memcmp(pStrcur, "Z_DIVIDE = ", strlen("Z_DIVIDE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("Z_DIVIDE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						nDepthDivide = atoi(pStrcur);
					}
					//���̃e�N�X�`�����W��ǂݍ���
					if (memcmp(pStrcur, "X_TEXUV = ", strlen("X_TEXUV = ")) == 0)
					{
						//���o��
						pStrcur += strlen("X_TEXUV = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						fWidthTexUV = (float)atoi(pStrcur);
					}
					//�c�̃e�N�X�`�����W��ǂݍ���
					if (memcmp(pStrcur, "Y_TEXUV = ", strlen("Y_TEXUV = ")) == 0)
					{
						//���o��
						pStrcur += strlen("Y_TEXUV = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						fHightTexUV = (float)atoi(pStrcur);
					}
					//���̒�����ǂݍ���
					if (memcmp(pStrcur, "X_LENGTH = ", strlen("X_LENGTH = ")) == 0)
					{
						//���o��
						pStrcur += strlen("X_LENGTH = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						fWidrhLength = (float)atoi(pStrcur);
					}
					//�c�̒�����ǂݍ���
					if (memcmp(pStrcur, "Z_LENGTH = ", strlen("Z_LENGTH = ")) == 0)
					{
						//���o��
						pStrcur += strlen("Z_LENGTH = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						fDepthLength = (float)atoi(pStrcur);
					}
					//�P���_�̍���
					if (memcmp(pStrcur, "VTX0_HEIGHT = ", strlen("VTX0_HEIGHT = ")) == 0)
					{
						//���o��
						pStrcur += strlen("VTX0_HEIGHT = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						fVtxHeight_No0 = (float)atoi(pStrcur);
					}
					//�Q���_�̍���
					if (memcmp(pStrcur, "VTX1_HEIGHT = ", strlen("VTX1_HEIGHT = ")) == 0)
					{
						//���o��
						pStrcur += strlen("VTX1_HEIGHT = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						fVtxHeight_No1 = (float)atoi(pStrcur);
					}
					//�R���_�̍���
					if (memcmp(pStrcur, "VTX2_HEIGHT = ", strlen("VTX2_HEIGHT = ")) == 0)
					{
						//���o��
						pStrcur += strlen("VTX2_HEIGHT = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						fVtxHeight_No2 = (float)atoi(pStrcur);
					}
					//�S���_�̍���
					if (memcmp(pStrcur, "VTX3_HEIGHT = ", strlen("VTX3_HEIGHT = ")) == 0)
					{
						//���o��
						pStrcur += strlen("VTX3_HEIGHT = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						fVtxHeight_No3 = (float)atoi(pStrcur);
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
						Pos.x = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						Pos.y = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						Pos.z = (float)atof(pStrcur);

					}
					else if (memcmp(pStrcur, "MESHFIELD_END", strlen("MESHFIELD_END")) == 0)
					{
						/*CMeshField::Create(Pos, nWidthDivide, nDepthDivide, fWidthTexUV, fHightTexUV,
										  fWidrhLength, fDepthLength, fVtxHeight_No0, fVtxHeight_No1, fVtxHeight_No2, fVtxHeight_No3, nTexType, 0);
						*/break;
					}
				}
			}
		}
	}
}

//=============================================================================
// �I�u�W�F�N�g�̐���
//=============================================================================
void CTutorial::ObjectCreate(void)
{
	//�t�@�C���p�ϐ�
	FILE *pFile;		//�t�@�C���|�C���^
	char *pStrcur;		//���݂̐擪�̕�����
	char aLine[256];	//������
	char aStr[256];		//�ꎞ�ۑ�������
	int nIndex = 0;		//���݂̃C���f�b�N�X
	int nWord = 0;		//�|�b�v�ŕԂ��ꂽ�l��ێ�

	//�ǂݍ��ݗp�ϐ�
	int nSetObjNum = -1;				//�I�u�W�F�N�g�̍ő吔
	int nType = -1;						//�^�C�v�ԍ�
	int nTexType = -1;					//�e�N�X�`���^�C�v
	int nCollision = 0;					//�R���W�����̐�
	D3DXVECTOR3 pos = VECTOR_ZERO;		//�ʒu
	D3DXVECTOR3 rot = VECTOR_ZERO;		//����
	D3DXVECTOR3 scale = VECTOR_ZERO;	//�X�P�[��

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
			nSetObjNum = atoi(pStrcur);
		}

		//�I�u�W�F�N�g�̐�����
		for (int nCntObject = 0; nCntObject < nSetObjNum; nCntObject++)
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
						nType = atoi(pStrcur);
					}
					//��ނ̂�ǂݍ���
					if (memcmp(pStrcur, "TEXTURETYPE = ", strlen("TEXTURETYPE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("TEXTURETYPE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						nTexType = atoi(pStrcur);
					}
					//�����蔻���ǂݍ���
					if (memcmp(pStrcur, "COLLISION = ", strlen("COLLISION = ")) == 0)
					{
						//���o��
						pStrcur += strlen("COLLISION = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						nCollision = atoi(pStrcur);
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
						pos.x = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						pos.y = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						pos.z = (float)atof(pStrcur);

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
						rot.x = (float)atof(pStrcur);

						//���������i�߂�
						pStrcur += nWord;
						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						rot.y = (float)atof(pStrcur);

						//���������i�߂�
						pStrcur += nWord;
						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						rot.z = (float)atof(pStrcur);
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
						scale.x = (float)atof(pStrcur);

						//���������i�߂�
						pStrcur += nWord;
						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						scale.y = (float)atof(pStrcur);

						//���������i�߂�
						pStrcur += nWord;
						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						scale.z = (float)atof(pStrcur);
					}
					else if (memcmp(pStrcur, "OBJECT_END", strlen("OBJECT_END")) == 0)
					{
						CObject::Create(pos, rot, scale, 0.0f, nTexType,nType, nCollision);
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
char * CTutorial::ReadLine(FILE *pFile, char *pDst)
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
char *CTutorial::GetLineTop(char * pStr)
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
int CTutorial::PopString(char * pStr, char * pDest)
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

//=============================================================================
// �ǂ̃e�N�X�`���̓ǂݍ���
//=============================================================================
void CTutorial::WallTexLoad(void)
{
	//�ϐ��錾
	int nMaxWall = 0;	//�ǂ̍ő吔

						//�t�@�C���p�ϐ�
	FILE *pFile;		//�t�@�C���|�C���^
	char *pStrcur;		//���݂̐擪�̕�����
	char aLine[256];	//������
	char aStr[256];		//�ꎞ�ۑ�������
	int nIndex = 0;		//���݂̃C���f�b�N�X
	int nWord = 0;		//�|�b�v�ŕԂ��ꂽ�l��ێ�

						//�ۑ��p�̕ϐ�
	D3DXVECTOR3 Pos = VECTOR_ZERO;
	D3DXVECTOR3 Rot = VECTOR_ZERO;
	float fWidthDivide = 0.0f;
	float fHightDivide = 0.0f;
	int nTexType = 0;

	//�t�@�C���|�C���^�̏���������
	pFile = NULL;

	//�t�@�C�����J�� �ǂݍ���
	pFile = fopen(TEXT_WALLNAME1, "r");

	//NULL�`�F�b�N
	if (pFile != NULL)
	{
		//������̐擪��ݒ�
		pStrcur = ReadLine(pFile, &aLine[0]);
		//����������o��
		strcpy(aStr, pStrcur);

		//������̃f�[�^ ��r���镶���� ��r���镶����
		if (memcmp(pStrcur, "WALL_SETNUM = ", strlen("WALL_SETNUM = ")) == 0)
		{
			//���o��
			pStrcur += strlen("WALL_SETNUM = ");
			//������̐擪��ݒ�
			strcpy(aStr, pStrcur);
			//�����񔲂��o��
			nMaxWall = atoi(pStrcur);
		}

		//�I�u�W�F�N�g�̐�����
		for (int nCntObject = 0; nCntObject < nMaxWall; nCntObject++)
		{
			//������̐擪��ݒ�
			pStrcur = ReadLine(pFile, &aLine[0]);
			//����������o��
			strcpy(aStr, pStrcur);

			if (memcmp(pStrcur, "WALL_START", strlen("WALL_START")) == 0)
			{
				while (1)
				{
					//������̐擪��ݒ�
					pStrcur = ReadLine(pFile, &aLine[0]);

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
						Pos.x = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						Pos.y = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						Pos.z = (float)atof(pStrcur);

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
						Rot.x = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						Rot.y = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						Rot.z = (float)atof(pStrcur);

					}
					//WIDTH��ǂݍ���
					if (memcmp(pStrcur, "WIDTH = ", strlen("WIDTH = ")) == 0)
					{
						//���o��
						pStrcur += strlen("WIDTH = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						fWidthDivide = (float)atof(pStrcur);
					}
					//HIGHT��ǂݍ���
					if (memcmp(pStrcur, "HIGHT = ", strlen("HIGHT = ")) == 0)
					{
						//���o��
						pStrcur += strlen("HIGHT = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						fHightDivide = (float)atof(pStrcur);
					}
					//HIGHT��ǂݍ���
					if (memcmp(pStrcur, "TEXTURE = ", strlen("TEXTURE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("TEXTURE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						nTexType = (int)atof(pStrcur);
					}
					else if (memcmp(pStrcur, "WALL_END", strlen("WALL_END")) == 0)
					{//�ǂ̐���
						CWall::Create(Pos, D3DXVECTOR2(fWidthDivide, fHightDivide), Rot, nTexType);
						break;
					}
				}
			}
		}
	}
}