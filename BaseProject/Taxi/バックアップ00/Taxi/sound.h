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

		SOUND_LABEL_BGM_GAME000,		// ゲームのBGM000
		SOUND_LABEL_BGM_GAME001,		// ゲームのBGM001
		SOUND_LABEL_BGM_GAME002,		// ゲームのBGM003
		SOUND_LABEL_BGM_GAME003,		// ゲームのBGM004

		SOUND_LABEL_BGM_RESULT,			// リザルトのBGM
		SOUND_LABEL_BGM_SELECT,			// セレクトのBGM

		SOUND_LABEL_SE_ACCEL,			// アクセル音
		SOUND_LABEL_SE_BACK,			// バック音
		SOUND_LABEL_SE_BRAKING,			// ブレーキ音
		SOUND_LABEL_SE_ENGINE,			// エンジン音
		SOUND_LABEL_SE_GEAR,			// ギアチェン音
		SOUND_LABEL_SE_ENGINESTART,		// エンジンスタート音
		SOUND_LABEL_SE_KLAXON,			// クラクション音

		SOUND_LABEL_SE_SKILL,			// スキル発動
		SOUND_LABEL_SE_INTRODUCTION,	// 案内
		SOUND_LABEL_SE_SKILLCANCEL,		// スキルキャンセル
		SOUND_LABEL_SE_SHOOT,			// 銃を撃つ
		SOUND_LABEL_SE_DAMAGE,			// ダメージ
		SOUND_LABEL_SE_PAUSE_SELECT,	// ポーズの選択
		SOUND_LABEL_SE_PAUSE_OPEN,		// ポーズを開く
		SOUND_LABEL_SE_PAUSE_CLOSE,		// ポーズを閉じる
		SOUND_LABEL_SE_PAUSE_MOVE,		// ポーズを動かす
		SOUND_LABEL_SE_TRAP,			// トラップに当たった際
		SOUND_LABEL_SE_KICK,			// キック音
		SOUND_LABEL_SE_CRASH_CAR,		// 衝突音（車）
		SOUND_LABEL_BGM_POLICE_SIREN,	// 警察サイレン
		SOUND_LABEL_SE_STAGESELECT,		// ステージセレクト音
		SOUND_LABEL_SE_SELECT_ENTER,	// セレクト決定音
		SOUND_LABEL_SE_TITLE_ENTER,		// タイトル決定音
		SOUND_LABEL_SE_RIVER,			// 川音
		SOUND_LABEL_SE_DOOR,			// 車のドア音
		SOUND_LABEL_SE_FOUNTAIN,		// 噴水の音
		SOUND_LABEL_SE_COMBO,			// コンボ時のお金の音
		SOUND_LABEL_SE_ADDMONEY,		// メーター加算音
		SOUND_LABEL_SE_VIBRATION,		// スマホの振動音
		SOUND_LABEL_SE_BIGCHEER,		// 大きな歓声
		SOUND_LABEL_SE_SMALLCHEER,		// 小さめの歓声

		// 男声
		SOUND_LABEL_MAN_THANK,			// 感謝
		SOUND_LABEL_MAN_ANGER,			// 怒る
		SOUND_LABEL_MAN_CALL,			// 呼ぶ
		SOUND_LABEL_MAN_AVOID,			// 回避

		// 女声
		SOUND_LABEL_WOMAN_THANK,		// 感謝
		SOUND_LABEL_WOMAN_CALL,			// 呼ぶ
		SOUND_LABEL_WOMAN_AVOID,		// 回避

		// 子供声
		SOUND_LABEL_CHILD_THANK,		// 感謝
		SOUND_LABEL_CHILD_ANGER,		// 怒る
		SOUND_LABEL_CHILD_CALL,			// 呼ぶ
		SOUND_LABEL_CHILD_AVOID,		// 回避

		//衝突音
		SOUND_LABEL_SE_SMALLBOX,		// 小さめの衝突音
		SOUND_LABEL_SE_STEAL,			// 金属音の衝突音
		SOUND_LABEL_SE_LEAF,			// 葉の衝突音
		SOUND_LABEL_SE_LEAF1,			// 葉1の衝突音
		SOUND_LABEL_SE_FANCE00,			// フェンスの音00
		SOUND_LABEL_SE_FANCE01,			// フェンスの音01
		SOUND_LABEL_SE_FANCE02,			// フェンスの音02
		SOUND_LABEL_SE_FANCE03,			// フェンスの音03
		SOUND_LABEL_SE_FANCE04,			// フェンスの音04
		SOUND_LABEL_SE_WOOD00,			// 木00
		SOUND_LABEL_SE_WOOD01,			// 木01

		//タイムアップ
		SOUND_LABEL_TIME_UP,			// TIMEUP

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
		{ "data/SOUND/BGM/TitleBGM.wav", -1 },

		{ "data/SOUND/BGM/bgm_game_000.wav", -1 },
		{ "data/SOUND/BGM/bgm_game_001.wav", -1 },
		{ "data/SOUND/BGM/bgm_game_003.wav", -1 },
		{ "data/SOUND/BGM/bgm_game_004.wav", -1 },

		{ "data/SOUND/BGM/ResultBGM.wav", -1 },
		{ "data/SOUND/BGM/SelectBGM.wav", -1 },


		{ "data/SOUND/SE/car/car-accel.wav", -1 },		// ループさせるのでBGMにする
		{ "data/SOUND/SE/car/car_back.wav", -1 },		// ループさせるのでBGMにする
		//SE一覧
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
		{ "data/SOUND/SE/衝突音/clash_car.wav", 0 },
		{ "data/SOUND/SE/police_siren.wav", -1 },
		{ "data/SOUND/SE/選択音/select.wav", 0 },
		{ "data/SOUND/SE/決定音/select_enter.wav", 0 },
		{ "data/SOUND/SE/決定音/title_enter.wav", 0 },
		{ "data/SOUND/SE/river.wav", -1 },
		{ "data/SOUND/SE/car/car-door.wav", 0 },
		{ "data/SOUND/SE/fountain.wav", -1 },
		{ "data/SOUND/SE/Conbo.wav", 0 },
		{ "data/SOUND/SE/AddMoney.wav", -1 },
		{ "data/SOUND/SE/vibration.wav", -1 },
		{ "data/SOUND/SE/BigCheer.wav", 0 },
		{ "data/SOUND/SE/SmalCeer.wav", 0 },


		// 男声
		{ "data/SOUND/SE/01_男性/00_タクシー感謝/01_ManThank.wav", 0 },
		{ "data/SOUND/SE/01_男性/01_タクシー評価怒る/01_MaAgryn.wav", 0 },
		{ "data/SOUND/SE/01_男性/02_タクシー呼び/01_ManCarStop.wav", 0 },
		{ "data/SOUND/SE/01_男性/03_タクシー回避/01_ManAvoid.wav", 0 },

		// 女声
		{ "data/SOUND/SE/00_女性/00_タクシー感謝/00_WomanThank.wav", 0 },
		{ "data/SOUND/SE/00_女性/02_タクシー呼ぶ/00_WomanCarStop.wav", 0 },
		{ "data/SOUND/SE/00_女性/03_タクシー回避/00_WomanAvoid.wav", 0 },

		// 子供声
		{ "data/SOUND/SE/02_子供/00_タクシー感謝/02_ChildThank.wav", 0 },
		{ "data/SOUND/SE/02_子供/01_タクシー評価怒る/02_ChildAgryn.wav", 0 },
		{ "data/SOUND/SE/02_子供/02_タクシー呼ぶ/02_ChildCarStop.wav", 0 },
		{ "data/SOUND/SE/02_子供/99_タクシー鳴き声/02_ChildAvoid.wav", 0 },

		//衝突音
		{ "data/SOUND/SE/衝突音/smallbox.wav", 0 },
		{ "data/SOUND/SE/衝突音/steel.wav", 0 },
		{ "data/SOUND/SE/衝突音/leaf.wav", 0 },
		{ "data/SOUND/SE/衝突音/leaf1.wav", 0 },
		{ "data/SOUND/SE/衝突音/fance00.wav", 0 },
		{ "data/SOUND/SE/衝突音/fance01.wav", 0 },
		{ "data/SOUND/SE/衝突音/fance02.wav", 0 },
		{ "data/SOUND/SE/衝突音/fance03.wav", 0 },
		{ "data/SOUND/SE/衝突音/fance04.wav", 0 },
		{ "data/SOUND/SE/衝突音/wood00.wav", 0 },
		{ "data/SOUND/SE/衝突音/wood01.wav", 0 },

		//タイムアップ
		{ "data/SOUND/SE/TimeUp/timeup.wav", 0 },
	};
};

#endif