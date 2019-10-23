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
		{ "data/SOUND/BGM/TitleBGM.wav", -1 }
	};
};

#endif