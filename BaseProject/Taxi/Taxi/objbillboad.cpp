//=============================================================================
//
// �I�u�W�F�N�g�r���{�[�h���� [objbillboad.cpp]
// Author : ���� ����
//
//=============================================================================
#include "objbillboad.h"
#include "manager.h"
#include "renderer.h"
#include "gamecamera.h"
#include "tutorial.h"
#include "player.h"
#include "object.h"

//==================================
// �}�N����`
//==================================
#define PRIORITY	(3)
#define VECTOR_ZERO	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))

//�e�N�X�`����
#define TEX_NAME00 ("data\\TEXTURE\\game\\ObjBillboad\\AroundKone.png")
#define TEX_NAME01 ("data\\TEXTURE\\game\\ObjBillboad\\wood.png")

//�T�C�Y
#define KONE_SIZE (D3DXVECTOR2(25.0f, 25.0f))
#define WOOD_SIZE (D3DXVECTOR2(150.0f, 350.0f))

//�`��͈�
#define KONE_LENGTH (6000.0f)	//�R�[���̕`��͈�
#define WOOD_LENGTH (20000.0f)	//�؂̕`��͈�

#define WOOD_MODEL_LENGTH (8000.0f)	//�؂̃��f���ɕς���^�C�~���O

//==================================
// �ÓI�����o�ϐ��錾
//==================================
LPDIRECT3DTEXTURE9 CObjBillboad::m_pTexture[OBJBILL_TEX] = {};

//=============================================================================
// ��������
//=============================================================================
CObjBillboad * CObjBillboad::Create(D3DXVECTOR3 pos, int nObjType)
{
	CObjBillboad * pObjBillboad = new CObjBillboad;
	pObjBillboad->SetPos(pos);
	pObjBillboad->SetTexNum(nObjType);
	pObjBillboad->Init();

	return pObjBillboad;
}

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CObjBillboad::CObjBillboad(): CScene(PRIORITY, OBJTYPE_BILLBOORD) 
{
	m_pos = VECTOR_ZERO;
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_nTexNum = -1;
	m_fDrawLength = 0.0f;
	m_fModelLength = 0.0f;
	m_bSetModel = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObjBillboad::~CObjBillboad(){}

//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
void CObjBillboad::LoadTexture()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateTextureFromFile(pDevice, TEX_NAME00, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, TEX_NAME01, &m_pTexture[1]);
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CObjBillboad::UnloadTexture()
{
	for (int nCntTex = 0; nCntTex < OBJBILL_TEX; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CObjBillboad::Init()
{
	//�ϐ��̏�����
	m_nCollision = 0;
	m_nModelNumTex = 0;
	m_ModelSize = VECTOR_ZERO;

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_��
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f);

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

	//���_�o�b�t�@�̃A�����b�N����
	m_pVtxBuff->Unlock();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CObjBillboad::Uninit()
{
	if (m_pVtxBuff != NULL)
	{// ���_���̔j��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//���g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CObjBillboad::Update()
{
	//�N���b�s���O�����̒ǉ�
	CManager::MODE mode = CManager::GetMode();
	CGameCamera * pGameCamera = NULL;
	D3DXVECTOR3 PlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (mode == CManager::MODE_GAME)
	{//�Q�[��
		pGameCamera = CGame::GetGameCamera();
		PlayerPos = CGame::GetPlayer()->GetPos();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//�`���[�g���A��
		pGameCamera = CTutorial::GetCamera();
		PlayerPos = CTutorial::GetPlayer()->GetPos();
	}

	//�v���C���[�Ƃ̋��������߂�
	D3DXVECTOR3 Distance = m_pos - PlayerPos;
	float fLength = sqrtf((Distance.x * Distance.x) + (Distance.z * Distance.z));

	//��苗���ȏゾ������
	if (fLength > m_fDrawLength)
	{
		SetDraw(false);
		return;
	}
	else
	{
		SetDraw(true);
	}

	//���f���ɐ؂�ւ��Ȃ��Ȃ烊�^�[��
	if (!m_bSetModel) { return; }

	//���f���̕\���͈͔���
	if (fLength < m_fModelLength)
	{
		////�I�u�W�F�N�g�̐���
		//CObject::Create(m_ModelPos, VECTOR_ZERO, m_ModelSize, 0.0f, m_nModelNumTex, m_nObjType,
		//				CModel3D::MOVETYPE_NOT, m_nCollision);

		////�r���{�[�h��j��
		//Uninit();

		SetDraw(false);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CObjBillboad::Draw()
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxView;	// �v�Z�p�}�g���b�N�X

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

	if (m_pVtxBuff != NULL)
	{
		//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture[m_nTexNum]);

		// ���C�e�B���O�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// ���C�e�B���O��L���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//=============================================================================
// �e�N�X�`���̔ԍ��ݒ�
//=============================================================================
void CObjBillboad::SetTexNum(int nObjType)
{
	m_nObjType = nObjType;

	if (m_nObjType == TYPE_KONE)
	{//�R�[��
		m_size = KONE_SIZE;
		m_nTexNum = TYPE_KONE;
		m_fDrawLength = KONE_LENGTH;
	}
	else if (m_nObjType == TYPE_TREE02)
	{//��
		m_size = WOOD_SIZE;
		m_nTexNum = TYPE_WOOD;
		m_fDrawLength = WOOD_LENGTH;
		m_fModelLength = WOOD_MODEL_LENGTH;
		m_ModelPos = m_pos;
		m_pos = D3DXVECTOR3(m_pos.x, m_pos.y + 150.0f, m_pos.z);
		m_bSetModel = true;
	}
}

//=============================================================================
// ���f���̏��ݒ�
//=============================================================================
void CObjBillboad::SetModelInfo(D3DXVECTOR3 Scale, int ModelTex, int nCollision)
{
	m_ModelSize = Scale;		//���f���̑傫��
	m_nModelNumTex = ModelTex;	//�e�N�X�`�����
	m_nCollision = nCollision;	//�����蔻��
}