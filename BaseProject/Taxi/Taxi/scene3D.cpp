
//=============================================================================
//
// �I�u�W�F�N�g�̏��� [scene3D.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "scene.h"
#include "scene3D.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CScene3D::CScene3D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{	
	m_pVtxBuff = NULL;
	m_pTexture = NULL;
	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);	//�ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
	m_Type = TYPE_NONE;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CScene3D::~CScene3D()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CScene3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ʒu
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����

	// ���_���̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL
	);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_��
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//�@���x�N�g��
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	if (m_Type == TYPE_WALL)
	{
		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
	}
	//���_�o�b�t�@�̃A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScene3D::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (m_pTexture != NULL) { m_pTexture = NULL; }

	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScene3D::Update(void){}

//=============================================================================
// �`�揈��
//=============================================================================
void CScene3D::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxView, mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	if (m_Type != TYPE_WALL && m_Type != TYPE_GROUND && m_Type != TYPE_SHADOW && m_Type != TYPE_BLOOD && m_Type != TYPE_SPOTLIGHT)
	{
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
	}
	else if (m_Type == TYPE_WALL || m_Type == TYPE_GROUND || m_Type == TYPE_SHADOW || m_Type == TYPE_BLOOD || m_Type == TYPE_SPOTLIGHT)
	{
		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	}

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_Type == TYPE_WALL)
	{
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// ���ʂ��J�����O
	}

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//�ǂ̏ꍇ
	if (m_Type == TYPE_WALL)
	{
		//�J�����O���Ȃ�
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				
	}
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CScene3D * CScene3D::Create(void)
{
	CScene3D *pScene3D = NULL;

	//NULL�`�F�b�N
	if (pScene3D == NULL)
	{//�������̓��I�m��

		pScene3D = new CScene3D;

		if (pScene3D != NULL)
		{
			//�I�u�W�F�N�g�N���X�̐���
			pScene3D->Init();
		}
	}

	return pScene3D;
}
//===============================================================================
// ���L�e�N�X�`���̊��蓖��
//===============================================================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 Texture)
{
	m_pTexture = Texture;
}
//=============================================================================
// �ǂ̐ݒ�
//=============================================================================
void CScene3D::SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 nSize)
{
	//���
	m_pos = pos;
	m_rot = rot * D3DX_PI;
	m_size = nSize;

	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, 0.0f, 0.0f);

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();	
}
//=============================================================================
// �Z�b�g����
//=============================================================================
void CScene3D::SetPosSize(D3DXVECTOR3 pos,D3DXVECTOR2 nSize)
{
	m_pos = pos;

	m_size = nSize;

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(- m_size.x,m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x,m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(- m_size.x,- m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x,- m_size.y, 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �Z�b�g����
//=============================================================================
void CScene3D::SetGroundPosSize(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	//�ʒu�ƃT�C�Y�̐ݒ�
	m_pos = pos;
	m_size = size;

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, m_size.y);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, 0.0f, m_size.y);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, -m_size.y);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, 0.0f, -m_size.y);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//===============================================================================
//  �J���[�̐ݒ菈��
//===============================================================================
void CScene3D::SetColor(D3DXCOLOR Color)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = Color;
	pVtx[1].col = Color;
	pVtx[2].col = Color;
	pVtx[3].col = Color;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �G�t�F�N�g�̏���
//=============================================================================
void CScene3D::SetVtxEffect(float fRadius)
{
	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(-fRadius, fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fRadius, fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fRadius, -fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fRadius, -fRadius, 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//===============================================================================
// �e�N�X�`���̃A�j���[�V����
//===============================================================================
void CScene3D::SetUV(D3DXVECTOR2 *uv)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = uv[0];
	pVtx[1].tex = uv[1];
	pVtx[2].tex = uv[2];
	pVtx[3].tex = uv[3];

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//===============================================================================
// �e�N�X�`���̖@���ݒ�
//===============================================================================
void CScene3D::SetNor(D3DXVECTOR3 Nor)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�@���x�N�g��
	pVtx[0].nor = Nor;
	pVtx[1].nor = Nor;
	pVtx[2].nor = Nor;
	pVtx[3].nor = Nor;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//===============================================================================
//  ��]����
//===============================================================================
void CScene3D::SetSpin(D3DXVECTOR3 pos, float fAngle, float fLength, D3DXVECTOR3 rot)
{
	m_pos = pos;

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�g�k��]����
	pVtx[0].pos = D3DXVECTOR3((m_pos.x + sinf(-D3DX_PI + fAngle + rot.x) * fLength),
		(m_pos.y + cosf(-D3DX_PI + fAngle + rot.y) * fLength), 0.0f);

	pVtx[1].pos = D3DXVECTOR3((m_pos.x + sinf(D3DX_PI - fAngle + rot.x) * fLength),
		(m_pos.y + cosf(D3DX_PI - fAngle + rot.y) * fLength), 0.0f);

	pVtx[2].pos = D3DXVECTOR3((m_pos.x + sinf(-fAngle + rot.x) * fLength),
		(m_pos.y + cosf(-fAngle + rot.y) * fLength), 0.0f);

	pVtx[3].pos = D3DXVECTOR3((m_pos.x + sinf(fAngle + rot.x) * fLength),
		(m_pos.y + cosf(fAngle + rot.y) * fLength), 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
