//=============================================================================
//
// �Q�[���J�������� [gamecamera.cpp]
// Author : ��������
//
//=============================================================================
#include "gamecamera.h"
#include "game.h"
#include "player.h"
#include "customer.h"
#include "scene.h"
#include "object.h"
#include "manager.h"
#include "tutorial.h"
#include "startsignal.h"
//===============================================================================
//�@�}�N����`
//===============================================================================
#define MOVE_CAMERA		(290.0f)							//�J�����̈ړ���
#define MOVE_ANGLE		(0.06f)								//�J�����p�x�̈ړ���
#define WINDY			(0.4f)								//����
#define HIGHT_V			(100.0f)							//���_�̍���
#define HIGHT_R			(60.0f)								//�����_�̍���
#define PUTON_DISTANCE	(D3DXVECTOR3(150.0f, 0.0f, 150.0f))	//��ԏ�Ԃ̃J��������
#define ADD_ANGLE		(0.04f)								//�p�x�̉��Z
#define POSV_DISTANCE	(200.0f)							//���_�̋���
#define POSV_MOVE		(0.1f)								//�ړ���

//===============================================================================
//�@�R���X�g���N�^
//===============================================================================
CGameCamera::CGameCamera(){}

//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CGameCamera::~CGameCamera(){}

//===============================================================================
//�@����������
//===============================================================================
HRESULT CGameCamera::Init(void)
{
	CCamera::Init();

	if (CManager::GetMode() == CManager::MODE_TUTORIAL) { m_mode = MODE_NORMAL; }
	else if(CManager::GetMode() == CManager::MODE_GAME) { m_mode = MODE_NORMAL; }

	//�ϐ��̏�����
	m_posV = D3DXVECTOR3(0.0f, 500.0f, 0.0f);	//���_�̏����l
	m_posR = VECTOR_ZERO;						//�����_�̏����l
	m_DestPosV = VECTOR_ZERO;					//�ړI�̈ʒu
	m_Move = VECTOR_ZERO;						//�ړ���
	m_fRotDest = 0;
	m_pCustomer = NULL;							//���q����̃|�C���^
	m_pStartSignal = NULL;
	m_bSet = true;								//�J�����̐ݒ�
	m_bSetAngle = false;						//�p�x��ݒ肵�����ǂ���
	m_fOldRotY = 0.0f;							//�O��̌���
	m_fAngle = 0.0f;							//�p�x
	m_fHoldAngle = 0.0f;						//�ۊǂ���p�x
	m_fAddDirecting = 0.0f;
	m_Directing = DIRECTING_000;				// �ŏ��̃J�������[�N��
	return S_OK;
}

//===============================================================================
//�@�I������
//===============================================================================
void CGameCamera::Uninit(void)
{
	//���q����̃|�C���^�j��
	m_pCustomer = NULL;
	
	// �X�^�[�g�\���̔j��
	if (m_pStartSignal != NULL)
	{
		m_pStartSignal->Uninit();
		delete m_pStartSignal;
		m_pStartSignal = NULL;
	}
	
	CCamera::Uninit();
}

//===============================================================================
//�@�X�V����
//===============================================================================
void CGameCamera::Update(void)
{
	//��Ԃ��Ƃ̍X�V����
	switch (m_mode)
	{
	case MODE_NORMAL:   UpdateNormal();   break;	//�ʏ���
	case MODE_PUTON:    UpdatePutOn();    break;	//���q������悹����
	case MODE_TAKEDOEN: UpdateTakeDown(); break;	//���q����������Ƃ�
	case MODE_START:    UpdateStart();	  break;	//�Q�[���X�^�[�g���
	}
}

//===============================================================================
//�@�J�����̐ݒ�
//===============================================================================
void CGameCamera::SetCamera(void)
{
	CCamera::SetCamera();
}

//=============================================================================
// �Q�[���J�����ʏ�X�V����
//=============================================================================
void CGameCamera::UpdateNormal(void)
{
	//�v���C���[�̏����擾����
	CPlayer * pPlayer = NULL;
	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{//�Q�[��
		pPlayer = CGame::GetPlayer();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//�`���[�g���A��
		pPlayer = CTutorial::GetPlayer();
	}

	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
	D3DXVECTOR3 PlayerRot = pPlayer->GetRot();
	D3DXVECTOR3 PlayerMove = pPlayer->GetMove();

	//�����_�̍X�V
	m_posR.x = PlayerPos.x;
	m_posR.z = PlayerPos.z;
	m_posR.y = PlayerPos.y + HIGHT_R;

	//�v���C���[�ɒǏ]�悤�ɂ���
	m_fRotDest = PlayerRot.y - m_rot.y;
	RemakeAngle(&m_fRotDest);

	m_rot.y += m_fRotDest * MOVE_ANGLE;
	RemakeAngle(&m_rot.y);

	float fRot = m_rot.y - D3DX_PI;
	RemakeAngle(&fRot);

	//���_�X�V
	m_posV.x = PlayerPos.x - sinf(fRot) * -MOVE_CAMERA;
	m_posV.z = PlayerPos.z - cosf(fRot) * -MOVE_CAMERA;
	m_posV.y = PlayerPos.y + HIGHT_V;
}

//=============================================================================
// �Q�[���J�������q������悹�����̍X�V
//=============================================================================
void CGameCamera::UpdatePutOn(void)
{
	if (m_pCustomer == NULL) { return; }	//NULL�`�F�b�N

	//���q����̈ʒu���擾�������擾����
	D3DXVECTOR3 CustomerPos = m_pCustomer->GetPos();
	D3DXVECTOR3 CustomerRot = m_pCustomer->GetRot();

	//�����_�����q����ɍ��킹��
	m_posR.x = CustomerPos.x;
	m_posR.z = CustomerPos.z;

	//m_fHoldAngle = CustomerRot.y + m_rot.y;

	//�����̐ݒ�
	m_rot.y += CustomerRot.y * 0.008f;
	RemakeAngle(&m_rot.y);
	
	//�v���C���[�̈ʒu���擾
	CPlayer * pPlayer = NULL;
	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{//�Q�[��
		pPlayer = CGame::GetPlayer();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//�`���[�g���A��
		pPlayer = CTutorial::GetPlayer();
	}

	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

	//���_�̈ʒu�����߂�
	m_posV.x = PlayerPos.x + (sinf(m_rot.y) * -150.0f);
	m_posV.z = PlayerPos.z + (cosf(m_rot.y) * -150.0f);

	//�`����Đݒ肷��
	DrawReset();
}

//=============================================================================
// �Q�[���J�������q����������Ƃ��̍X�V
//=============================================================================
void CGameCamera::UpdateTakeDown(void)
{
	if (m_pCustomer == NULL) { return; }	//NULL�`�F�b�N

	//���q����̈ʒu���擾�������擾����
	D3DXVECTOR3 CustomerPos = m_pCustomer->GetPos();
	D3DXVECTOR3 CustomerRot = m_pCustomer->GetRot();

	//�����_�����q����ɍ��킹��
	m_posR.x = CustomerPos.x;
	m_posR.z = CustomerPos.z;

	//�p�x�����Z����
	m_rot.y += ADD_ANGLE;
	RemakeAngle(&m_rot.y);

	//���_��ݒ肷��
	m_posV.x = m_posR.x + sinf(m_rot.y) * -POSV_DISTANCE;
	m_posV.z = m_posR.z + cosf(m_rot.y) * -POSV_DISTANCE;
}

//=============================================================================
// �Q�[���J�����X�^�[�g�̂Ƃ��̍X�V
//=============================================================================
void CGameCamera::UpdateStart(void) 
{
	//�v���C���[�̏����擾����
	CPlayer * pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
	D3DXVECTOR3 PlayerRot = pPlayer->GetRot();

	//�����_�̍X�V
	m_posR.x = PlayerPos.x;
	m_posR.z = PlayerPos.z;
	m_posR.y = PlayerPos.y;

	switch (m_Directing)
	{
	case DIRECTING_000:		// �ŏ��̃J�������[�N
		//�p�x�����Z����
		m_rot.y += 0.01f;
		RemakeAngle(&m_rot.y);

		//���_�X�V
		m_posV.x = PlayerPos.x - sinf(m_rot.y) * 80.0f;
		m_posV.z = PlayerPos.z + -140.0f;
		m_posV.y = PlayerPos.y + 60.0f;

		if (m_rot.y >= 1.0f) { m_Directing = DIRECTING_001; m_rot.y = 0.0f; }	// ���̉��o��
		StartSkip();
		break;

	case DIRECTING_001:		// 2�ڂ̃J�������[�N
		m_rot.y += 0.01f;
		RemakeAngle(&m_rot.y);
		m_fAddDirecting += 0.3f;
		//���_�X�V
		m_posV.x = PlayerPos.x + sinf(m_rot.y) * -100.0f;
		m_posV.z = PlayerPos.z + cosf(m_rot.y) * -100.0f;
		m_posV.y = PlayerPos.y + 70.0f + m_fAddDirecting;

		if (m_rot.y >= 1.5f) { m_Directing = DIRECTING_002; m_rot.y = 0.0f; m_fAddDirecting = 0.0f; }	// ���̉��o��

		StartSkip();
		break;

	case DIRECTING_002:	// 3�ڂ̃J�������[�N
		m_rot.y += 0.01f;
		RemakeAngle(&m_rot.y);
		m_fAddDirecting += 0.3f;
		if (m_rot.y >= 1.0f) { m_Directing = DIRECTING_END; m_rot.y = 0.0f; m_fAddDirecting = 0.0f;}	// ���̉��o��

		//���_�X�V
		m_posV.x = PlayerPos.x - sinf(m_rot.y) * -200.0f - m_fAddDirecting;
		m_posV.z = PlayerPos.z - cosf(m_rot.y) * -200.0f - m_fAddDirecting;
		m_posV.y = PlayerPos.y + 100.0f;

		StartSkip();
		break;

	case DIRECTING_END:	// HERE WA GO��\��
		if (m_pStartSignal == NULL)
		{
			m_pStartSignal = CStartSignal::Create(D3DXVECTOR2(70.0f, 70.0f), CStartSignal::SIGNAL_HERE);
		}
		m_Directing = DIRECTING_NONE;
		break;

	default:		// �f�t�H���g�̏ꍇ
		UpdateNormal();
		if (m_pStartSignal != NULL)
		{
			if (m_pStartSignal->GetEnd() == true) 
			{ //�V�O�i���̔j��
				m_pStartSignal->Uninit(); 
				delete m_pStartSignal;
				m_pStartSignal = NULL; 
				m_mode = MODE_NORMAL; 
			}
			else { m_pStartSignal->Updata(); }
		}
		break;
	}
}

//=============================================================================
// �X�^�[�g���̃J�����X�L�b�v�@�\
//=============================================================================
void CGameCamera::StartSkip(void)
{
	if (CManager::GetInput()->GetKeyboardTrigger(DIK_RETURN))
	{	// ���o���X�L�b�v
		m_Directing = DIRECTING_END;
	}
}
//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void CGameCamera::SetModeCamera(MODECAMERA mode)
{
	//���������ɖ߂�
	if (mode == MODE_PUTON || mode == MODE_TAKEDOEN) { m_fOldRotY = m_rot.y; }

	m_oldmode = m_mode;		//�O��̏�Ԃ�ۑ�
	m_mode = mode;			//��Ԑݒ�
	m_bSet = false;	
}

//=============================================================================
// �`�悷����̂��Đݒ肷��
//=============================================================================
void CGameCamera::DrawReset(void)
{
	//�`���Ԃɂ���
	if (!m_pCustomer->GetDraw())
	{
		m_pCustomer->SetDraw(true);
		m_pCustomer->SetClipping(false);
	}

	//�I�u�W�F�N�g�̕`����Č����E�Đݒ肷��
	CScene *pScene;

	//�v���C�I���e�B�[�`�F�b�N
	pScene = CScene::GetTop(OBJECT_PRIOTITY);

	//NULL�`�F�b�N
	while (pScene != NULL)
	{
		//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
		{//�^�C�v����Q��������
			CObject *pObject = (CObject*)pScene;
			
			if (pObject->GetDraw() == false)
			{//������x�`�攻�������
				pObject->SetDraw(Clipping(pObject->GetVtxMin(), pObject->GetVtxMax()));
			}
		}

		//Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}

//=============================================================================
// �X�^�[�g���}���t�F�[�h�O�ɍ폜������
//=============================================================================
void CGameCamera::UninitSignal(void)
{
	// �X�^�[�g�\���̔j��
	if (m_pStartSignal != NULL)
	{
		m_pStartSignal->Uninit();
		delete m_pStartSignal;
		m_pStartSignal = NULL;
	}
}