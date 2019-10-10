//=============================================================================
//
// 3D�p�[�e�B�N������ [3Dparticle.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _3DPARTICLE_H_
#define _3DPARTICLE_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// �O���錾
//=============================================================================
class CRotationShader;


//=============================================================================
// 3D�|���S���N���X
//=============================================================================
class C3DParticle : public CScene
{
public:
	C3DParticle();
	C3DParticle(int nPriority, CScene::OBJTYPE obj);
	~C3DParticle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static C3DParticle * Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; };	//�e�N�X�`���̊�����

	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };					//�ʒu�̍X�V
	void SetColor(D3DXCOLOR col);
	void SetMoveTex(float fMoveX, float fMoveY);
	void SetDivision(D3DXVECTOR2 Division) { m_Division = Division; };			//�e�N�X�`���̕�����
	void SetUV(int nWidth, int nHight);											//UV�̐ݒ�
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };								//�����̐ݒ�
	D3DXVECTOR3 GetRot(void) { return m_rot; }									//�����̎擾
	D3DXVECTOR3 GetPos(void) { return m_pos; };									//�ʒu�̎擾

private:
	//�ϐ��錾

	//�����o�ϐ�
	LPDIRECT3DTEXTURE9		m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_pos;				//���S���W
	D3DXVECTOR3				m_rot;				//����
	D3DXVECTOR3				m_size;				//�|���S���̃T�C�Y
	D3DXVECTOR3				m_vtxMin;			//�|���S���̍ŏ��l
	D3DXVECTOR3				m_vtxMax;			//�|���S���̍ŏ��l
	D3DXMATRIX				m_mtxWorld;			//���[���h�}�g���b�N�X
	D3DXVECTOR2				m_Division;			//������
	CRotationShader *		m_pRotationShader;	//�V�F�[�_�[���

	D3DXVECTOR3 m_nor[2];
};
#endif
