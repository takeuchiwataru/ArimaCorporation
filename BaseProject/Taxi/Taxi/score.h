//=============================================================================
//
// �X�R�A���� [score.h]
// Author : �L�n�@���u
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
//�@�O���錾
//*****************************************************************************
class CNumber;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCORE_MAX		(7)		// �X�R�A�̌���

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScore : public CScene
{
public:
	typedef enum
	{
		STATE_NONE = 0,	// �����Ȃ�
		STATE_SET,		// �ݒ���
		STATE_ROULETTE,	// ���[���b�g���
		STATE_DESSCORE,	// �X�R�A�̕\��
		STATE_ADD_TOTAL,// �g�[�^���X�R�A�ɉ��Z
		STATE_ADD,		// �X�R�A�ɉ��Z
		STATE_RESET,	// ���s�̏ꍇ
		STATE_MAX		// ����
	}STATE;

	typedef enum
	{
		TYPE_USE = 0,
		TYPE_NOT_USE,
		TYPE_MAX
	}TYPE;

	CScore(int nPriority = 6, OBJTYPE objType = OBJTYPE_SCORE);
	~CScore();

	static CScore *Create(D3DXVECTOR3 pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void Reset(void) { m_nScore = 0; m_state = STATE_RESET; };
	static void AddTotalScore(void);				// �g�[�^���X�R�A�ɉ��Z

	//===================
	// �ݒ� �擾 �̊֐�
	//===================
	static int GetScore(void) { return m_nScore; };
	static void AddScore(int nScore);
	static void SetScore(int nScore);
	static TYPE GetType() { return m_type; }
	static void SetType(TYPE type) { m_type = type; }
	void SetType();
private:
	void DebugKey(void);
	void NumberRoulette(int nTexData);
	void pow(void);

	CNumber *m_apNumber[SCORE_MAX];	// �i���o�[�ւ̃|�C���^
	static int m_nScore;			// �X�R�A
	static int m_nScoreNum;			// �X�R�A�̐�����
	D3DXVECTOR3 m_pos;				// �ʒu
	static STATE	m_state;		// ���
	int				m_nCntRoulette;	//���[���b�g�̃J�E���^�[
	int				m_nCntNum;		// �؂�ւ��ԍ��L�^
	int				m_nCntAdd;		// ���[���b�g���A���������Z����
	static TYPE			m_type;

};

#endif