//=============================================================================
//
// UI�Ȃǂ̐e�̏��� [scene2D.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_SIZE		(50)					//�e�N�X�`���T�C�Y
#define TEXTURE_ANIM_UV_U	(0.125f)				//�e�N�X�`���A�j���[�V����U�͈�
#define TEXTURE_ANIM_UV_V	(1.0f)					//�e�N�X�`���A�j���[�V����V�͈�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CScene2D::CScene2D(int nPriority,CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	//�l���N���A
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle = 0;
	m_fLength = 0;
	m_fRadius = 0;
	m_nSize = D3DXVECTOR2(0.0f, 0.0f);
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CScene2D::~CScene2D()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CScene2D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL
	);

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//���_�J���[�ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScene2D::Uninit(void)
{
	//���_�o�b�t�@�̔j��
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
void CScene2D::Update(void)
{

}
//=============================================================================
// �`�揈��
//=============================================================================
void CScene2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_pVtxBuff != NULL)
	{
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

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CScene2D * CScene2D::Create()
{
	CScene2D *pScene2D = NULL;

	//NULL�`�F�b�N
	if (pScene2D == NULL)
	{//�������̓��I�m��

		pScene2D = new CScene2D;

		if (pScene2D != NULL)
		{
			//�I�u�W�F�N�g�N���X�̐���
			pScene2D->Init();
		}
	}

	return pScene2D;
}
//===============================================================================
// ���L�e�N�X�`���̊��蓖��
//===============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 Texture)
{
	m_pTexture = Texture;
}
//=============================================================================
// �Z�b�g����
//=============================================================================
void CScene2D::SetPosSize(D3DXVECTOR3 pos, D3DXVECTOR2 nSize)
{
	//pos�l���
	m_pos = pos;

	m_nSize = nSize;

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_nSize.x, m_pos.y - m_nSize.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_nSize.x, m_pos.y - m_nSize.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_nSize.x, m_pos.y + m_nSize.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_nSize.x, m_pos.y + m_nSize.y, 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �Z�b�g����(���S�_��)
//=============================================================================
void CScene2D::SetPosSizeSide(D3DXVECTOR3 pos, D3DXVECTOR2 nSize)
{
	//pos�l���
	m_pos = pos;

	m_nSize = nSize;

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_nSize.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_nSize.x, m_pos.y - m_nSize.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_nSize.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_nSize.x, m_pos.y + m_nSize.y, 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �Ђ��`�̃|���S��
//=============================================================================
void CScene2D::SetPosDiamond(D3DXVECTOR3 pos, D3DXVECTOR2 nSize, D3DXVECTOR4 VtxAngle)
{
	//pos�l���
	m_pos = pos;

	m_nSize = nSize;

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_nSize.x + VtxAngle.x, m_pos.y - m_nSize.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_nSize.x + VtxAngle.y, m_pos.y - m_nSize.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_nSize.x + VtxAngle.z, m_pos.y + m_nSize.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_nSize.x + VtxAngle.w, m_pos.y + m_nSize.y, 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//===============================================================================
// ����������
//===============================================================================
void CScene2D::SetRot(float fAngle, D3DXVECTOR2 size)
{
	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(fAngle) * size.y + cosf(fAngle) * size.x, m_pos.y - sinf(fAngle) * size.x + cosf(fAngle) * size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(fAngle) * size.y - cosf(fAngle) * size.x, m_pos.y + sinf(fAngle) * size.x + cosf(fAngle) * size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - sinf(fAngle) * size.y + cosf(fAngle) * size.x, m_pos.y - sinf(fAngle) * size.x - cosf(fAngle) * size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x - sinf(fAngle) * size.y - cosf(fAngle) * size.x, m_pos.y + sinf(fAngle) * size.x - cosf(fAngle) * size.y, 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//===============================================================================
// �傫����ς���
//===============================================================================
void CScene2D::SetVtxPos(D3DXVECTOR3 *VxtPos)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = VxtPos[0];
	pVtx[1].pos = VxtPos[1];
	pVtx[2].pos = VxtPos[2];
	pVtx[3].pos = VxtPos[3];

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//===============================================================================
// �F�𒸓_���ɕς���
//===============================================================================
void CScene2D::SetColor(D3DXCOLOR *Color)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = *Color;
	pVtx[1].col = *Color;
	pVtx[2].col = *Color;
	pVtx[3].col = *Color;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//===============================================================================
//  ��]����
//===============================================================================
void CScene2D::SetSpin(D3DXVECTOR3 pos, float fAngle, float fLength, D3DXVECTOR3 rot)
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

//=============================================================================
// �A�j���[�V�����̏���
//=============================================================================
void CScene2D::SetTexture(int PatternAnim, int X, int Y, int nNum)
{
	VERTEX_2D *pVtx;//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2((PatternAnim % X) * (1.0f / X) * (1 * nNum), (PatternAnim / X) * (1.0f / Y));
	pVtx[1].tex = D3DXVECTOR2((PatternAnim % X) * (1.0f / X) + (1.0f / X), (PatternAnim / X) * (1.0f / Y));
	pVtx[2].tex = D3DXVECTOR2((PatternAnim % X) * (1.0f / X) * (1 * nNum), (PatternAnim / X) * (1.0f / Y) + (1.0f / Y));
	pVtx[3].tex = D3DXVECTOR2((PatternAnim % X) * (1.0f / X) + (1.0f / X), (PatternAnim / X) * (1.0f / Y) + (1.0f / Y));

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// UV�̐ݒ�
//=============================================================================
void CScene2D::SetUV(float fPattern, int nNum)
{
	VERTEX_2D *pVtx;//���_���ւ̃|�C���^

					//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, fPattern * nNum);
	pVtx[1].tex = D3DXVECTOR2(1.0f, fPattern * nNum);
	pVtx[2].tex = D3DXVECTOR2(0.0f, (fPattern * nNum) + (fPattern));
	pVtx[3].tex = D3DXVECTOR2(1.0f, (fPattern * nNum) + (fPattern));

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// UV�̉��̐ݒ�
//=============================================================================
void CScene2D::SetUVWidth(float fPattern, int nNum)
{
	VERTEX_2D *pVtx;//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(fPattern * nNum, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((fPattern * nNum) + (fPattern), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(fPattern * nNum, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((fPattern * nNum) + (fPattern), 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}