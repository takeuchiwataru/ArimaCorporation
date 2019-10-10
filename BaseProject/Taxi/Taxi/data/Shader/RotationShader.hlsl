//=============================================================================
//
// ���[�e�[�V�����V�F�[�_�[ [baceshader.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================

//*****************************************************************************
// �\���̂̒�`
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
// �ϐ�
// =========================================================================
float4x4 g_mtxWorld;		// �����p�}�g���N�X
float4x4 g_mtxView;			// �r���[�}�g���b�N�X
float4x4 g_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
float4x4 g_mtxRot;			// �����}�g���b�N�X
float	 g_Col_a;			// �F�̃��l

sampler g_mainTexture : register(s0);	// �T���v���[(s)��0�Ԃ��w��

// =========================================================================
// ���_�֐�
// =========================================================================
OutputVertex Vertex(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	//�����}�g���b�N�X�ƃ��[���h�}�g���b�N�X���|�����킹��
	float4x4 matWorld = mul(g_mtxRot, g_mtxWorld);

	//���[���h�}�g���b�N�X�E�r���[�}�g���b�N�X���|�����킹��
	matWorld = mul(matWorld, g_mtxView);

	//�|�����킹�����̂ƃv���W�F�N�V�����}�g���b�N�X���|�����킹��
	matWorld = mul(matWorld, g_mtxProjection);

	// ���W
	output.position = mul(input.position, matWorld);

	// �@��
	output.normal = input.normal;

	// UV
	output.uv = input.uv;
	return output;
}

// =========================================================================
// �s�N�Z���֐�
// =========================================================================
float4 Pixel(OutputVertex input) : COLOR0
{
	float4 color = tex2D(g_mainTexture, input.uv);

	//���l�����߂�ɂ�RGB��A�l���|�����킹��
	//color *= g_Col_a;

return color;
}

// =========================================================================
// �e�N�j�b�N�錾
// =========================================================================
technique StandardShader
{
	pass Pass0
	{
		vertexShader = compile vs_3_0 Vertex();
		pixelShader = compile ps_3_0 Pixel();
	}
}