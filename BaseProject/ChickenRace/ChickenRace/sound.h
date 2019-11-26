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
		//==========================
		//           BGM
		//==========================	
		SOUND_LABEL_BGM_TITLE = 0,			//タイトルの曲
		SOUND_LABEL_BGM_CHARACTERSERECT,	//キャラクター選択の曲
		SOUND_LABEL_BGM_GAME,				//ゲームの曲
		SOUND_LABEL_BGM_RANKING,			//ランキングの曲

											//==========================
											//            SE
											//==========================
											SOUND_LABEL_SE_CHARACTERSERECT,		//キャラクター選択音
											SOUND_LABEL_SE_STARTCOUNT,			//スタートカウント
											SOUND_LABEL_SE_SPEEDDOWN,			//スピードダウン
											SOUND_LABEL_SE_TITLEFADE,			//タイトル決定音
											SOUND_LABEL_SE_DAMAGE,				//ダメージ
											SOUND_LABEL_SE_HITKICK,				//殴る蹴る
											SOUND_LABEL_SE_ATTACKWARNING,		//攻撃警告
											SOUND_LABEL_SE_RAN,					//走る
											SOUND_LABEL_SE_EGGBREAK,			//卵が割れる
											SOUND_LABEL_SE_EGGLAY,				//卵が産まれる

											SOUND_LABEL_SE_THROW,				//卵を投げる
											SOUND_LABEL_SE_JUMP,				//ジャンプ
											SOUND_LABEL_SE_EAT,					//餌を食べる
											SOUND_LABEL_SE_CURSORMOVEMENT,		//メニュー選択音
											SOUND_LABEL_SE_CHICKEXPLOSION,		//爆発に巻き込まれ
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
	void SetFrequency(SOUND_LABEL label, float fRatio);		// 音声の周波数の取得

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