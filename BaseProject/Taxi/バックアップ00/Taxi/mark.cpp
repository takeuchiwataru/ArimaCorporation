//=============================================================================
//
// �q�̓���ɏo��}�[�N�̏��� [mark.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "mark.h"
#include "manager.h"
#include "renderer.h"
#include "customer.h"
#include "game.h"
#include "tutorial.h"
#include "player.h"

//==================================
// �}�N����`
//==================================
#define MARK_TEXTURE	"data/TEXTURE/game/CustomerMark/CustomerMark.png"			//�ǂݍ��ރe�N�X�`���t�@�C����
#define MARK_MODEL		"data/MODEL/���q����}�[�N/CustomerMark.x"
#define MARK_LENGTH		(12500)	//�`��͈�
#define UPDATE_LENGTH	(7500)	//�X�V�͈�


//==================================
// �ÓI�����o�ϐ��錾
//==================================
LPD3DXMESH CMark::m_pLoadMesh = NULL;
LPD3DXBUFFER CMark::m_pLoadBuffMat = NULL;
DWORD CMark::m_nLoadNumMat = 0;

//==================================
// ��������
//==================================
CMark* CMark::Create(D3DXVECTOR3 pos)
{
	CMark *pMark;

	pMark = new CMark;	// ���������m��

	if (pMark != NULL)
	{// �������m�ې���
		pMark->m_pos = pos;
		pMark->Init();
	}

	return pMark;
}

//=========================================
// �R���X�g���N�^
//=========================================
CMark::CMark()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_fCntPos = 0.0f;
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
}

//=========================================
// �f�X�g���N�^
//=========================================
CMark::~CMark(){}

//=========================================
// ���f���̓ǂݍ��ݏ���
//=========================================
void CMark::LoadModel(void)
{
	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MARK_MODEL,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pLoadBuffMat,
		NULL,
		&m_nLoadNumMat,
		&m_pLoadMesh);
}

//=========================================
// ���f���̔j��
//=========================================
void CMark::UnloadModel(void)
{
	//���b�V���̔j��
	if (m_pLoadMesh != NULL)
	{
		m_pLoadMesh->Release();
		m_pLoadMesh = NULL;
	}

	//�}�e���A���̔j��
	if (m_pLoadBuffMat != NULL)
	{
		m_pLoadBuffMat->Release();
		m_pLoadBuffMat = NULL;
	}

	//�}�e���A���̍ő吔��������
	m_nLoadNumMat = 0;
}

//=========================================
// ����������
//=========================================
HRESULT CMark::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`������
	D3DXCreateTextureFromFile(pDevice, MARK_TEXTURE, &m_pTexture);

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	m_size = D3DXVECTOR2(50.0f, 50.0f);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_��
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f) * 0.5f;
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f) * 0.5f;
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f) * 0.5f;
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f) * 0.5f;

	//�@���x�N�g��
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//���_�J���[
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@�̃A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CMark::Uninit(void)
{
	// ������j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CMark::Update(void)
{
	//�v���C���[�Ƃ̋��������߂�
	CManager::MODE mode = CManager::GetMode();
	D3DXVECTOR3 PlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (mode == CManager::MODE_GAME)
	{
		PlayerPos = CGame::GetPlayer()->GetPos();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{
		PlayerPos = CTutorial::GetPlayer()->GetPos();
	}

	D3DXVECTOR3 Distance = PlayerPos - m_pos;
	float fLength = sqrtf((Distance.x * Distance.x) + (Distance.z * Distance.z));

	if (fLength < UPDATE_LENGTH)
	{//�X�V�͈�
	 // ������ς���
		m_fCntPos += 0.05f;
		m_pos.y += sinf(m_fCntPos) * 1.0f;

		// ���f������]������
		m_rot.y += 0.02f;
	}
	else if (fLength < MARK_LENGTH)
	{
		SetDraw(true);	//�`���Ԃɂ���
	}
	else
	{//�`�悵�Ȃ���Ԃɂ���
		SetDraw(false);
	}
}

//=========================================
// �`�揈��
//=========================================
void CMark::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxView, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	//�A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//���f���̕`��
	DrawModel();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�t�s���ݒ�
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//�A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=========================================
// ���f���̕`�揈��
//=========================================
void CMark::DrawModel(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;					//���݂̃}�e���A����ۑ�
	D3DXMATERIAL *pMat;						//�}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pLoadBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nLoadNumMat; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pMat[nCntMat].MatD3D.Diffuse = m_col;
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`����NULL�ɂ���
		pDevice->SetTexture(0, NULL);

		//�I�u�W�F�N�g(�p�[�c)�̕`��
		m_pLoadMesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=========================================
// �F�̐ݒ�
//=========================================
void CMark::SetColor(D3DXCOLOR col)
{
	m_col = col;

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//���_�o�b�t�@�̃A�����b�N����
	m_pVtxBuff->Unlock();
}
