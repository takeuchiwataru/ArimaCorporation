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
#include "DispEffect.h"
#include "player.h"
#include "gamecamera.h"

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
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
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

	m_DrawType = C2D::DRAW_TYPE_NORMAL;
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
void CScene3D::Update(void) {}

//=============================================================================
// �`�揈��
//=============================================================================
void CScene3D::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxView, mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	if (C2D::DrawPrepare(m_DrawType, pDevice)) { return; }
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
	C2D::DrawPrepare(C2D::DRAW_TYPE_NORMAL, pDevice);
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
void CScene3D::SetPosSize(D3DXVECTOR3 pos, D3DXVECTOR2 nSize)
{
	m_pos = pos;
	m_size = nSize;

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f);

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
//=============================================================================
// �A�j���[�V�����̏���
//=============================================================================
void CScene3D::SetTexture(int PatternAnim, int X, int Y, int nNum)
{
	VERTEX_3D *pVtx;//���_���ւ̃|�C���^

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
//===============================================================================
// �ݒ�֐�
//===============================================================================
void CScene3D::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	m_Type = TYPE_GROUND;
	SetPosSize(pos, size);
	m_rot = rot;
}

//===============================================================================
// �����֐�
//===============================================================================
C3DPolygon *C3DPolygon::Create(TYPE Type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority)
{
	C3DPolygon *p3D = NULL;
	p3D = new C3DPolygon(nPriority);

	if (p3D != NULL) { p3D->Setting(Type, pos, rot); }

	return p3D;
}
//===============================================================================
// �����ݒ�֐�
//===============================================================================
void	C3DPolygon::Setting(TYPE &Type, D3DXVECTOR3 &pos, D3DXVECTOR3 &rot)
{
	D3DXVECTOR2 size = D3DXVECTOR2(10.0f, 10.0f);
	D3DXCOLOR col = INIT_COL;
	float fCntState = 0.0f;

	Init();
	CScene3D::Set(pos, rot, size);

	switch (Type)
	{
	case TYPE_Pin:
		BindTexture(CDispEffect::GetpTexAll(CDispEffect::TEX_Pin));
		GetDrawType() = C2D::DRAW_TYPE_OVERLAY;
		size = D3DXVECTOR2(PLAYER_LENGTH * 1.0f, PLAYER_LENGTH * 30.0f);
		pos.y -= 30.0f;
		SetWall(pos, INIT_VECTOR, size);
		break;
	case TYPE_Light:
		BindTexture(CDispEffect::GetpTexAll(CDispEffect::TEX_Light));
		GetDrawType() = C2D::DRAW_TYPE_ADD;
		size = D3DXVECTOR2(PLAYER_LENGTH * 3.0f, PLAYER_LENGTH * 60.0f);
		pos.y -= 100.0f;
		SetWall(pos, INIT_VECTOR, size);
		break;
	case TYPE_Shadow:
		BindTexture(CDispEffect::GetpTexAll(CDispEffect::TEX_Shadow));
		GetDrawType() = C2D::DRAW_TYPE_SUBTRACT;
		size = D3DXVECTOR2(PLAYER_LENGTH, PLAYER_LENGTH);
		SetGroundPosSize(pos, size);
		break;
	case TYPE_FootSteps:
		BindTexture(CDispEffect::GetpTexAll(CDispEffect::TEX_FootSteps));
		GetDrawType() = C2D::DRAW_TYPE_SUBTRACT;
		pos.y += 3.0f;
		size = D3DXVECTOR2(3.0f, 3.0f);
		SetGroundPosSize(pos, size);
		fCntState = 0.01f;
		break;
	}
	Set(Type, fCntState, col);
}
//===============================================================================
// �ݒ�֐�
//===============================================================================
void	C3DPolygon::Set(TYPE &Type, float &fCntState, D3DXCOLOR &col)
{
	m_Initpos = GetposR();
	m_Type = Type;
	m_fCntState = fCntState;
	m_col = col;
	SetColor(m_col);
}
//===============================================================================
// �������֐�
//===============================================================================
HRESULT C3DPolygon::Init(void)
{
	CScene3D::Init();
	m_Type = TYPE_MAX;
	m_fCntState = 0.0f;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	return S_OK;
}
//===============================================================================
// �X�V�֐�
//===============================================================================
void C3DPolygon::Update(void)
{
	if (m_Type == TYPE_MAX) { return; }
	switch (m_Type)
	{
	case TYPE_Light:
		if (GetDrawType() != C2D::DRAW_TYPE_NO)
		{
			if (m_col.a < 1.0f)
			{
				m_col.a += 0.1f;
				if (m_col.a > 1.0f) { m_col.a = 1.0f; }
				SetColor(m_col);
			}
		}
		else { m_col.a = 0.0f; }
		break;
	case TYPE_Shadow:
		if (m_fCntState <= 0.0f)
		{
			m_col.a -= 0.05f;
			SetColor(m_col);
		}
		else { m_fCntState -= 1.0f; }
		break;
	case TYPE_FootSteps:
		m_col.a -= m_fCntState;
		SetColor(m_col);
		if (m_col.a < 0.0f) { Uninit(); return; }
		break;
	}
}
//===============================================================================
// �`��֐�
//===============================================================================
void C3DPolygon::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�A���t�@�e�X�g����(�������ɂ��摜�̐؂��茻�ۂ�L���ɂ���)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 30);//REF = Reference:�Q��
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);//FUNC = Function:�@�\ //GREATER = �`���傫��
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	switch (m_Type)
	{
	case TYPE_Light:
		SetRotY();
		GoStraight(15.0f);
		break;
	case TYPE_Pin:
		SetRotY();
		break;
	}
	CScene3D::Draw();

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);    // ���ʂ��J�����O
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}
//===============================================================================
// �`��֐�
//===============================================================================
void C3DPolygon::SetShadow(D3DXVECTOR3 pos)
{
	GetposR() = pos;
	m_fCntState = 1.0f;

	m_col.a = 1.0f;
	SetColor(m_col);
}
//===============================================================================
// �p�xY���r���{�[�h��
//===============================================================================
void	C3DPolygon::SetRotY(void)
{
	CGameCamera *pCamera = CGame::GetGameCamera(CGame::GetCameraNumber());
	if (pCamera == NULL)
	{
		pCamera = CGame::GetCourseCamera();
		if (pCamera == NULL) { return; }
	}
	D3DXVECTOR3 pos = pCamera->GetCameraPosV();
	D3DXVECTOR3 &m_pos = GetposR();
	GetrotR().y = atan2f(m_pos.x - pos.x, m_pos.z - pos.z);
}
//===============================================================================
// �p�xY���r���{�[�h��
//===============================================================================
void	C3DPolygon::GoStraight(float fPlus)
{
	float fRotY = GetrotR().y;
	GetposR() = m_Initpos - D3DXVECTOR3(sinf(fRotY), 0.0f, cosf(fRotY)) * fPlus;
}

//===============================================================================
// �����֐�
//===============================================================================
C3DAnim *C3DAnim::Create(TYPE Type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority)
{
	C3DAnim *p3D = NULL;
	p3D = new C3DAnim(nPriority);

	if (p3D != NULL)
	{
		p3D->C3DPolygon::Setting(Type, pos, rot);
		p3D->Setting(Type);
	}

	return p3D;
}
//===============================================================================
// �ݒ�֐�
//===============================================================================
void	C3DAnim::Setting(TYPE &Type)
{

	switch (Type)
	{
	case TYPE_Light:
		m_nPtn = 0;		m_nPtnX = 3;	m_nPtnY = 1;
		m_nSpeed = 12;	m_Anim = ANIM_LOOP;
		break;
	}
}
//===============================================================================
// �������֐�
//===============================================================================
HRESULT C3DAnim::Init(void)
{
	C3DPolygon::Init();
	m_Anim = ANIM_END;
	m_nPtn = 0;		m_nPtnX = 1;		m_nPtnY = 1;
	m_nSpeed = 1;	m_nCntAnim = -1;
	return S_OK;
}
//===============================================================================
// �X�V�֐�
//===============================================================================
void C3DAnim::Update(void)
{
	if (m_nCntAnim >= m_nSpeed)
	{//���Ԍo�߂�
		m_nCntAnim = 0;		m_nPtn++;

		if (m_nPtn > m_nPtnX * m_nPtnY)
		{//�I���m�F
			if (EndCheck()) { Uninit(); return; }
		}

		//UV�ύX
		SetTexture(m_nPtn, m_nPtnX, m_nPtnY, 1);
	}
	else { m_nCntAnim++; }

	C3DPolygon::Update();
}
//===============================================================================
// �I���`�F�b�N
//===============================================================================
bool	C3DAnim::EndCheck(void)
{
	if (m_Anim == ANIM_END) { return true; }
	if (m_Anim == ANIM_LOOP) { m_nPtn = 0; return false; }
	m_Anim = (ANIM)(m_Anim - 1);
	return false;
}
