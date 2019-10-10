//=============================================================================
//
// �O�Ղ̏��� [meshorbit.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "scene.h"
#include "meshorbit.h"
#include "manager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME_1		"data\\TEXTURE\\gradation005.jpg"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CMeshOrbit::m_pTexture[MAX_MESHORBIT_TEXTURE] = {};

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CMeshOrbit::CMeshOrbit() : CScene(2, CScene::OBJTYPE_MESHORBIT)
{
	m_pVtxBuff = NULL;								// ���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = NULL;								// �C���f�b�N�X�ւ̃|�C���^
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
	m_nType = 0;									// ���
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CMeshOrbit::~CMeshOrbit()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshOrbit::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//����������
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
	m_nType = 0;									// ���
	m_bOrbitOnOff = false;							// �O�Ղ̕\����ONOFF

	//���[�̃I�t�Z�b�g�̏�����
	m_aOffSet[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_aOffSet[1] = D3DXVECTOR3(0.0f, 50.0f,0.0f);

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * MESHORBIT_X,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL
	);

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;	

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMesh = 0; nCntMesh < MESHORBIT_X; nCntMesh+= 2)
	{
		// ���_�̍��W
		pVtx[nCntMesh].pos = D3DXVECTOR3(0.0f, 0.0f, nCntMesh * MESH_ORBITWIDTH);
		pVtx[nCntMesh + 1].pos = D3DXVECTOR3(0.0f, MESH_ORBITDEPTH, nCntMesh * MESH_ORBITWIDTH);
	}

	for (int nCntMesh = 0; nCntMesh < MESHORBIT_X; nCntMesh++)
	{
		// �@���̐ݒ�
		pVtx[nCntMesh].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���_�J���[
		pVtx[nCntMesh].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		
		//�e�N�X�`�����W
		pVtx[nCntMesh].tex = D3DXVECTOR2(nCntMesh * 1.0f, nCntMesh * 1.0f);
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMeshOrbit::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMeshOrbit::Update(void)
{
	CDebugProc::Print("�����I\n");
}
//=============================================================================
// �`�揈��
//=============================================================================
void CMeshOrbit::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;	

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxTrans;

	for (int nCount = (MESHORBIT_X / 2) - 1; nCount > 0; nCount--)
	{//��Ԍ�납�����ւ��Ă���
		pVtx[nCount * 2] = pVtx[(nCount * 2) - 2];
		pVtx[nCount * 2 + 1] = pVtx[(nCount * 2) - 2 + 1];
	}

	for (int nCount = 0; nCount < 2; nCount++)
	{//�x�N�g�����g�����X�t�H�[��
		D3DXVec3TransformCoord(&pVtx[nCount].pos, &m_aOffSet[nCount], m_pMtxParent);
	}

	//�A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//�A���t�@�e�X�g����
	//�f�t�H���g�̏ꍇFALSE�ɂȂ��Ă��邩��TRUE�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �J�����O���Ȃ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture[0]);

	//�U������Ƃ��̂ݕ`�悷��
	//if (m_bOrbitOnOff == true)
	{// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, MESHORBIT_X - 2);
	}

	// ���ʂ��J�����O
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�f�t�H���g�̏ꍇFALSE�ɂȂ��Ă��邩��TRUE�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CMeshOrbit * CMeshOrbit::Create()
{
	CMeshOrbit *pMeshOrbit = NULL;

	//NULL�`�F�b�N
	if (pMeshOrbit == NULL)
	{//�������̓��I�m��

		pMeshOrbit = new CMeshOrbit;

		if (pMeshOrbit != NULL)
		{
			//�I�u�W�F�N�g�N���X�̐���
			pMeshOrbit->Init();
		}
	}

	return pMeshOrbit;
}
//===============================================================================
// �e�N�X�`���̓ǂݍ���
//===============================================================================
HRESULT CMeshOrbit::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pTexture[0]);

	return S_OK;
}
//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CMeshOrbit::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_MESHORBIT_TEXTURE; nCount++)
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
// ���L�e�N�X�`���̊��蓖��
//===============================================================================
void CMeshOrbit::BindTexture(LPDIRECT3DTEXTURE9 Texture)
{
	m_pTexture[0] = Texture;
}
//===============================================================================
//�@�ʒu�̑��
//===============================================================================
void CMeshOrbit::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//�X�L�����b�V��
//�N�H�[�^�j�I��
//�X�v���C��
//�X�e���V���V���h�E
//�X�e���V���~���[
//�t�B�[�h�o�b�N�G�t�F�N�g
