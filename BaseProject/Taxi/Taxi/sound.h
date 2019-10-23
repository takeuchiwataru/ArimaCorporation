//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 有馬 武志
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//=====================
//  基本クラス
//=====================
class CSound
{
public:
	typedef enum
	{
		SOUND_LABEL_BGM_TITLE = 0,		// タイトルのBGM

		SOUND_LABEL_MAX
	}SOUND_LABEL;

	CSound();
	~CSound();
	HRESULT InitSound(HWND hWnd);
	void UninitSound(void);
	HRESULT PlaySound(SOUND_LABEL label); //ゲームの初期化近く　
	void StopSound(SOUND_LABEL label);	  //同じ名前を書いてストップさせる
	void StopSound(void);
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	float GetVolume(SOUND_LABEL label);						// 音量の取得
	void SetVolume(SOUND_LABEL label, float fVolume);		// 音量の設定

private:
	typedef struct
	{
		char *m_pFilename;		// ファイル名
		int m_nCntLoop;			// ループカウント(-1でループ再生)
	} SOUNDPARAM;

	IXAudio2				*m_pXAudio2;							// XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice	*m_pMasteringVoice;						// マスターボイスへのポインタ
	IXAudio2SourceVoice		*m_apSourceVoice[SOUND_LABEL_MAX];		// ソースボイスへのポインタ
	BYTE					*m_apDataAudio[SOUND_LABEL_MAX];		// オーディオデータへのポインタ
	DWORD					m_aSizeAudio[SOUND_LABEL_MAX];			// オーディオデータサイズ

	// 各音素材のパラメータ
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{
		//-1はループ　　0は単発
		//BGM一覧
		{ "data/SOUND/BGM/TitleBGM.wav", -1 }
	};
};

#endif