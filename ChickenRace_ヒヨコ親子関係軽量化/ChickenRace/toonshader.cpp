//=============================================================================
//
// �g�D�[���V�F�[�_�[��{���� [ToonShader.cpp]
// Author : �L�n�@���u
//
//=============================================================================
#include "toonshader.h"
#include "manager.h"
#include "renderer.h"

LPDIRECT3DTEXTURE9	CToonShader::m_pTexture = NULL;				//�g�D�[���e�N�X�`���̏��
LPDIRECT3DTEXTURE9	CToonShader::m_pTextureMap = NULL;			//�}�b�v�e�N�X�`���̏��
LPDIRECT3DTEXTURE9	CToonShader::m_pTextureLine = NULL;			//���C���e�N�X�`���̏��
LPDIRECT3DTEXTURE9	CToonShader::m_pTextureMapLine = NULL;		//�}�b�v�e�N�X�`���̏��

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CToonShader::CToonShader()
{
	//m_pTexture = NULL;
	m_pShader = NULL;
	//m_pTextureLine = NULL;
	//m_pTextureMapLine = NULL;
	//m_pTextureMap = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CToonShader::~CToonShader() {}

//=============================================================================
// ����������
//=============================================================================
void CToonShader::Init(char * FileName)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�V�F�[�_�[�̓ǂݍ���
	D3DXCreateEffectFromFile(pDevice,
		FileName,
		NULL,						//�v���v���Z�b�T�[���s���\����
		NULL,						//�C���N���[�h������s���C���^�[�t�F�C�X
		0,							//�ǂݍ��ݎ��̃I�v�V����
		0,							//�V�F�[�_�Ԃł���肷��ϐ�
		&m_pShader,					//�V�F�[�_�̃|�C���^
		NULL);						//�G���[���

	//�f�o�b�N�p���C�g�ʒu
	//0.22f, -0.87f, 0.44f, 0.0f
	//-0.18f, -0.88f, -0.44f, 0.0f
	//0.89f, -0.11f, 0.44f, 0.0f
	//0.72f, -0.80f, 0.54f, 0.0f

	//���C�g�̐ݒ�
	m_pShader->SetVector("LightPos", &D3DXVECTOR4(0.89f, -0.11f, 0.44f, 0.0f));
}

//=============================================================================
// �I������
//=============================================================================
void CToonShader::Uninit(void)
{
	if (m_pTexture != NULL) { m_pTexture = NULL; }
	if (m_pTextureLine != NULL) { m_pTextureLine = NULL; }
	if (m_pTextureMapLine != NULL) { m_pTextureMapLine = NULL; }
	if (m_pTextureMap != NULL) { m_pTextureMap = NULL; }

	//�V�F�[�_�[���̔j��
	if (m_pShader != NULL)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}
}
//=============================================================================
// �I������
//=============================================================================
void CToonShader::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�V�F�[�_�̃e�N�X�`��
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Shader\\Shade.bmp", &m_pTexture)))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
	}
	//�V�F�[�_�̃��C���e�N�X�`��
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Shader\\outline01.bmp", &m_pTextureLine)))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
	}
	//�V�F�[�_�̃}�b�v���C���e�N�X�`��
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Shader\\outline05.bmp", &m_pTextureMapLine)))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
	}
	//�V�F�[�_�̃}�b�v���C���e�N�X�`��
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Shader\\Shade05.bmp", &m_pTextureMap)))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
	}
}

//=============================================================================
// �V�F�[�_�[�̏���n������
//=============================================================================
LPD3DXEFFECT CToonShader::GetShader(void)
{
	return m_pShader;
}

//=============================================================================
// �g�D�[���e�N�X�`���̏���n������
//=============================================================================
LPDIRECT3DTEXTURE9 CToonShader::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
// �A�E�g���C���̏���n������
//=============================================================================
LPDIRECT3DTEXTURE9 CToonShader::GetLineTexture(void)
{
	return m_pTextureLine;
}

//=============================================================================
// �A�E�g���C���̏���n������
//=============================================================================
LPDIRECT3DTEXTURE9 CToonShader::GetLineMapTexture(void)
{
	return m_pTextureMapLine;
}

//=============================================================================
// �g�D�[���e�N�X�`���̏���n������
//=============================================================================
LPDIRECT3DTEXTURE9 CToonShader::GetMapTexture(void)
{
	return m_pTextureMap;
}