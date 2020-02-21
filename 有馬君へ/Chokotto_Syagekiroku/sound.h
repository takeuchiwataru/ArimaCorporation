//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*************************************
// サウンドファイル
//*************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_RANKING000,		// RANKING
	SOUND_LABEL_TUTORIAL000,	// TUTORIAL

	SOUND_LABEL_FBGM000,		// FIELDBGM0
	SOUND_LABEL_FBGM001,		// FIELDBGM0
	SOUND_LABEL_FBGM002,		// FIELDBGM0
	SOUND_LABEL_FBGM003,		// FIELDBGM0

	SOUND_LABEL_BOTTON000,		//決定音 && チャージ音
	SOUND_LABEL_BOTTON001,		//決定音
	SOUND_LABEL_BOTTON002,		//カーソル移動音
	SOUND_LABEL_BOTTON003,		//削除音
	SOUND_LABEL_PAUSE000,		//PAUSE音
	SOUND_LABEL_SHOT000,		//弾発射音0
	SOUND_LABEL_SHOT001,		//弾発射音1
	SOUND_LABEL_SHOT002,		//弾発射音2
	SOUND_LABEL_SHOT003,		//弾発射音3
	SOUND_LABEL_SHOT004,		//弾発射音4
	SOUND_LABEL_KNIFE000,		//ナイフ使用時の音
	SOUND_LABEL_HIT_Shot000,	//弾がヒットしたときの音
	SOUND_LABEL_HIT_KNIFE000,	//ナイフがヒットしたときの音
	SOUND_LABEL_GUARD000,		//ガード成功時の音
	SOUND_LABEL_BOMB000,		//雑魚敵の爆発
	SOUND_LABEL_BOMB001,		//ボスの爆発
	SOUND_LABEL_KIRA,			//止まっている弾のSTART音
	SOUND_LABEL_MODE_CHANGE,	//BOSSモードチェンジ
	SOUND_LABEL_WIND,			//BOSSモードチェンジの時の風の音
	SOUND_LABEL_GAMEOVER000,	//ゲームオーバー
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
