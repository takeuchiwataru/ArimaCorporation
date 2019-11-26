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
		//==========================
		//           BGM
		//==========================	
		SOUND_LABEL_BGM_TITLE = 0,			//�^�C�g���̋�
		SOUND_LABEL_BGM_CHARACTERSERECT,	//�L�����N�^�[�I���̋�
		SOUND_LABEL_BGM_GAME,				//�Q�[���̋�
		SOUND_LABEL_BGM_RANKING,			//�����L���O�̋�

											//==========================
											//            SE
											//==========================
											SOUND_LABEL_SE_CHARACTERSERECT,		//�L�����N�^�[�I����
											SOUND_LABEL_SE_STARTCOUNT,			//�X�^�[�g�J�E���g
											SOUND_LABEL_SE_SPEEDDOWN,			//�X�s�[�h�_�E��
											SOUND_LABEL_SE_TITLEFADE,			//�^�C�g�����艹
											SOUND_LABEL_SE_DAMAGE,				//�_���[�W
											SOUND_LABEL_SE_HITKICK,				//����R��
											SOUND_LABEL_SE_ATTACKWARNING,		//�U���x��
											SOUND_LABEL_SE_RAN,					//����
											SOUND_LABEL_SE_EGGBREAK,			//���������
											SOUND_LABEL_SE_EGGLAY,				//�����Y�܂��

											SOUND_LABEL_SE_THROW,				//���𓊂���
											SOUND_LABEL_SE_JUMP,				//�W�����v
											SOUND_LABEL_SE_EAT,					//�a��H�ׂ�
											SOUND_LABEL_SE_CURSORMOVEMENT,		//���j���[�I����
											SOUND_LABEL_SE_CHICKEXPLOSION,		//�����Ɋ������܂�
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
	void SetFrequency(SOUND_LABEL label, float fRatio);		// �����̎��g���̎擾

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
		//==========================
		//           BGM
		//==========================
		{ "data/SOUND/BGM/Title_BGM.wav", -1 },
		{ "data/SOUND/BGM/CharacterSerect_BGM.wav", -1 },
		{ "data/SOUND/BGM/Game_BGM.wav", -1 },
		{ "data/SOUND/BGM/Ranking_BGM.wav", -1 },

		//==========================
		//            SE
		//==========================
		{ "data/SOUND/SE/CharacterSerect_SE.wav", 0 },
		{ "data/SOUND/SE/StartCount_SE.wav", 0 },
		{ "data/SOUND/SE/SpeedDown_SE.wav", 0 },
		{ "data/SOUND/SE/TitleSerect_SE.wav", 0 },
		{ "data/SOUND/SE/Damage_SE.wav", 0 },
		{ "data/SOUND/SE/HitKick_SE.wav", 0 },
		{ "data/SOUND/SE/AttackWarning_SE.wav", 0 },
		{ "data/SOUND/SE/Ran_SE.wav", 0 },
		{ "data/SOUND/SE/EggBreak_SE.wav", 0 },
		{ "data/SOUND/SE/EggLay_SE.wav", 0 },
		{ "data/SOUND/SE/Throw_SE.wav", 0 },
		{ "data/SOUND/SE/Jump_SE.wav", 0 },
		{ "data/SOUND/SE/Eat_SE.wav", 0 },
		{ "data/SOUND/SE/Cursormovement_SE.wav", 0 },
		{ "data/SOUND/SE/Chickexplosion_SE.wav", 0 }
	};
};

#endif