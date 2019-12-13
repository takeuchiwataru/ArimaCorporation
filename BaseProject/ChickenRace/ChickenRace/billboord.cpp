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
CBillBoord::CBillBoord() : CScene3D(4, CScene::OBJTYPE_BILLBOORD)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);

	m_bMap = false;
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
}
//=============================================================================
// �`�揈��
//=============================================================================
void CBillBoord::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�ʒu�̏�����
	D3DXVECTOR3 BillPos = CScene3D::GetPosition();
	//�Q�[���̏��
	CManager::MODE pMode = CManager::GetMode();

	// Z���C�g
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	CScene3D::Draw();

	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	// Z���C�g
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CBillBoord * CBillBoord::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, bool bMap)
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
			//�ʒu�̊��蓖��
			pBillBoord->SetPosSize(pos, size);
			//�T�C�Y������
			pBillBoord->m_size = size;
			
			pBillBoord->m_bMap = bMap;
		}
	}

	return pBillBoord;
}