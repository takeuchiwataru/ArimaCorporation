//=============================================================================
//
// 木のエフェクト処理 [3Dparticle.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "woodeffect.h"
#include "player.h"
#include "game.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SIZE	(D3DXVECTOR3(5.0f, 0.0f, 5.0f))
#define LIFE	(60 * 8)							//表示時間
#define LENGTH	(3000.0f)							//距離

//=============================================================================
// 生成処理
//=============================================================================
CWoodEffect * CWoodEffect::Create(D3DXVECTOR3 Pos)
{
	CWoodEffect * pWoodEffect = new CWoodEffect;

	pWoodEffect->Init();			//初期化処理
	pWoodEffect->Set(Pos, SIZE);	//位置の設定

	return pWoodEffect;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CWoodEffect::CWoodEffect(){}

//=============================================================================
// デストラクタ
//=============================================================================
CWoodEffect::~CWoodEffect(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CWoodEffect::Init(void)
{
	C3DParticle::Init();

	//変数の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle = (float)((rand() % 6) - 3); 
	m_fSpeed = (float)((rand() % 5) + 5) * 0.1f;
	m_move.x = sinf(m_fAngle);
	m_move.z = cosf(m_fAngle);
	m_fMoveRot = (float)((rand() % 3) + 3) * 0.01f;
	m_nLife = LIFE;
	m_fCol_a = 1.0f;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CWoodEffect::Uninit(void)
{
	C3DParticle::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CWoodEffect::Update(void)
{
	C3DParticle::Update();

	//描画状態を取得する
	bool bDraw = GetDraw();

	//位置の取得
	D3DXVECTOR3 pos = C3DParticle::GetPos();

	if (bDraw)
	{
		//プレイヤーとの距離を求める
		D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
		D3DXVECTOR3 Distance = pos - PlayerPos;
		float fLength = sqrtf((Distance.x * Distance.x) + (Distance.z * Distance.z));

		if (fLength > LENGTH)
		{ //一定距離以上なら
			bDraw = false;
			SetDraw(bDraw);
		}
	}

	//向きの更新
	D3DXVECTOR3 rot = C3DParticle::GetRot();
	rot.z += m_fMoveRot;
	rot.x += m_fMoveRot;
	rot.y += 0.01f;

	//位置の更新
	pos.y -= m_fSpeed;

	//移動量の設定
	pos.x += m_move.x;
	pos.z += m_move.z;

	//向き・位置の設定
	C3DParticle::SetRot(rot);
	C3DParticle::SetPos(pos);

	//表示時間を減らす
	m_nLife--;

	//表示時間が0以下で破棄
	if (m_nLife < 0) { Uninit(); }
}

//=============================================================================
// 描画処理
//=============================================================================
void CWoodEffect::Draw(void)
{
	C3DParticle::Draw();
}
