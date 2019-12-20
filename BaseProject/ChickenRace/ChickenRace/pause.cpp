//=============================================================================
//
// �|�[�Y�̏��� [pause.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "scene.h"
#include "pause.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME_1			"data\\TEXTURE\\pause\\pause000.png"	//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_2			"data\\TEXTURE\\pause\\pause001.png"	//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_3			"data\\TEXTURE\\pause\\pause002.png"	//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_4			"data\\TEXTURE\\pause\\pause_icon.png"	//�ǂݍ��ރe�N�X�`���t�@�C��
#define MAX_PAUSE				(128)									//�|�[�Y�̍ő吔
#define PAUSE_POS_X				(0)										//�|�[�Y�̍���X���W
#define PAUSE_POS_Y				(0)										//�|�[�Y�̍���Y���W
#define PAUSE_WIDTH				(SCREEN_WIDTH)							//�|�[�Y�̕�
#define PAUSE_HEIGHT			(SCREEN_HEIGHT)							//�|�[�Y�̍���
#define PAUSE_TEXTURE_UV_U		(1.0f)									//�e�N�X�`���A�j���[�V����U�͈�
#define PAUSE_TEXTURE_UV_V		(1.0f)									//�e�N�X�`���A�j���[�V����V�͈�
#define PAUSE_SPEED				(0.009f)								//�|�[�Y�_�ŃX�s�[�h
#define PAUSE_LIMIT				(0.5f)									//�_�ł�����
#define PAUSE_COLOR_INIT		(1.0f)									//�|�[�Y�̐F�̏����l

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPause::m_pTexture[MAX_PAUSE_TEXTURE] = {};
bool			   CPause::m_bPause = false;
bool			   CPause::m_bSerectEnd = false;

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CPause::CPause(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_SelectColor = 0.0f;
	m_nCntState = 0;
	m_SelectNum = 0;
	m_nSize = D3DXVECTOR2(0.0f, 0.0f);
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CPause::~CPause()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CPause::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_pos = D3DXVECTOR3((PAUSE_POS_X + PAUSE_WIDTH) / 2, (PAUSE_POS_Y + PAUSE_HEIGHT) / 2, 0.0f);
	m_SelectNum = 0;
	m_SelectColor = PAUSE_COLOR_INIT;

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

						// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_PAUSE_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL
	);

	//�|�[�Y�̍��W
	float PosPause = (SCREEN_HEIGHT / 2) - 270;

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_PAUSE_TEXTURE; nCount++)
	{//����ȊO�̏ꍇ���ׂĂ̏�����

		if (nCount == 0)
		{//�J�E���g�O�������ꍇ
		 //���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_HEIGHT, 0.0f);
			//���_�J���[�ݒ�
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
		}
		else if (nCount == 4)
		{
			//���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3(PAUSE_WIDTH / 2 - 320, (SCREEN_HEIGHT / 2) - 195 - 50, 0.0f);		//����
			pVtx[1].pos = D3DXVECTOR3(PAUSE_WIDTH / 2 + 320, (SCREEN_HEIGHT / 2) - 195 - 50, 0.0f);		//�E��
			pVtx[2].pos = D3DXVECTOR3(PAUSE_WIDTH / 2 - 320, (SCREEN_HEIGHT / 2) - 195 + 50, 0.0f);		//����
			pVtx[3].pos = D3DXVECTOR3(PAUSE_WIDTH / 2 + 320, (SCREEN_HEIGHT / 2) - 195 + 50, 0.0f);		//�E��

																										//���_�J���[�ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{//����ȊO�̏ꍇ
		 //���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3(PAUSE_WIDTH / 2 - 250, PosPause, 0.0f);			//����
			pVtx[1].pos = D3DXVECTOR3(PAUSE_WIDTH / 2 + 250, PosPause, 0.0f);			//�E��
			pVtx[2].pos = D3DXVECTOR3(PAUSE_WIDTH / 2 - 250, PosPause + 150, 0.0f);		//����
			pVtx[3].pos = D3DXVECTOR3(PAUSE_WIDTH / 2 + 250, PosPause + 150, 0.0f);		//�E��

			if (nCount == 1)
			{
				//���_�J���[�ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{
				//���_�J���[�ݒ�
				pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			}
			//�I�𗓂̕�
			PosPause += 200;
		}

		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//�e�N�X�`���ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(PAUSE_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, PAUSE_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(PAUSE_TEXTURE_UV_U, PAUSE_TEXTURE_UV_V);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPause::Uninit(void)
{
	//�I��ł��Ԃ�߂�
	m_bSerectEnd = false;

	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//���g�̍폜
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPause::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(CGame::GetControllerNum(0));		//�W���C�p�b�g�̎擾
	CInputMouse *pCInputMouse = CManager::GetInputMouse();

	//�T�E���h�̏��
	CSound *pSound = CManager::GetSound();

	//�t�F�[�h�̃|�C���g
	CFade::FADE pFade = CFade::GetFade();

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (m_bSerectEnd == false &&
		(pCInputKeyBoard->GetKeyboardTrigger(DIK_UP) == true || pCInputKeyBoard->GetKeyboardTrigger(DIK_W) == true ||
			pXpad->GetTrigger(INPUT_UP) == true || pXpad->GetTrigger(INPUT_LS_U) == true))
	{//���L�[�̏���������Ƃ�

	 //�|�[�Y�̑I�����ړ���
	 //pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_MOVE);

	 // ���_�J���[�ݒ�
		pVtx[m_SelectNum * 4 + 4].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[m_SelectNum * 4 + 1 + 4].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[m_SelectNum * 4 + 2 + 4].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[m_SelectNum * 4 + 3 + 4].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

		m_SelectNum = (m_SelectNum + 2) % 3;
		m_SelectColor = PAUSE_COLOR_INIT;
	}
	if (m_bSerectEnd == false &&
		(pCInputKeyBoard->GetKeyboardTrigger(DIK_DOWN) == true || pCInputKeyBoard->GetKeyboardTrigger(DIK_S) == true ||
			pXpad->GetTrigger(INPUT_DOWN) == true || pXpad->GetTrigger(INPUT_LS_D) == true))
	{//���L�[�̉����������Ƃ�

	 //�|�[�Y�̑I�����ړ���
	 //pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_MOVE);

	 // ���_�J���[�ݒ�
		pVtx[m_SelectNum * 4 + 4].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[m_SelectNum * 4 + 1 + 4].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[m_SelectNum * 4 + 2 + 4].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[m_SelectNum * 4 + 3 + 4].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

		m_SelectNum = (m_SelectNum + 1) % 3;
		m_SelectColor = PAUSE_COLOR_INIT;
	}

	//�_�ŏ���
	//�I�����Ă���J���[�̒l�����炵�Ă���
	m_SelectColor -= PAUSE_SPEED;

	if (m_SelectColor < PAUSE_LIMIT)
	{//�M�����Ă���J���[��0.5��؂�����J���[�̒l�������Ă�
		m_SelectColor = PAUSE_COLOR_INIT;
	}

	// ���_�J���[�ݒ�
	pVtx[m_SelectNum * 4 + 4].col = D3DXCOLOR(m_SelectColor, m_SelectColor, m_SelectColor, 1.0f);
	pVtx[m_SelectNum * 4 + 1 + 4].col = D3DXCOLOR(m_SelectColor, m_SelectColor, m_SelectColor, 1.0f);
	pVtx[m_SelectNum * 4 + 2 + 4].col = D3DXCOLOR(m_SelectColor, m_SelectColor, m_SelectColor, 1.0f);
	pVtx[m_SelectNum * 4 + 3 + 4].col = D3DXCOLOR(m_SelectColor, m_SelectColor, m_SelectColor, 1.0f);


	//���_�ݒ�
	pVtx[(4 * 4) + 0].pos = D3DXVECTOR3(PAUSE_WIDTH / 2 - 320.0f, (SCREEN_HEIGHT / 2) - 195.0f - 50.0f + (m_SelectNum * 200.0f), 0.0f);		//����
	pVtx[(4 * 4) + 1].pos = D3DXVECTOR3(PAUSE_WIDTH / 2 + 320.0f, (SCREEN_HEIGHT / 2) - 195.0f - 50.0f + (m_SelectNum * 200.0f), 0.0f);		//�E��
	pVtx[(4 * 4) + 2].pos = D3DXVECTOR3(PAUSE_WIDTH / 2 - 320.0f, (SCREEN_HEIGHT / 2) - 195.0f + 50.0f + (m_SelectNum * 200.0f), 0.0f);		//����
	pVtx[(4 * 4) + 3].pos = D3DXVECTOR3(PAUSE_WIDTH / 2 + 320.0f, (SCREEN_HEIGHT / 2) - 195.0f + 50.0f + (m_SelectNum * 200.0f), 0.0f);		//�E��


																																			//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	if (m_bSerectEnd == false &&
		(pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true || pCInputKeyBoard->GetKeyboardTrigger(DIK_Z) == true ||
			pXpad->GetTrigger(INPUT_B) == true || pXpad->GetTrigger(INPUT_R2) == true))
	{
		if (m_SelectNum == 0)
		{
			//�|�[�Y�����
			CGame::SetPause(false);
		}
		else if (m_SelectNum == 1)
		{
			//�Q�[���̍ŏ��ɖ߂�
			CFade::Create(CManager::MODE_GAME);

			//�|�[�Y�őI�񂾏��
			m_bSerectEnd = true;
		}
		else if (m_SelectNum == 2)
		{
			//�Q�[���̃^�C�g���ɖ߂�
			CFade::Create(CManager::MODE_TITLE);

			//�|�[�Y�őI�񂾏��
			m_bSerectEnd = true;
		}

		if (pFade == CFade::FADE_NONE)
		{
			//�|�[�Y�̑I���̌��艹
			//pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_SELECT);
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CPause::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�|���S���̕`��
	for (int nCount = 0; nCount < MAX_PAUSE_TEXTURE; nCount++)
	{//�r���{�[�h���J�E���g
	 // ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource
		(
			0,
			m_pVtxBuff,
			0,
			sizeof(VERTEX_2D)
		);

		//���_�t�H�[�}�b�g
		pDevice->SetFVF(FVF_VERTEX_2D);

		pDevice->SetTexture(0, m_pTexture[nCount]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
	}
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CPause * CPause::Create()
{
	CPause *pPause = NULL;

	//NULL�`�F�b�N
	if (pPause == NULL)
	{//�������̓��I�m��

		pPause = new CPause;

		if (pPause != NULL)
		{
			//�I�u�W�F�N�g�N���X�̐���
			pPause->Init();
		}
	}

	return pPause;
}
//===============================================================================
// �e�N�X�`���̓ǂݍ���
//===============================================================================
HRESULT CPause::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	m_pTexture[0] = NULL;
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_2, &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_3, &m_pTexture[3]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_4, &m_pTexture[4]);

	return S_OK;
}
//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CPause::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_PAUSE_TEXTURE; nCount++)
	{
		//�e�N�X�`���[�̔j��
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}