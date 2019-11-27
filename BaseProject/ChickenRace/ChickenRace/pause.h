//=============================================================================
//
// �|�[�Y���� [pause.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PAUSE_TEXTURE	(5)

//=====================
//  CScene�̔h���N���X
//=====================
class CPause : public CScene
{
public://�N�ł�������
	typedef enum
	{
		PAUSE_NORMAL = 0,		//�I�����Ă��Ȃ����
		PAUSE_SELECT,			//�I���������
		PAUSE_MAX				//�I�����̍ő吔
	}PAUSESTATE;

	typedef enum
	{
		CONTINU = 0,			//�R���e�j���[	�y�Q�[���ɖ߂�z
		RESULT,					//���U���g		�y�ŏ������蒼���z
		QUIT,					//�N�C�b�g		�y�Q�[�����I���z
		MAX_MEMU				//�I�����̍ő吔
	}MENU;

	static HRESULT Load(void);
	static void UnLoad(void);
	CPause(int nPriority = 7, CScene::OBJTYPE objType = CScene::OBJTYPE_PAUSE);
	~CPause();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPause *Create();
	static bool &GetbPause() { return m_bPause; }
	static bool GetPauseEnd() { return m_bSerectEnd; }

private://�l�ł̂ݎg��
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_PAUSE_TEXTURE];//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_pos;							//�ʒu
	D3DXCOLOR				m_col;							//�F
	float					m_SelectColor;					//�I�����Ă���Ƃ��̐F
	int						m_nCntState;					//�J�E���g���Ă�����
	int						m_SelectNum;					//�Z���N�g�̐�
	static bool				m_bSerectEnd;					//�I�яI��������
	MENU					m_menu;							//���j���[�̍\����
	PAUSESTATE				m_state;						//�|�[�Y�̏��
	D3DXVECTOR2				m_nSize;						//�T�C�Y
	static bool				m_bPause;
};
#endif