//=============================================================================
//
// 3D�p�[�e�B�N������ [3Dparticle.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "3Dparticle.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "shadermgr.h"
#include "rotationshader.h"
#include "game.h"
#include "gamecamera.h"

//=============================================================================
// ��������
//=============================================================================
C3DParticle * C3DParticle::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�C���X�^���X�̐���
	C3DParticle * p3DParticle;
	p3DParticle = new C3DParticle;

	//����������
	p3DParticle->Init();

	//�ݒ菈��
	p3DParticle->Set(pos, size);

	return p3DParticle;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
C3DParticle::C3DParticle() : CScene(4, OBJTYPE_EFFECT)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
C3DParticle::C3DParticle(int nPriority, CScene::OBJTYPE obj) : CScene(nPriority, obj){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
C3DParticle::~C3DParticle(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT C3DParticle::Init(void)
{
	//�ϐ��錾
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pRotationShader = NULL;
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
	m_Division = D3DXVECTOR2(1, 1);

	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-50.0f, 50.0f, 50.0f);
	pVtx[1].pos = D3DXVECTOR3(50.0f, 0.0f, 50.0f);
	pVtx[2].pos = D3DXVECTOR3(-50.0f, 50.0f, -50.0f);
	pVtx[3].pos = D3DXVECTOR3(50.0f, 0.0f, -50.0f);

	pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//UV�l�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//�V�F�[�_�[�̎擾
	/*m_pRotationShader = NULL;
	m_pRotationShader = CGame::GetShaderMgr()->GetRotatonShader();
	m_pShader = m_pRotationShader->GetShader();*/

	/*if (m_pRotationShader != NULL)
	{
		m_pRotationShader->SetCol_a(1.0f);
	}*/
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void C3DParticle::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture = NULL;
	}

	//���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void C3DParticle::Update(void)
{
	//�N���b�s���O�����̒ǉ�
	CGameCamera * pGameCamera = CGame::GetGameCamera();
	SetDraw(pGameCamera->ClippingPos(m_pos));
}

//=============================================================================
// �`�揈��
//=============================================================================
void C3DParticle::Draw(void)
{
	if (m_pVtxBuff != NULL)
	{
		//�����_�����O�N���X���擾
		CRenderer * pRenderer = NULL;
		pRenderer = CManager::GetRenderer();

		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		D3DXMATRIX		  mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			//���ʃJ�����O				
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);					// ���C�e�B���O�𖳌��ɂ���

		//�V�F�[�_�[�̐ݒ�
		//if(m_pRotationShader != NULL){ m_pRotationShader->SetBegin(); }
		//m_pShader->SetTechnique("StandardShader");
		//m_pShader->Begin(0, 0);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//�e�}�g���b�N�X���V�F�[�_�[�ɓn��
		if (m_pRotationShader != NULL)
		{
			//CGameCamera * pCamera = CGame::GetGameCamera();
			//D3DXMATRIX mtxView = pCamera->GetViewMatrix();
			//D3DXMATRIX mtxProjection = pCamera->GetProjectionMatrix();
			//m_pRotationShader->SetMatrix(&mtxRot, &m_mtxWorld, &mtxView, &mtxProjection);
			//m_pShader->SetMatrix("g_mtxRot", &mtxRot);					//����
			//m_pShader->SetMatrix("g_mtxWorld", &m_mtxWorld);				//���[���h�}�g���b�N�X
			//m_pShader->SetMatrix("g_mtxView", &mtxView);				//�r���[�}�g���b�N�X
			//m_pShader->SetMatrix("g_mtxProjection", &mtxProjection);	//�v���W�F�N�V�����}�g���b�N�X

			//�V�F�[�_�[�̕`����n�߂�
			//m_pRotationShader->SetBeginPass();
			//�V�F�[�_�[�̕`����n�߂�
			//m_pShader->BeginPass(0);
		}

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);

		//�V�F�[�_�[�̏������݂��I������
		//if (m_pRotationShader != NULL) { m_pRotationShader->EndShader(); }
		//�V�F�[�_�[�̏������݂��I������
		//m_pShader->EndPass();
		//m_pShader->End();

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			//�J�����O
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);					// ���C�e�B���O�𖳌��ɂ���
	}
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void C3DParticle::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_pos = pos;
	m_size = size;

	//�ϐ��錾
	D3DXVECTOR3 Vec0[2] = {};
	D3DXVECTOR3 Vec1[2] = {};

	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-size.x, 0.0f, size.z);
	pVtx[1].pos = D3DXVECTOR3(size.x, 0.0f, size.z);
	pVtx[2].pos = D3DXVECTOR3(-size.x, 0.0f, -size.z);
	pVtx[3].pos = D3DXVECTOR3(size.x, 0.0f, -size.z);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void C3DParticle::SetColor(D3DXCOLOR col)
{
	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//�F�̃��l��ݒ肷��
	m_pRotationShader->SetCol_a(col.a);
}

//=============================================================================
// �e�N�X�`���̍X�V
//=============================================================================
void C3DParticle::SetMoveTex(float fMoveX, float fMoveY)
{
	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f * m_Division.x + fMoveX, 0.0f * m_Division.y + fMoveY);
	pVtx[1].tex = D3DXVECTOR2(1.0f * m_Division.x + fMoveX, 0.0f * m_Division.y + fMoveY);
	pVtx[2].tex = D3DXVECTOR2(0.0f * m_Division.x + fMoveX, 1.0f * m_Division.y + fMoveY);
	pVtx[3].tex = D3DXVECTOR2(1.0f * m_Division.x + fMoveX, 1.0f * m_Division.y + fMoveY);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// UV�̐ݒ�
//=============================================================================
void C3DParticle::SetUV(int nWidth, int nHight)
{
	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float PosX = 1.0f * m_Division.x * nWidth;
	float PosY = 1.0f * m_Division.x * nHight;

	//���_���W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f * m_Division.x + PosX, 0.0f * m_Division.y + PosY);
	pVtx[1].tex = D3DXVECTOR2(1.0f * m_Division.x + PosX, 0.0f * m_Division.y + PosY);
	pVtx[2].tex = D3DXVECTOR2(0.0f * m_Division.x + PosX, 1.0f * m_Division.y + PosY);
	pVtx[3].tex = D3DXVECTOR2(1.0f * m_Division.x + PosX, 1.0f * m_Division.y + PosY);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}