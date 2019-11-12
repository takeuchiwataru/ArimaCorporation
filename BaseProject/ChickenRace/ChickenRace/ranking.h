//=============================================================================
//
// �����L���O���� [ranking.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_RANKING				(5)		// �����x�̐�
#define MAX_RANKING_NUMBER		(7)		// �����L���O�̐���
#define MAX_LEVEL_NUM			(2)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CTotalScore;

//=====================
// ��{�N���X
//=====================
class CRanking
{
public:
	CRanking();								// �R���X�g���N�^
	~CRanking();							// �f�X�g���N�^
	HRESULT Init();							// ����������
	void	Uninit(void);					// �I������
	void	Update(void);					// �X�V����
	void	Draw(void);						// �`�揈��

	static void	RankingSave(int nTotalScore, int nLvevl);		// �����L���O�f�[�^�̏�������

private:
	void	RankingLoad(void);								// �����L���O�f�[�^�̓ǂݍ���

	static int			m_nNum;
	int					m_nCntFrame;		// �t�F�[�h����
	bool				m_bOnOff;
};
#endif