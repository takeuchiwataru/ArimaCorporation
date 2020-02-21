//=========================================================================================================//
//
// タイトルの処理 [Title.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall突破
#include <stdio.h>				//インクルドファイル セーブロードのため
#include "game.h"
#include "main.h"
#include"input.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//


//=========================================================================================================//
// * ゲームの初期化処理
//=========================================================================================================//
void InitResult(void)
{
	InitExplosion();
	InitGameWindow();
	InitFont();
	InitScore();

	//初期化！
	Result *pResult = GetResult();

	pResult[0].nCntScore = 0;
	pResult[0].nCntResult = 1;
	pResult[0].nRankin = 0;
	pResult[0].nHightFont = 0;
	pResult[0].nWideFont = 0;
	strcpy(&pResult[0].aStr[0], "");

	//ロード
	FILE *pFile;
	SCORE *pScore = GetScore();
	Difficulty *pDifficulty = GetDifficulty();

	pFile = fopen("Score.bin", "rb");

	if (pFile != NULL)
	{//Save　成功
		fread(&pScore[0], sizeof(SCORE), MAX_SCORE, pFile);

		fclose(pFile);
	}
	else
	{//失敗
		MessageBox(0, "", "ロードに失敗しました", MB_OK);
	}

	//BGだよ！
	SetExplosion(D3DXVECTOR3{ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f }, COLLAR{ 255,255,255 }, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, 6, 3, 2, ANIMATION_LOOP);
	//難易度のロゴ
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.1f, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 1.0f }, 300, 45, 2, WINDOWSTATE_NORMAL, WINDOWUSE_RANKING);
	//スコア表示の枠
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.3f, 0.0f), D3DXCOLOR{ 0.6f, 0.55f, 0.0f, 1.0f }, 450, 45, 3, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL);
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.45f, 0.0f), D3DXCOLOR{ 0.6f, 0.6f, 0.6f, 1.0f }, 450, 45, 3, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL);
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.6f, 0.0f), D3DXCOLOR{ 0.6f, 0.5f, 0.4f, 1.0f }, 450, 45, 3, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL);
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.75f, 0.0f), D3DXCOLOR{ 0.06f, 0.08f, 0.6f, 1.0f }, 450, 45, 3, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL);
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.9f, 0.0f), D3DXCOLOR{ 0.06f, 0.08f, 0.6f, 1.0f }, 450, 45, 3, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL);
	//一位とかの王冠
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.3f, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 1.0f }, 30, 30, 4, WINDOWSTATE_NORMAL, WINDOWUSE_RANKNUMBER);
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.45f, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 1.0f }, 30, 30, 4, WINDOWSTATE_NORMAL, WINDOWUSE_RANKNUMBER);
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.6f, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 1.0f }, 30, 30, 4, WINDOWSTATE_NORMAL, WINDOWUSE_RANKNUMBER);
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.75f, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 1.0f }, 30, 30, 4, WINDOWSTATE_NORMAL, WINDOWUSE_RANKNUMBER);
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.9f, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 1.0f }, 30, 30, 4, WINDOWSTATE_NORMAL, WINDOWUSE_RANKNUMBER);
	//それぞれのスコア
	SetScore(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.3f, 0.0f), D3DXCOLOR{ 1.0f, 0.99f, 0.04f, 0.0f }, pScore[pDifficulty[0].nNumber * 5].g_nScore, 1, SCORESTATE_FADE);
	SetScore(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.45f, 0.0f), D3DXCOLOR{ 1.0f, 0.99f, 0.04f, 0.0f }, pScore[pDifficulty[0].nNumber * 5 + 1].g_nScore, 2, SCORESTATE_FADE);
	SetScore(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.6f, 0.0f), D3DXCOLOR{ 1.0f, 0.99f, 0.04f, 0.0f }, pScore[pDifficulty[0].nNumber * 5 + 2].g_nScore, 3, SCORESTATE_FADE);
	SetScore(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.75f, 0.0f), D3DXCOLOR{ 1.0f, 0.99f, 0.04f, 0.0f }, pScore[pDifficulty[0].nNumber * 5 + 3].g_nScore, 4, SCORESTATE_FADE);
	SetScore(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.9f, 0.0f), D3DXCOLOR{ 1.0f, 0.99f, 0.04f, 0.0f }, pScore[pDifficulty[0].nNumber * 5 + 4].g_nScore, 5, SCORESTATE_FADE);
	//それぞれの名前
	SetFont(RECT{ 700 , 195, 1200, 300 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5].aStr[0], 0, FONTSTATE_NONE);
	SetFont(RECT{ 700 , 305, 1200, 410 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5 + 1].aStr[0], 1, FONTSTATE_NONE);
	SetFont(RECT{ 700 , 415, 1200, 520 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5 + 2].aStr[0], 2, FONTSTATE_NONE);
	SetFont(RECT{ 700 , 525, 1200, 630 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5 + 3].aStr[0], 3, FONTSTATE_NONE);
	SetFont(RECT{ 700 , 635, 1200, 740 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5 + 4].aStr[0], 4, FONTSTATE_NONE);
}
//=========================================================================================================//
// * ゲームの終了処理
//=========================================================================================================//
void UninitResult(void)
{
	//セーブ
	FILE *pFile;
	SCORE *pScore = GetScore();

	pFile = fopen("Score.bin", "wb");

	if (pFile != NULL)
	{//Save　成功
		fwrite(&pScore[0], sizeof(SCORE), MAX_SCORE, pFile);

		fclose(pFile);
	}
	else
	{//失敗
		MessageBox(0, "", "セーブに失敗しました", MB_OK);
	}

	BreakExplosion();
	BreakGameWindow();
	UninitExplosion();
	UninitScore();
	UninitFont();
}
//=========================================================================================================//
// * ゲームの更新処理
//=========================================================================================================//
void UpdateResult(void)
{
	int *nSetFont = GetSetFont();
	Result *pResult = GetResult();
	SCORE *nGameScore = GetGameScore();
	SCORE *pScore = GetScore();
	Difficulty *pDifficulty = GetDifficulty();
	int nCntRank;	//ランキングを数える
	int nLength;	//文字列の長さを図るため

	//RANKING入れ替え
	if (pResult[0].nCntResult % 20 == 0 && pResult[0].nCntResult > 80 && pResult[0].nCntScore < 5)
	{

		if (pScore[pDifficulty[0].nNumber * 5 + pResult[0].nCntScore].g_nScore < nGameScore[0].g_nScore)
		{//ハイスコアが出た場合
			PlaySound(SOUND_LABEL_HIT_KNIFE000);
			SetScore(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * (0.3f + pResult[0].nCntScore * 0.15f) - 100, 0.0f), D3DXCOLOR{ 1.0f, 0.99f, 0.04f, 0.0f }, nGameScore[0].g_nScore, 0, SCORESTATE_SET);	//セットスコア
			SetAnimation(D3DXVECTOR3{ SCREEN_WIDTH * 0.37f, SCREEN_HEIGHT * (0.3f + pResult[0].nCntScore * 0.15f), 0.0f }, COLLAR{ 255,30,30 }, 50, 50, 9, 1, 2, 0, ANIMATION_NORMAL);	//セットEXPLOSION

			for (nCntRank = 4; pResult[0].nCntScore <= nCntRank; nCntRank--)
			{//下のほうからスコア入れ替え
				pScore[pDifficulty[0].nNumber * 5 + nCntRank].g_nScore = pScore[pDifficulty[0].nNumber * 5 + nCntRank - 1].g_nScore;	//更新
				strcpy(&pScore[pDifficulty[0].nNumber * 5 + nCntRank].aStr[0], &pScore[pDifficulty[0].nNumber * 5 + nCntRank - 1].aStr[0]);	//名前更新
				if (nCntRank == 4)
				{//5位脱落 見た目的に
					ChangeScore(nCntRank + 1, SCORESTATE_DELETE);	//デリートスコア
					ChangeFont(true, "", nCntRank, FONTSTATE_DELETE);//デリートネーム
				}
				else
				{//サガール
					ChangeScore(nCntRank + 1, SCORESTATE_RANKDOWN);	//DOWNスコア
					ChangeFont(true, "", nCntRank, FONTSTATE_RANKDOWN);//DOWNネーム
				}
			}
			pScore[pDifficulty[0].nNumber * 5 + pResult[0].nCntScore].g_nScore = nGameScore[0].g_nScore;		//更新
			strcpy(&pScore[pDifficulty[0].nNumber * 5 + pResult[0].nCntScore].aStr[0], "名無しのプログラマー");	//名前代入(仮)
			pResult[0].nRankin = pDifficulty[0].nNumber * 5 + pResult[0].nCntScore;								//場所保存
			pResult[0].nCntScore = 5; //更新終了
			pResult[0].nCntResult = -60;//ハイスコアの判定
		}
		else
		{//ハイスコアでなかった場合
			PlaySound(SOUND_LABEL_GUARD000);
			SetAnimation(D3DXVECTOR3{ SCREEN_WIDTH * 0.37f, SCREEN_HEIGHT * (0.3f + pResult[0].nCntScore * 0.15f), 0.0f }, COLLAR{ 255,255,255 }, 50, 50, 9, 1, 2, 0, ANIMATION_NORMAL);	//セットEXPLOSION
			pResult[0].nCntScore++;	//次のスコア確認へ
		}
	}

	UpdateExplosion();
	UpdateScore();

	if (pResult[0].nCntResult == 0 && pResult[0].nCntScore > 4)
	{//名前入力処理 ハイスコアが出た場合
		if (pResult[0].nCntScore == 5)
		{//window展開 50音と選択枠
			BreakFont();
			nSetFont[0] = 1;
			SetFont(RECT{ 450 , 100, 1200, 200 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pResult[0].aStr[0], 5, FONTSTATE_NONE);
			SetFont(RECT{ 300 , 100, 1200, 200 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, "名前入力 > ", 6, FONTSTATE_NONE);
			SetFont(RECT{ 300 , 670, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, "* Z/決定", 7, FONTSTATE_NONE);
			SetFont(RECT{ 550 , 670, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, "* X/戻る", 8, FONTSTATE_NONE);
			SetFont(RECT{ 800 , 670, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, "* CTRL/終わり", 9, FONTSTATE_NONE);
			SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 0.0f, 0.0f, 0.0f, 0.8f }, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL);
			SetWindow(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, SCREEN_HEIGHT / 16, 21, 2, 0, WINDOWSTATE_SELECTON, WINDOWUSE_FONT);
			pResult[0].nCntScore++;
		}
		if (GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_LEFT) == true || GetKeyboardTrigger(DIK_RIGHT) == true)
		{ PlaySound(SOUND_LABEL_BOTTON002); }

		if (GetKeyboardTrigger(DIK_UP) == true)
		{//一番上じゃなければ 選択を上へ
			if (pResult[0].nHightFont == 5)
			{//終わり, 平等に出るとき　それぞれ真上に合わせる
				if		(pResult[0].nWideFont == 0)	{ pResult[0].nWideFont = 5; }
				else if (pResult[0].nWideFont == 1)	{ pResult[0].nWideFont = 7; }
				else								{ pResult[0].nWideFont = 9; }
			}
			pResult[0].nHightFont--;
			if (pResult[0].nHightFont == -1)
			{
				if (pResult[0].nWideFont > 8)		{ pResult[0].nWideFont = 2; }	//終わりに合わせる
				else if (pResult[0].nWideFont > 5)	{ pResult[0].nWideFont = 1; }	//カナに合わせる
				else								{ pResult[0].nWideFont = 0; }	//ひらに合わせる
				pResult[0].nHightFont = 5;
			}
		}
		if (GetKeyboardTrigger(DIK_DOWN) == true)
		{//一番下じゃなければ 選択を下へ
			if (pResult[0].nHightFont == 4)
			{//終わり, 平等に入るとき
				if		(pResult[0].nWideFont > 8)	{ pResult[0].nWideFont = 2; }	//終わりに合わせる
				else if (pResult[0].nWideFont > 5)	{ pResult[0].nWideFont = 1; }	//カナに合わせる
				else								{ pResult[0].nWideFont = 0; }	//ひらに合わせる
			}
			pResult[0].nHightFont++;
			if (pResult[0].nHightFont == 6)
			{//終わり, 平等に出るとき　それぞれ真上に合わせる
				if (pResult[0].nWideFont == 0)		{ pResult[0].nWideFont = 5; }
				else if (pResult[0].nWideFont == 1) { pResult[0].nWideFont = 7; }
				else								{ pResult[0].nWideFont = 9; }
				pResult[0].nHightFont = 0;
			}
		}
		if (GetKeyboardTrigger(DIK_LEFT) == true)
		{//一番左じゃなければ 選択を左へ
			pResult[0].nWideFont--;
			if (pResult[0].nHightFont == 5 && pResult[0].nWideFont == -1) { pResult[0].nWideFont = 2; }
			else if (pResult[0].nWideFont == -1) { pResult[0].nWideFont = 10; }
		}
		if (GetKeyboardTrigger(DIK_RIGHT) == true)
		{//一番右じゃなければ 選択を右へ
			pResult[0].nWideFont++;
			if (pResult[0].nHightFont == 5 && pResult[0].nWideFont == 3) { pResult[0].nWideFont = 0; }
			else if (pResult[0].nWideFont == 11) { pResult[0].nWideFont = 0; }
		}
		if (GetKeyboardTrigger(DIK_LCONTROL) == true)
		{//終わりに合わせる
			pResult[0].nWideFont = 2;
			pResult[0].nHightFont = 5;
		}
		//枠の更新
		FontWindowMove(pResult[0].nWideFont, pResult[0].nHightFont);

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true)
		{//選択に応じた処理
			if (pResult[0].nHightFont == 5 && pResult[0].nWideFont == 2)
			{//終了
				PlaySound(SOUND_LABEL_BOTTON000);
				nLength = (int)strlen(&pResult[0].aStr[0]);		//文字数確認
				if(nLength != 0) {strcpy(&pScore[pResult[0].nRankin].aStr[0], &pResult[0].aStr[0]);}
				nSetFont[0] = 0;
				BackWindow(1);
				BackGameWindow(1);
				pResult[0].nCntResult++;
				//フォントの張り直しと入力用の削除
				ChangeFont(false, "", 5, FONTSTATE_NONE);
				ChangeFont(false, "", 6, FONTSTATE_NONE);
				ChangeFont(false, "", 7, FONTSTATE_NONE);
				ChangeFont(false, "", 8, FONTSTATE_NONE);
				ChangeFont(false, "", 9, FONTSTATE_NONE);
				SetFont(RECT{ 700 , 195, 1200, 300 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5].aStr[0], 0, FONTSTATE_NONE);
				SetFont(RECT{ 700 , 305, 1200, 410 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5 + 1].aStr[0], 1, FONTSTATE_NONE);
				SetFont(RECT{ 700 , 415, 1200, 520 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5 + 2].aStr[0], 2, FONTSTATE_NONE);
				SetFont(RECT{ 700 , 525, 1200, 630 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5 + 3].aStr[0], 3, FONTSTATE_NONE);
				SetFont(RECT{ 700 , 635, 1200, 740 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5 + 4].aStr[0], 4, FONTSTATE_NONE);
			}
			else if (pResult[0].nHightFont == 5 && pResult[0].nWideFont == 1)
			{
				nSetFont[0] = 2;
				PlaySound(SOUND_LABEL_BOTTON001);
			}	//カタカナに
			else if (pResult[0].nHightFont == 5 && pResult[0].nWideFont == 0)
			{
				nSetFont[0] = 1;
				PlaySound(SOUND_LABEL_BOTTON001);
			}	//ひらがなに
			else
			{ 
				nLength = (int)strlen(&pResult[0].aStr[0]);		//文字数確認
				if (nLength < 23) { InputName(nSetFont[0]); }	//文字数が余っているなら
				else
				{//文字数が最大
					if		(pResult[0].nHightFont == 1 || pResult[0].nHightFont == 3 && pResult[0].nWideFont == 3) { InputName(nSetFont[0]); }	//変換なら
					else if (pResult[0].nHightFont == 4 && pResult[0].nWideFont == 0) { InputName(nSetFont[0]); }	//変換なら
					else { PlaySound(SOUND_LABEL_BOTTON003); }//エラー音
				}
			}
		}
		if (GetKeyboardTrigger(DIK_BACK) == true || GetKeyboardTrigger(DIK_X) == true)
		{//ひとつ文字を消す
			PlaySound(SOUND_LABEL_BOTTON003);
			nLength = (int)strlen(&pResult[0].aStr[0]);//文字数確認
			if (nLength > 1)
			{ //文字が入っていれば
				strcpy(&pResult[0].aStr[nLength - 2], "\0");
				ChangeFont(true, &pResult[0].aStr[0], 5, FONTSTATE_NONE);
				nLength = (int)strlen(&pResult[0].aStr[0]);//文字数確認
				if (nLength == 0)
				{ 
					ChangeFont(false, &pResult[0].aStr[0], 5, FONTSTATE_NONE);
					SetFont(RECT{ 450 , 100, 1200, 200 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pResult[0].aStr[0], 5, FONTSTATE_NONE);
				}
				
				//削除音
			}
			else {}//削除失敗音
		}
	}
	else if (pResult[0].nCntScore > 4 && pResult[0].nCntResult > 0)
	{//名前入力処理が終了 || ハイスコアが出ていない Titleへ
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true)
		{
			SetFade(MODE_TITLE, 0);
		}
	}
	else
	{//時間カウント
		pResult[0].nCntResult++;
	}

	UpdateFont();
}
//=========================================================================================================//
// * ゲームの描画処理
//=========================================================================================================//
void DrawResult(void)
{
	DrawExplosion();
	DrawGameWindow();
	DrawScore();
	DrawFont();
}
//=========================================================================================================//
// * セーブロードのため
//=========================================================================================================//
Result *GetResult(void)
{
	static Result result;

	return &result;
}
//=========================================================================================================//
// * セーブロードのため
//=========================================================================================================//
SCORE *GetScore(void)
{
	static SCORE score[MAX_SCORE];
	//int nCntScore;

	//for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	//{//ランキングの初期化
	//	score[nCntScore].bUse = false;
	//	score[nCntScore].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	//	score[nCntScore].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//	score[nCntScore].state = SCORESTATE_NONE;
	//	strcpy(&score[nCntScore].aStr[0], "名無しのプログラマー");	//名前代入(仮)

	//	if (nCntScore < 5)
	//	{//EASY
	//		score[nCntScore].g_nScore = 0;
	//	}
	//	else if (nCntScore < 10)
	//	{//NORMAL
	//		score[nCntScore].g_nScore = 0;
	//	}
	//	else if (nCntScore < 15)
	//	{//HARD
	//		score[nCntScore].g_nScore = 0;
	//	}
	//	else if (nCntScore < 20)
	//	{//ルナティック
	//		score[nCntScore].g_nScore = 0;
	//	}
	//}

	return &score[0];
}
//=========================================================================================================//
// * 名前入力の選択に応じた処理
//=========================================================================================================//
void InputName(int nMode)
{
	Result *pResult = GetResult();

	PlaySound(SOUND_LABEL_BOTTON001);
	if (nMode == 1)
	{//ひら
		if (pResult[0].nWideFont == 0)
		{//ん行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ん"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "！"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "？"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "　"); }
			if (pResult[0].nHightFont == 4) { InputName3(); }
		}
		else if (pResult[0].nWideFont == 1)
		{//わ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "わ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "ー"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "‐"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "～"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "を"); }
		}
		else if (pResult[0].nWideFont == 2)
		{//ら行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ら"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "り"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "る"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "れ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "ろ"); }
		}
		else if (pResult[0].nWideFont == 3)
		{//や行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "や"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ゆ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "よ"); }
			if (pResult[0].nHightFont == 1) { InputName1(); }
			if (pResult[0].nHightFont == 3) { InputName2(); }
		}
		else if (pResult[0].nWideFont == 4)
		{//ま行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ま"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "み"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "む"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "め"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "も"); }
		}
		else if (pResult[0].nWideFont == 5)
		{//は行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "は"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "ひ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ふ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "へ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "ほ"); }
		}
		else if (pResult[0].nWideFont == 6)
		{//な行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "な"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "に"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ぬ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "ね"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "の"); }
		}
		else if (pResult[0].nWideFont == 7)
		{//た行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "た"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "ち"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "つ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "て"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "と"); }
		}
		else if (pResult[0].nWideFont == 8)
		{//さ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "さ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "し"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "す"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "せ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "そ"); }
		}
		else if (pResult[0].nWideFont == 9)
		{//か行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "か"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "き"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "く"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "け"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "こ"); }
		}
		else if (pResult[0].nWideFont == 10)
		{//あ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "あ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "い"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "う"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "え"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "お"); }
		}
	}
	else if (nMode == 2)//---------------------------------------------------------------------//
	{//カナ
		if (pResult[0].nWideFont == 0)
		{//ン行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ン"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "！"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "？"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "　"); }
			if (pResult[0].nHightFont == 4) { InputName3(); }
		}
		else if (pResult[0].nWideFont == 1)
		{//ワ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ワ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "ー"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "‐"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "～"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "ヲ"); }
		}
		else if (pResult[0].nWideFont == 2)
		{//ラ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ラ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "リ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ル"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "レ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "ロ"); }
		}
		else if (pResult[0].nWideFont == 3)
		{//ヤ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ヤ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ユ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "ヨ"); }
			if (pResult[0].nHightFont == 1) { InputName1(); }
			if (pResult[0].nHightFont == 3) { InputName2(); }
		}
		else if (pResult[0].nWideFont == 4)
		{//マ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "マ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "ミ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ム"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "メ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "モ"); }
		}
		else if (pResult[0].nWideFont == 5)
		{//ハ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ハ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "ヒ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "フ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "ヘ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "ホ"); }
		}
		else if (pResult[0].nWideFont == 6)
		{//ナ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ナ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "ニ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ヌ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "ネ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "ノ"); }
		}
		else if (pResult[0].nWideFont == 7)
		{//タ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "タ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "チ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ツ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "テ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "ト"); }
		}
		else if (pResult[0].nWideFont == 8)
		{//サ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "サ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "シ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ス"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "セ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "ソ"); }
		}
		else if (pResult[0].nWideFont == 9)
		{//カ行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "カ"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "キ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ク"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "ケ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "コ"); }
		}
		else if (pResult[0].nWideFont == 10)
		{//ア行
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "ア"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "イ"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "ウ"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "エ"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "オ"); }
		}
	}

	//文字をセットする
	ChangeFont(true, &pResult[0].aStr[0], 5, FONTSTATE_NONE);
}
//=========================================================================================================//
// * 名前入力の選択に応じた処理　〃変換
//=========================================================================================================//
void InputName1(void)
{
	Result *pResult = GetResult();
	int nLength;
	nLength = (int)strlen(&pResult[0].aStr[0]);//文字数確認

	if (nLength > 1)
	{//〃に変換
		if (strcmp(&pResult[0].aStr[nLength - 2], "う") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ヴ"); }
		//か行
		else if	(strcmp(&pResult[0].aStr[nLength - 2], "か") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "が"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "き") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぎ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "く") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぐ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "け") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "げ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "こ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ご"); }
		//さ行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "さ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ざ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "し") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "じ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "す") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ず"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "せ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぜ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "そ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぞ"); }
		//た行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "た") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "だ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ち") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぢ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "つ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "づ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "て") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "で"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "と") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ど"); }
		//は行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "は") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ば"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ひ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "び"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ふ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぶ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "へ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "べ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ほ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぼ"); }

		//カ行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ウ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ヴ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "カ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ガ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "キ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ギ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ク") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "グ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ケ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ゲ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "コ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ゴ"); }
		//サ行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "サ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ザ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "シ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ジ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ス") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ズ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "セ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ゼ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ソ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ゾ"); }
		//タ行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "タ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ダ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "チ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ヂ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ツ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ヅ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "テ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "デ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ト") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ド"); }
		//ハ行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ハ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "バ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ヒ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ビ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "フ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ブ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ヘ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ベ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ホ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ボ"); }
	}
	else {}//失敗音
}
//=========================================================================================================//
// * 名前入力の選択に応じた処理　゜変換
//=========================================================================================================//
void InputName2(void)
{
	Result *pResult = GetResult();
	int nLength;
	nLength = (int)strlen(&pResult[0].aStr[0]);//文字数確認

	if (nLength > 1)
	{//〃に変換
	 //か行
		//は行
		if		(strcmp(&pResult[0].aStr[nLength - 2], "は") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぱ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ひ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぴ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ふ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぷ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "へ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぺ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ほ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぽ"); }
		//ハ行
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ハ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "パ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ヒ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ピ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "フ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "プ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ヘ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぺ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ホ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ポ"); }
	}
	else {}//失敗音
}
//=========================================================================================================//
// * 名前入力の選択に応じた処理　小文字変換
//=========================================================================================================//
void InputName3(void)
{
	Result *pResult = GetResult();
	int nLength;
	nLength = (int)strlen(&pResult[0].aStr[0]);//文字数確認

	if (nLength > 1)
	{//小文字に変換
		if		(strcmp(&pResult[0].aStr[nLength - 2], "あ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぁ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "い") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぃ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "う") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぅ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "え") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぇ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "お") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ぉ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "つ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "っ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "や") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ゃ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ゆ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ゅ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "よ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ょ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "か") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ヵ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "け") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ヶ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "わ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ゎ"); }

		//カタカナ
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ア") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ァ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "イ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ィ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ウ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ゥ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "エ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ェ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "オ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ォ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ツ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ッ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ヤ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ャ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ユ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ュ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ヨ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ョ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "カ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ヵ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ケ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ヶ"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "ワ") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "ヮ"); }
	}
	else {}//失敗音
}