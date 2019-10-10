//=============================================================================
//
// ���q����̏��� [customer.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "customer.h"
#include "manager.h"
#include "debugproc.h"
#include "input.h"
#include "gamecamera.h"
#include "scene.h"
#include "model.h"
#include "motion.h"
#include "wall.h"
#include "loadText.h"
#include "meshCylinder.h"
#include "destination.h"
#include "time.h"
#include "score.h"
#include "arrow.h"
#include "mark.h"
#include "reputation.h"
#include "satisfactionlevel.h"
#include "tutorial.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define LENGTH			(500.0f)	//�v���C���[�̔��a
#define PUTIN_LENGTH	(100.0f)	//��Ԕ��a
#define MOVE_SPEED		(2.8f)		//�ړ��X�s�[�h
#define MOVE_ANGLE		(0.2f)		//�p�x�̕ω���
#define DOWN_POS		(70.0f)		//�~�Ԉʒu
#define COUNT_TIME		(60 * 2)	//�ړ�����
#define COUNT_ESCAPE	(60)		//�����鎞��
#define MONEY_ADD		(10)		//���Z���邨��
#define METER			(100.0f)	//100���[�g��
#define ESCAPE_LENGTH	(100.0f)	//������͈�
#define CALLTAXI_LENGTH	(2000.0f)	//�^�N�V�[���ĂԔ͈�
#define MOTION_LENGTH	(1700.0f)	//���[�V�����Đ�����
#define DRAW_LENGTH		(4500)		//�l�̕`�拗��

//�q�����N�̋���
#define LOW_RANK		(15000.0f)
#define NORMAL_RANK		(20000.0f)
#define HIGHT_RANK		(25000.0f)

//�q�]���b��
#define BAD			(5)
#define SLOW		(9)
#define NORMAL		(14)

//���U���g�̍ő吔
#define MAX_RESULT	(3)

//�F�̒萔
#define RED				(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f))		//�ԐF
#define YELLOW			(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f))		//���F
#define GREEN			(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.5f))		//�ΐF
#define MAX_COL			(3)										//���q����̐F�̍ő吔
#define GOALCOL			(D3DXCOLOR(0.1f, 1.0f, 0.0f, 0.5f))		//�ړI�n�̐F
#define COLOR_ZERO		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))		//�F�̏����l

//�V�����_�[�̒萔
#define LAP				(8)			//���̒��_��
#define STAGE			(1)			//�i��
#define HIGHT			(40.0f)		//����
#define GOAL_HIGHT		(100.0f)	//�S�[������

// ���̒萔
#define ARROW_POS		(D3DXVECTOR3(CPlayer::GetPos().x, CPlayer::GetPos().y + 100.0f, CPlayer::GetPos().z))	// ���̈ʒu

//�}�[�N�̒萔
#define OFFSET			(200.0f)	//�I�t�Z�b�g

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================
bool CCustomer::m_bPutIn = false;

//=============================================================================
// ��������
//=============================================================================
CCustomer * CCustomer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 GoalPos, const int nGoalTex, int nAddTime, int nRideTime, MODEL_TYPE type)
{
	//�C���X�^���X�̐���
	CCustomer * pCustomer;
	pCustomer = new CCustomer;

	//�ݒ菈��
	pCustomer->SetModelType(type);

	//����������
	pCustomer->Init();

	//�ݒ菈��
	pCustomer->Set(pos, GoalPos);
	pCustomer->SetGoalTex(nGoalTex);
	pCustomer->SetAddTime(nAddTime);
	pCustomer->SetRideTime(nRideTime);
	return pCustomer;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCustomer::CCustomer() : CHumanBace(3, CScene::OBJTYPE_CUSTOMER){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCustomer::~CCustomer(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCustomer::Init(void)
{
	//�e�N���X�̏�����
	CHumanBace::Init();

	//�ϐ��̏�����
	m_OldPos	= VECTOR_ZERO;		//�O��̍��W
	m_OldDiffuse = VECTOR_ZERO;		//�O��̍���
	m_MoveMotion = VECTOR_ZERO;		//���[�V�����̈ړ�
	m_state = STATE_MAX;			//��Ԑݒ�
	m_OldState = STATE_MAX;			//�O��̏�Ԃ�ۑ�����
	m_fAngle = 0.0f;				//�p�x
	m_pMeshCylinder = NULL;			//���b�V���V�����_�[
	m_pDestination = NULL;			//�ړI�n�摜
	m_nAddTime = 0;					//���Z���鎞��
	m_pArrow = NULL;				//���̃|�C���^
	m_pMark = NULL;					//�}�[�N�̃|�C���^
	m_Color = COLOR_ZERO;			//�F
	m_bDrawMeshCylinder = false;	//�V�����_�[�̕`��t���O
	m_bGotOn = false;				//��������
	m_bResult = false;				//���ʂ����߂����ǂ���
	m_MotionLength = MOTION_LENGTH;	//���[�V�����̍X�V�͈�
	m_bPutIn = false;				//��Ԓ����ǂ���
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCustomer::Uninit(void)
{
	//�V�����_�[�̔j��
	if (m_pMeshCylinder != NULL) { m_pMeshCylinder = NULL; }

	// �ړI�n�ʐ^�̔j��
	if (m_pDestination != NULL) { m_pDestination->Uninit(); m_pDestination = NULL; }

	// ���̔j��
	if (m_pArrow != NULL) { m_pArrow->Uninit(); m_pArrow = NULL; }

	//�}�[�N�̔j��
	if (m_pMark != NULL) { m_pMark = NULL; }

	//�e�N���X�̔j��
	CHumanBace::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCustomer::Update(void)
{
	//�v���C���[���擾
	CManager::MODE mode = CManager::GetMode();
	CPlayer * pPlayer = NULL;
	CGameCamera * pGameCamera = NULL;
	bool bUpdate = true;

	if (mode == CManager::MODE_GAME)
	{//�Q�[��
		pPlayer = CGame::GetPlayer();
		pGameCamera = CGame::GetGameCamera();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//�`���[�g���A��
		pPlayer = CTutorial::GetPlayer();
		pGameCamera = CTutorial::GetCamera();
	}

	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();				//�v���C���[�̈ʒu���擾
	bool bPlayerPutin = pPlayer->GetPutin();				//��ԏ�Ԃ̎擾
	m_fLength = CHumanBace::GetLength(m_pos, PlayerPos);	//���������߂�

	//---------------------
	// �~�̕\��ON�EOFF
	//---------------------
	if (!m_bDrawMeshCylinder)	//�V�����_�[���`���ԂȂ�
	{
		if (bPlayerPutin == true)
		{//�V�����_�[�̔j��
			UninitMeshCylinder();
		}
		else
		{//�V�����_�[�̐���
			if (m_pMeshCylinder == NULL) { m_pMeshCylinder = CMeshCylinder::Create(m_pos, LAP, STAGE, LENGTH, HIGHT, m_Color); }
		}
		if (m_pMark != NULL)
		{//	�}�[�N�̈ʒu�ݒ�
			m_pMark->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + OFFSET, m_pos.z));
		}

		//�`���[�g���A����Ԃ�������
		if (mode == CManager::MODE_TUTORIAL)
		{
			if ((CTutorial::GetCustomerPutin() == false) && (m_pMeshCylinder != NULL))
			{//���b�V���V�����_�[�̔j��
				m_pMeshCylinder->Uninit();
				m_pMeshCylinder = NULL;
			}
			else if ((CTutorial::GetCustomerPutin() == true) && (m_pMeshCylinder == NULL))
			{//�V�����_�[�̐���
				m_pMeshCylinder = CMeshCylinder::Create(m_pos, LAP, STAGE, LENGTH, HIGHT, m_Color);
			}

			if (bPlayerPutin == true)
			{//�V�����_�[�̔j��
				UninitMeshCylinder();
			}
		}
	}

	//�`�攻��
	if (m_fLength > DRAW_LENGTH)
	{
		SetDraw(false);		//�`�悵�Ȃ�
		return;
	}
	else
	{
		if (!bPlayerPutin){ SetDraw(true); }
	}

	//NPC�͈͔̔���
	if ((pPlayer->GetCustomerStop() == true) && (!m_bGotOn))
	{
		float fLength = CHumanBace::GetLength(m_FirstPos, PlayerPos);	//���������߂�

		if ((m_state == STATE_CALLTAXI) || (m_state == STATE_ESCAPE) || (m_state == STATE_RETURN))
		{//��ԏ�Ԕ���
			if ((fLength < LENGTH) && (bPlayerPutin == false))
			{
				//��Ԃ��Ƃ̍X�V����
				if ((mode == CManager::MODE_TUTORIAL))
				{
					if (CTutorial::GetCustomerPutin() != true) { bUpdate = false; }
				}

				if (bUpdate)
				{//�X�V�\��Ԃ��������ԏ�Ԃɂ���
					SetState(STATE_PUTIN);
					pGameCamera->SetModeCamera(CGameCamera::MODE_PUTON);	//���q������悹���Ԃɂ���
					pGameCamera->SetCustomerInfo(this);						//���q����̏����J�����ɓn��
					pPlayer->SetCustomerMove(true);
				}
			}
		}
		else if(m_state == STATE_MOVE)
		{//�~�ԏ�Ԕ���
			float fLength = CHumanBace::GetLength(m_GoalPos, PlayerPos);	//���������߂�
			if ((fLength < LENGTH)) { SetState(STATE_DOWN); }				//�~�ԏ�Ԃɂ���
			pPlayer->SetCustomerMove(false);
		}
	}
	else if ((m_fLength < ESCAPE_LENGTH) && (m_state != STATE_MOVE) && (m_state != STATE_DOWN) && (m_state != STATE_DOWNMOVE) && (m_state != STATE_MISS))
	{
		//�T�E���h�̏��
		CSound *pSound = CManager::GetSound();

		if (m_ModelType == 0)
		{//	�j�����
			pSound->SetVolume(CSound::SOUND_LABEL_MAN_AVOID, 30.0f);
			pSound->PlaySound(CSound::SOUND_LABEL_MAN_AVOID);
		}
		else if (m_ModelType == 1)
		{//	�������
			pSound->SetVolume(CSound::SOUND_LABEL_WOMAN_AVOID, 2.0f);
			pSound->PlaySound(CSound::SOUND_LABEL_WOMAN_AVOID);
		}
		else
		{//	�q�����
			pSound->SetVolume(CSound::SOUND_LABEL_CHILD_AVOID, 11.0f);
			pSound->PlaySound(CSound::SOUND_LABEL_CHILD_AVOID);
		}

		if (m_state != STATE_ESCAPE) { SetState(STATE_ESCAPE); }
	}
	else if ((m_fLength < CALLTAXI_LENGTH) && (m_state == STATE_MAX) && (bPlayerPutin == false))
	{//�^�N�V�[���Ăԃ��[�V�����ɂ���
		SetState(STATE_CALLTAXI);
	}
	else if((m_state == STATE_CALLTAXI) && ((m_fLength > CALLTAXI_LENGTH)))
	{//�j���[�g�������[�V�����ɂ���
		if (m_MotionType != MOTION_NEUTRAL) { SetNextMotion(MOTION_NEUTRAL); }
		SetState(STATE_MAX);
	}

	//��Ԃ��Ƃ̍X�V����
	if ((mode == CManager::MODE_TUTORIAL))
	{
		if (CTutorial::GetCustomerPutin() != true) { bUpdate = false; }
	}

	if(bUpdate)
	{//�X�V�\��ԂȂ��
		switch (m_state)
		{
		case STATE_PUTIN:	 UpdatePutin(pPlayer, pGameCamera);		break;		// ��ԏ���
		case STATE_MOVE:	 UpdateMove(PlayerPos);					break;		// ��Ԉړ�����
		case STATE_DOWN:	 UpdateDown(pGameCamera);				break;		// �~�ԏ���
		case STATE_MISS:	 UpdateMiss(pPlayer, pGameCamera);		break;		// ���}���s����
		case STATE_DOWNMOVE: UpdateDownMove();						break;		// �~�Ԍ�̈ړ�����
		case STATE_ESCAPE:	 UpdateEscape();						break;		// �������
		case STATE_CALLTAXI: UpdateCallTaxi(PlayerPos);				break;		// �^�N�V�[���Ăԏ���
		case STATE_RETURN:	 UpdateReturn();						break;		// ���̈ʒu�ɖ߂鏈��
		case STATE_END:		 UpdateEnd(pPlayer);					break;		// �I����ԏ���
		}
	}

	//�e�N���X�̍X�V����
	CHumanBace::Update();

	// �ړI�n�ʐ^�̔j��
	if (m_pDestination != NULL && m_pDestination->GetDelet() == true)
	{
		m_pDestination->Uninit();
		m_pDestination = NULL;
	}

	//���̈ʒu�ݒ�
	if (m_pArrow != NULL) { m_pArrow->SetPosition(ARROW_POS); }
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCustomer::Draw(void)
{
	//�e�N���X�̕`�揈��
	CHumanBace::Draw(1.0f);
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void CCustomer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_pos = pos;		//�ʒu
	m_GoalPos = size;	//�S�[���̈ʒu
	m_FirstPos = pos;	//�����ʒu

	//���������N�̐ݒ�
	m_Color = SetRank();

	//�V�����_�[�̐���
	if (m_pMeshCylinder == NULL)
	{
		m_pMeshCylinder = CMeshCylinder::Create(pos, LAP, STAGE, LENGTH, HIGHT, m_Color);
	}

	//�}�[�N�̐���
	if (m_pMark == NULL)
	{
		CModel ** pModel = CHumanBace::GetpModel();
		m_pMark = CMark::Create(D3DXVECTOR3(m_pos.x, m_pos.y + OFFSET, m_pos.z));

		//�F�̐ݒ�
		if (m_pMark != NULL)
		{
			m_pMark->SetColor(D3DXCOLOR(m_Color.r, m_Color.g, m_Color.b, 1.0f));
			m_pMark->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + OFFSET, m_pos.z));
		}
	}
}

//=============================================================================
// ��ԏ��
//=============================================================================
void CCustomer::UpdatePutin(CPlayer * pPlayer, CGameCamera * pGameCamera)
{

	//	��Ԓ��ɐݒ�
	m_bPutIn = true;

	////�v���C���[���擾
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

	//�v���C���[�𑀍�ł��Ȃ���Ԃɂ���
	pPlayer->SetControl(false);

	//�}�[�N��j������
	if (m_pMark != NULL)
	{
		m_pMark->Uninit();
		m_pMark = NULL;
	}

	if (m_fLength > PUTIN_LENGTH)
	{//��Ԕ͈͊O��������
		//�v���C���[�ɋ߂Â�
		D3DXVECTOR3 Distance = m_pos - PlayerPos;
		float fAngle = atan2f(Distance.x, Distance.z);		//�v���C���[�Ƃ̊p�x�����߂�
		float fDiffese = fAngle - m_fAngle;					//���݂̊p�x�ƖړI�̊p�x�̍������߂�

		CHumanBace::RemakeAngle(&fDiffese);	//�p�x�̏C��
		m_fAngle += fDiffese * MOVE_ANGLE;	//�p�x�̕ω��ʂ����߂�
		CHumanBace::RemakeAngle(&m_fAngle);

		//�G�̕ύX�p�x�̍��W�����߂�
		m_move.x = sinf(m_fAngle) * -MOVE_SPEED;
		m_move.z = cosf(m_fAngle) * -MOVE_SPEED;

		m_rot.y = m_fAngle;		//�����̍X�V

		// �ړI�n�ʐ^�̐���
		if (m_pDestination == NULL) { m_pDestination = CDestination::Create(m_nGoalTex); }

		//�ړ����[�V�����ɂ���
		if (m_MotionType != MOTION_WALK) { SetNextMotion(MOTION_WALK); }
	}
	else
	{//��Ԕ͈͓���������
		m_move = VECTOR_ZERO;
		SetDraw(false);
		m_bClipping = false;				//�N���b�s���O�𖳌��ɂ���
		m_bDrawMeshCylinder = true;
		SetState(STATE_MOVE);				//�ړ���Ԃɂ���
		pPlayer->SetControl(true);			//�v���C���[�𑀍�ł����Ԃɂ���
		pPlayer->SetPutin(true);			//���q������悹����Ԃɂ���
		CTime::AddTime(m_nAddTime);			//���Ԃ̉��Z
		CTime::SetCutomerTime(m_nRideTime);	//���q����̏�Ԑ�������
		SetShadowColor_A(0.0f);				//�e�𓧖��ɂ���

		//�J������ʏ��Ԃɖ߂�
		pGameCamera->SetModeCamera(CGameCamera::MODE_NORMAL);

		//�T�E���h�̏��
		CSound *pSound = CManager::GetSound();

			if (m_ModelType == 0)
			{//	�j������
				pSound->SetVolume(CSound::SOUND_LABEL_MAN_THANK, 9.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_MAN_THANK);
			}
			else if (m_ModelType == 1)
			{//	��������
				pSound->SetVolume(CSound::SOUND_LABEL_WOMAN_THANK, 8.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_WOMAN_THANK);
			}
			else
			{//	�q���{��
				pSound->SetVolume(CSound::SOUND_LABEL_CHILD_THANK, 11.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_CHILD_THANK);
			}

		//���Z����X�R�A�����߂�
		SetFirstScore();
		CScore::SetScore(m_nFirstScore);	//���Z����X�R�A

		//�V�����_�[�̔j��
		UninitMeshCylinder();

		//�S�[���V�����_�[�̐���
		if (m_pMeshCylinder == NULL)
		{
			m_pMeshCylinder = CMeshCylinder::Create(m_GoalPos, LAP, STAGE, LENGTH, GOAL_HIGHT, GOALCOL);
		}
		//���̐���
		if (m_pArrow == NULL) { m_pArrow = CArrow::Create(CArrow::TYPE_DESTINATION, ARROW_POS, m_GoalPos, m_pMeshCylinder->GetRadius()); }

	}
	//	��Ԓ��̉���
	m_bPutIn = false;
}

//=============================================================================
// �ړ���Ԃ̏���
//=============================================================================
void CCustomer::UpdateMove(D3DXVECTOR3 PlayerPos)
{//���s������Ԃɂ���
	if (CTime::GetRideEnd() == true){	SetState(STATE_MISS); }
	m_pos = PlayerPos;
}

//=============================================================================
// �~�ԏ��
//=============================================================================
void CCustomer::UpdateDown(CGameCamera * pGameCamera)
{
	if (!m_bResult)
	{
		//�J�����̌������擾
		float fCameraRot = pGameCamera->GetRot();

		//�p�x�̐ݒ�E�C��
		float fAngle = D3DX_PI * -0.5f + fCameraRot;
		CHumanBace::RemakeAngle(&fAngle);

		//�Ԃ̉��ɂ��q������o��������
		m_pos.x += sinf(fAngle) * DOWN_POS;
		m_pos.z += cosf(fAngle) * DOWN_POS;

		//�p�x��ݒ肷��
		m_rot.y = fAngle;

		SetDraw(true);				//���f����`�悷��
		m_bClipping = true;			//�N���b�s���O��L���ɂ���
		SetShadowColor_A(1.0f);		//�e��\������
		UninitMeshCylinder();		//�V�����_�[�̔j��
		if (m_pArrow != NULL) { m_pArrow->Uninit(); m_pArrow = NULL; }		// ���̔j��

		//�J�����̃��[�h�ݒ�
		pGameCamera->SetModeCamera(CGameCamera::MODE_TAKEDOEN);

		//�]������
		CustomerResult();

		//�T�E���h�̏��
		CSound *pSound = CManager::GetSound();

		if (m_MotionType == MOTION_SHINOBU)
		{//���Ӄ��[�V����
			if (m_ModelType == 0)
			{//	�j������
				pSound->SetVolume(CSound::SOUND_LABEL_MAN_THANK, 9.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_MAN_THANK);
			}
			else if (m_ModelType == 1)
			{//	��������
				pSound->SetVolume(CSound::SOUND_LABEL_WOMAN_THANK, 8.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_WOMAN_THANK);
			}
			else
			{//	�q���{��
				pSound->SetVolume(CSound::SOUND_LABEL_CHILD_THANK, 11.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_CHILD_THANK);
			}
		}

		if (m_MotionType == MOTION_KICK || m_MotionType == MOTION_ANGER)
		{//�{�郂�[�V����
			if (m_ModelType == 0)
			{//	�j���{��
				pSound->SetVolume(CSound::SOUND_LABEL_MAN_ANGER, 9.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_MAN_ANGER);
			}
			else if (m_ModelType != 0 && m_ModelType != 1)
			{//	�q���{��
				pSound->SetVolume(CSound::SOUND_LABEL_CHILD_ANGER, 10.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_CHILD_ANGER);
			}
		}
		return;
	}

	//�~�肽��Ԃɂ���
	if (m_MotionType == MOTION_NEUTRAL)
	{
		SetState(STATE_DOWNMOVE);	//��Ԃ�ݒ肷��

		//�������[�V�����ɂ���
		if (m_MotionType != MOTION_WALK) { SetNextMotion(MOTION_WALK); }

		//�p�x�ݒ�
		float fCameraRot = pGameCamera->GetRot();
		float fAngle = D3DX_PI * -0.5f + fCameraRot;
		CHumanBace::RemakeAngle(&fAngle);
		m_rot.y = fAngle + D3DX_PI;
		CHumanBace::RemakeAngle(&m_rot.y);

		//�J�����̃��[�h�ݒ�
		pGameCamera->SetModeCamera(CGameCamera::MODE_NORMAL);

		//�ړ�������
		m_move.x += sinf(fAngle) * MOVE_SPEED;
		m_move.z += cosf(fAngle) * MOVE_SPEED;
	}
}

//=============================================================================
// ���}���s
//=============================================================================
void CCustomer::UpdateMiss(CPlayer * pPlayer, CGameCamera * pGameCamera)
{
	//�J�����̌������擾
	float fCameraRot = pGameCamera->GetRot();

	//�p�x�̐ݒ�E�C��
	float fAngle = D3DX_PI * -0.5f + fCameraRot;
	CHumanBace::RemakeAngle(&fAngle);

	//�Ԃ̉��ɂ��q������o��������
	m_pos.x += sinf(fAngle) * DOWN_POS;
	m_pos.z += cosf(fAngle) * DOWN_POS;

	//�ړ�������
	m_move.x += sinf(fAngle) * MOVE_SPEED;
	m_move.z += cosf(fAngle) * MOVE_SPEED;
	fAngle *= -1;
	RemakeAngle(&fAngle);

	//�p�x��ݒ肷��
	m_rot.y = fAngle;

	//���f����`�悷��
	SetDraw(true);
	m_bClipping = true;		//�N���b�s���O��L���ɂ���

	//�e��\������
	SetShadowColor_A(1.0f);

	//�V�����_�[�̔j��
	UninitMeshCylinder();

	// ���̔j��
	if (m_pArrow != NULL) { m_pArrow->Uninit(); m_pArrow = NULL; }

	//�ړ����[�V�����ɂ���
	if (m_MotionType != MOTION_WALK) { SetNextMotion(MOTION_WALK); }

	// �]���̐���
	CReputation::Create(-1);

	pPlayer->SetPutin(false);	//���q������悹�����Ԃɂ���
	CScore::Reset();			//�X�R�A�̒l��j������
	SetState(STATE_DOWNMOVE);	//�~�肽��Ԃɂ���

}

//=============================================================================
// �~�肽��Ԃ̍X�V
//=============================================================================
void CCustomer::UpdateDownMove(void)
{
	m_nCountTime++;
	m_bGotOn = true;	//�������Ԃɂ���

	if (m_nCountTime > COUNT_TIME)
	{
		m_move = VECTOR_ZERO;	//�ړ��ʂ�����������
		m_nCountTime = 0;		//�J�E���^�[�̏�����
		SetState(STATE_END);
	}
}

//=============================================================================
// �����鏈��
//=============================================================================
void CCustomer::UpdateEscape(void)
{

	//������ړ�����
	CHumanBace::MoveEscape();

	//�����郂�[�V�����ɂ���
	if (m_MotionType != MOTION_ESCAPE) { SetNextMotion(MOTION_ESCAPE);}

	//�ʏ�̏�Ԃɖ߂�
	if (m_nCountTime > COUNT_ESCAPE)
	{
		m_move = VECTOR_ZERO;	//�ړ��ʂ�����������
		m_nCountTime = 0;		//�J�E���^�[�̏�����

		SetState(STATE_RETURN);	//���̈ʒu�ɖ߂��Ԃɂ���
	}
}

//=============================================================================
// �^�N�V�[���Ă�
//=============================================================================
void CCustomer::UpdateCallTaxi(D3DXVECTOR3 PlayerPos)
{
	//�^�N�V�[���Ăԃ��[�V�����ɂ���
	if (m_MotionType != MOTION_TAXICOLL) { SetNextMotion(MOTION_TAXICOLL); }

	//�^�N�V�[�̕����Ɍ�����
	D3DXVECTOR3 Distance = m_pos - PlayerPos;
	float fAngle = atan2f(Distance.x, Distance.z);
	RemakeAngle(&fAngle);
	m_rot.y = fAngle;
}

//=============================================================================
// ��Ԍ�̏���
//=============================================================================
void CCustomer::UpdateEnd(CPlayer * pPlayer)
{
	//�v���C���[�̈ʒu���擾
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

	//�j���[�g�������[�V�����ɂ���
	if (m_MotionType != MOTION_NEUTRAL) { SetNextMotion(MOTION_NEUTRAL); }
}

//=============================================================================
// ���q����̈ʒu��߂�����
//=============================================================================
void CCustomer::UpdateReturn(void)
{
	//�v���C���[�ɋ߂Â�
	D3DXVECTOR3 Distance = m_pos - m_OldPos;
	float fLength = GetLength(m_pos, m_OldPos);			//���݂̈ʒu�ƑO��̈ʒu�̋��������߂�

	if (fLength < 1.0f && fLength > -1.0f)
	{
		SetState(m_OldState);	//�O��̏�Ԃɖ߂�
		m_move = VECTOR_ZERO;
	}
	else
	{
		float fAngle = atan2f(Distance.x, Distance.z);		//�v���C���[�Ƃ̊p�x�����߂�
		float fDiffese = fAngle - m_fAngle;					//���݂̊p�x�ƖړI�̊p�x�̍������߂�

		CHumanBace::RemakeAngle(&fDiffese);	//�p�x�̏C��
		m_fAngle += fDiffese * MOVE_ANGLE;	//�p�x�̕ω��ʂ����߂�
		CHumanBace::RemakeAngle(&m_fAngle);

		//�G�̕ύX�p�x�̍��W�����߂�
		m_move.x = sinf(m_fAngle) * -MOVE_SPEED;
		m_move.z = cosf(m_fAngle) * -MOVE_SPEED;

		m_rot.y = m_fAngle;		//�����̍X�V

		//�ړ����[�V�����ɂ���
		if (m_MotionType != MOTION_WALK) { SetNextMotion(MOTION_WALK); }
	}
}

//=============================================================================
// ���������N�����߂�
//=============================================================================
D3DXCOLOR CCustomer::SetRank(void)
{
	//�ϐ��錾
	D3DXCOLOR aCol[MAX_COL + 1] = {RED, YELLOW, GREEN, GREEN };					//�F�̐ݒ�
	float faDistance[MAX_COL + 1] = { 0, LOW_RANK, NORMAL_RANK, HIGHT_RANK };	//���������N�̐ݒ�
	int nCntRank = 0;

	//���݂̈ʒu�ƃS�[���܂ł̋��������߂�
	float fLength = CHumanBace::GetLength(m_pos, m_GoalPos);

	for (nCntRank = 0; nCntRank < MAX_COL; nCntRank++)
	{//�����N�͈͓̔����ǂ���
		if ((faDistance[nCntRank] <= fLength) && (fLength < faDistance[nCntRank + 1])) { break; }
	}

	return aCol[nCntRank];
}

//=============================================================================
// ��ԗ��������߂�
//=============================================================================
void CCustomer::SetFirstScore(void)
{
	//���������߂�
	float fLength = CHumanBace::GetLength(m_pos, m_GoalPos);
	float fMeter = (float)(fLength / METER);
	m_nFirstScore = (int)(fMeter * MONEY_ADD);
}

//=============================================================================
// �V�����_�[�̔j��
//=============================================================================
void CCustomer::UninitMeshCylinder(void)
{
	if (m_pMeshCylinder != NULL)
	{
		m_pMeshCylinder->Uninit();
		m_pMeshCylinder = NULL;
	}
}

//=============================================================================
// ���q����̕]������
//=============================================================================
void CCustomer::CustomerResult(void)
{
	//�ϐ��錾
	int nCntRank;
	int nTimeRank[MAX_RESULT + 1] = {0, BAD, SLOW, NORMAL};												//���Ԃ̐ݒ�
	MOTION_TYPE MotionType[MAX_RESULT + 1] = {MOTION_ANGER, MOTION_KICK, MOTION_SHINOBU, MOTION_BOW};	//���[�V�����̐ݒ�

	//���Ԃ̎擾
	int nTime = CTime::GetCutomerTime();

	//�ǂ̃����N�ɂ���̂�
	for (nCntRank = 0; nCntRank < MAX_RESULT; nCntRank++)
	{
		if ((nTimeRank[nCntRank] < nTime) && (nTimeRank[nCntRank + 1] >= nTime))
		{
			break;
		}
	}

	//���[�V������ݒ肷��
	if (m_ModelType != MotionType[nCntRank]) { SetNextMotion(MotionType[nCntRank]); }

	// �]���̐���
	CReputation::Create(nCntRank);

	CTime::ResetCutomerTime();	//�������Ԃ����Z�b�g����
	CScore::AddTotalScore();	//�g�[�^���X�R�A���Z
	m_bResult = true;

	CManager::MODE mode = CManager::GetMode();		//���[�h�̎擾
	CPlayer * pPlayer = NULL;

	if (mode == CManager::MODE_GAME)
	{//�Q�[��
		pPlayer = CGame::GetPlayer();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//�`���[�g���A��
		pPlayer = CTutorial::GetPlayer();
	}

	pPlayer->SetPutin(false);	//���q������悹�����Ԃɂ���

}

//=============================================================================
// ��Ԃ̐ݒ�
//=============================================================================
void CCustomer::SetState(STATE state)
{
	m_OldState = m_state;	//�O��̏�Ԃ�ۑ�����
	m_state = state;		//��Ԃ�ݒ肷��

	if (m_OldState == STATE_ESCAPE) 
	{
		m_OldState = STATE_END;
	}

	if (m_state == STATE_ESCAPE)
	{
		m_OldPos = m_pos;
	}
}