//**************************************************************
//
//		テクスチャの処理[texture.cpp]
//		Auther : Shun Yokomichi
//
//**************************************************************
#include "texture.h"
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー

//**************************************************************
//			マクロ定義
//**************************************************************

//**************************************************************
//		静的メンバ変数
//**************************************************************
LPDIRECT3DTEXTURE9 *CTexture::m_apTexture = NULL;		// Textureのポインタ
const char *CTexture::m_apFilename[TYPE_MAX] =
{
	//================
	// ゲーム内
	//================
	"data\\TEXTURE\\game\\GearChange\\Drive&Accelerator.jpg",
	"data\\TEXTURE\\game\\TimeLogo\\gametime.png",
	"data\\TEXTURE\\game\\Score\\totalscore.png",
	"data\\TEXTURE\\game\\Score\\Score.png",
	"data\\TEXTURE\\game\\Score\\Frame.png",
	"data\\TEXTURE\\money&ten.png",
	"data\\TEXTURE\\game\\Score\\plus.png",
	"data\\TEXTURE\\game\\TimeUp\\time_up.png",
	"data\\TEXTURE\\game\\空車乗車.png",
	//================
	// コンボ関係
	//================
	"data\\TEXTURE\\game\\Combo\\combo.png",
	"data\\Texture\\game\\Combo\\Crazy_Through.png",
	"data\\Texture\\game\\Combo\\Crazy_Jump.png",
	"data\\Texture\\game\\Combo\\Crazy_Drift.png",

	//=================
	// 満足度
	//=================
	"data\\TEXTURE\\game\\Satisfaction level\\Satisfaction_Level.png",
	"data\\TEXTURE\\game\\Satisfaction level\\Evaluation\\Evaluation_Frame.png",
	"data\\TEXTURE\\game\\Satisfaction level\\Evaluation\\スマホ.png",
	"data\\TEXTURE\\game\\Satisfaction level\\Evaluation\\comment_000.png",
	"data\\TEXTURE\\game\\Satisfaction level\\Evaluation\\comment_001.png",
	"data\\TEXTURE\\game\\Satisfaction level\\Evaluation\\comment_002.png",
	"data\\TEXTURE\\game\\Satisfaction level\\Evaluation\\comment_003.png",
	"data\\TEXTURE\\game\\Satisfaction level\\Evaluation\\comment_004.png",
	"data\\TEXTURE\\game\\Satisfaction level\\Evaluation\\comment_005.png",
	"data\\TEXTURE\\game\\Satisfaction level\\good.png",
	"data\\TEXTURE\\game\\Satisfaction level\\bad.png",

	//=================
	// 目的地写真
	//=================
	"data\\TEXTURE\\game\\Destination\\Place\\Sappro.jpg",
	"data\\TEXTURE\\game\\Destination\\Place\\Main Street & TV Tower.jpg",
	"data\\TEXTURE\\game\\Destination\\Place\\ClockTower.jpg",
	"data\\TEXTURE\\game\\Destination\\Place\\Tanukikoji.jpg",
	"data\\TEXTURE\\game\\Destination\\Place\\Norvesa.jpg",
	"data\\TEXTURE\\game\\Destination\\Place\\Red Brick.jpg",
	"data\\TEXTURE\\game\\Destination\\Place\\Susukino_Street.jpg",
	"data\\TEXTURE\\game\\Destination\\Place\\Fountain.jpg",
	"data\\TEXTURE\\game\\Destination\\Place\\Tapiocar.jpg",
	"data\\TEXTURE\\game\\Destination\\Place\\Hospital.jpg",

	//=================
	// 目的地名
	//=================
	"data\\TEXTURE\\game\\Destination\\logo\\Sappro.png",
	"data\\TEXTURE\\game\\Destination\\logo\\Main Street.png",
	"data\\TEXTURE\\game\\Destination\\logo\\TV Tower.png",
	"data\\TEXTURE\\game\\Destination\\logo\\ClockTower.png",
	"data\\TEXTURE\\game\\Destination\\logo\\Tanukikoji.png",
	"data\\TEXTURE\\game\\Destination\\logo\\Norvesa.png",
	"data\\TEXTURE\\game\\Destination\\logo\\Red Brick.png",
	"data\\TEXTURE\\game\\Destination\\logo\\Susukino Street.png",
	"data\\TEXTURE\\game\\Destination\\logo\\Fountain.png",
	"data\\TEXTURE\\game\\Destination\\logo\\Tapiocar.png",
	"data\\TEXTURE\\game\\Destination\\logo\\Hospital.png",

	//=================
	// 命令ロゴ
	//=================
	"data\\TEXTURE\\game\\Destination\\Instructions000.png",
	"data\\TEXTURE\\game\\Destination\\Instructions001.png",

	//=================
	// 数字
	//=================
	"data\\TEXTURE\\number\\number_time.png",
	"data\\TEXTURE\\number\\number_Score.png",

	//=================
	// 2D背景
	//=================
	"data\\TEXTURE\\Background_000.jpg",
	"data\\TEXTURE\\Background_001.jpg",

	//=================
	// タイトル
	//=================
	"data\\TEXTURE\\Title\\Titlelogo.png",
	"data\\TEXTURE\\Title\\inJapan.png",
	"data\\TEXTURE\\Title\\press_start_button.png",
	"data\\TEXTURE\\Title\\Four_seasons.png",

	//=================
	// リザルト
	//=================
	"data\\TEXTURE\\result\\result_logo.jpg",
	"data\\TEXTURE\\result\\totalearned_logo.png",
	"data\\TEXTURE\\result\\class_logo.png",
	"data\\TEXTURE\\result\\press_key.jpg",

	//=================
	// ランキング
	//=================
	"data\\TEXTURE\\Ranking\\Ranking_logo.png",
	"data\\TEXTURE\\Ranking\\rank.png",
	"data\\TEXTURE\\Ranking\\class_logo_ranking.jpg",
	"data\\TEXTURE\\Ranking\\totalearned_logo_ranking.jpg",

	//=================
	// セレクト
	//=================
	"data\\TEXTURE\\Select\\logo.png",
	"data\\TEXTURE\\Select\\SelectCircle.png",
	"data\\TEXTURE\\Select\\totalearned_logo_ranking.png",
	"data\\TEXTURE\\Select\\Map.jpg",
	"data\\TEXTURE\\Select\\pause002.png",
	"data\\TEXTURE\\Select\\press_key001.jpg",
	"data\\TEXTURE\\Select\\図4.png",
	"data\\TEXTURE\\Select\\図5.png",
	"data\\TEXTURE\\Select\\図6.png",
	"data\\TEXTURE\\Select\\図7.png",
	"data\\TEXTURE\\Select\\flam_Select_000.png",
	"data\\TEXTURE\\Select\\flam_Select_001.png",
	"data\\TEXTURE\\Select\\flam_back.png",

	//=================
	// ポーズ
	//=================
	"data\\TEXTURE\\pause\\icon.png",
	"data\\TEXTURE\\pause\\press_key002.jpg",
	"data\\TEXTURE\\pause\\pause.png",
	"data\\TEXTURE\\pause\\pause000.png",
	"data\\TEXTURE\\pause\\pause001.png",
	"data\\TEXTURE\\pause\\pause002.png",
	"data\\TEXTURE\\pause\\logo.png",
	"data\\TEXTURE\\pause\\gradation003.jpg",

	//=================
	// チュートリアル
	//=================
	"data\\TEXTURE\\Tutorial\\flam.png",
	"data\\TEXTURE\\Tutorial\\logo.png",
	"data\\TEXTURE\\Tutorial\\Keyboard\\SkipSpace.png",
	"data\\TEXTURE\\Tutorial\\gamepad\\SkipStart.png",
	"data\\TEXTURE\\Tutorial\\icon.jpg",
	"data\\TEXTURE\\Tutorial\\Keyboard\\PressEnter.png",
	"data\\TEXTURE\\Tutorial\\gamepad\\PressX.png",
	"data\\TEXTURE\\Tutorial\\Stop.png",
	"data\\TEXTURE\\Tutorial\\Go.png",
	"data\\TEXTURE\\Tutorial\\gamepad\\Right_Pad.png",
	"data\\TEXTURE\\Tutorial\\gamepad\\Left_Pad.png",
	"data\\TEXTURE\\Tutorial\\gamepad\\A_pad.png",
	"data\\TEXTURE\\Tutorial\\gamepad\\B_Pad.png",
	"data\\TEXTURE\\Tutorial\\gamepad\\Left.png",
	"data\\TEXTURE\\Tutorial\\gamepad\\Right.png",
	"data\\TEXTURE\\Tutorial\\Keyboard\\Aボタン.png",
	"data\\TEXTURE\\Tutorial\\Keyboard\\Dボタン.png",
	"data\\TEXTURE\\Tutorial\\Keyboard\\Kボタン.png",
	"data\\TEXTURE\\Tutorial\\Keyboard\\Lボタン.png",
	"data\\TEXTURE\\Tutorial\\Keyboard\\Sボタン.png",
	"data\\TEXTURE\\Tutorial\\Keyboard\\Wボタン.png",

	//=================
	// スタート合図
	//=================
	"data\\TEXTURE\\game\\start\\HERE.png",
	"data\\TEXTURE\\game\\start\\WE.png",
	"data\\TEXTURE\\game\\start\\GO.png",

	//=================
	// フェード
	//=================
	"data\\TEXTURE\\Fade\\NowLoading.png",
	"data\\TEXTURE\\Fade\\LoadingAnim.png",

	//=================
	// その他
	//=================
	"data\\TEXTURE\\bg_fream.png"
};

//**************************************************************
//		コンストラクタ
//**************************************************************
CTexture::CTexture() {}

//**************************************************************
//		デストラクタ
//**************************************************************
CTexture::~CTexture() {}

//**************************************************************
//		テクスチャの読み込み
//**************************************************************
HRESULT CTexture::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	m_apTexture = new LPDIRECT3DTEXTURE9[TYPE_MAX];

	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, m_apFilename[nCntTex], &m_apTexture[nCntTex]);
	}
	return S_OK;
}

//**************************************************************
//		テクスチャの開放
//**************************************************************
void CTexture::Unload(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{	// NULL以外なら
			m_apTexture[nCntTex]->Release();		// 破棄
			m_apTexture[nCntTex] = NULL;			// NULLに
		}
	}

	if (m_apTexture != NULL)
	{
		delete m_apTexture;
		m_apTexture = NULL;
	}
}