//=============================================================================
//
// �r���{�[�h�̏��� [scene2D.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "scene.h"
#include "billboord.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "game.h"
#include "player.h"
#include "logo.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BILL_TEXTURE_NAME_1		"data\\TEXTURE\\State\\none.png"			//�ǂݍ��ރe�N�X�`���t�@�C��
#define BILL_TEXTURE_NAME_2		"data\\TEXTURE\\State\\exclamation.png"		//�ǂݍ��ރe�N�X�`���t�@�C��
#define BILL_TEXTURE_NAME_3		"data\\TEXTURE\\State\\attack.png"			//�ǂݍ��ރe�N�X�`���t�@�C��
#define BILL_TEXTURE_NAME_4		"data\\TEXTURE\\State\\qusetion.png"		//�ǂݍ��ރe�N�X�`���t�@�C��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3DTEXTURE9	CBillBoord::m_pTexture[MAX_BILLBOORD_TEX] = {};

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CBillBoord::CBillBoord() : CScene3D(3, CScene::OBJTYPE_BILLBOORD)
{
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_bOnOff = false;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CBillBoord::~CBillBoord()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CBillBoord::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CScene3D::Init();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBillBoord::Uninit(void)
{
	CScene3D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CBillBoord::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CScene3D::Update();

	//�v���C���[�̈ʒu���
	//D3DXVECTOR3 EnemyPos = CGame::GetEnemy()->GetPos();

	//CDebugProc::Print("�����ꏊ x = %.1f,y = %.1f,z = %.1f\n", EnemyPos.x, EnemyPos.y + 100.0f, EnemyPos.z);

	//�����̏��
	if (m_State == BILLBOORD_EXCLAMATION)
	{
		//����
		BindTexture(m_pTexture[1]);
		//�\���̏�Ԃ�n��
		CLogo::SetLogoState(2, 7);
	}
	//�ǔ��̏��
	else if (m_State == BILLBOORD_ATTACK)
	{
		//�ǔ�
		BindTexture(m_pTexture[2]);
		//�\���̏�Ԃ�n��
		CLogo::SetLogoState(3,8);

	}	
	//�^��̏��
	else if (m_State == BILLBOORD_QUESTION)
	{
		//�^��
		BindTexture(m_pTexture[3]);
		//�\���̏�Ԃ�n��
		CLogo::SetLogoState(1,6);
	}
	//�������Ă��Ȃ����
	else if (m_State == BILLBOORD_NONE)
	{
		//�������Ă��Ȃ����
		BindTexture(m_pTexture[0]);
		//�\���̏�Ԃ�n��
		CLogo::SetLogoState(0,5);
	}

	//�r���{�[�h
	//CScene3D::SetPosition(D3DXVECTOR3(EnemyPos.x, EnemyPos.y + 140.0f, EnemyPos.z));
}
//=============================================================================
// �`�揈��
//=============================================================================
void CBillBoord::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�ʒu�̏�����
	D3DXVECTOR3 BillPos = CScene3D::GetPosition();
	//�v���C���[�̈ʒu���
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	//�Q�[���̏��
	CManager::MODE pMode = CManager::GetMode();

	CScene3D::Draw();
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CBillBoord * CBillBoord::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nTexType)
{
	CBillBoord *pBillBoord = NULL;

	//NULL�`�F�b�N
	if (pBillBoord == NULL)
	{//�������̓��I�m��

		pBillBoord = new CBillBoord;

		if (pBillBoord != NULL)
		{
			//�I�u�W�F�N�g�N���X�̐���
			pBillBoord->Init();
			//�e�N�X�`���̊��蓖��
			pBillBoord->BindTexture(m_pTexture[nTexType]);
			//�ʒu�̊��蓖��
			pBillBoord->SetPosSize(pos, size);
			//�T�C�Y������
			pBillBoord->m_size = size;
		}
	}

	return pBillBoord;
}
//===============================================================================
// �e�N�X�`���̓ǂݍ���
//===============================================================================
HRESULT CBillBoord::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, BILL_TEXTURE_NAME_1, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, BILL_TEXTURE_NAME_2, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, BILL_TEXTURE_NAME_3, &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, BILL_TEXTURE_NAME_4, &m_pTexture[3]);

	return S_OK;
}
//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CBillBoord::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_BILLBOORD_TEX; nCount++)
	{
		//�e�N�X�`���[�̔j��
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}
//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CBillBoord::ResetBillboord(void)
{
	CScene *pScene;

	//�v���C�I���e�B�[�`�F�b�N
	pScene = CScene::GetTop(4);

	//NULL�`�F�b�N
	while (pScene != NULL)
	{
		//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{//�^�C�v���ǂ�������
			if (pScene->GetObjType() == OBJTYPE_BILLBOORD)
			{
				//�������Ă��Ȃ����
				pScene->Uninit();
			}
		}
		//Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
	//�\���̏�Ԃ�n��
	//CLogo::SetLogoState(0, 5);
}

