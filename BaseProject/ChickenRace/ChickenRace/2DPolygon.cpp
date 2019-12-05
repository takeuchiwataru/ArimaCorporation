//=============================================================================
//
// 2DPolygon���� [2DPolygon.cpp]
// Author : Ryo Sugimoto
//
//=============================================================================
#include "main.h"
#include "scene.h"
#include "2DPolygon.h"
#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SLIDE_SPEED	(10.0f)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//==================================================================================================//
//    * �����֐� *
//==================================================================================================//
C2D	*C2D::Create(void)
{
	C2D *p2D = NULL;	p2D = new C2D;
	if (p2D != NULL) { p2D->Init(); }
	return p2D;
}
//==================================================================================================//
//    * ���͊֐� *
//==================================================================================================//
void C2D::Set(D3DXVECTOR3 pos, float fLengthX, float fLengthY, float fRot, int nUV, int nUV_X, int nUV_Y, D3DXCOLOR col, DRAW_TYPE Drawtype)
{
	m_DrawType = Drawtype;
	m_pos = pos;
	m_fLengthX = fLengthX;
	m_fLengthY = fLengthY;

	//Vtx�̕ύX
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (sinf(fRot) * m_fLengthY) + (cosf(fRot) * m_fLengthX), m_pos.y - (sinf(fRot) * m_fLengthX) + (cosf(fRot) * m_fLengthY), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (sinf(fRot) * m_fLengthY) - (cosf(fRot) * m_fLengthX), m_pos.y + (sinf(fRot) * m_fLengthX) + (cosf(fRot) * m_fLengthY), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (sinf(fRot) * m_fLengthY) + (cosf(fRot) * m_fLengthX), m_pos.y - (sinf(fRot) * m_fLengthX) - (cosf(fRot) * m_fLengthY), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x - (sinf(fRot) * m_fLengthY) - (cosf(fRot) * m_fLengthX), m_pos.y + (sinf(fRot) * m_fLengthX) - (cosf(fRot) * m_fLengthY), 0.0f);

	//�J���[�ݒ�
	pVtx[0].col = col;	// A = �����x
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//�e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y));
	pVtx[1].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y));
	pVtx[2].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y) + (1.0f / nUV_Y));
	pVtx[3].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y) + (1.0f / nUV_Y));

	//���_BUFFER���A�����b�N����
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT  C2D::Init(void)
{
	// �|���S���̈ʒu��ݒ�
	m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0);
	m_fLengthX = 50;
	m_fLengthY = 50;
	m_pTex = NULL;
	CManager::GetRenderer();

	LPDIRECT3DDEVICE9		pD3DDevice = CManager::GetRenderer()->GetDevice();
	// ���_�o�b�t�@�̐���
	pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,		//�m�ۂ���BUFFER�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	float fRot = 0.0f;

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_BUFFER���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void C2D::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@�̔j��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	delete this;
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void C2D::Update(void)
{

}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void C2D::Draw(void)
{
	LPDIRECT3DDEVICE9	pD3DDevice = CManager::GetRenderer()->GetDevice();
	int					nNumDraw = 1;
	DrawPrepare(m_DrawType, pD3DDevice);
	//���_BUFFER���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	pD3DDevice->SetFVF(FVF_VERTEX_2D);	// ���_�t�H�[�}�b�g�̐ݒ�

										//�e�N�X�`���̐ݒ�
										//�e�N�X�`���̐ݒ�
	pD3DDevice->SetTexture(0, m_pTex);

	if (m_DrawType == DRAW_TYPE_ADD) { nNumDraw = 1; }
	//�|���S���̕`��
	for (int nCount = 0; nCount < nNumDraw; nCount++)
	{//�w��񐔕�
		pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	pD3DDevice->SetTexture(0, NULL);
	DrawPrepare(DRAW_TYPE_NORMAL, pD3DDevice);
}
//==================================================================================================//
//    * �`�揀���֐� *
//==================================================================================================//
bool C2D::DrawPrepare(DRAW_TYPE DrawType, LPDIRECT3DDEVICE9 &pD3DDevice)
{
	if (DrawType == DRAW_TYPE_NO) { return true; }

	if (DrawType >= DRAW_TYPE_ZNORMAL)
	{// Z�e�X�g����
		pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	}
	else
	{// Z�e�X�g�n��
		pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	}
	switch (DrawType % 10)
	{//�ق�Ƃ�3�s���邱�Ƃ�Y��Ă͂����Ȃ�...
	case DRAW_TYPE_NORMAL:// �ʏ�
		pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	case DRAW_TYPE_ADD:// ���u�����f�B���O�����Z�����ɐݒ�
		pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case DRAW_TYPE_SUBTRACT:// ���Z
		pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case DRAW_TYPE_MULTIPLY:
		pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO | D3DBLEND_ONE);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);
		break;
	case DRAW_TYPE_OVERLAY:
		pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO | D3DBLEND_ONE);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case DRAW_TYPE_AER:
		pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	}

	return false;
}
//=============================================================================
// ���_���̐ݒ�
//=============================================================================
void C2D::SetPosition(float fRot)
{
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (sinf(fRot) * m_fLengthY) + (cosf(fRot) * m_fLengthX), m_pos.y - (sinf(fRot) * m_fLengthX) + (cosf(fRot) * m_fLengthY), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (sinf(fRot) * m_fLengthY) - (cosf(fRot) * m_fLengthX), m_pos.y + (sinf(fRot) * m_fLengthX) + (cosf(fRot) * m_fLengthY), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (sinf(fRot) * m_fLengthY) + (cosf(fRot) * m_fLengthX), m_pos.y - (sinf(fRot) * m_fLengthX) - (cosf(fRot) * m_fLengthY), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x - (sinf(fRot) * m_fLengthY) - (cosf(fRot) * m_fLengthX), m_pos.y + (sinf(fRot) * m_fLengthX) - (cosf(fRot) * m_fLengthY), 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
//=============================================================================
// ���_�{���̐ݒ�
//=============================================================================
void C2D::SetPosition2(float fSize)
{
	VERTEX_2D *pVtx;
	float fLengthX = m_fLengthX * fSize;
	float fLengthY = m_fLengthY * fSize;
	float fRot = D3DX_PI;

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (sinf(fRot) * fLengthY) + (cosf(fRot) * fLengthX), m_pos.y - (sinf(fRot) * fLengthX) + (cosf(fRot) * fLengthY), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (sinf(fRot) * fLengthY) - (cosf(fRot) * fLengthX), m_pos.y + (sinf(fRot) * fLengthX) + (cosf(fRot) * fLengthY), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (sinf(fRot) * fLengthY) + (cosf(fRot) * fLengthX), m_pos.y - (sinf(fRot) * fLengthX) - (cosf(fRot) * fLengthY), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x - (sinf(fRot) * fLengthY) - (cosf(fRot) * fLengthX), m_pos.y + (sinf(fRot) * fLengthX) - (cosf(fRot) * fLengthY), 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
//==================================================================================================//
//    * UV�ύX *
//==================================================================================================//
void C2D::SetUV(int nUV, int nUV_X, int nUV_Y)
{
	//Vtx�̕ύX
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y));
	pVtx[1].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y));
	pVtx[2].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y) + (1.0f / nUV_Y));
	pVtx[3].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y) + (1.0f / nUV_Y));

	pVtx[0].tex += D3DXVECTOR2(0.01f, 0.01f);
	pVtx[1].tex += D3DXVECTOR2(-0.01f, 0.01f);
	pVtx[2].tex += D3DXVECTOR2(0.01f, -0.01f);
	pVtx[3].tex += D3DXVECTOR2(-0.01f, -0.01f);

	//���_BUFFER���A�����b�N����
	m_pVtxBuff->Unlock();
}
//==================================================================================================//
//    * UV�ύX *
//==================================================================================================//
void C2D::SetUV_Camera(float fUV_X, float fUV_Y, float fDiameter)
{
	//Vtx�̕ύX
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2((pos.x * fDiameter) + fUV_X, (pos.z * (-fDiameter * 1.65f)) + fUV_Y);
	pVtx[1].tex = D3DXVECTOR2((pos.x * fDiameter) + 1.0f + fUV_X, (pos.z * (-fDiameter * 1.65f)) + fUV_Y);
	pVtx[2].tex = D3DXVECTOR2((pos.x * fDiameter) + fUV_X, (pos.z * (-fDiameter * 1.65f)) + 1.0f + fUV_Y);
	pVtx[3].tex = D3DXVECTOR2((pos.x * fDiameter) + 1.0f + fUV_X, (pos.z * (-fDiameter * 1.65f)) + 1.0f + fUV_Y);


	//���_BUFFER���A�����b�N����
	m_pVtxBuff->Unlock();
}
//==================================================================================================//
//    * UV���Q�[�W�^�ɕύX *
//==================================================================================================//
void	C2D::SetGageUV(float fMaxLengthX)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((m_fLengthX / fMaxLengthX), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((m_fLengthX / fMaxLengthX), 1.0f);


	//���_BUFFER���A�����b�N����
	m_pVtxBuff->Unlock();
}
//==================================================================================================//
//    * �Q�[�W�^�ɕύX *
//==================================================================================================//
void C2D::SetGage(float fLengthX, int nDirection)
{
	//Vtx�̕ύX
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	m_fLengthX = fLengthX;

	//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fRot = D3DX_PI - (D3DX_PI / 2.0f) * nDirection;
	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (sinf(fRot) * m_fLengthY), m_pos.y + (cosf(fRot) * m_fLengthY), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (sinf(fRot) * m_fLengthY) - (cosf(fRot) * fLengthX), m_pos.y + (sinf(fRot) * fLengthX) + (cosf(fRot) * m_fLengthY), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (sinf(fRot) * m_fLengthY), m_pos.y - (cosf(fRot) * m_fLengthY), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x - (sinf(fRot) * m_fLengthY) - (cosf(fRot) * fLengthX), m_pos.y + (sinf(fRot) * fLengthX) - (cosf(fRot) * m_fLengthY), 0.0f);

	//���_BUFFER���A�����b�N����
	m_pVtxBuff->Unlock();
}
//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void C2D::SetColor(D3DXCOLOR col)
{
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
//=============================================================================
// ���_�J���[�̉��Z����
//=============================================================================
void	C2D::PlusColor(D3DXCOLOR col)
{
	VERTEX_2D *pVtx;
	D3DXCOLOR WKcol;
	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	for (int nCount = 0; nCount < 4; nCount++)
	{
		WKcol = pVtx[nCount].col;
		WKcol += col;
		if (WKcol.r > 1.0f) { WKcol.r = 1.0f; }
		else if (WKcol.r < 0.0f) { WKcol.r = 0.0f; }
		if (WKcol.g > 1.0f) { WKcol.g = 1.0f; }
		else if (WKcol.g < 0.0f) { WKcol.g = 0.0f; }
		if (WKcol.b > 1.0f) { WKcol.b = 1.0f; }
		else if (WKcol.b < 0.0f) { WKcol.b = 0.0f; }
		if (WKcol.a > 1.0f) { WKcol.a = 1.0f; }
		else if (WKcol.a < 0.0f) { WKcol.a = 0.0f; }

		pVtx[nCount].col = WKcol;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
//=============================================================================
// ���_�J���[�����x(���E)�̐ݒ�
//=============================================================================
void	C2D::SetSideAlpha(float fCol, float fMinCol, int nID)
{
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	D3DXCOLOR col = pVtx[0 + nID].col;
	col.a += fCol;
	if (col.a >= 1.0f) { col.a = 1.0f; }
	if (col.a <= fMinCol) { col.a = fMinCol; }

	pVtx[0 + nID].col = col;
	pVtx[2 + nID].col = col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void C2D::PlusUV(D3DXVECTOR2 uv)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

							// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	for (int nCount = 0; nCount < 4; nCount++) { pVtx[nCount].tex += uv; }

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==================================================================================================//
//    * �����֐� *
//==================================================================================================//
C2DAnim	*C2DAnim::Create(void)
{
	C2DAnim *p2D = NULL;	p2D = new C2DAnim;
	if (p2D != NULL) { p2D->Init(); }
	return p2D;
}
//=============================================================================
// �A�j���[�V�����|���S���̐ݒ�
//=============================================================================
void	C2DAnim::Set(D3DXVECTOR3 pos, float fLengthX, float flengthY, float fRot, int nPtnAnim, int nPtnWide, int nPtnHeight, int nPtnSpeed,
	ANIMATION animation, D3DXCOLOR col, DRAW_TYPE Drawtype)
{
	C2D::Set(pos, fLengthX, flengthY, fRot, nPtnAnim, nPtnWide, nPtnHeight, col, Drawtype);
	m_animation = animation;
	m_nCntAnim = 0;
	m_nPtnAnim = nPtnAnim;
	m_nPtnWide = nPtnWide;
	m_nPtnHeight = nPtnHeight;
	m_nPtnSpeed = nPtnSpeed;
	m_col = col;
	m_fStateSpd = 1.0f;
	m_fCntSize = 0.0f;
}
//=============================================================================
// �A�j���[�V�����|���S���̏�Ԑݒ�
//=============================================================================
void	C2DAnim::SetState(STATE state, float fCntState)
{
	m_state = state;
	if (fCntState == 99.9f)
	{//�������ݒ肳��Ă��Ȃ��Ȃ�
		fCntState = 1.0f / (float)((m_nPtnHeight * m_nPtnWide) * m_nPtnSpeed);
	}
	m_fCntState = fCntState;
	if (m_state == STATE_FADEIN) { m_fCntSize = 0.3f; }
	if (m_state == STATE_FADEIN_D) { m_fCntSize = -0.3f; }
}
//=============================================================================
// �A�j���[�V�����|���S���̍X�V����
//=============================================================================
void	C2DAnim::Update(void)
{
	switch (m_state)
	{
	case STATE_NORMAL:
		m_fCntSize += (0.05f - m_fCntSize) * 0.05f;
		SetPosition2(1.0f + m_fCntSize);
		break;
	case STATE_FADEIN:
		m_col.a += m_fCntState * m_fStateSpd;
		if (m_col.a > 1.0f) { m_col.a = 1.0f; m_state = STATE_NORMAL; }
		SetColor(m_col);
		m_fCntSize += (0.0f - m_fCntSize) * 0.25f;
		SetPosition2(1.0f + m_fCntSize);
		break;
	case STATE_FADEOUT:
		m_col.a -= m_fCntState * m_fStateSpd;
		if (m_col.a < 0.0f) { m_col.a = 0.0f; }
		SetColor(m_col);
		m_fCntSize += (0.1f - m_fCntSize) * 0.02f;
		SetPosition2(1.0f + m_fCntSize);
		break;
	case 	STATE_FADEIN_D:
		m_col.a += m_fCntState * m_fStateSpd;
		if (m_col.a > 1.0f) { m_col.a = 1.0f; m_state = STATE_FADEOUT_D; }
		SetColor(m_col);
		m_fCntSize += (1.0f - m_fCntSize) * 0.25f;
		SetPosition2(1.0f + m_fCntSize);
		break;
	case	STATE_FADEOUT_D:
		m_col.a -= m_fCntState * m_fStateSpd * 0.5f;
		if (m_col.a < 0.0f) { m_animation = ANIMATION_END; return; }
		SetColor(m_col);
		m_fCntSize += (0.0f - m_fCntSize) * 0.005f;
		SetPosition2(1.0f + m_fCntSize);
		break;
	}
	m_nCntAnim++;
	if (m_nCntAnim % m_nPtnSpeed == 0)
	{//UV�ύX
		m_nPtnAnim++;
		if (m_nPtnAnim >= m_nPtnWide * m_nPtnHeight)
		{//�摜�͈̔͊O
			if (m_animation == ANIMATION_LOOP) { m_nPtnAnim = 0; }
			else { m_animation = ANIMATION_END; return; }
		}//�A�j���[�V�����X�V
		SetUV(m_nPtnAnim, m_nPtnWide, m_nPtnHeight);
	}
}

//=============================================================================
// �J�����t���Y���|���S���̐ݒ菈��
//=============================================================================
void C2DCameraBG::Set(D3DXVECTOR3 pos, float fLengthX, float flengthY, float fRot, float fUV_X, float fUV_Y, float fDiameter, D3DXCOLOR col)
{
	C2D::Set(pos, fLengthX, flengthY, fRot, 0, 1, 1, col);
	m_fUV_X = 0.0f;
	m_fUV_Y = 0.0f;
	m_fUV_XPlus = fUV_X;
	m_fUV_YPlus = fUV_Y;
	m_fDiameter = fDiameter;
}
//=============================================================================
// �J�����t���Y���|���S���̍X�V����
//=============================================================================
void	C2DCameraBG::Update(void)
{
	m_fUV_X += m_fUV_XPlus;
	m_fUV_Y += m_fUV_YPlus;
	SetUV_Camera(m_fUV_X, m_fUV_Y, m_fDiameter);
}

//==================================================================================================//
//    * �����֐� *
//==================================================================================================//
C2DPolygon	*C2DPolygon::Create(void)
{
	C2DPolygon *p2D = NULL;	p2D = new C2DPolygon;
	if (p2D != NULL) { p2D->Init(); }
	return p2D;
}
//==================================================================================================//
//    * ���͊֐� *
//==================================================================================================//
void C2DPolygon::Set(D3DXVECTOR3 pos, float fLengthX, float fLengthY, float fRot, int nUV, int nUV_X, int nUV_Y, float fCntState, STATE state, D3DXCOLOR col)
{
	m_col = col;
	m_fCntState = fCntState;
	m_state = state;
	C2D::Set(pos, fLengthX, fLengthY, fRot, nUV, nUV_X, nUV_Y, col);
}
//=============================================================================
// �ʏ�|���S���̍X�V����
//=============================================================================
void	C2DPolygon::Update(void)
{
	if (m_state != STATE_NORMAL)
	{
		float fWKstate;

		switch (m_state)
		{
		case STATE_FADEOUT:
			m_col.a -= m_fCntState / FRAME_SPEED;
			if (m_col.a <= 0.0f) { m_col.a = 0.0f; m_state = STATE_NORMAL; }
			SetColor(m_col);
			break;
		case STATE_FADEIN:
			m_col.a += m_fCntState / FRAME_SPEED;
			if (m_col.a >= 1.0f) { m_col.a = 1.0f; m_state = STATE_NORMAL; }
			SetColor(m_col);
			break;
		case STATE_FADE:
			m_col.a += m_fCntState / FRAME_SPEED;
			if (m_col.a >= 1.0f) { m_col.a = 1.0f; m_state = STATE_FADEOUT; }
			SetColor(m_col);
			break;
		case STATE_SLIDEIN:
			GetPosition().x -= (1.0f - m_col.a) * 2.0f;
			SetPosition(D3DX_PI);
			m_col.a += m_fCntState;
			if (m_col.a >= 1.0f) { m_col.a = 1.0f; m_state = STATE_NORMAL; }
			SetColor(m_col);
			break;
		case STATE_SLIDEOUT:
			GetPosition() += D3DXVECTOR3(powf(SLIDE_SPEED * m_col.a, 2), 0.0f, 0.0f);
			SetPosition(D3DX_PI);
			m_col.a -= m_fCntState;
			if (m_col.a <= 0.0f) { m_col.a = 0.0f; m_state = STATE_NORMAL; }
			SetColor(m_col);
			break;
		case STATE_SLIDEIN2:
			GetPosition() += D3DXVECTOR3(powf(SLIDE_SPEED * (1.0f - m_col.a), 2), powf(SLIDE_SPEED * (1.0f - m_col.a), 2), 0.0f);
			SetPosition(D3DX_PI);
			m_col.a += m_fCntState;
			if (m_col.a >= 1.0f) { m_col.a = 1.0f; m_state = STATE_NORMAL; }
			SetColor(m_col);
			break;
		case STATE_SLIDEOUT2:
			GetPosition() += D3DXVECTOR3(powf(SLIDE_SPEED * m_col.a, 2), powf(SLIDE_SPEED * m_col.a, 2), 0.0f);
			SetPosition(D3DX_PI);
			m_col.a -= m_fCntState;
			if (m_col.a <= 0.0f) { m_col.a = 0.0f; m_state = STATE_NORMAL; }
			SetColor(m_col);
			break;
		case STATE_BIG:
			if (m_fCntState < 125.0f)
			{
				m_fCntState += 4.0f;
				if (m_fCntState >= 125.0f) { m_fCntState = 125.0f; }
				SetPosition2(m_fCntState * 0.01f);
			}
			break;
		case STATE_MEDIUM:
			if (m_fCntState > 100.0f)
			{
				m_fCntState -= 4.0f;
				if (m_fCntState <= 100.0f) { m_fCntState = 100.0f; }
				SetPosition2(m_fCntState * 0.01f);
			}
			else if (m_fCntState < 100.0f)
			{
				m_fCntState += 4.0f;
				if (m_fCntState >= 100.0f) { m_fCntState = 100.0f; }
				SetPosition2(m_fCntState * 0.01f);
			}
			break;
		case STATE_SMALL:
			if (m_fCntState > 75.0f)
			{
				m_fCntState -= 4.0f;
				if (m_fCntState <= 75.0f) { m_fCntState = 75.0f; }
				SetPosition2(m_fCntState * 0.01f);
			}
			break;
		case STATE_FLASH:
			m_fCntState = m_fCntState + 1.0f;
			fWKstate = m_fCntState * 0.01f;
			SetColor(D3DXCOLOR(fWKstate, fWKstate, fWKstate, 1.0f));
			if (m_fCntState >= 100.0f) { m_state = STATE_FLASH2; m_fCntState = 100.0f; }
			break;
		case STATE_FLASH2:
			m_fCntState = m_fCntState - 1.5f;
			fWKstate = m_fCntState * 0.01f;
			SetColor(D3DXCOLOR(fWKstate, fWKstate, fWKstate, 1.0f));
			if (m_fCntState <= 25.0f) { m_state = STATE_FLASH; m_fCntState = 25.0f; }
			break;
		}
	}
}