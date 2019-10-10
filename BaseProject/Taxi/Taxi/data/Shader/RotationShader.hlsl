//=============================================================================
//
// ローテーションシェーダー [baceshader.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================

//*****************************************************************************
// 構造体の定義
//*****************************************************************************
typedef struct
{
	float4 position		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
} InputVertex;

typedef struct
{
	float4 position		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
} OutputVertex;

// =========================================================================
// 変数
// =========================================================================
float4x4 g_mtxWorld;		// 合成用マトリクス
float4x4 g_mtxView;			// ビューマトリックス
float4x4 g_mtxProjection;	// プロジェクションマトリックス
float4x4 g_mtxRot;			// 向きマトリックス
float	 g_Col_a;			// 色のα値

sampler g_mainTexture : register(s0);	// サンプラー(s)の0番を指定

// =========================================================================
// 頂点関数
// =========================================================================
OutputVertex Vertex(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	//向きマトリックスとワールドマトリックスを掛け合わせる
	float4x4 matWorld = mul(g_mtxRot, g_mtxWorld);

	//ワールドマトリックス・ビューマトリックスを掛け合わせる
	matWorld = mul(matWorld, g_mtxView);

	//掛け合わせたものとプロジェクションマトリックスを掛け合わせる
	matWorld = mul(matWorld, g_mtxProjection);

	// 座標
	output.position = mul(input.position, matWorld);

	// 法線
	output.normal = input.normal;

	// UV
	output.uv = input.uv;
	return output;
}

// =========================================================================
// ピクセル関数
// =========================================================================
float4 Pixel(OutputVertex input) : COLOR0
{
	float4 color = tex2D(g_mainTexture, input.uv);

	//α値を求めるにはRGBにA値を掛け合わせる
	//color *= g_Col_a;

return color;
}

// =========================================================================
// テクニック宣言
// =========================================================================
technique StandardShader
{
	pass Pass0
	{
		vertexShader = compile vs_3_0 Vertex();
		pixelShader = compile ps_3_0 Pixel();
	}
}