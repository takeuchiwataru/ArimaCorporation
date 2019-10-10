//=============================================================================
//
// �g�[�^���X�R�A���� [totalscore.h]
// Author : �L�n�@���u
//
//=============================================================================
#ifndef _TOTALSCORE_H_
#define _TOTALSCORE_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
//�@�O���錾
//*****************************************************************************
class CNumber;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TOTALSCORE_MAX	(7)		// �^�C�}�[�̌���

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTotalScore : public CScene
{
public:
	typedef enum
	{
		STATE_NONE = 0,		// �����Ȃ�
		STATE_ROULETTE,		// ���[���b�g���
		STATE_DISSCORE,		// �g�[�^���X�R�A�̕\��
		STATE_MAX			// ����
	}STATE;

	typedef enum
	{
		TYPE_USE = 0,
		TYPE_NOT_USE,
		TYPE_MAX
	}TYPE;


	// ��{�I�Ȋ֐�
	CTotalScore(int nPriority = 6, OBJTYPE objType = OBJTYPE_TOTALSCORE);
	~CTotalScore();

	static CTotalScore *Create(D3DXVECTOR3 pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �֐�
	void RouletteNone(void);						//	���[���b�g��Ԃɂ��Ȃ�
	void SetRankingScore(int nTotalScore);			// �����L���O�p�̃g�[�^���X�R�A�ݒ�
	void SetCol(D3DXCOLOR col) { m_col = col; }		// �F�̐ݒ�
	void ChangeCol(D3DXCOLOR col);					// �F�̓_��
	void SetType();


	// �ݒ�Ǝ擾�̊֐�
	static int GetScore(void) { return m_nTotalScore; };
	static void AddTotalScore(int nTotalScore);

private:
	int NumberRoulette(int nTexData, CManager::MODE mode, int nNumData);

	CNumber			*m_apNumber[TOTALSCORE_MAX];	// �i���o�[�ւ̃|�C���^
	D3DXVECTOR3		m_pos;							// �ʒu
	D3DXCOLOR		m_col;							// �F
	static STATE	m_state;						// ���
	static int		m_nTotalScore;					// �X�R�A
	static int		m_nTotalNum;					// �g�[�^���X�R�A�̐�����
	int				m_nCntRoulette;					//���[���b�g�̃J�E���^�[
	int				m_nCntNum;						// �؂�ւ��ԍ��L�^
	int				m_nCntAdd;						// ���[���b�g���A���������Z����
	TYPE			m_type;


	// �����L���O�p�̕ϐ�
	int				m_nRankingScore;				// �����L���O�p�̃X�R�A�ۑ��p
	int				m_nRankingTotalNum;				// �����L���O�p�̃X�R�A������
	int				m_nCntColTime;					// �F�̕ω��̎���
	int				m_nCountTime;					// ���Ԃ̃J�E���^�[
	bool			m_bChangeCol;					// �F�̕ω��̃t���O
	bool			m_bPlaySound;					// ���̍Đ��t���O
	bool			m_bPlaySoundResult;				// ���U���g�p�̉��̍Đ��t���O
};

#endif