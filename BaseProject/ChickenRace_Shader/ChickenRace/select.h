//=============================================================================
//
// �Z���N�g���� [select.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _SELECT_H_
#define _SELECT_H_

#include "main.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SELECT_STAGE		(2)		// �X�e�[�W��
#define MAX_SELECT_NUM			(3)
//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;

//=====================
// ��{�N���X
//=====================
class CSelect
{
public:
	CSelect();								// �R���X�g���N�^
	~CSelect();								// �f�X�g���N�^
	HRESULT Init();							// ����������
	void	Uninit(void);					// �I������
	void	Update(void);					// �X�V����
	void	Draw(void);						// �`�揈��
	static int GetSelectLevel(void) { return m_nStageNumber;}
private:
	void	SlectStage(void);				// �X�e�[�W�I��
	void	Select(void);					// ��Փx�I��
	void	SelectLevel(void);				// ��Փx�I��
	void	TexAnim(void);					// �e�N�X�`���A�j���[�V����
	int		RankingLoad(int nType, int SelectLevel);			// �����L���O�̓ǂݍ���
	void	SelectHuman(void);
	void	Fade(void);						//�t�F�[�h����
	void	BackUninit(void);

	int				m_nStageNum;					// �X�e�[�W�ԍ�
	static int		m_nStageNumber;					// �X�e�[�W�ԍ�
	int				m_nScore;						// �X�R�A
	int				m_nCntAnim;						// �A�j���[�V�����J�E���^�[
	int				m_nCntPattern;					// �p�^�[���J�E���^�[
	D3DXVECTOR2		m_nSizeOld[MAX_SELECT_STAGE];	// �T�C�Y�̕ۑ�
	CScene2D		*m_pFream;						// �t���[��
	CScene2D		*m_pBg;							// ��Փx�I�����̔w�i
	bool			m_bPress;						// �������̃t���O
	bool			m_bSame;						// �����ꍇ�̃t���O
	bool			m_bSelectFlag;					// �I���t���O
	bool			m_bLevelFlag;					// ��Փx�I���t���O
	float			m_fAddRot;						// �����̉��Z
	bool			m_bEnter;						// ���艟�������ǂ���
	bool			m_bMove;
};
#endif