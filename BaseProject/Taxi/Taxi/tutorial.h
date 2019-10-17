//=============================================================================
//
// �`���[�g���A������ [tutorial.h]
// Author : ���� ����
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SCORE_UI_TUTORIAL	(6)
//*****************************************************************************
// �O���錾
//*****************************************************************************
class CGameCamera;
class CMeshField;
class CPlayer;
class CLoadTextMotion;
class CGround;
class CScene2D;
class CInputKeyBoard;
class CXInput;
class CLoadTextEffect;
class CLogo;

//=================================
// �`���[�g���A���N���X
//=================================
class CTutorial
{
public:
	typedef enum
	{
		TYPE_START = 0,
		TYPE_START1,
		TYPE_START2,
		TYPE_START3,
		TYPE_ACCEL,
		TYPE_BRAK,
		TYPE_LIGHT_HANDLE,
		TYPE_LEFT_HAMDLE,
		TYPE_MIDDLE1,
		TYPE_BACK,
		TYPE_FRONT,
		TYPE_MIDDLE2,
		TYPE_RIDE1,
		TYPE_RIDE2,
		TYPE_MIDDLE3,
		TYPE_MIDDLE4,
		TYPE_MIDDLE5,
		TYPE_RIDESTART,
		TYPE_END,
		TYPE_END1,
		TYPE_END2,
		TYPE_END3,
		TYPE_MAX,
	}TYPE;

	CTutorial();							// �R���X�g���N�^
	~CTutorial();							// �f�X�g���N�^
	HRESULT Init();							// ����������
	void	Uninit(void);					// �I������
	void	Update(void);					// �X�V����
	void	Draw(void);						// �`�揈��

	//---------------------------
	// Geter
	//---------------------------
	static CMeshField *GetMeshField(void) { return m_pMeshField; }				//���b�V���t�B�[���h
	static CGameCamera * GetCamera(void) { return m_pCamera; }					//�J����
	static CPlayer * GetPlayer(void) { return m_pPlayer; }						//�v���C���[
	static CLoadTextMotion * GetPlayerMotion(void) { return m_pPlayerMotion; }	//�v���C���[���[�V����
	static CGround * GetRiver(void) { return m_pGround; }						//��̃|�C���^
	static CLoadTextEffect * GetLoadEffect(void) { return m_pLoadEffect; }		//�ǂݍ��񂾃G�t�F�N�g�̃|�C���^
	static CLoadTextMotion * GetBoyMotion(void) { return m_pBoyMotion; }		//�j�����[�V�����̎擾
	static CLoadTextMotion * GetGirlMotion(void) { return m_pGirlMotion; }		//�������[�V�����̎擾
	static bool GetCustomerPutin(void) { return m_bCustomer; }							//���q����̏�ԏ��
	static CLogo * GetScoreUI(int nNum) { return m_pScoreUI[nNum]; }						//�X�R�A�֌W��UI�̎擾
	static TYPE GetType(void) { return m_type; }

private:
	void MeshFildCreate(void);	//���b�V���t�B�[���h�̐���
	void ObjectCreate(void);	//�I�u�W�F�N�g�̐���
	void CreateLine(void);		//���͂̐���
	void UpdateEnd(void);		//�I��
	void ChangeLine(void);		//���͂̕ύX
	void UpdateFade(void);		//�t�F�[�h
	void WallTexLoad(void);		//�ǂ̃e�N�X�`���ǂݍ���
	void UninitBottun(void);	//�{�^���̔j��
	void CreateBottun(void);	//�{�^���̐���
	void CreateSkip(bool bInput);		//�X�L�b�v�L�[�̐���
	void UninitControlBottun(void);	//�R���g���[���[�̃{�^���̔j��
	void UpdateAnim(void);

	//�t�@�C���ǂݍ��ݗp�̕ϐ�
	char *ReadLine(FILE *pFile, char *pDst);	//1�s�ǂݍ���
	char *GetLineTop(char *pStr);				//�s�̐擪���擾
	int  PopString(char *pStr, char *pDest);	//�s�̍Ō��؂�̂�

	static CMeshField *m_pMeshField;			//���b�V���t�B�[���h�̃|�C���^
	static CGameCamera * m_pCamera;				//�J����
	static CPlayer * m_pPlayer;					//�v���C���[
	static CLoadTextMotion * m_pPlayerMotion;	//�v���C���[���[�V����
	static CGround * m_pGround;					//��̃|�C���^
	static CLoadTextEffect * m_pLoadEffect;		//�G�t�F�N�g�̓ǂݍ��݃|�C���^
	static CLoadTextMotion * m_pBoyMotion;		//�j�����[�V�����ǂݍ���
	static CLoadTextMotion * m_pGirlMotion;		//�������[�V�����ǂݍ���
	static bool m_bCustomer;				//���q����̏�ԏ�ԃt���O
	static CLogo *m_pScoreUI[MAX_SCORE_UI_TUTORIAL];		//���S�ւ̃|�C���^ �X�R�AUI�Ɋ֌W���Ă��镨

	LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`��
	static TYPE m_type;					//�^�C�v
	bool m_bLine[TYPE_MAX];			//���͂̃t���O
	CScene2D * m_pLine;				//���̓|���S��
	int	m_nCountTime;				//���ԃJ�E���^�[
	CLogo * m_pSpace;				//�X�y�[�X�L�[
	CLogo * m_pImage;				//�C���[�W�摜
	CLogo * m_pSkip;				//�X�L�b�v�L�[
	CLogo * m_pBotunn;				//�{�^���摜
	bool m_bInput;					//���͏��
	int	 m_nCountTimeKey;			//���Ԃ̃J�E���^�[
	int  m_nAinNum;					//�A�j���[�V�����p�^�[��
};
#endif