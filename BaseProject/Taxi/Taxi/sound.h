//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : �L�n ���u
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//=====================
//  ��{�N���X
//=====================
class CSound
{
public:
	typedef enum
	{
		SOUND_LABEL_BGM_TITLE = 0,		// �^�C�g����BGM

		SOUND_LABEL_BGM_GAME000,		// �Q�[����BGM000
		SOUND_LABEL_BGM_GAME001,		// �Q�[����BGM001
		SOUND_LABEL_BGM_GAME002,		// �Q�[����BGM003
		SOUND_LABEL_BGM_GAME003,		// �Q�[����BGM004

		SOUND_LABEL_BGM_RESULT,			// ���U���g��BGM
		SOUND_LABEL_BGM_SELECT,			// �Z���N�g��BGM

		SOUND_LABEL_SE_ACCEL,			// �A�N�Z����
		SOUND_LABEL_SE_BACK,			// �o�b�N��
		SOUND_LABEL_SE_BRAKING,			// �u���[�L��
		SOUND_LABEL_SE_ENGINE,			// �G���W����
		SOUND_LABEL_SE_GEAR,			// �M�A�`�F����
		SOUND_LABEL_SE_ENGINESTART,		// �G���W���X�^�[�g��
		SOUND_LABEL_SE_KLAXON,			// �N���N�V������

		SOUND_LABEL_SE_SKILL,			// �X�L������
		SOUND_LABEL_SE_INTRODUCTION,	// �ē�
		SOUND_LABEL_SE_SKILLCANCEL,		// �X�L���L�����Z��
		SOUND_LABEL_SE_SHOOT,			// �e������
		SOUND_LABEL_SE_DAMAGE,			// �_���[�W
		SOUND_LABEL_SE_PAUSE_SELECT,	// �|�[�Y�̑I��
		SOUND_LABEL_SE_PAUSE_OPEN,		// �|�[�Y���J��
		SOUND_LABEL_SE_PAUSE_CLOSE,		// �|�[�Y�����
		SOUND_LABEL_SE_PAUSE_MOVE,		// �|�[�Y�𓮂���
		SOUND_LABEL_SE_TRAP,			// �g���b�v�ɓ���������
		SOUND_LABEL_SE_KICK,			// �L�b�N��
		SOUND_LABEL_SE_CRASH_CAR,		// �Փˉ��i�ԁj
		SOUND_LABEL_BGM_POLICE_SIREN,	// �x�@�T�C����
		SOUND_LABEL_SE_STAGESELECT,		// �X�e�[�W�Z���N�g��
		SOUND_LABEL_SE_SELECT_ENTER,	// �Z���N�g���艹
		SOUND_LABEL_SE_TITLE_ENTER,		// �^�C�g�����艹
		SOUND_LABEL_SE_RIVER,			// �쉹
		SOUND_LABEL_SE_DOOR,			// �Ԃ̃h�A��
		SOUND_LABEL_SE_FOUNTAIN,		// �����̉�
		SOUND_LABEL_SE_COMBO,			// �R���{���̂����̉�
		SOUND_LABEL_SE_ADDMONEY,		// ���[�^�[���Z��
		SOUND_LABEL_SE_VIBRATION,		// �X�}�z�̐U����
		SOUND_LABEL_SE_BIGCHEER,		// �傫�Ȋ���
		SOUND_LABEL_SE_SMALLCHEER,		// �����߂̊���

		// �j��
		SOUND_LABEL_MAN_THANK,			// ����
		SOUND_LABEL_MAN_ANGER,			// �{��
		SOUND_LABEL_MAN_CALL,			// �Ă�
		SOUND_LABEL_MAN_AVOID,			// ���

		// ����
		SOUND_LABEL_WOMAN_THANK,		// ����
		SOUND_LABEL_WOMAN_CALL,			// �Ă�
		SOUND_LABEL_WOMAN_AVOID,		// ���

		// �q����
		SOUND_LABEL_CHILD_THANK,		// ����
		SOUND_LABEL_CHILD_ANGER,		// �{��
		SOUND_LABEL_CHILD_CALL,			// �Ă�
		SOUND_LABEL_CHILD_AVOID,		// ���

		//�Փˉ�
		SOUND_LABEL_SE_SMALLBOX,		// �����߂̏Փˉ�
		SOUND_LABEL_SE_STEAL,			// �������̏Փˉ�
		SOUND_LABEL_SE_LEAF,			// �t�̏Փˉ�
		SOUND_LABEL_SE_LEAF1,			// �t1�̏Փˉ�
		SOUND_LABEL_SE_FANCE00,			// �t�F���X�̉�00
		SOUND_LABEL_SE_FANCE01,			// �t�F���X�̉�01
		SOUND_LABEL_SE_FANCE02,			// �t�F���X�̉�02
		SOUND_LABEL_SE_FANCE03,			// �t�F���X�̉�03
		SOUND_LABEL_SE_FANCE04,			// �t�F���X�̉�04
		SOUND_LABEL_SE_WOOD00,			// ��00
		SOUND_LABEL_SE_WOOD01,			// ��01

		//�^�C���A�b�v
		SOUND_LABEL_TIME_UP,			// TIMEUP

		SOUND_LABEL_MAX
	}SOUND_LABEL;

	CSound();
	~CSound();
	HRESULT InitSound(HWND hWnd);
	void UninitSound(void);
	HRESULT PlaySound(SOUND_LABEL label); //�Q�[���̏������߂��@
	void StopSound(SOUND_LABEL label);	  //�������O�������ăX�g�b�v������
	void StopSound(void);
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	float GetVolume(SOUND_LABEL label);						// ���ʂ̎擾
	void SetVolume(SOUND_LABEL label, float fVolume);		// ���ʂ̐ݒ�

private:
	typedef struct
	{
		char *m_pFilename;		// �t�@�C����
		int m_nCntLoop;			// ���[�v�J�E���g(-1�Ń��[�v�Đ�)
	} SOUNDPARAM;

	IXAudio2				*m_pXAudio2;							// XAudio2�I�u�W�F�N�g�ւ̃|�C���^
	IXAudio2MasteringVoice	*m_pMasteringVoice;						// �}�X�^�[�{�C�X�ւ̃|�C���^
	IXAudio2SourceVoice		*m_apSourceVoice[SOUND_LABEL_MAX];		// �\�[�X�{�C�X�ւ̃|�C���^
	BYTE					*m_apDataAudio[SOUND_LABEL_MAX];		// �I�[�f�B�I�f�[�^�ւ̃|�C���^
	DWORD					m_aSizeAudio[SOUND_LABEL_MAX];			// �I�[�f�B�I�f�[�^�T�C�Y

	// �e���f�ނ̃p�����[�^
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{
		//-1�̓��[�v�@�@0�͒P��
		//BGM�ꗗ
		{ "data/SOUND/BGM/TitleBGM.wav", -1 },

		{ "data/SOUND/BGM/bgm_game_000.wav", -1 },
		{ "data/SOUND/BGM/bgm_game_001.wav", -1 },
		{ "data/SOUND/BGM/bgm_game_003.wav", -1 },
		{ "data/SOUND/BGM/bgm_game_004.wav", -1 },

		{ "data/SOUND/BGM/ResultBGM.wav", -1 },
		{ "data/SOUND/BGM/SelectBGM.wav", -1 },


		{ "data/SOUND/SE/car/car-accel.wav", -1 },		// ���[�v������̂�BGM�ɂ���
		{ "data/SOUND/SE/car/car_back.wav", -1 },		// ���[�v������̂�BGM�ɂ���
		//SE�ꗗ
		{ "data/SOUND/SE/car/car-braking.wav", 0 },
		{ "data/SOUND/SE/car/car-engine.wav", 0 },
		{ "data/SOUND/SE/car/car-gear.wav", 0 },
		{ "data/SOUND/SE/car/car-enginestart.wav", 0 },
		{ "data/SOUND/SE/car/car-klaxon.wav", 0 },

		{ "data/SOUND/SE/Skill.wav", 0 },
		{ "data/SOUND/SE/IntroductionSE.wav", 0 },
		{ "data/SOUND/SE/SkillCancellationSE.wav", 0 },
		{ "data/SOUND/SE/ShootSE.wav", 0 },
		{ "data/SOUND/SE/Damage.wav", 0 },
		{ "data/SOUND/SE/pause_select.wav", 0 },
		{ "data/SOUND/SE/pause_open.wav", 0 },
		{ "data/SOUND/SE/pause_close.wav", 0 },
		{ "data/SOUND/SE/pause_move.wav", 0 },
		{ "data/SOUND/SE/TrapSE.wav", 0 },
		{ "data/SOUND/SE/kick.wav", 0 },
		{ "data/SOUND/SE/�Փˉ�/clash_car.wav", 0 },
		{ "data/SOUND/SE/police_siren.wav", -1 },
		{ "data/SOUND/SE/�I����/select.wav", 0 },
		{ "data/SOUND/SE/���艹/select_enter.wav", 0 },
		{ "data/SOUND/SE/���艹/title_enter.wav", 0 },
		{ "data/SOUND/SE/river.wav", -1 },
		{ "data/SOUND/SE/car/car-door.wav", 0 },
		{ "data/SOUND/SE/fountain.wav", -1 },
		{ "data/SOUND/SE/Conbo.wav", 0 },
		{ "data/SOUND/SE/AddMoney.wav", -1 },
		{ "data/SOUND/SE/vibration.wav", -1 },
		{ "data/SOUND/SE/BigCheer.wav", 0 },
		{ "data/SOUND/SE/SmalCeer.wav", 0 },


		// �j��
		{ "data/SOUND/SE/01_�j��/00_�^�N�V�[����/01_ManThank.wav", 0 },
		{ "data/SOUND/SE/01_�j��/01_�^�N�V�[�]���{��/01_MaAgryn.wav", 0 },
		{ "data/SOUND/SE/01_�j��/02_�^�N�V�[�Ă�/01_ManCarStop.wav", 0 },
		{ "data/SOUND/SE/01_�j��/03_�^�N�V�[���/01_ManAvoid.wav", 0 },

		// ����
		{ "data/SOUND/SE/00_����/00_�^�N�V�[����/00_WomanThank.wav", 0 },
		{ "data/SOUND/SE/00_����/02_�^�N�V�[�Ă�/00_WomanCarStop.wav", 0 },
		{ "data/SOUND/SE/00_����/03_�^�N�V�[���/00_WomanAvoid.wav", 0 },

		// �q����
		{ "data/SOUND/SE/02_�q��/00_�^�N�V�[����/02_ChildThank.wav", 0 },
		{ "data/SOUND/SE/02_�q��/01_�^�N�V�[�]���{��/02_ChildAgryn.wav", 0 },
		{ "data/SOUND/SE/02_�q��/02_�^�N�V�[�Ă�/02_ChildCarStop.wav", 0 },
		{ "data/SOUND/SE/02_�q��/99_�^�N�V�[����/02_ChildAvoid.wav", 0 },

		//�Փˉ�
		{ "data/SOUND/SE/�Փˉ�/smallbox.wav", 0 },
		{ "data/SOUND/SE/�Փˉ�/steel.wav", 0 },
		{ "data/SOUND/SE/�Փˉ�/leaf.wav", 0 },
		{ "data/SOUND/SE/�Փˉ�/leaf1.wav", 0 },
		{ "data/SOUND/SE/�Փˉ�/fance00.wav", 0 },
		{ "data/SOUND/SE/�Փˉ�/fance01.wav", 0 },
		{ "data/SOUND/SE/�Փˉ�/fance02.wav", 0 },
		{ "data/SOUND/SE/�Փˉ�/fance03.wav", 0 },
		{ "data/SOUND/SE/�Փˉ�/fance04.wav", 0 },
		{ "data/SOUND/SE/�Փˉ�/wood00.wav", 0 },
		{ "data/SOUND/SE/�Փˉ�/wood01.wav", 0 },

		//�^�C���A�b�v
		{ "data/SOUND/SE/TimeUp/timeup.wav", 0 },
	};
};

#endif