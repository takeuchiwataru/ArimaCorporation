//=============================================================================
//
// �t���p�l���̏��� [LDCpanel.h]
// Author : Takuto Ishida
//
//=============================================================================
#include "LCDpanel.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"

//==================================
// �}�N����`
//==================================
#define LCD_INTERVAL		(120)		// �\���ؑւ̊Ԋu

//==================================
// �ÓI�����o�ϐ��錾
//==================================
LPDIRECT3DTEXTURE9 CLCDpanel::m_pTexture[MAX_LCD_TEXTURE] = {};

//==================================
// ��������
//==================================
CLCDpanel* CLCDpanel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fScale)
{
	CLCDpanel *pLCDpanel;

	pLCDpanel = new CLCDpanel;	// ���������m��

	if (pLCDpanel != NULL)
	{// �������m�ې���
		pLCDpanel->Init(pos, rot, fScale);
	}

	return pLCDpanel;
}

//===============================================================================
// �e�N�X�`���̓ǂݍ���
//===============================================================================
HRESULT CLCDpanel::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	const char *m_pTextureName[MAX_LCD_TEXTURE] =
	{// �e�N�X�`���ւ̃p�X
		{ "data\\TEXTURE\\game\\Destination\\Place\\ClockTower.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\Fountain.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\Hospital.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\Main Street & TV Tower.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\Norvesa.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\place000.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\Red Brick.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\Sappro.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\Susukino_Street.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\Tanukikoji.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\Tapiocar.jpg" },
	};

	for (int nCntTexture = 0; nCntTexture < MAX_LCD_TEXTURE; nCntTexture++)
	{// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, m_pTextureName[nCntTexture], &m_pTexture[nCntTexture]);
	}

	return S_OK;
}

//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CLCDpanel::Unload(void)
{
	for (int nCount = 0; nCount < MAX_LCD_TEXTURE; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{//�e�N�X�`���̔j��
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CLCDpanel::CLCDpanel()
{
	m_nCntDisp = 0;
	m_nCurTex = 0;
	m_pObject = NULL;
}

//=========================================
// �f�X�g���N�^
//=========================================
CLCDpanel::~CLCDpanel()
{
}

//=========================================
// ����������
//=========================================
HRESULT CLCDpanel::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fScale)
{
	CScene3D::Init();

	// �e�N�X�`���̐ݒ�
	CScene3D::BindTexture(m_pTexture[m_nCurTex]);

	// �ʒu�ƃT�C�Y�̐ݒ�
	CScene3D::SetPosSize(pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR2(50.0f, 30.0f) * fScale);

	// �����̐ݒ�
	CScene3D::SetRotation(rot);

	if (NULL == m_pObject)
	{// ���f���̐���
		m_pObject = CObject::Create(D3DXVECTOR3(0.0f,0.0f,0.0f), GetRotation(), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.0f, 0, 37, CModel3D::MOVETYPE_NONE, 1);
		m_pObject->SetPosition(pos);
		m_pObject->SetScale(D3DXVECTOR3(3.5f, 3.8f, 4.0f) * fScale);
	}

	CScene3D::SetTypeNumber(CScene3D::TYPE_BLOOD);
	return S_OK;
}

//=========================================
// �I������
//=========================================
void CLCDpanel::Uninit(void)
{
	CScene3D::Uninit();
}

//=========================================
// �X�V����
//=========================================
void CLCDpanel::Update(void)
{
	// �؂�ւ���܂ł̎��Ԃ��J�E���g�A�b�v
	m_nCntDisp = (m_nCntDisp + 1) % LCD_INTERVAL;

	if (0 == m_nCntDisp)
	{// �e�N�X�`����؂�ւ���
		m_nCurTex = (m_nCurTex + 1) % MAX_LCD_TEXTURE;
		CScene3D::BindTexture(m_pTexture[m_nCurTex]);
	}
}

//=========================================
// �`�揈��
//=========================================
void CLCDpanel::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �`�揈��
	CScene3D::Draw();

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���f���̕`��
	m_pObject->Draw();
}

//=========================================
// �X�P�[���{���̐ݒ�
//=========================================
void CLCDpanel::SetScale(float fScale)
{
	// �l�̍X�V
	m_fScale = fScale;

	// �T�C�Y�̐ݒ�
	D3DXVECTOR3 pos = GetPosition();
	CScene3D::SetPosSize(D3DXVECTOR3(pos.x, pos.y * m_fScale, pos.z), D3DXVECTOR2(50.0f, 30.0f) * m_fScale);
	m_pObject->SetScale(D3DXVECTOR3(3.5f, 3.8f, 4.0f) * m_fScale);
}