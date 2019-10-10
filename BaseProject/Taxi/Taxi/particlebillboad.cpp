//=============================================================================
//
// �p�[�e�B�N���r���{�[�h���� [particlebillboad.cpp]
// Author : ���� ����
//
//=============================================================================
#include "particlebillboad.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "shadermgr.h"
#include "rotationshader.h"
#include "gamecamera.h"
#include "loadText.h"
#include "tutorial.h"

//==================================
// �}�N����`
//==================================
#define MAX_LIFE_GRAND		(30)
#define CHANGE_SIZE			(0.1f)
#define VECTOR_ZERO			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//�x�N�g���̏�����
#define FLAM				(8)
#define CHANGE_A			(0.06f)
#define CHANGE_A_STAR		(0.14f)
#define RAND_ANGLE			(314)
#define CICLE_MIN			(-200)
#define CICLE_MAX			(400)
#define LIGHT_MIN			(18.0f)
#define LIGHT_MAX			(32.0f)
#define SIZE_UP				(0.15f)
#define SPEW_MIN			(-100)
#define SPEW_MAX			(200)

//==================================
// �ÓI�����o�ϐ��錾
//==================================
LPDIRECT3DTEXTURE9 * CParticleBillboad::m_ppTexture = NULL;
int CParticleBillboad::m_nMaxTexture = 0;

//=============================================================================
// ��������
//=============================================================================
CParticleBillboad * CParticleBillboad::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nNumTex, DRAWTYPE DrawType, TYPE type, D3DXCOLOR col, int nLife, float fSpeed, int nMinHight, int nMaxHight, bool bZBuffu, int nPriority)
{
	//�C���X�^���X�̐���
	CParticleBillboad * pParticle = NULL;

	if (type == TYPE_NORMAL)
	{//�ʏ�
		pParticle = new CParticleBillboad(nPriority);
	}
	else if (type == TYPE_SPEW)
	{//�����^
		pParticle = new CSpewParticle(nPriority);
	}
	else if (type == TYPE_GRANDSMOKE)
	{//���̏�Ŋg��
		pParticle = new CGrandSmokeParticle(nPriority);
	}
	else if (type == TYPE_LIGHT)
	{//�g��k��
		pParticle = new CLightParticle(nPriority);
	}
	else if (type == TYPE_CICLE)
	{//�~�`�^
		pParticle = new CCicleParticle(nPriority);
	}
	else if (type == TYPE_STAR)
	{//�����^
		pParticle = new CStarParticle(nPriority);
	}

	if (pParticle != NULL)
	{
		pParticle->SetSpeed(fSpeed);						//�X�s�[�h�̐ݒ�
		pParticle->SetMinHight(nMinHight);					//�����̍ŏ��l
		pParticle->SetMaxHight(nMaxHight);					//�����̍ő�l
		pParticle->SetSize(D3DXVECTOR2(size.x, size.y));	//�T�C�Y�̐ݒ�

		pParticle->Init();					//����������

		//�ݒ菈��
		pParticle->SetPos(pos);				//�ʒu�̐ݒ�
		pParticle->SetDrawType(DrawType);	//�`��^�C�v�̐ݒ�
		pParticle->SetColor(col);			//�F�̐ݒ�
		pParticle->SetColorA(col.a);		//�F��a�l�̐ݒ�
		pParticle->SetZbuffu(bZBuffu);		//Z�o�b�t�@�̐ݒ�
		pParticle->SetLife(nLife);			//���C�t�̐ݒ�

		if (m_ppTexture[nNumTex] != NULL)
		{
			pParticle->BindTexture(m_ppTexture[nNumTex]);		//�e�N�X�`���̊�����
		}
	}
	else
	{
		MessageBox(NULL, TEXT("pParticle��NULL�ł�"), TEXT("CEffectTool::Create()"), MB_OK);
	}

	return pParticle;
}

//=========================================
// �f�t�H���g�R���X�g���N�^
//=========================================
CParticleBillboad::CParticleBillboad() : CScene(3, CScene::OBJTYPE_EFFECT)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
}

//=========================================
// �R���X�g���N�^
//=========================================
CParticleBillboad::CParticleBillboad(int nPriority) : CScene(nPriority, CScene::OBJTYPE_EFFECT)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
}

//=========================================
// �f�X�g���N�^
//=========================================
CParticleBillboad::~CParticleBillboad(){}

//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
void CParticleBillboad::LoadTexture(void)
{
	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�e�N�X�`�����̎擾
	CLoadTextEffect * pLoadTextTexture = {};
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		pLoadTextTexture = CGame::GetLoadEffect();	//�p�[�e�B�N���e�N�X�`���̃|�C���^���擾
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		pLoadTextTexture = CTutorial::GetLoadEffect();	//�p�[�e�B�N���e�N�X�`���̃|�C���^���擾
	}
	CLoadText::TEX_INFO TexInfo = pLoadTextTexture->GetTexInfo();	//�e�N�X�`�����̎擾
	m_nMaxTexture = TexInfo.nMaxTex;								//�e�N�X�`���̍ő吔���擾


	//�e�N�X�`�����T�C�Y���q�[�v�̈悩�烁�����𓮓I�m�ۂ���
	if (m_ppTexture == NULL)
	{
		m_ppTexture = new LPDIRECT3DTEXTURE9[m_nMaxTexture];
	}

	// �e�N�X�`���̐���
	for (int nCntTex = 0; nCntTex < m_nMaxTexture; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			TexInfo.ppFileName[nCntTex],
			&m_ppTexture[nCntTex]);
	}
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CParticleBillboad::UnloadTexture(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < m_nMaxTexture; nCntTex++)
	{
		if (m_ppTexture[nCntTex] != NULL)
		{
			m_ppTexture[nCntTex]->Release();
			m_ppTexture[nCntTex] = NULL;
		}
	}

	//�e�N�X�`���|�C���^�̔j��
	if (m_ppTexture != NULL)
	{
		delete[] m_ppTexture;
		m_ppTexture = NULL;
	}
}

//=========================================
// ����������
//=========================================
HRESULT CParticleBillboad::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bDeth = false;

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
void CParticleBillboad::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{// ���_���̔j��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (m_pTexture != NULL)
	{
		m_pTexture = NULL;
	}

	// ������j��
	Release();
}

//=========================================
// �X�V����
//=========================================
void CParticleBillboad::Update(void)
{
	//�N���b�s���O�����̒ǉ�
	CManager::MODE mode = CManager::GetMode();
	CGameCamera * pGameCamera = NULL;

	if (mode == CManager::MODE_GAME)
	{//�Q�[��
		pGameCamera = CGame::GetGameCamera();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//�`���[�g���A��
		pGameCamera = CTutorial::GetCamera();
	}

	SetDraw(pGameCamera->ClippingPos(m_pos));
}

//=========================================
// �`�揈��
//=========================================
void CParticleBillboad::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxView;	// �v�Z�p�}�g���b�N�X

	//�A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (m_DrawType == DRAWTYPE_ADD)
	{
		//���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	if (m_bZBuffu)
	{
		//�f�t�H���g�̏ꍇFALSE�ɂȂ��Ă��邩��TRUE�ɂ���
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}

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
		pDevice->SetTexture(0, m_pTexture);

		// ���C�e�B���O�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// ���C�e�B���O��L���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	if (m_DrawType == DRAWTYPE_ADD)
	{
		//���u�����f�B���O�����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	if (m_bZBuffu)
	{
		//�f�t�H���g�̏ꍇFALSE�ɂȂ��Ă��邩��TRUE�ɂ���
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}
}

//=========================================
// �F�̐ݒ�
//=========================================
void CParticleBillboad::SetColor(D3DXCOLOR col)
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

//=========================================
// �T�C�Y�̍Đݒ�
//=========================================
void CParticleBillboad::RemakeSize(D3DXVECTOR2 size)
{
	m_size = size;

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_��
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f);

	//���_�o�b�t�@�̃A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ��]�̏C������
//=============================================================================
void CParticleBillboad::SetAngle(float * pAngle)
{
	//�p�x�̏C��
	if (*pAngle < -D3DX_PI)
	{
		*pAngle += D3DX_PI * 2.0f;
	}
	if (*pAngle > D3DX_PI)
	{
		*pAngle -= D3DX_PI * 2.0f;
	}
}

//=========================================
// UV���W�̐ݒ�
//=========================================
void CParticleBillboad::SetUV(D3DXVECTOR2 MinUV, D3DXVECTOR2 MaxUV)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(MinUV.x, MaxUV.y);
	pVtx[1].tex = D3DXVECTOR2(MaxUV.x, MaxUV.y);
	pVtx[2].tex = D3DXVECTOR2(MinUV.x, MinUV.y);
	pVtx[3].tex = D3DXVECTOR2(MaxUV.x, MinUV.y);

	//���_�o�b�t�@�̃A�����b�N����
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
//
// ���^�p�[�e�B�N���N���X
//
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSpewParticle::CSpewParticle(int nPriority) : CParticleBillboad(nPriority) {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSpewParticle::~CSpewParticle() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSpewParticle::Init(void)
{
	CParticleBillboad::Init();

	//�ϐ��̏�����
	m_nCountFlam = 0;		//�t���[���J�E���^�[
	m_nLife = 30;			//�\������
	m_fChangeCol_a = 0.0f;	//�����x�̕ω���;
	m_bGravity = true;		//�d�͂�ON�EOFF
	m_fGravity = 0.0f;		//�d��

	float fPosX = (float)(SPEW_MIN + rand() % SPEW_MAX);
	float fPosZ = (float)(SPEW_MIN + rand() % SPEW_MAX);

	//�ړI�̊p�x�����߂�
	float fAngle = atan2f(fPosX, fPosZ);

	//�ړ��ʂ����߂�
	m_move.x = sinf(fAngle) * m_fSpeed;
	m_move.z = cosf(fAngle) * m_fSpeed;

	//�����ݒ�
	float fHight = (float)(m_nMinHight + rand() % m_nMaxHight);
	m_move.y = fHight;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSpewParticle::Uninit(void)
{
	CParticleBillboad::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSpewParticle::Update(void)
{
	//�d�͂𑫂�
	if (m_bGravity == true)
	{
		m_move.y -= m_fGravity;
	}

	//���Ԃ̉��Z
	m_nCountFlam++;

	if (m_nCountFlam > FLAM)	//���̃t���[�������o������
	{
		m_nLife--;						//�\�����Ԃ̌��Z

		if (m_nLife < 0)
		{
			m_colA -= m_fChangeCol_a;	//�����x��������

			if (m_colA < 0.0f)
			{
				m_bDeth = true;
			}
			else
			{
				D3DXCOLOR col = CParticleBillboad::GetColor();
				CParticleBillboad::SetColor(D3DXCOLOR(col.r, col.g, col.b, m_colA));
			}
		}
	}

	D3DXVECTOR3 pos = CParticleBillboad::GetPos();
	pos += m_move;
	CParticleBillboad::SetPos(pos);

	CParticleBillboad::Update();

	//���S��Ԃ�������j��
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSpewParticle::Draw(void)
{
	CParticleBillboad::Draw();
}

//*****************************************************************************
//
// �y���p�[�e�B�N���N���X
//
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGrandSmokeParticle::CGrandSmokeParticle(int nPriority) : CParticleBillboad(nPriority) {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGrandSmokeParticle::~CGrandSmokeParticle() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGrandSmokeParticle::Init(void)
{
	CParticleBillboad::Init();

	m_nCountFlam = 0;		//�t���[���J�E���^�[
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGrandSmokeParticle::Uninit(void)
{
	CParticleBillboad::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGrandSmokeParticle::Update(void)
{
	//���Ԃ̉��Z
	m_nCountFlam++;

	m_nLife--;				//�\�����Ԃ̌��Z

	if (m_nLife < 0)
	{
		m_colA -= CHANGE_A;	//�����x��������

		if (m_colA < 0.0f)
		{
			m_bDeth = true;
		}
		else
		{
			D3DXCOLOR col = CParticleBillboad::GetColor();
			CParticleBillboad::SetColor(D3DXCOLOR(col.r, col.g, col.b, m_colA));
		}
	}

	//�T�C�Y�̍X�V
	D3DXVECTOR2 size = CParticleBillboad::GetSize();
	size.x += SIZE_UP;
	size.y += SIZE_UP;

	CParticleBillboad::SetSize(size);
	CParticleBillboad::Update();

	//���S��Ԃ�������j��
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGrandSmokeParticle::Draw(void)
{
	CParticleBillboad::Draw();
}

//*****************************************************************************
//
// ���C�g�p�[�e�B�N��
//
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLightParticle::CLightParticle(int nPriority) : CParticleBillboad(nPriority) {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLightParticle::~CLightParticle() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLightParticle::Init(void)
{
	CParticleBillboad::Init();
	m_fChangeSize = CHANGE_SIZE;
	m_nLife = MAX_LIFE_GRAND;	//�\������
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLightParticle::Uninit(void)
{
	CParticleBillboad::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLightParticle::Update(void)
{
	//�T�C�Y�̕ۑ�
	D3DXVECTOR2 size = CParticleBillboad::GetSize();

	//�p�[�e�B�N���̃T�C�Y��ύX����
	if (size.x > LIGHT_MAX || size.x < LIGHT_MIN)
	{
		m_fChangeSize *= -1;	//�����̕ύX
	}

	//�T�C�Y�̕ύX
	size.x += m_fChangeSize;
	size.y += m_fChangeSize;

	//�T�C�Y�̐ݒ�
	SetSize(size);

	CParticleBillboad::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLightParticle::Draw(void)
{
	CParticleBillboad::Draw();
}

//*****************************************************************************
//
// �~�`�p�[�e�B�N���N���X
//
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCicleParticle::CCicleParticle(int nPriority) : CParticleBillboad(nPriority) {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCicleParticle::~CCicleParticle() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCicleParticle::Init(void)
{
	CParticleBillboad::Init();

	m_nCountFlam = 0;						//�t���[���J�E���^�[
	m_nLife = 30;							//�\������
	m_fChangeCol_a = 0.0f;					//�����x�̕ω���;
	m_bGravity = true;						//�d�͂�ON�EOFF
	m_fGravity = 0.0f;						//�d��

	float fPosX = (float)(CICLE_MIN + rand() % CICLE_MAX);
	float fPosY = (float)(CICLE_MIN + rand() % CICLE_MAX);

	//�ړI�̊p�x�����߂�
	float fAngle = atan2f(fPosX, fPosY);

	//�ړ��ʂ����߂�
	m_move.x = sinf(fAngle) * m_fSpeed;
	m_move.z = cosf(fAngle) * m_fSpeed;
	m_move.y = cosf(fAngle) * m_fSpeed;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCicleParticle::Uninit(void)
{
	CParticleBillboad::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCicleParticle::Update(void)
{
	//�d�͂𑫂�
	if (m_bGravity == true)
	{
		m_move.y -= m_fGravity;
	}

	m_nLife--;						//�\�����Ԃ̌��Z

	if (m_nLife < 0)
	{
		m_colA -= m_fChangeCol_a;	//�����x��������

		if (m_colA < 0.0f)
		{
			m_bDeth = true;
		}
		else
		{
			D3DXCOLOR col = CParticleBillboad::GetColor();
			CParticleBillboad::SetColor(D3DXCOLOR(col.r, col.g, col.b, m_colA));
		}
	}

	CParticleBillboad::Update();

	//���S��Ԃ�������j��
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCicleParticle::Draw(void)
{
	CParticleBillboad::Draw();
}


//*****************************************************************************
//
// ���p�[�e�B�N���N���X
//
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CStarParticle::CStarParticle(int nPriority) : CParticleBillboad(nPriority)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CStarParticle::~CStarParticle()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CStarParticle::Init(void)
{
	CParticleBillboad::Init();

	m_nCountFlam = 0;			//�t���[���J�E���^�[
								//�����ݒ�
	//m_rot.x = (float)(rand() % RAND_ANGLE);
	//m_rot.z = (float)(rand() % RAND_ANGLE);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CStarParticle::Uninit(void)
{
	CParticleBillboad::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CStarParticle::Update(void)
{
	//���Ԃ̉��Z
	m_nCountFlam++;

	m_nLife--;				//�\�����Ԃ̌��Z

	if (m_nLife < 0)
	{
		m_colA -= CHANGE_A_STAR;	//�����x��������

		if (m_colA < 0.0f)
		{
			m_bDeth = true;
		}
		else
		{
			D3DXCOLOR col = CParticleBillboad::GetColor();
			CParticleBillboad::SetColor(D3DXCOLOR(col.r, col.g, col.b, m_colA));
		}
	}

	CParticleBillboad::Update();

	//���S��Ԃ�������j��
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// �`�揈��
//=============================================================================
void CStarParticle::Draw(void)
{
	CParticleBillboad::Draw();
}