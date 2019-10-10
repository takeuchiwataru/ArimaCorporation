//*****************************************************************************
//
//		テクスチャの処理[texture.h]
//		Auther : shun yokomichi
//
//*****************************************************************************
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene2D.h"	// 2Dポリゴン

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
//		マクロ定義
//*****************************************************************************

//*****************************************************************************
//		クラス定義(ポリゴン)
//*****************************************************************************
class CTexture
{
public:
	typedef enum
	{
		//================
		// ゲーム内
		//================
		TYPE_GEARCHANGE = 0,	// ギア切替
		TYPE_GAMETIMELOGO,		// 制限時間(ロゴ)
		TYPE_TOTALSCORELOGO,	// トータルスコア(ロゴ)
		TYPE_SCORELOGO,			// スコア(ロゴ)
		TYPE_FREAME,			// スコア関係のフレーム
		TYPE_MONEY_TEN,			// \マーク & .マーク
		TYPE_PLUS_MARK,			// +のマーク
		TYPE_TIME_UP,			// タイムアップのロゴ
		TYPE_BOARDING,			// 乗車のロゴ

		//================
		// コンボ関係
		//================
		TYPE_COMBO,				// コンボ(ロゴ)
		TYPE_THROUGH,			// コンボ000
		TYPE_JUMP,				// コンボ001
		TYPE_DRIFT,				// コンボ002

		//=================
		// 満足度
		//=================
		TYPE_SATISFACTION,		// 満足度の星
		TYPE_COMMENT_FREAME,	// コメントのフレーム
		TYPE_IPHONE,			// iPhone
		TYPE_COMMENT000,		// コメント000
		TYPE_COMMENT001,		// コメント001
		TYPE_COMMENT002,		// コメント002
		TYPE_COMMENT003,		// コメント003
		TYPE_COMMENT004,		// コメント004
		TYPE_COMMENT005,		// コメント005
		TYPE_GOOD,				// スマイル
		TYPE_BAD,				// ダウン

		//=================
		// 目的地写真
		//=================
		TYPE_PLACE_SAPPRO,		// 札幌
		TYPE_PLACE_MAIN_STREET_OR_TV_TOWER,	// 大通り or テレビ塔
		TYPE_PLACE_CLOCKTOWER,	// 時計台
		TYPE_PLACE_TANUKIKOJI,	// 狸小路
		TYPE_PLACE_NORVESA,		// ノルベサ
		TYPE_PLACE_RED_BRICK,	// 赤レンガ
		TYPE_PLACE_SUSUKI_SRREET,// すすきの通り
		TYPE_PLACE_FOUNTAIN,	// 噴水
		TYPE_PLACE_TAPIOCAR,	// タピオカ―
		TYPE_PLACE_HOSPITAL,	// 病院

		//=================
		// 目的地名
		//=================
		TYPE_NAME_SAPPRO,		// 札幌
		TYPE_NAME_MAIN_STREET,	// 大通り
		TYPE_NAME_MAIN_TV_TOWER,// テレビ塔
		TYPE_NAME_CLOCKTOWER,	// 時計台
		TYPE_NAME_TANUKIKOJI,	// 狸小路
		TYPE_NAME_NORVESA,		// ノルベサ
		TYPE_NAME_RED_BRICK,	// 赤レンガ
		TYPE_NAME_SUSUKI_SRREET,// すすきの通り
		TYPE_NAME_FOUNTAIN,		// 噴水
		TYPE_NAME_TAPIOCAR,		// タピオカ―
		TYPE_NAME_HOSPITAL,		// 病院

		//=================
		// 命令ロゴ
		//=================
		TYPE_COMMAND_000,		// マーク内で車を止めろ
		TYPE_COMMAND_001,		// ブレーキ

		//=================
		// 数字
		//=================
		TYPE_TIME_NUMBER,		// 時間の数字
		TYPE_SCORE_NUMBER,		// スコアの数字

		//=================
		// 2D背景
		//=================
		TYPE_BACKGROUND000,		// タイトル画面背景
		TYPE_BACKGROUND001,		// セレクト or ランキング画面背景

		//=================
		// タイトル
		//=================
		TYPE_TITLELOGO000,		// タイトルのロゴ(CrazyTaxi)
		TYPE_TITLELOGO001,		// タイトルのロゴ(in Japan)
		TYPE_TITLE_PRESS,		// Press Start Buttonのロゴ
		TYPE_SEASONEFFECT,		// 四季のエフェクト用

		//=================
		// リザルト
		//=================
		TYPE_RESLUT_LOGO000,	// リザルトのロゴ(RESULT)
		TYPE_RESLUT_LOGO001,	// リザルトのロゴ(トータルスコア表示)
		TYPE_RESULT_LOGO002,	// リザルトのロゴ(満足度の表示)
		TYPE_RESULT_BUTTON,		// リザルトのロゴ(決定ボタン)

		//=================
		// ランキング
		//=================
		TYPE_RANKING_LOGO000,	// ランキングのロゴ(Ranking)
		TYPE_RANKING_RANK,		// 順位のロゴ
		TYPE_RANKING_CLASS,		// クラスのロゴ
		TYPE_RANKING_TOTALLOGO,	// トータルスコアのロゴ

		//=================
		// セレクト
		//=================
		TYPE_SELECT_LOGO,		// セレクトのロゴ
		TYPE_SELECT_CIRCLE,		// セレクト時のサークル
		TYPE_SELECT_HIGH_SCORE,	// ハイスコア
		TYPE_SELECT_GAME,		// ゲーム画面の画像
		TYPE_SELECT_QUIT,		// タイトルにもどる
		TYPE_SELECT_BUTTON,		// Prsee Button
		TYPE_SELECT_HUMAN000,
		TYPE_SELECT_HUMAN001,
		TYPE_SELECT_HUMAN002,
		TYPE_SELECT_HUMAN003,
		TYPE_SELECT_EASY,
		TYPE_SELECT_HARD,
		TYPE_SELECT_BACK,


		//=================
		// ポーズ
		//=================
		TYPE_PAUSE_ICON,		// 選択時アイコン
		TYPE_PAUSE_PRESS_BUTTON,// PRESS BUTTON
		TYPE_PAUSE_BG,			// ポーズ時の背景
		TYPE_PAUSE_CONTINUE,	// コンティニュー
		TYPE_PAUSE_RETRY,		// リトライ
		TYPE_PAUSE_QUIT,		// タイトルにもどる
		TYPE_PAUSE_LOGO,		// ポーズのロゴ
		TYPE_PAUSE_OPERATION,	// 操作方法

		//=================
		// チュートリアル
		//=================
		TYPE_TUTORIAL_FLAM,				// フレーム
		TYPE_TUTORIAL_LOGO,				// チュートリアルロゴ
		TYPE_TUTORIAL_SKIP_KEYBOAD,		// スキップキー(キーボード)
		TYPE_TUTORIAL_SKIP_PAD,			// スキップキー(ゲームパッド)
		TYPE_TUTORIAL_ICON,				// アイコン
		TYPE_TUTORIAL_ENTER_KEYBOAD,	// エンターキー(キーボード)
		TYPE_TUTORIAL_ENTER_PAD,		// エンターキー(ゲームパッド)
		TYPE_TUTORIAL_STOP,				// ストップ画像
		TYPE_TUTORIAL_GO,				// 目的の画像
		TYPE_TUTORIAL_R1,				// R１ボタン
		TYPE_TUTORIAL_L1,				// L1ボタン
		TYPE_TUTORIAL_A_PAD,			// Aボタン
		TYPE_TUTORIAL_B_PAD,			// Bボタン
		TYPE_TUTORIAL_LEFT,				// 左に倒す
		TYPE_TUTORIAL_RIGHT,			// 右に倒す
		TYPE_TUTORIAL_A_KEY,			// Aボタン
		TYPE_TUTORIAL_D_KEY,			// Dボタン
		TYPE_TUTORIAL_K_KEY,			// Kボタン
		TYPE_TUTORIAL_L_KEY,			// Lボタン
		TYPE_TUTORIAL_S_KEY,			// Sボタン
		TYPE_TUTORIAL_W_KEY,			// Wボタン

		//=================
		// スタート合図
		//=================
		TYPE_START_SIGNAL000,	// スタート合図(HERE)
		TYPE_START_SIGNAL001,	// スタート合図(WE)
		TYPE_START_SIGNAL002,	// スタート合図(GO!)

		//=================
		// フェードl
		//=================
		TYPE_FADE_NOWLOADING,
		TYPE_FADE_ANIM,

		//=================
		// その他
		//=================
		TYPE_BG_FREAM,			// 満足度やトータルスコアを見やすくするための枠
		TYPE_MAX				// 総数
	}TEXTYPE;

	CTexture();
	~CTexture();

	static HRESULT Load(void);	//	読み込む関数(テクスチャ)
	static void Unload(void);	// 開放する関数(テクスチャ)
	static LPDIRECT3DTEXTURE9 *GetTexture(int nIdx) { return &m_apTexture[nIdx]; }

private:
	// 静的メンバ変数
	static LPDIRECT3DTEXTURE9 *m_apTexture;
	static const char *m_apFilename[];
};
#endif