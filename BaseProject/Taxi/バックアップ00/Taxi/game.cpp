//=============================================================================
//
// �Q�[���̏��� [game.cpp]
// Author : �L�n�@���u
//
//=============================================================================
#include "game.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "player.h"
#include "input.h"
#include "fade.h"
#include "pause.h"
#include "logo.h"
#include "sound.h"
#include "billboord.h"
#include "meshfield.h"
#include "object.h"
#include "fade.h"
#include "effect.h"
#include "time.h"
#include "wall.h"
#include "shadow.h"
#include "loadText.h"
#include "customer.h"
#include "tire.h"
#include "totalscore.h"
#include "score.h"
#include "satisfactionlevel.h"
#include "destination.h"
#include "customer.h"
#include "numberdisplay.h"
#include "combo.h"
#include "arrow.h"
#include "car.h"
#include "texture.h"
#include "humanbace.h"
#include "car.h"
#include "car_point.h"
#include "gamecamera.h"
#include "humannpc.h"
#include "mark.h"
#include "shadermgr.h"
#include "effectmgr.h"
#include "smokeeffect.h"
#include "sparkeffect .h"
#include "policelamp.h"
#include "ground.h"
#include "particlebillboad.h"
#include "effecttool.h"
#include "objbillboad.h"
#include "LCDpanel.h"
#include "grasseffect.h"
#include "addcoin.h"
#include "select.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXT_OBJECTNAME1		"data\\TEXT\\�X�e�[�W6\\objecy.txt"			// �ǂݍ��ރe�L�X�g�t�@�C��
#define TEXT_MESHFIELDNAME1		"data\\TEXT\\�X�e�[�W6\\meshfield.txt"		// �ǂݍ��ރe�L�X�g�t�@�C��
#define TEXT_WALLNAME1			"data\\TEXT\\�X�e�[�W6\\wall.txt"			// �ǂݍ��ރe�L�X�g�t�@�C��
#define TEXT_PLAYER_MOTION		"data\\TEXT\\Player\\Player.txt"			// �v���C���[�̃��[�V�����t�@�C��
#define TEXT_BOY_MOTION			"data\\TEXT\\���q����\\motion_boy.txt"		// �j�����[�V�����t�@�C��
#define TEXT_GIRL_MOTION		"data\\TEXT\\���q����\\motion_girl.txt"		// �������[�V�����t�@�C��
#define TEXT_CUSTOMER_SUMMER	"data\\TEXT\\���q����\\Summer2.txt"			// ���q����̏��i�āj
#define TEXT_CUSTOMER_HARD		"data\\TEXT\\���q����\\Summer.txt"			// ���q����̏�� (�n�[�h)
#define TEXT_HUMAN_NPC			"data\\TEXT\\HumanNPC.txt"					// �lNPC�̏��
#define VECTOR_ZERO				(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define TEXT_POINTNAME1			"data\\TEXT\\�X�e�[�W6\\point.txt"			// �Ԃ̃|�C���g���
#define TEXT_ROUTENAME1			"data\\TEXT\\�X�e�[�W6\\route.txt"			// �Ԃ̃��[�g���
#define TEXT_EFFECT				"data\\TEXT\\particle.txt"					// �G�t�F�N�g���
#define TEXT_EFFECT_TOOL		"data\\TEXT\\Particle.txt"					// �G�t�F�N�g���
#define TEXT_OBJBILL			"data\\TEXT\\�X�e�[�W6\\objbillboad.txt"	// �I�u�W�F�N�g�r���{�[�h���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CPlayer *CGame::m_pPlayer = NULL;
CNumber *CGame::m_pNumber = NULL;
CBillBoord *CGame::m_pBillBoord = NULL;
CPolygon3D *CGame::m_pPolygon3D = NULL;
CMeshField *CGame::m_pMeshField = NULL;
CObject *CGame::m_pObject = NULL;
CCarBase *CGame::m_pCar = NULL;
CPause *CGame::m_pPause = NULL;
CModel *CGame::m_pModel = NULL;
CWall *CGame::m_pWall = NULL;
CShadow *CGame::m_pShadow = NULL;
CLoadTextMotion * CGame::m_pPlayerMotion = NULL;
CLoadTextMotion * CGame::m_pBoyMotion = NULL;
CLoadTextMotion * CGame::m_pGirlMotion = NULL;
CSatisfaction * CGame::m_pSatisfaction = NULL;
CGameCamera * CGame::m_pGameCamera = NULL;
CShaderMgr * CGame::m_pShaderMgr = NULL;
CPoliceLamp *CGame::m_pPoliceLamp = NULL;
CGround * CGame::m_pGround = NULL;
CLoadTextEffect * CGame::m_pLoadTextEffect = NULL;
CLogo *CGame::m_pScoreUI[MAX_SCORE_UI] = {};

bool CGame::m_bHelp = false;
bool CGame::m_bPause = false;
CGame::GAMESTATE CGame::m_gameState = GAMESTATE_NONE;
int	CGame::m_nCntSetStage = 0;								// �X�e�[�W�Z�b�g�J�E���^
CGame::STAGENUM CGame::m_StageState = STAGENUM_1;			// �X�e�[�W���

CGame::Route CGame::m_Route = {};
CGame::Point CGame::m_Point = {};

int CGame::m_nStopRoute = 0;
int CGame::m_nGameCounter = 0;

//�E�H�[�N�X���[�p
bool CGame::m_bDrawUI = false;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CGame::CGame()
{
	m_gameState = GAMESTATE_NONE;
	m_nCounterGameState = 0;
	m_NowGameState = GAMESTATE_NONE;
	CGame::GetPlayer() = NULL;
	m_StageState = STAGENUM_1;			// �X�e�[�W���
	m_nCntSetStage = 0;
	m_bPause = false;
	m_nEnemyCount = 0;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGame::~CGame(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGame::Init()
{
	//===================================
	//		 Load�̓ǂݍ��ݏꏊ
	//===================================
	//�t�F�[�h�̃e�N�X�`���̓ǂݍ���
	CFade::Load();

	//���b�V���t�B�[���h�̃e�N�X�`���̓ǂݍ���
	m_pMeshField->Load();

	//�r���{�[�h�e�N�X�`���̓ǂݍ���
	m_pBillBoord->Load();

	//�I�u�W�F�N�g�̃e�N�X�`���̓ǂݍ���
	//m_pObject->Load();

	//NPC�̃��f���ƃe�N�X�`���̓ǂݍ���
	m_pCar->Load();

	//�ǂ̃e�N�X�`���̓ǂݍ���
	m_pWall->Load();

	//�e�̃e�N�X�`���ǂݍ���
	m_pShadow->Load();

	//�^�C���̃e�N�X�`���ǂݍ���
	CTire::LoadTexture();

	//���̓ǂݍ���
	CArrow::Load();

	//�}�[�N���f���̓ǂݍ���
	CMark::LoadModel();

	//�G�t�F�N�g�e�N�X�`���̓ǂݍ���
	CEffectMgr::LoadTex();

	//���e�N�X�`���̓ǂݍ���
	CSmokeEffect::LoadTex();

	//�Ήԃe�N�X�`���̓ǂݍ���
	CSparkEffect::LoadTex();

	//	�x�@�̃����v�̃e�N�X�`���̓ǂݍ���
	m_pPoliceLamp->Load();

	// ��e�N�X�`���̓ǂݍ���
	CGround::Load();

	// �G�t�F�N�g�̓ǂݍ���
	m_pLoadTextEffect = CLoadTextEffect::Create(TEXT_EFFECT_TOOL);
	CEffectTool::LoadEffect();
	CParticleBillboad::LoadTexture();		//�e�N�X�`���̓ǂݍ���

	//�I�u�W�F�N�g�r���{�[�h�̃e�N�X�`����ǂݍ���
	CObjBillboad::LoadTexture();

	// �t���L���e�N�X�`���̓ǂݍ���
	CLCDpanel::Load();

	//���e�N�X�`���̓ǂݍ���
	CGrassEffect::LoadTex();

	//�R�C���̃e�N�X�`���ǂݍ���
	CAddCoin::LoadTex();

	//===================================
	//		Create
	//===================================
	//�V�F�[�_�[�}�l�[�W���[�̐���
	//if (m_pShaderMgr == NULL) { m_pShaderMgr = CShaderMgr::Create(); }

	if (m_pPlayerMotion == NULL) { m_pPlayerMotion = CLoadTextMotion::Create(TEXT_PLAYER_MOTION); }	//�v���C���[�̃��[�V�����ǂݍ���
	CPlayer::LoadModel();	//���f���̓ǂݍ���

	if (m_pBoyMotion == NULL) { m_pBoyMotion = CLoadTextMotion::Create(TEXT_BOY_MOTION); }		//�j�����[�V�����ǂݍ���
	if (m_pGirlMotion == NULL) { m_pGirlMotion = CLoadTextMotion::Create(TEXT_GIRL_MOTION); }	//�������[�V�����ǂݍ���
	CHumanBace::LoadModel();	//�l���f���̓ǂݍ���

	//�^�C�}�[�̐���
	CTime::Create(D3DXVECTOR3(150.0f, 70.0f, 0.0f));

	//�v���C���[�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, -300.0f));
	m_pPlayer->LoadText();

	//���q����̐���
	/*if (CSelect::GetSelectLevel() == 0)
	{	// ��ՓxEasy
		CLoadTextCustomer::Create(TEXT_CUSTOMER_SUMMER); //�C�[�W�[�p
	}
	else if (CSelect::GetSelectLevel() == 1)
	{	// ��ՓxHard
		CLoadTextCustomer::Create(TEXT_CUSTOMER_HARD); //�n�[�h�p
	}*/

	 //�l��NPC����
	//CLoadTextHumanNPC::Create(TEXT_HUMAN_NPC);

	// UI
	/*CLogo::Create(D3DXVECTOR3(1200.0f, 650.0f, 0.0f), D3DXVECTOR2(60.0f, 60.0f), CTexture::TYPE_GEARCHANGE, 0, CLogo::TYPE_LOGO);		// �M�A�ؑ�
	CLogo::Create(D3DXVECTOR3(1200.0f, 560.0f, 0.0f), D3DXVECTOR2(60.0f, 30.0f), CTexture::TYPE_BOARDING, 0, CLogo::TYPE_LOGO);		// ��� or ���
	m_pScoreUI[0] = CLogo::Create(D3DXVECTOR3(1080.0f, 95.0f, 0.0f), D3DXVECTOR2(160.0f, 25.0f), CTexture::TYPE_FREAME, 0, CLogo::TYPE_LOGO);			// �g�[�^���X�R�A�̘g
	m_pScoreUI[1] = CLogo::Create(D3DXVECTOR3(1080.0f, 155.0f, 0.0f), D3DXVECTOR2(150.0f, 25.0f), CTexture::TYPE_FREAME, 0, CLogo::TYPE_LOGO);			// �X�R�A�̘g
	m_pScoreUI[2] = CLogo::Create(D3DXVECTOR3(1080.0f, 60.0f, 0.0f), D3DXVECTOR2(180.0f, 15.0f), CTexture::TYPE_TOTALSCORELOGO, 0, CLogo::TYPE_LOGO);	// �g�[�^���X�R�A
	m_pScoreUI[3] = CLogo::Create(D3DXVECTOR3(1080.0f, 120.0f, 0.0f), D3DXVECTOR2(160.0f, 15.0f), CTexture::TYPE_SCORELOGO, 0, CLogo::TYPE_LOGO);		// �X�R�A
	m_pScoreUI[4] = CLogo::Create(D3DXVECTOR3(950.0f, 95.0f, 0.0f), D3DXVECTOR2(13.0f, 13.0f), CTexture::TYPE_MONEY_TEN, 0, CLogo::TYPE_LOGO);			// \�}�[�N�\��(�g�[�^���X�R�A)
	m_pScoreUI[5] = CLogo::Create(D3DXVECTOR3(958.0f, 155.0f, 0.0f), D3DXVECTOR2(13.0f, 13.0f), CTexture::TYPE_MONEY_TEN, 0, CLogo::TYPE_LOGO);		// \�}�[�N�\��(�X�R�A)
	
	// �����x�̐���
	m_pSatisfaction = CSatisfaction::Create(D3DXVECTOR3(30.0f, 650.0f, 0.0f), D3DXVECTOR2(15.0f, 35.0f), 3);

	// �g�[�^���X�R�A�̐���
	CTotalScore::Create(D3DXVECTOR3(1205.0f, 95.0f, 0.0f));

	// �X�R�A�̐���
	CScore::Create(D3DXVECTOR3(1200.0f, 156.0f, 0.0f));
	*/
	// ��̐���
	m_pGround = CGround::Create(D3DXVECTOR3(0.0f, -185.0f, 9000.0f), D3DXVECTOR3(-0.2f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(20000.0f, 300.0f), 0);
	
	// �t���L���̐���
	/*CLCDpanel::Create(D3DXVECTOR3(8100.0f, 450.0f, -5000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3.5f);
	CLCDpanel::Create(D3DXVECTOR3(8100.0f, 450.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3.5f);
	CLCDpanel::Create(D3DXVECTOR3(8100.0f, 450.0f, 5000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3.5f);

	CLCDpanel::Create(D3DXVECTOR3(12854.0f, 450.0f, 1750.0f), D3DXVECTOR3(0.0f, -0.5f * D3DX_PI, 0.0f), 4.5f);
	CLCDpanel::Create(D3DXVECTOR3(12550.0f, 450.0f, 2008.0f), D3DXVECTOR3(0.0f, 1.0f * D3DX_PI, 0.0f), 4.5f);

	CLCDpanel::Create(D3DXVECTOR3(-14705.0f, 400.0f, -1754.0f), D3DXVECTOR3(0.0f, 0.5f * D3DX_PI, 0.0f), 4.5f);
	CLCDpanel::Create(D3DXVECTOR3(-14456.0f, 400.0f, -2007.0f), D3DXVECTOR3(0.0f, 0.0f * D3DX_PI, 0.0f), 4.5f);
	*/
	//�Q�[���J�����̐���
	if (m_pGameCamera == NULL)
	{
		m_pGameCamera = new CGameCamera;

		if (m_pGameCamera != NULL) { m_pGameCamera->Init(); }
	}

	//===================================
	//		�ϐ��̏�����
	//===================================
	m_gameState = GAMESTATE_NORMAL;		//�ʏ��Ԃ�
	m_nCntSetStage = 0;					//�ǂ��̃X�e�[�W����J�n���邩
	m_nEnemyCount = 0;					//�G�̐�������
	m_StageState = STAGENUM_1;			// �X�e�[�W���
	m_bPause = false;					//�|�[�Y��������
	m_nGameCounter = 0;					//�J�E���^�[�̏�����

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CGame::Uninit(void)
{
	//�|�[�Y�폜
	if (m_pPause != NULL)
	{
		m_pPause->Uninit();
		m_pPause = NULL;
	}

	// �g�[�^���X�R�A�Ɩ����x�̐ݒ�
	if (m_pSatisfaction != NULL)
	{
		CResult::SetGrades(CTotalScore::GetScore(), m_pSatisfaction->GetLevel());
		CRanking::RankingSave(CTotalScore::GetScore(), m_pSatisfaction->GetLevel());
	}

	//===================================
	//	�@�@UnLoad�̔j������ꏊ
	//===================================
	//���b�V���t�B�[���h�e�N�X�`���̔j��
	m_pMeshField->UnLoad();

	//�t�F�[�h�̃e�N�X�`���̔j��
	CFade::UnLoad();

	//�r���{�[�h�e�N�X�`���̔j��
	m_pBillBoord->UnLoad();

	//�I�u�W�F�N�g�̃e�N�X�`���̔j��
	m_pObject->UnLoad();

	//NPC�̃��f���ƃe�N�X�`���̔j��
	m_pCar->UnLoad();

	//�ǂ̃e�N�X�`���̔j��
	m_pWall->UnLoad();

	//�e�̃e�N�X�`���̔j��
	m_pShadow->UnLoad();

	//�^�C���̃e�N�X�`���j��
	CTire::UnloadTexture();

	// ���̔j��
	CArrow::UnLoad();

	//�}�[�N���f���̔j��
	CMark::UnloadModel();

	//�G�t�F�N�g�e�N�X�`���̔j��
	CEffectMgr::UnloadTex();

	//���e�N�X�`���̔j��
	CSmokeEffect::Unload();

	//�Ήԃe�N�X�`���̔j��
	CSparkEffect::Unload();

	//	�x�@�̃����v�̔j��
	m_pPoliceLamp->UnLoad();

	// ��e�N�X�`���̔j��
	CGround::UnLoad();

	//�G�t�F�N�g�e�N�X�`���̔j��
	CParticleBillboad::UnloadTexture();

	// �G�t�F�N�g�̔j��
	CEffectTool::UnloadEffect();

	//�I�u�W�F�N�g�e�N�X�`���̔j��
	CObjBillboad::UnloadTexture();

	// �t���L���e�N�X�`���̔j��
	CLCDpanel::Unload();

	//���e�N�X�`���̔j��
	CGrassEffect::Unload();

	//�R�C���e�N�X�`���̔j��
	CAddCoin::UnloadTex();

	//===================================
	//	�@�@�N���X�̔j��
	//===================================
	//���b�V���t�B�[���h�̔j��
	if (m_pMeshField != NULL)
	{
		m_pMeshField->Uninit();
		m_pMeshField = NULL;
	}

	// �X�^�[�g���}�̔j��
	m_pGameCamera->UninitSignal();

	// UI�̔j��
	for (int nCnt = 0; nCnt < MAX_SCORE_UI; nCnt++)
	{
		if (m_pScoreUI[nCnt] != NULL)
		{
			m_pScoreUI[nCnt]->Uninit();
			m_pScoreUI[nCnt] = NULL;
		}
	}

	// �I�u�W�F�N�g�̔j��
	if (m_pObject != NULL)
	{
		m_pObject->Uninit();
		m_pObject = NULL;
	}

	// �ǂ̔j��
	if (m_pWall != NULL)
	{
		m_pWall->Uninit();
		m_pWall = NULL;
	}

	// �Ԃ̔j��
	if (m_pCar != NULL)
	{
		m_pCar->Uninit();
		m_pCar = NULL;
	}

	// �e�[�������v(�x�@)�̔j��
	if (m_pPoliceLamp != NULL)
	{
		m_pPoliceLamp->Uninit();
		m_pPoliceLamp = NULL;
	}

	// �r���{�[�h�̔j��
	if (m_pBillBoord != NULL)
	{
		m_pBillBoord->Uninit();
		m_pBillBoord = NULL;
	}
	
	//�t�F�[�h�ȊO�̔j��
	CScene::NotFadeReleseAll();

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

	//�v���C���[���f���̔j��
	CPlayer::UnloadModel();

	//�l���f���̔j��
	CHumanBace::UnloadModel();

	//�J�����̔j��
	if (m_pGameCamera != NULL)
	{
		m_pGameCamera->Uninit();
		delete m_pGameCamera;
		m_pGameCamera = NULL;
	}

	//�V�F�[�_�[�}�l�[�W���[�̔j��
	if (m_pShaderMgr != NULL)
	{
		m_pShaderMgr->Uninit();
		delete m_pShaderMgr;
		m_pShaderMgr = NULL;
	}

	//�ǂݍ��񂾃G�t�F�N�g�̔j��
	if (m_pLoadTextEffect != NULL)
	{
		m_pLoadTextEffect->Uninit();
		delete m_pLoadTextEffect;
		m_pLoadTextEffect = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void CGame::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CXInput *pCInputJoypad = CManager::GetXInput();

	//	�J�E���^�[�i�߂�
	m_nGameCounter++;

	//�T�E���h�̏��
	CSound *pSound = CManager::GetSound();

	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true)
	{
		//m_bDrawUI = m_bDrawUI ? false : true;

		//�|�[�Y�̑I���̌��艹
		CFade::Create(CManager::MODE_RESULT);
	}

	if (m_pPause == false)
	{//�J����
		//���݂̏�Ԃ�ۑ�
		m_NowGameState = GetGameState();

		//�J�����̍X�V����
		if (m_pGameCamera != NULL) { m_pGameCamera->Update(); }

		switch (m_NowGameState)
		{
		case GAMESTATE_NORMAL:	//�ʏ�̏��

			//�X�e�[�W�ݒ�
			SetStage();

			break;
		case GAMESTATE_CLEAR:	//�Q�[�����N���A�������
			m_nCounterGameState++;

			if (m_nCounterGameState >= 60)
			{//��ʁi���[�h�j�̐ݒ�
				CFade::Create(CManager::MODE_RESULT);
			}
			break;
		case GAMESTATE_OVER:	//�Q�[���ŕ������Ƃ��̏��
			m_nCounterGameState++;

			if (m_nCounterGameState >= 60)
			{//��ʁi���[�h�j�̐ݒ�
				CFade::Create(CManager::MODE_RESULT);
			}
			break;
		}
	}

	if (CTime::GetTime() == 0) { return; }
	//�|�[�Y�̏���
	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_P) == true || pCInputJoypad->GetTrigger(CXInput::XIJS_BUTTON_4))
	{//P�L�[�������ꂽ��
		int nType = 0;
		if (pCInputJoypad->GetTrigger(CXInput::XIJS_BUTTON_4)) { nType = 1; }
		m_bPause = m_bPause ? false : true;

		switch (m_bPause)
		{
		case true:
			if (m_pPause == NULL)
			{
				//�|�[�Y���J����
				pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_OPEN);

				//�|�[�Y�̐���
				m_pPause = CPause::Create();
				m_pPause->SetTexType(nType);

				//�|�[�Y�ƃt�F�[�h������
				CScene::SetUpdatePri(7);
			}
			break;
		case false:
			if (m_pPause != NULL)
			{
				if (CPause::GetbPause() == false)
				{
					//�|�[�Y����鉹
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_CLOSE);

					//�|�[�Y���폜
					m_pPause->Uninit();
					m_pPause = NULL;

					//�A�b�v�f�[�g���Ԃ����ׂĉ�
					CScene::SetUpdatePri(0);
				}
			}
		}
	}

	if (CManager::GetInput()->GetKeyboardTrigger(DIK_5))
	{
		m_pSatisfaction->AddLevel(1);
	}
	else if (CManager::GetInput()->GetKeyboardTrigger(DIK_6))
	{
		m_pSatisfaction->Subtraction(1);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGame::Draw(void)
{
	//�J�����̐ݒ�
	if (m_pGameCamera != NULL) { m_pGameCamera->SetCamera(); }
}

//=============================================================================
// �|�[�Y�����������ǂ���
//=============================================================================
void CGame::SetPause(bool bPause)
{
	m_bPause = bPause;

	switch (m_bPause)
	{
	case true:
		if (m_pPause == NULL)
		{
			//�|�[�Y�̐���
			m_pPause = CPause::Create();

			CScene::SetUpdatePri(7);
		}
		break;
	case false:
		if (m_pPause != NULL)
		{
			m_pPause->Uninit();
			m_pPause = NULL;

			CScene::SetUpdatePri(0);
		}
	}
}
//=============================================================================
// �X�e�[�W���
//=============================================================================
void CGame::SetNumState(STAGENUM stagenum)
{
	m_StageState = stagenum;
}

//=============================================================================
// �Q�[���ݒ菈��
//=============================================================================
void CGame::SetStageState(STAGENUM stage)
{
	//�폜���鏈��
	if (m_StageState != stage)
	{
		m_pWall->DeleteWall();
		m_pObject->DeleteObject();
		m_pBillBoord->ResetBillboord();
		m_pMeshField->DeleteMeshField();
	}

	//��Ԃ��L��
	m_StageState = stage;

	//�폜���鏈��
	if (m_StageState == stage)
	{
		//m_pPlayer->ResetPlayer();
	}
	m_nCntSetStage = 0;
}
//=============================================================================
// �X�e�[�W�ݒ�
//=============================================================================
void CGame::SetStage(void)
{
	//�T�E���h�̃|�C���^
	CSound *pSound = CManager::GetSound();

	if (m_nCntSetStage == 0)
	{
		//=====================================================================
		//	�X�e�[�W1
		//=====================================================================
		if (m_StageState == STAGENUM_1)
		{
			//�}�b�v��ǂݍ���
			TextLoad(6);
			WallTextLoad(6);
			MeshTextLoad(6);
			NpcTextLoad(6);
			PointTextLoad(6);
			RouteLoad(6);

			/*for (int nCount = 0; nCount < m_nSetObjectNum; nCount++)
			{
				//�I�u�W�F�N�g�̐���
				m_pObject = CObject::Create(m_Map[nCount].m_pos, m_Map[nCount].m_rot, m_Map[nCount].m_scale,0.0f, m_Map[nCount].m_nTexType, m_Map[nCount].m_nType, CModel3D::MOVETYPE_NOT, m_Map[nCount].m_nCollision);
			}

			//�I�u�W�F�N�g�r���{�[�h�̐���
			CLoadTextObjBill::Create(TEXT_OBJBILL);

			for (int nCount = 0; nCount < m_nSetWallNum; nCount++)
			{
				//�ǂ̐���
				m_pWall = CWall::Create(m_aWall[nCount].m_pos, D3DXVECTOR2(m_aWall[nCount].m_fWidthDivide, m_aWall[nCount].m_fHightDivide), m_aWall[nCount].m_rot, m_aWall[nCount].m_nTexType);
			}*/
			for (int nCount = 0; nCount < m_nSetMeshFieldNum; nCount++)
			{
				//�t�B�[���h�̐���
				m_pMeshField = CMeshField::Create(m_Mesh[nCount].m_pos, m_Mesh[nCount].m_nWidthDivide, m_Mesh[nCount].m_nDepthDivide, m_Mesh[nCount].m_fTexXUV, m_Mesh[nCount].m_fTexYUV,
					m_Mesh[nCount].m_fWidthLength, m_Mesh[nCount].m_fDepthLength,
					m_Mesh[nCount].m_fVtxHeight_No0, m_Mesh[nCount].m_fVtxHeight_No1, m_Mesh[nCount].m_fVtxHeight_No2, m_Mesh[nCount].m_fVtxHeight_No3, m_Mesh[nCount].m_nTexType, 0);
			}
			/*for (int nCount = 0; nCount < m_nSetNpcNum; nCount++)
			{
				//�Ԃ̐���
				m_pCar = CCarBase::Create(m_aNpc[nCount].m_pos, m_aNpc[nCount].m_rot, m_aNpc[nCount].m_scale, m_aNpc[nCount].m_nTexType,
					m_aNpc[nCount].m_nType, m_aNpc[nCount].m_nCollision, m_aNpc[nCount].m_nRoute, m_aNpc[nCount].bLoop);
			}*/
		}

		m_nCntSetStage = 1;
	}
}
//===============================================================================
// �t�@�C�����烍�[�h
//===============================================================================
void CGame::TextLoad(int nLoadNumber)
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

	//m_pMarkObject->SetObjectNum(CMarkObject::GetObjectNum() + m_nSetObjectNum);
}
//===============================================================================
// ���b�V���t�B�[���h���t�@�C�����烍�[�h
//===============================================================================
void CGame::MeshTextLoad(int nLoadNumber)
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
			m_nSetMeshFieldNum = atoi(pStrcur);
		}

		//�I�u�W�F�N�g�̐�����
		for (int nCntObject = 0; nCntObject < m_nSetMeshFieldNum; nCntObject++)
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
						m_Mesh[nCntObject].m_nTexType = atoi(pStrcur);
					}
					//���̕�������ǂݍ���
					if (memcmp(pStrcur, "X_DIVIDE = ", strlen("X_DIVIDE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("X_DIVIDE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_nWidthDivide = atoi(pStrcur);
					}
					//�c�̕�������ǂݍ���
					if (memcmp(pStrcur, "Z_DIVIDE = ", strlen("Z_DIVIDE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("Z_DIVIDE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_nDepthDivide = atoi(pStrcur);
					}
					//���̃e�N�X�`�����W��ǂݍ���
					if (memcmp(pStrcur, "X_TEXUV = ", strlen("X_TEXUV = ")) == 0)
					{
						//���o��
						pStrcur += strlen("X_TEXUV = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fTexXUV = (float)atoi(pStrcur);
					}
					//�c�̃e�N�X�`�����W��ǂݍ���
					if (memcmp(pStrcur, "Y_TEXUV = ", strlen("Y_TEXUV = ")) == 0)
					{
						//���o��
						pStrcur += strlen("Y_TEXUV = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fTexYUV = (float)atoi(pStrcur);
					}
					//���̒�����ǂݍ���
					if (memcmp(pStrcur, "X_LENGTH = ", strlen("X_LENGTH = ")) == 0)
					{
						//���o��
						pStrcur += strlen("X_LENGTH = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fWidthLength = (float)atoi(pStrcur);
					}
					//�c�̒�����ǂݍ���
					if (memcmp(pStrcur, "Z_LENGTH = ", strlen("Z_LENGTH = ")) == 0)
					{
						//���o��
						pStrcur += strlen("Z_LENGTH = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fDepthLength = (float)atoi(pStrcur);
					}
					//�P���_�̍���
					if (memcmp(pStrcur, "VTX0_HEIGHT = ", strlen("VTX0_HEIGHT = ")) == 0)
					{
						//���o��
						pStrcur += strlen("VTX0_HEIGHT = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fVtxHeight_No0 = (float)atoi(pStrcur);
					}
					//�Q���_�̍���
					if (memcmp(pStrcur, "VTX1_HEIGHT = ", strlen("VTX1_HEIGHT = ")) == 0)
					{
						//���o��
						pStrcur += strlen("VTX1_HEIGHT = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fVtxHeight_No1 = (float)atoi(pStrcur);
					}
					//�R���_�̍���
					if (memcmp(pStrcur, "VTX2_HEIGHT = ", strlen("VTX2_HEIGHT = ")) == 0)
					{
						//���o��
						pStrcur += strlen("VTX2_HEIGHT = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fVtxHeight_No2 = (float)atoi(pStrcur);
					}
					//�S���_�̍���
					if (memcmp(pStrcur, "VTX3_HEIGHT = ", strlen("VTX3_HEIGHT = ")) == 0)
					{
						//���o��
						pStrcur += strlen("VTX3_HEIGHT = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fVtxHeight_No3 = (float)atoi(pStrcur);
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
						m_Mesh[nCntObject].m_pos.x = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Mesh[nCntObject].m_pos.y = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Mesh[nCntObject].m_pos.z = (float)atof(pStrcur);

					}
					else if (memcmp(pStrcur, "MESHFIELD_END", strlen("MESHFIELD_END")) == 0)
					{
						break;
					}
				}
			}
		}
	}

	//m_pMarkMeshField->SetMeshFieldNum(CMarkMeshField::GetMeshFieldNum() + m_nSetMeshFieldNum);
}

//===============================================================================
// �ǂ��t�@�C�����烍�[�h
//===============================================================================
void CGame::WallTextLoad(int nLoadNumber)
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
			m_nSetWallNum = atoi(pStrcur);
		}

		//�I�u�W�F�N�g�̐�����
		for (int nCntObject = 0; nCntObject < m_nSetWallNum; nCntObject++)
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
						m_aWall[nCntObject].m_pos.x = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aWall[nCntObject].m_pos.y = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aWall[nCntObject].m_pos.z = (float)atof(pStrcur);

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
						m_aWall[nCntObject].m_rot.x = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aWall[nCntObject].m_rot.y = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aWall[nCntObject].m_rot.z = (float)atof(pStrcur);

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
						m_aWall[nCntObject].m_fWidthDivide = (float)atof(pStrcur);
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
						m_aWall[nCntObject].m_fHightDivide = (float)atof(pStrcur);
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
						m_aWall[nCntObject].m_nTexType = (int)atof(pStrcur);
					}
					else if (memcmp(pStrcur, "WALL_END", strlen("WALL_END")) == 0)
					{
						break;
					}
				}
			}
		}
	}

	//m_pMarkWall->SetWallNum(CMarkWall::GetWallNum() + m_nSetWallNum);
}
//===============================================================================
// �t�@�C�����烍�[�h
//===============================================================================
void CGame::NpcTextLoad(int nLoadNumber)
{
	//�t�@�C���p�ϐ�
	FILE *pFile;		//�t�@�C���|�C���^
	char *pStrcur;		//���݂̐擪�̕�����
	char aLine[256];	//������
	char aStr[256];		//�ꎞ�ۑ�������
	int nIndex = 0;		//���݂̃C���f�b�N�X
	int nWord = 0;		//�|�b�v�ŕԂ��ꂽ�l��ێ�

						//�t�@�C�����J�� �ǂݍ���
	pFile = fopen(TEXT_CARNAME, "r");

	//NULL�`�F�b�N
	if (pFile != NULL)
	{
		//������̐擪��ݒ�
		pStrcur = ReadLine(pFile, &aLine[0]);
		//����������o��
		strcpy(aStr, pStrcur);

		//������̃f�[�^ ��r���镶���� ��r���镶����
		if (memcmp(pStrcur, "NPC_SETNUM = ", strlen("NPC_SETNUM = ")) == 0)
		{
			//���o��
			pStrcur += strlen("NPC_SETNUM = ");
			//������̐擪��ݒ�
			strcpy(aStr, pStrcur);
			//�����񔲂��o��
			m_nSetNpcNum = atoi(pStrcur);
		}

		//�I�u�W�F�N�g�̐�����
		for (int nCntObject = 0; nCntObject < m_nSetNpcNum; nCntObject++)
		{
			//������̐擪��ݒ�
			pStrcur = ReadLine(pFile, &aLine[0]);
			//����������o��
			strcpy(aStr, pStrcur);

			if (memcmp(pStrcur, "NPC_START", strlen("NPC_START")) == 0)
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
						m_aNpc[nCntObject].m_nType = atoi(pStrcur);
					}
					//��ނ̂�ǂݍ���
					if (memcmp(pStrcur, "TEXTURETYPE = ", strlen("TEXTURETYPE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("TEXTURETYPE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_aNpc[nCntObject].m_nTexType = atoi(pStrcur);
					}
					//�����蔻���ǂݍ���
					if (memcmp(pStrcur, "COLLISION = ", strlen("COLLISION = ")) == 0)
					{
						//���o��
						pStrcur += strlen("COLLISION = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_aNpc[nCntObject].m_nCollision = atoi(pStrcur);
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
						m_aNpc[nCntObject].m_pos.x = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aNpc[nCntObject].m_pos.y = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aNpc[nCntObject].m_pos.z = (float)atof(pStrcur);

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
						m_aNpc[nCntObject].m_rot.x = (float)atof(pStrcur);

						//���������i�߂�
						pStrcur += nWord;
						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aNpc[nCntObject].m_rot.y = (float)atof(pStrcur);

						//���������i�߂�
						pStrcur += nWord;
						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aNpc[nCntObject].m_rot.z = (float)atof(pStrcur);
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
						m_aNpc[nCntObject].m_scale.x = (float)atof(pStrcur);

						//���������i�߂�
						pStrcur += nWord;
						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aNpc[nCntObject].m_scale.y = (float)atof(pStrcur);

						//���������i�߂�
						pStrcur += nWord;
						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aNpc[nCntObject].m_scale.z = (float)atof(pStrcur);
					}
					//�|�C���g�̔ԍ��ǂݍ���
					if (memcmp(pStrcur, "ROUTE = ", strlen("ROUTE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("ROUTE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aNpc[nCntObject].m_nRoute = (int)atof(pStrcur);

					}
					//�|�C���g�̔ԍ��ǂݍ���
					if (memcmp(pStrcur, "LOOP = ", strlen("LOOP = ")) == 0)
					{
						//���o��
						pStrcur += strlen("LOOP = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);

						//������ϊ�
						if ((int)atof(pStrcur) == 0) {
							(bool)m_aNpc[nCntObject].bLoop = false;
						}
						else if ((int)atof(pStrcur) == 1) {
							(bool)m_aNpc[nCntObject].bLoop = true;
						}
						else { (bool)m_aNpc[nCntObject].bLoop = false; }

					}
					else if (memcmp(pStrcur, "NPC_END", strlen("NPC_END")) == 0)
					{
						break;
					}
				}
			}
		}
	}
}

//===============================================================================
// �t�@�C�����烍�[�h
//===============================================================================
void CGame::PointTextLoad(int nLoadNumber)
{
	//�t�@�C���p�ϐ�
	FILE *pFile;		//�t�@�C���|�C���^
	char *pStrcur;		//���݂̐擪�̕�����
	char aLine[256];	//������
	char aStr[256];		//�ꎞ�ۑ�������
	int nIndex = 0;		//���݂̃C���f�b�N�X
	int nWord = 0;		//�|�b�v�ŕԂ��ꂽ�l��ێ�

						//	�ϐ��̏����ݒ�
	m_Point.nCount = 0;

	//�t�@�C�����J�� �ǂݍ���
	pFile = fopen(TEXT_POINTNAME1, "r");

	//NULL�`�F�b�N
	if (pFile != NULL)
	{
		//������̐擪��ݒ�
		pStrcur = ReadLine(pFile, &aLine[0]);
		//����������o��
		strcpy(aStr, pStrcur);

		while (memcmp(pStrcur, "POINT_START", strlen("POINT_START")) == 0)
		{
			while (1)
			{
				//������̐擪��ݒ�
				pStrcur = ReadLine(pFile, &aLine[0]);

				//�ԍ��̂�ǂݍ���
				if (memcmp(pStrcur, "NUM = ", strlen("NUM = ")) == 0)
				{
					//���o��
					pStrcur += strlen("NUM = ");
					//������̐擪��ݒ�
					strcpy(aStr, pStrcur);
					//�����񔲂��o��
					m_Point.nNum[m_Point.nCount] = atoi(pStrcur);
				}
				//POS��ǂݍ���
				else if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
				{
					//���o��
					pStrcur += strlen("POS = ");
					//������̐擪��ݒ�
					strcpy(aStr, pStrcur);

					//��������Ԃ��Ă��炤
					nWord = PopString(pStrcur, &aStr[0]);
					//������ϊ�
					m_Point.pos[m_Point.nCount].x = (float)atof(pStrcur);
					//���������i�߂�
					pStrcur += nWord;

					//��������Ԃ��Ă��炤
					nWord = PopString(pStrcur, &aStr[0]);
					//������ϊ�
					m_Point.pos[m_Point.nCount].y = (float)atof(pStrcur);
					//���������i�߂�
					pStrcur += nWord;

					//��������Ԃ��Ă��炤
					nWord = PopString(pStrcur, &aStr[0]);
					//������ϊ�
					m_Point.pos[m_Point.nCount].z = (float)atof(pStrcur);

				}
				else if (memcmp(pStrcur, "POINT_END", strlen("POINT_END")) == 0)
				{
					//������̐擪��ݒ�
					pStrcur = ReadLine(pFile, &aLine[0]);
					//����������o��
					strcpy(aStr, pStrcur);
					break;
				}
			}
			m_Point.nCount++;		//	�J�E���^�[�i�߂�
		}
	}
	fclose(pFile);
}

//===============================================================================
//	�o�H�̃p�^�[���̓ǂݍ���
//===============================================================================
void CGame::RouteLoad(int nLoadNumber)
{
	//�t�@�C���p�ϐ�
	FILE *pFile;		//�t�@�C���|�C���^
	char *pStrcur;		//���݂̐擪�̕�����
	char aLine[256];	//������
	char aStr[256];		//�ꎞ�ۑ�������
	int nIndex = 0;		//���݂̃C���f�b�N�X
	int nWord = 0;		//�|�b�v�ŕԂ��ꂽ�l��ێ�

						//	�ϐ��̏����ݒ�
	for (int nCount = 0; nCount < MAX_MAP_ROUTE; nCount++)
	{
		m_Route.nPointCount[nCount] = 0;
	}
	m_Route.nRouteCount = 0;


	//�t�@�C�����J�� �ǂݍ���
	pFile = fopen(TEXT_ROUTENAME1, "r");

	//NULL�`�F�b�N
	if (pFile != NULL)
	{
		//������̐擪��ݒ�
		pStrcur = ReadLine(pFile, &aLine[0]);
		//����������o��
		strcpy(aStr, pStrcur);

		while (memcmp(pStrcur, "ROUTE_START", strlen("ROUTE_START")) == 0)
		{
			while (1)
			{
				//������̐擪��ݒ�
				pStrcur = ReadLine(pFile, &aLine[0]);

				//�|�C���g�̂�ǂݍ���
				if (memcmp(pStrcur, "POINT = ", strlen("POINT = ")) == 0)
				{
					//���o��
					pStrcur += strlen("POINT = ");
					//������̐擪��ݒ�
					strcpy(aStr, pStrcur);
					//�����񔲂��o��
					m_Route.nIndexPoint[m_Route.nRouteCount][m_Route.nPointCount[m_Route.nRouteCount]] = atoi(pStrcur);
					m_Route.nPointCount[m_Route.nRouteCount]++;	//	�J�E���^�[�i�߂�
				}
				else if (memcmp(pStrcur, "ROUTE_END", strlen("ROUTE_END")) == 0)
				{
					//������̐擪��ݒ�
					pStrcur = ReadLine(pFile, &aLine[0]);
					//����������o��
					strcpy(aStr, pStrcur);
					break;
				}
			}
			m_Route.nRouteCount++;	//	�J�E���^�[�i�߂�
		}
	}
	fclose(pFile);
}

//=============================================================================
//�@�t�@�C���ǂݍ��ݖ�������r��
//=============================================================================
char *CGame::ReadLine(FILE *pFile, char *pDst)
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
char *CGame::GetLineTop(char * pStr)
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
int CGame::PopString(char * pStr, char * pDest)
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