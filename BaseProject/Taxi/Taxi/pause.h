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
// �O���錾
//*****************************************************************************
class CLogo;
class CBackGround;
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PAUSE_POLYGON	(5)

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

	CPause(int nPriority = 7, CScene::OBJTYPE objType = CScene::OBJTYPE_PAUSE);
	~CPause();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPause *Create();
	static bool &GetbPause() { return m_bPause; }
	static bool GetPauseEnd() { return m_bSerectEnd; }
	void SetTexType(int nType) { m_OperationNum = nType; }

private://�l�ł̂ݎg��
	D3DXVECTOR3 SelectMove(D3DXVECTOR3 pos);				// �I�����A�C�R���̈ړ�
	void PauseEnd(void);									// �|�[�Y�I���̊֐�
	void ColChange(void);									// �F�̐ؑ�
	void SelectNumOperation(void);							// �I��ԍ��ɂ���ď������A��
	//void MenuMove(int nNum ,D3DXVECTOR3 **pos);			// ���j���[�̈ړ�

	D3DXVECTOR3				m_pos;							//�ʒu
	D3DXCOLOR				m_col;							//�F
	float					m_SelectColor;					//�I�����Ă���Ƃ��̐F
	int						m_nCntState;					//�J�E���g���Ă�����
	int						m_SelectNum;					//�Z���N�g�̐�
	int						m_OperationNum;					// ������@�̐ؑ�
	int						m_nCntPauseEnd;					// �|�[�Y����鎞�̃J�E���^�[
	int						m_nPauseCounter;				//Pause�̃J�E���g
	static bool				m_bSerectEnd;					//�I�яI��������
	MENU					m_menu;							//���j���[�̍\����
	PAUSESTATE				m_state;						//�|�[�Y�̏��
	D3DXVECTOR2				m_nSize;						//�T�C�Y
	static bool				m_bPause;
	CBackGround				*m_pBg;
	CLogo					*m_apLogo[MAX_PAUSE_POLYGON];	// ���S
	CLogo					*m_pSelectIcon;					// �A�C�R��
};
#endif