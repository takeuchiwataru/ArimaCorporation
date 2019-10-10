//=============================================================================
//
// メッシュの処理 [mesh.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "scene.h"
#include "mesh.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXT_MESHNAME			"data\\TEXT\\mesh.txt"					// 読み込むテキストファイル

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CMesh::CMesh() : CScene(MESH_PRIOTITY, CScene::OBJTYPE_MESH)
{
	m_pTexture = NULL;								// テクスチャへのポインタ
	m_pVtxBuff = NULL;								// 頂点バッファへのポインタ
	m_pIdxBuff = NULL;								// インデックスへのポインタ
	m_fMeshTexX = 1.0f;
	m_fMeshTexY = 1.0f;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	m_nType = 0;									// 種類
	m_fVtx0 = 0.0f;
	m_fVtxMeshX = 0.0f;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CMesh::~CMesh()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMesh::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//変数宣言
	int nCntMeshZ;
	int nCntMeshX;
	int nCntVertical;
	int nCntCorss;
	int nCntIdx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((m_nMeshX + 1) * (m_nMeshZ + 1)),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((m_nMeshX * m_nMeshZ) * 2 + (4 * (m_nMeshZ - 1)) + 2),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16ビットのデータを確保
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_nType)
	{
		//=======================
		//	メッシュフィールド
		//=======================
	case 0:
		//頂点、法線、カラー、テクスチャの頂点設定
		for (nCntMeshZ = 0; nCntMeshZ <= m_nMeshZ; nCntMeshZ++)
		{//縦軸
			for (nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++)
			{//横軸
				// 頂点の座標
				pVtx[0].pos = D3DXVECTOR3(((nCntMeshX * m_fMeshWidth) / m_nMeshX), 0.0f/*rand() % 100 * 1.0f*/, ((nCntMeshZ * -m_fMeshDepth) / m_nMeshZ));

				// 法線の設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(nCntMeshX * m_fMeshTexX, nCntMeshZ * m_fMeshTexY);

				//頂点を変更
				if (nCntMeshZ == 0 && nCntMeshX == 0)
				{//左上
					pVtx[0].pos.y = m_fVtx0;
				}
				else if (nCntMeshX > 0 && nCntMeshZ == 0 && nCntMeshX == m_nMeshX)
				{//右上
					pVtx[0].pos.y = m_fVtxMeshX;
				}
				//頂点を変更
				else if (nCntMeshZ == nCntMeshZ && nCntMeshX == 0)
				{//左下
					pVtx[0].pos.y = m_fVtxMeshZ;
				}
				else if (nCntMeshX > 0 && nCntMeshZ > 0 && nCntMeshZ == nCntMeshZ && nCntMeshX == m_nMeshX)
				{//右下
					pVtx[0].pos.y = m_fVtxMeshXMeshZ;
				}

				pVtx += 1;
			}
		}

		break;
		//=======================
		//	メッシュシリンダー
		//=======================
	case 1:
		//頂点、法線、カラー、テクスチャの頂点設定
		for (nCntVertical = 0; nCntVertical <= m_nMeshZ; nCntVertical++)
		{//縦軸
			for (nCntCorss = 0; nCntCorss < m_nMeshX + 1; nCntCorss++)
			{//横軸
				//pVtx[頂点番号].x,,z = sinf((3.14の円周　これに-をつけると、カリング方向が逆になる　* (カウントの回数　* 指定した数を計算して角度調整)))* 半径;
				pVtx[0].pos.x = sinf((-D3DX_PI * (nCntCorss * (1.0f / (m_nMeshX / 2)))))* 2000.0f;
				//pVtx[頂点番号].y = 縦のカウント回数　* 縦の幅
				pVtx[0].pos.y = nCntVertical * 700.0f;
				pVtx[0].pos.z = cosf((-D3DX_PI * (nCntCorss * (1.0f / (m_nMeshX / 2)))))* 2000.0f;

				//法線
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//テクスチャ
				pVtx[0].tex = D3DXVECTOR2(nCntCorss * 1.0f, nCntVertical * 1.0f);

				//ずらし
				pVtx += 1;
			}
		}

		break;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//メッシュフィールドのみ法線の計算をする
	if(m_nType != 1)
	{//法線の計算
		CMesh::Nor();
	}

	WORD *pIdx;		// インデックスデータへのポインタ

	// インデックスバッファをロックし、インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	switch (m_nType)
	{
		//=======================
		//	メッシュフィールド
		//=======================
	case 0:
		//縦のブロック
		for (nCntMeshZ = 0, nCntIdx = 0; nCntMeshZ < m_nMeshZ; nCntMeshZ++)
		{
			//横のブロック
			for (nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++, nCntIdx++)
			{
				//pIdx[番号] = (指定した横 + １（この１は０からスタートした時に１足りないから） + インデックスの回数)
				pIdx[0] = (m_nMeshX + 1) + nCntIdx;
				pIdx[1] = 0 + nCntIdx;

				pIdx += 2;

				if (nCntMeshZ < m_nMeshZ - 1 && nCntMeshX == m_nMeshX)
				{// 折り返し時
					pIdx[0] = 0 + nCntIdx;
					pIdx[1] = (m_nMeshX + 1) + nCntIdx + 1;

					pIdx += 2;
				}
			}
		}
		break;
		//=======================
		//	メッシュシリンダー
		//=======================
	case 1:
		//縦のブロック
		for (nCntVertical = 0, nCntIdx = 0; nCntVertical < m_nMeshZ; nCntVertical++)
		{
			//横のブロック
			for (nCntCorss = 0; nCntCorss <= m_nMeshX; nCntCorss++, nCntIdx++)
			{//横軸カウント
				//pIdx[番号] = (指定した横 + １（この１は０からスタートした時に１足りないから） + インデックスの回数)
				pIdx[0] = (m_nMeshX + 1) + nCntIdx;
				pIdx[1] = 0 + nCntIdx;

				//２頂点ずらし
				pIdx += 2;

				if (nCntVertical < m_nMeshZ - 1 && nCntCorss == m_nMeshX)
				{//指定した縦-1に行ったときと横が指定した数まで行ったとき
					pIdx[0] = 0 + nCntIdx;
					pIdx[1] = (m_nMeshX + 1) + nCntIdx + 1;

					//２頂点文ずらす
					pIdx += 2;
				}
			}
		}
		break;
	}

	// 頂点バッファをアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMesh::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// インデックスバッファの開放
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMesh::Update(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	if (pCInputKeyBoard->GetKeyboardPress(DIK_N) == true)
	{//矢印の上
		if (m_nType != 1)
		{//法線の計算
			CMesh::Nor();
		}
	}

	//CDebugProc::Print("m_fVtx0 : %.1f\n", m_fVtx0);
	//CDebugProc::Print("m_fVtxMeshX : %.1f\n", m_fVtxMeshX);
}
//=============================================================================
// 描画処理
//=============================================================================
void CMesh::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX	mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, ((m_nMeshX + 1) * (m_nMeshZ + 1)), 0, ((m_nMeshX * m_nMeshZ * 2) + (4 * (m_nMeshZ - 1))));

}
//===============================================================================
//　クリエイト
//===============================================================================
CMesh * CMesh::Create()
{
	CMesh *pMesh = NULL;

	//NULLチェック
	if (pMesh == NULL)
	{//メモリの動的確保

		pMesh = new CMesh;

		if (pMesh != NULL)
		{
			//オブジェクトクラスの生成
			pMesh->Init();
		}
	}

	return pMesh;
}
//===============================================================================
// 共有テクスチャの割り当て
//===============================================================================
void CMesh::BindTexture(LPDIRECT3DTEXTURE9 Texture)
{
	m_pTexture = Texture;
}
//===============================================================================
//　位置の代入
//===============================================================================
void CMesh::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//===============================================================================
//　メッシュの情報
//===============================================================================
void CMesh::SetMeshInfo(int nMeshX, int nMeshZ, float fMeshTexX, float fMeshTexY, float fMeshDepth, float fMeshWidth)
{
	m_nMeshX = nMeshX;			//分割数X
	m_nMeshZ = nMeshZ;			//分割数Z
	m_fMeshTexX = fMeshTexX;	//テクスチャX座標
	m_fMeshTexY = fMeshTexY;	//テクスチャY座標
	m_fMeshDepth = fMeshDepth;	//奥行
	m_fMeshWidth = fMeshWidth;	//横幅
}
//===============================================================================
//　法線
//===============================================================================
void CMesh::Nor(void)
{
	//変数宣言
	D3DXVECTOR3 vec0;
	D3DXVECTOR3 vec1;
	D3DXVECTOR3 nor;
	D3DXVECTOR3 VtxNor[2];

	int nCntMeshZ;
	int nCntMeshX;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//pVtxの初期化
	//縦のブロック
	for (nCntMeshZ = 0; nCntMeshZ < m_nMeshZ; nCntMeshZ++)
	{
		//横のブロック
		for (nCntMeshX = 0; nCntMeshX < m_nMeshX; nCntMeshX++)
		{
			pVtx[nCntMeshX + nCntMeshZ * (m_nMeshX + 1)].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		}
	}

	//縦のブロック
	for (nCntMeshZ = 0; nCntMeshZ < m_nMeshZ; nCntMeshZ++)
	{
		//横のブロック
		for (nCntMeshX = 0; nCntMeshX < m_nMeshX; nCntMeshX++)
		{
			//現在の乗っかっているブロックの頂点の出し方
			int	nMeshLU = nCntMeshX + nCntMeshZ * (m_nMeshX + 1);					//例　0番目の頂点
			int	nMeshRU = (nCntMeshX + 1) + nCntMeshZ * (m_nMeshX + 1);				//例　1番目の頂点
			int nMeshLD = nCntMeshX + (nCntMeshZ + 1) * (m_nMeshX + 1);				//例　3番目の頂点
			int nMeshRD = (nCntMeshX + 1) + (nCntMeshZ + 1) * (m_nMeshX + 1);		//例　4番目の頂点

			//======================
			//  1ポリゴンのnor
			//======================
			//左下のnor
			vec0 = pVtx[nMeshLU].pos - pVtx[nMeshLD].pos;
			vec1 = pVtx[nMeshRD].pos - pVtx[nMeshLD].pos;
			//外積計算
			D3DXVec3Cross(&VtxNor[0], &vec0, &vec1);
			D3DXVec3Normalize(&VtxNor[0], &VtxNor[0]);

			//======================
			//  ２ポリゴンのnor
			//======================
			//右上のnor
			vec0 = pVtx[nMeshRD].pos - pVtx[nMeshRU].pos;
			vec1 = pVtx[nMeshLU].pos - pVtx[nMeshRU].pos;
			//外積計算
			D3DXVec3Cross(&VtxNor[1], &vec0, &vec1);
			D3DXVec3Normalize(&VtxNor[1], &VtxNor[1]);

			pVtx[nMeshLD].nor += VtxNor[0];
			pVtx[nMeshLU].nor += VtxNor[0] + VtxNor[1];
			pVtx[nMeshRD].nor += VtxNor[0] + VtxNor[1];
			pVtx[nMeshRU].nor += VtxNor[1];
		}
	}

	//縦のブロック
	for (nCntMeshZ = 0; nCntMeshZ <= m_nMeshZ; nCntMeshZ++)
	{
		//横のブロック
		for (nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++)
		{
			if (nCntMeshZ == 0)
			{//縦の列
				if (nCntMeshX > 0 && nCntMeshX < m_nMeshX)
				{
					pVtx[nCntMeshX + nCntMeshZ * (m_nMeshX + 1)].nor /= 3.0f;
				}
				else if(nCntMeshX == 0)
				{
					pVtx[nCntMeshX + nCntMeshZ * (m_nMeshX + 1)].nor /= 2.0f;
				}
			}
			else if (nCntMeshZ == m_nMeshZ)
			{//下
				if (nCntMeshX > 0 && nCntMeshX < m_nMeshX)
				{
					pVtx[nCntMeshX + nCntMeshZ * (m_nMeshX + 1)].nor /= 3.0f;
				}
				else if (nCntMeshX == m_nMeshX)
				{
					pVtx[nCntMeshX + nCntMeshZ * (m_nMeshX + 1)].nor /= 2.0f;
				}
			}
			else
			{//横の列
				if (nCntMeshX > 0 && nCntMeshX < m_nMeshX)
				{
					pVtx[nCntMeshX + nCntMeshZ * (m_nMeshX + 1)].nor /= 6.0f;
				}
				else
				{
					pVtx[nCntMeshX + nCntMeshZ * (m_nMeshX + 1)].nor /= 3.0f;
				}
			}
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//===============================================================================
//　プレイヤーのpos.yを内積を利用して出す
//===============================================================================
bool CMesh::GetHeight(D3DXVECTOR3 &pos)
{
	//変数宣言
	D3DXVECTOR3 vec0;
	D3DXVECTOR3 vec1;
	D3DXVECTOR3 nor;
	D3DXVECTOR3 VtxPos[3];
	float		fHeight;

	//プレイヤーとメッシュフィールドの位置の差分を出す
	D3DXVECTOR3 posMtx = pos - m_pos;

	//現在プレイヤーが何ブロック目にいるかを確認する式
	int			nMeshX = ((int)(posMtx.x)) / ((int)(m_fMeshWidth / m_nMeshX));
	int			nMeshZ = ((int)(posMtx.z)) / ((int)(m_fMeshDepth / m_nMeshZ)) * -1;

	//現在の乗っかっているブロックの頂点の出し方
	int			nMeshLU = nMeshX + nMeshZ * (m_nMeshX + 1);					//例　0番目の頂点
	int			nMeshRU = (nMeshX + 1) + nMeshZ * (m_nMeshX + 1);			//例　1番目の頂点
	int			nMeshLD = nMeshX + (nMeshZ + 1) * (m_nMeshX + 1);			//例　3番目の頂点
	int			nMeshRD = (nMeshX + 1) + (nMeshZ + 1) * (m_nMeshX + 1);		//例　4番目の頂点

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//============================
	//		三角形
	//============================
	//ベクトル計算式 目的 - 現在
	vec0 = pVtx[nMeshLU].pos - pVtx[nMeshRD].pos;
	//ベクトル計算式 自分の位置　- 現在
	vec1 = posMtx - pVtx[nMeshRD].pos;

	if ((vec0.x * vec1.z) - (vec0.z * vec1.x) <= 0)
	{//三角形より右にいる場合
		//CDebugProc::Print("MeshField△右範囲 : TURE\n");
		vec0 = (posMtx - pVtx[nMeshLU].pos);

		//各頂点の保管
		VtxPos[0] = pVtx[nMeshRU].pos;
		VtxPos[1] = pVtx[nMeshRD].pos;
		VtxPos[2] = pVtx[nMeshLU].pos;
	}
	else if ((vec0.x * vec1.z) - (vec0.z * vec1.x) >= 0)
	{//三角形より左にいる場合
		//CDebugProc::Print("MeshField△左範囲 : TURE\n");
		vec0 = (posMtx - pVtx[nMeshRD].pos);

		//各頂点の保管
		VtxPos[0] = pVtx[nMeshLD].pos;
		VtxPos[1] = pVtx[nMeshLU].pos;
		VtxPos[2] = pVtx[nMeshRD].pos;
	}

	//頂点バッファのアンロックする
	m_pVtxBuff->Unlock();

	//引く法線を引く前に保存する
	fHeight = VtxPos[0].y;

	//3頂点に0の値をひいてあげて法線とプレイヤーのベクトルを直交させるため
	VtxPos[2].y -= VtxPos[0].y;
	VtxPos[1].y -= VtxPos[0].y;
	VtxPos[0].y -= VtxPos[0].y;

	//法線の計算
	vec0 = VtxPos[1] - VtxPos[0];

	vec1 = VtxPos[2] - VtxPos[0];

	//外積の計算
	D3DXVec3Cross(&nor, &vec0, &vec1);

	//正規化
	D3DXVec3Normalize(&nor, &nor);

	//プレイヤーへのベクトル
	vec0 = posMtx - VtxPos[0];

	//内積計算式
	vec0.y = (-(vec0.x * nor.x) - (vec0.z * nor.z)) / nor.y;

	//プレイヤーのyに入れる
	posMtx.y = vec0.y + fHeight + m_pos.y;

	if (pos.y < posMtx.y)
	{// プレイヤーを地面に乗せる
		pos.y = posMtx.y;
		return true;
	}
	return false;
}

//===============================================================================
//　プレイヤーのpos.yを内積を利用して出す
//===============================================================================
float CMesh::GetHeightMesh(D3DXVECTOR3 pos)
{
	//変数宣言
	D3DXVECTOR3 vec0;
	D3DXVECTOR3 vec1;
	D3DXVECTOR3 nor;
	D3DXVECTOR3 VtxPos[3];
	float		fHeight;

	//プレイヤーとメッシュフィールドの位置の差分を出す
	D3DXVECTOR3 posMtx = pos - m_pos;

	//現在プレイヤーが何ブロック目にいるかを確認する式
	int			nMeshX = ((int)(posMtx.x)) / ((int)(m_fMeshWidth / m_nMeshX));
	int			nMeshZ = ((int)(posMtx.z)) / ((int)(m_fMeshDepth / m_nMeshZ)) * -1;

	//現在の乗っかっているブロックの頂点の出し方
	int			nMeshLU = nMeshX + nMeshZ * (m_nMeshX + 1);					//例　0番目の頂点
	int			nMeshRU = (nMeshX + 1) + nMeshZ * (m_nMeshX + 1);			//例　1番目の頂点
	int			nMeshLD = nMeshX + (nMeshZ + 1) * (m_nMeshX + 1);			//例　3番目の頂点
	int			nMeshRD = (nMeshX + 1) + (nMeshZ + 1) * (m_nMeshX + 1);		//例　4番目の頂点

																			//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//============================
	//		三角形
	//============================
	//ベクトル計算式 目的 - 現在
	vec0 = pVtx[nMeshLU].pos - pVtx[nMeshRD].pos;
	//ベクトル計算式 自分の位置　- 現在
	vec1 = posMtx - pVtx[nMeshRD].pos;

	if ((vec0.x * vec1.z) - (vec0.z * vec1.x) <= 0)
	{//三角形より右にいる場合
	 //CDebugProc::Print("MeshField△右範囲 : TURE\n");
		vec0 = (posMtx - pVtx[nMeshLU].pos);

		//各頂点の保管
		VtxPos[0] = pVtx[nMeshRU].pos;
		VtxPos[1] = pVtx[nMeshRD].pos;
		VtxPos[2] = pVtx[nMeshLU].pos;
	}
	else if ((vec0.x * vec1.z) - (vec0.z * vec1.x) >= 0)
	{//三角形より左にいる場合
	 //CDebugProc::Print("MeshField△左範囲 : TURE\n");
		vec0 = (posMtx - pVtx[nMeshRD].pos);

		//各頂点の保管
		VtxPos[0] = pVtx[nMeshLD].pos;
		VtxPos[1] = pVtx[nMeshLU].pos;
		VtxPos[2] = pVtx[nMeshRD].pos;
	}

	//頂点バッファのアンロックする
	m_pVtxBuff->Unlock();

	//引く法線を引く前に保存する
	fHeight = VtxPos[0].y;

	//3頂点に0の値をひいてあげて法線とプレイヤーのベクトルを直交させるため
	VtxPos[2].y -= VtxPos[0].y;
	VtxPos[1].y -= VtxPos[0].y;
	VtxPos[0].y -= VtxPos[0].y;

	//法線の計算
	vec0 = VtxPos[1] - VtxPos[0];

	vec1 = VtxPos[2] - VtxPos[0];

	//外積の計算
	D3DXVec3Cross(&nor, &vec0, &vec1);

	//正規化
	D3DXVec3Normalize(&nor, &nor);

	//プレイヤーへのベクトル
	vec0 = posMtx - VtxPos[0];

	//内積計算式
	vec0.y = (-(vec0.x * nor.x) - (vec0.z * nor.z)) / nor.y;

	//プレイヤーのyに入れる
	posMtx.y = vec0.y + fHeight + m_pos.y;

	return posMtx.y;
}

//===============================================================================
//　プレイヤーのposを利用して、上下に上げ下げする
//===============================================================================
void CMesh::SetHeight(D3DXVECTOR3 pos, float fValue, float fRange)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fLength;

	//プレイヤーとメッシュフィールドの位置の差分を出す
	D3DXVECTOR3 posMtx = pos - m_pos;

	//縦のブロック
	for (int nCntMeshZ = 0; nCntMeshZ <= m_nMeshZ; nCntMeshZ++)
	{
		//横のブロック
		for (int nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++)
		{
			//posから対象の頂点までの距離
			fLength = powf(pVtx->pos.x - posMtx.x , 2) + powf(pVtx->pos.z - posMtx.z, 2);

			//対象の頂点が範囲内
			if (fLength <= powf(fRange,2))
			{
				//範囲内での距離の比率に応じた高さ
				float fHight = cosf((D3DX_PI / 2) / (fRange / sqrtf(fLength))) * fValue;

				pVtx->pos.y += fHight;
			}

			pVtx++;
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//===============================================================================
// ファイルにセーブ
//===============================================================================
void CMesh::TextSave(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//ファイルのポインタ
	FILE *pFile;

	//ファイル設定
	pFile = fopen(TEXT_MESHNAME, "wb");

	//プレイヤーのテキストデータの読み込み
	if (pFile != NULL)
	{
		//縦のブロック
		for (int nCntMeshZ = 0; nCntMeshZ <= m_nMeshZ; nCntMeshZ++)
		{
			//横のブロック
			for (int nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++)
			{
				//ファイルへ バイトのデータを書き込む
				fwrite(&pVtx[(nCntMeshZ + (m_nMeshX + 1) + nCntMeshX)].pos.y, sizeof(float), ((m_nMeshX + 1) * (m_nMeshZ + 1)), pFile);
			}
		}
		//ファイルを閉じる
		fclose(pFile);
	}
	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//===============================================================================
// ファイルにロード
//===============================================================================
void CMesh::TextLoad(void)
{
	//ファイルのポインタ
	FILE *pFile;

	//ファイル設定
	pFile = fopen(TEXT_MESHNAME, "rb");

	//プレイヤーのテキストデータの読み込み
	if (pFile != NULL)
	{
		//縦のブロック
		for (int nCntMeshZ = 0; nCntMeshZ <= m_nMeshZ; nCntMeshZ++)
		{
			//横のブロック
			for (int nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++)
			{

			}
		}
		//ファイルを閉じる
		fclose(pFile);
	}
}
//スキンメッシュ
//クォータニオン
//スプライン
//ステンシルシャドウ
//ステンシルミラー
//フィードバックエフェクト

//=========================================================================================================================
// 自分の向いている方向に対しての傾斜の角度の取得（上り下りの判定に必要）
//=========================================================================================================================
float CMesh::GetInclineAngle(D3DXVECTOR3 pos, float fRot)
{
	float fSpeed = 0.0f;	// 返す速度
	D3DXVECTOR3 vecPlayer;	// プレイヤーの向きのベクトル
	float fAngle;			// プレイヤーの向きと法線のなす角
	D3DXVECTOR3 vec0;
	D3DXVECTOR3 vec1;
	D3DXVECTOR3 VtxPos[3];
	D3DXVECTOR3 nor;

	//プレイヤーとメッシュフィールドの位置の差分を出す
	D3DXVECTOR3 posMtx = pos - m_pos;

	//現在プレイヤーが何ブロック目にいるかを確認する式
	int			nMeshX = ((int)(posMtx.x)) / ((int)(m_fMeshWidth / m_nMeshX));
	int			nMeshZ = ((int)(posMtx.z)) / ((int)(m_fMeshDepth / m_nMeshZ)) * -1;

	//現在の乗っかっているブロックの頂点の出し方
	int			nMeshLU = nMeshX + nMeshZ * (m_nMeshX + 1);					//例　0番目の頂点
	int			nMeshRU = (nMeshX + 1) + nMeshZ * (m_nMeshX + 1);			//例　1番目の頂点
	int			nMeshLD = nMeshX + (nMeshZ + 1) * (m_nMeshX + 1);			//例　3番目の頂点
	int			nMeshRD = (nMeshX + 1) + (nMeshZ + 1) * (m_nMeshX + 1);		//例　4番目の頂点

																			//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//============================
	//		三角形
	//============================
	//ベクトル計算式 目的 - 現在
	vec0 = pVtx[nMeshLU].pos - pVtx[nMeshRD].pos;
	//ベクトル計算式 自分の位置　- 現在
	vec1 = posMtx - pVtx[nMeshRD].pos;

	if ((vec0.x * vec1.z) - (vec0.z * vec1.x) <= 0)
	{//三角形より右にいる場合
	 //CDebugProc::Print("MeshField△右範囲 : TURE\n");
		vec0 = (posMtx - pVtx[nMeshLU].pos);

		//各頂点の保管
		VtxPos[0] = pVtx[nMeshRU].pos;
		VtxPos[1] = pVtx[nMeshRD].pos;
		VtxPos[2] = pVtx[nMeshLU].pos;
	}
	else if ((vec0.x * vec1.z) - (vec0.z * vec1.x) >= 0)
	{//三角形より左にいる場合
	 //CDebugProc::Print("MeshField△左範囲 : TURE\n");
		vec0 = (posMtx - pVtx[nMeshRD].pos);

		//各頂点の保管
		VtxPos[0] = pVtx[nMeshLD].pos;
		VtxPos[1] = pVtx[nMeshLU].pos;
		VtxPos[2] = pVtx[nMeshRD].pos;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	// プレイヤーのいるポリゴンの法線を出す
	vec0 = VtxPos[1] - VtxPos[0];			// 左側
	vec1 = VtxPos[2] - VtxPos[0];			// 右側
	D3DXVec3Cross(&nor, &vec0, &vec1);			// 外積を求める
	D3DXVec3Normalize(&nor, &nor);	// 正規化

									// プレイヤーの向きのベクトルの生成
	vecPlayer = D3DXVECTOR3(pos.x + sinf(fRot), pos.y, pos.z + cosf(fRot)) - pos;

	// プレイヤーの向きと法線のなす角を求める
	fAngle = (vecPlayer.x * nor.x) + (vecPlayer.y * nor.y) + (vecPlayer.z * nor.z);
	fAngle = fAngle / (sqrtf(powf(vecPlayer.x, 2) + powf(vecPlayer.y, 2) + powf(vecPlayer.z, 2)) * (sqrtf(powf(nor.x, 2) + powf(nor.y, 2) + powf(nor.z, 2))));

	return fAngle;
}

//=========================================================================================================================
// 法線の取得
//=========================================================================================================================
D3DXVECTOR3 CMesh::GetNor(D3DXVECTOR3 pos)
{
	// プレイヤーのベクトルを求める
	D3DXVECTOR3 vecPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 vec0;
	D3DXVECTOR3 vec1;
	D3DXVECTOR3 VtxPos[3];
	int			nNumber;
	D3DXVECTOR3 nor;

	//プレイヤーとメッシュフィールドの位置の差分を出す
	D3DXVECTOR3 posMtx = pos - m_pos;

	//現在プレイヤーが何ブロック目にいるかを確認する式
	int			nMeshX = ((int)(posMtx.x)) / ((int)(m_fMeshWidth / m_nMeshX));
	int			nMeshZ = ((int)(posMtx.z)) / ((int)(m_fMeshDepth / m_nMeshZ)) * -1;

	//現在の乗っかっているブロックの頂点の出し方
	int			nMeshLU = nMeshX + nMeshZ * (m_nMeshX + 1);					//例　0番目の頂点
	int			nMeshRU = (nMeshX + 1) + nMeshZ * (m_nMeshX + 1);			//例　1番目の頂点
	int			nMeshLD = nMeshX + (nMeshZ + 1) * (m_nMeshX + 1);			//例　3番目の頂点
	int			nMeshRD = (nMeshX + 1) + (nMeshZ + 1) * (m_nMeshX + 1);		//例　4番目の頂点

																			//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//============================
	//		三角形
	//============================
	//ベクトル計算式 目的 - 現在
	vec0 = pVtx[nMeshLU].pos - pVtx[nMeshRD].pos;
	//ベクトル計算式 自分の位置　- 現在
	vec1 = posMtx - pVtx[nMeshRD].pos;

	if ((vec0.x * vec1.z) - (vec0.z * vec1.x) <= 0)
	{//三角形より右にいる場合
		vec0 = (posMtx - pVtx[nMeshLU].pos);

		//各頂点の保管
		VtxPos[0] = pVtx[nMeshRU].pos;
		VtxPos[1] = pVtx[nMeshRD].pos;
		VtxPos[2] = pVtx[nMeshLU].pos;

		nNumber = 3;
	}
	else if ((vec0.x * vec1.z) - (vec0.z * vec1.x) >= 0)
	{//三角形より左にいる場合
		vec0 = (posMtx - pVtx[nMeshRD].pos);

		//各頂点の保管
		VtxPos[0] = pVtx[nMeshLD].pos;
		VtxPos[1] = pVtx[nMeshLU].pos;
		VtxPos[2] = pVtx[nMeshRD].pos;

		nNumber = 0;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	// プレイヤーのいるポリゴンの法線を出す
	vec0 = VtxPos[1] - VtxPos[0];			// 左側
	vec1 = VtxPos[2] - VtxPos[0];			// 右側
	D3DXVec3Cross(&nor, &vec0, &vec1);			// 外積を求める
	D3DXVec3Normalize(&nor, &nor);	// 正規化

	return nor;	// 法線ベクトルを返す
}

//=========================================================================================================================
// 現在いる地面かどうかの判定
//=========================================================================================================================
bool CMesh::OnField(D3DXVECTOR3 pos, float fMaxHeight)
{
	//プレイヤーとメッシュフィールドの位置の差分を出す
	D3DXVECTOR3 posMtx = pos - m_pos;

	//現在プレイヤーが何ブロック目にいるかを確認する式
	int			nMeshX = ((int)(posMtx.x)) / ((int)(m_fMeshWidth / m_nMeshX));
	int			nMeshZ = ((int)(posMtx.z)) / ((int)(m_fMeshDepth / m_nMeshZ)) * -1;

	if (nMeshX >= m_nMeshX || nMeshX < 0)
	{
		return false;
	}
	if (nMeshZ >= m_nMeshZ || nMeshZ < 0)
	{
		return false;
	}

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (posMtx.x < 0 || posMtx.x > pVtx[m_nMeshX].pos.x)
	{
		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();

		return false;
	}
	if (posMtx.z > 0 || posMtx.z > pVtx[m_nMeshX].pos.z)
	{
		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();

		return false;
	}

	//現在の乗っかっているブロックの頂点の出し方
	int	nMeshLU = nMeshX + nMeshZ * (m_nMeshX + 1);					//例　0番目の頂点
	int	nMeshRU = (nMeshX + 1) + nMeshZ * (m_nMeshX + 1);			//例　1番目の頂点
	int	nMeshLD = nMeshX + (nMeshZ + 1) * (m_nMeshX + 1);			//例　3番目の頂点
	int	nMeshRD = (nMeshX + 1) + (nMeshZ + 1) * (m_nMeshX + 1);		//例　4番目の頂点

	int nMostUnder, nUnder0, nUnder1;

	if (pVtx[nMeshLU].pos.y < pVtx[nMeshRU].pos.y) { nUnder0 = nMeshLU; }
	else { nUnder0 = nMeshRU; }

	if (pVtx[nMeshLD].pos.y < pVtx[nMeshRD].pos.y) { nUnder1 = nMeshLD; }
	else { nUnder1 = nMeshRD; }

	if (pVtx[nUnder0].pos.y < pVtx[nUnder1].pos.y) { nMostUnder = nUnder0; }
	else { nMostUnder = nUnder1; }

	if (pVtx[nMostUnder].pos.y + m_pos.y > pos.y + fMaxHeight + 10.0f)
	{
		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();

		return false;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return true;
}