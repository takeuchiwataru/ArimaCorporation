//**************************************************************
//
//		�e�N�X�`���̏���[texture.cpp]
//		Auther : Shun Yokomichi
//
//**************************************************************
#include "texture.h"
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[

//**************************************************************
//			�}�N����`
//**************************************************************

//**************************************************************
//		�ÓI�����o�ϐ�
//**************************************************************
LPDIRECT3DTEXTURE9 *CTexture::m_apTexture = NULL;		// Texture�̃|�C���^
const char *CTexture::m_apFilename[TYPE_MAX] =
{
	//================
	// �Q�[����
	//================
	"data\\TEXTURE\\game\\GearChange\\Drive&Accelerator.jpg",
	"data\\TEXTURE\\game\\TimeLogo\\gametime.png",
	"data\\TEXTURE\\game\\Score\\totalscore.png",
	"data\\TEXTURE\\game\\Score\\Score.png",
	"data\\TEXTURE\\game\\Score\\Frame.png",
	"data\\TEXTURE\\money&ten.png",
	"data\\TEXTURE\\game\\Score\\plus.png",
	"data\\TEXTURE\\game\\TimeUp\\time_up.png",
	"data\\TEXTURE\\game\\��ԏ��.png",
	//================
	// �R���{�֌W
	//================
	"data\\TEXTURE\\game\\Combo\\combo.png",
	"data\\Texture\\game\\Combo\\Crazy_Through.png",
	"data\\Texture\\game\\Combo\\Crazy_Jump.png",
	"data\\Texture\\game\\Combo\\Crazy_Drift.png",

	//=================
	// �����x
	//=================
	"data\\TEXTURE\\game\\Satisfaction level\\Satisfaction_Level.png",
	"data\\TEXTURE\\game\\Satisfaction level\\Evaluation\\Evaluation_Frame.png",
	"data\\TEXTURE\\game\\Satisfaction level\\Evaluation\\�X�}�z.png",
	"data\\TEXTURE\\game\\Satisfaction level\\Evaluation\\comment_000.png",
	"data\\TEXTURE\\game\\Satisfaction level\\Evaluation\\comment_001.png",
	"data\\TEXTURE\\game\\Satisfaction level\\Evaluation\\comment_002.png",
	"data\\TEXTURE\\game\\Satisfaction level\\Evaluation\\comment_003.png",
	"data\\TEXTURE\\game\\Satisfaction level\\Evaluation\\comment_004.png",
	"data\\TEXTURE\\game\\Satisfaction level\\Evaluation\\comment_005.png",
	"data\\TEXTURE\\game\\Satisfaction level\\good.png",
	"data\\TEXTURE\\game\\Satisfaction level\\bad.png",

	//=================
	// �ړI�n�ʐ^
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
	// �ړI�n��
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
	// ���߃��S
	//=================
	"data\\TEXTURE\\game\\Destination\\Instructions000.png",
	"data\\TEXTURE\\game\\Destination\\Instructions001.png",

	//=================
	// ����
	//=================
	"data\\TEXTURE\\number\\number_time.png",
	"data\\TEXTURE\\number\\number_Score.png",

	//=================
	// 2D�w�i
	//=================
	"data\\TEXTURE\\Background_000.jpg",
	"data\\TEXTURE\\Background_001.jpg",

	//=================
	// �^�C�g��
	//=================
	"data\\TEXTURE\\Title\\Titlelogo.png",
	"data\\TEXTURE\\Title\\inJapan.png",
	"data\\TEXTURE\\Title\\press_start_button.png",
	"data\\TEXTURE\\Title\\Four_seasons.png",

	//=================
	// ���U���g
	//=================
	"data\\TEXTURE\\result\\result_logo.jpg",
	"data\\TEXTURE\\result\\totalearned_logo.png",
	"data\\TEXTURE\\result\\class_logo.png",
	"data\\TEXTURE\\result\\press_key.jpg",

	//=================
	// �����L���O
	//=================
	"data\\TEXTURE\\Ranking\\Ranking_logo.png",
	"data\\TEXTURE\\Ranking\\rank.png",
	"data\\TEXTURE\\Ranking\\class_logo_ranking.jpg",
	"data\\TEXTURE\\Ranking\\totalearned_logo_ranking.jpg",

	//=================
	// �Z���N�g
	//=================
	"data\\TEXTURE\\Select\\logo.png",
	"data\\TEXTURE\\Select\\SelectCircle.png",
	"data\\TEXTURE\\Select\\totalearned_logo_ranking.png",
	"data\\TEXTURE\\Select\\Map.jpg",
	"data\\TEXTURE\\Select\\pause002.png",
	"data\\TEXTURE\\Select\\press_key001.jpg",
	"data\\TEXTURE\\Select\\�}4.png",
	"data\\TEXTURE\\Select\\�}5.png",
	"data\\TEXTURE\\Select\\�}6.png",
	"data\\TEXTURE\\Select\\�}7.png",
	"data\\TEXTURE\\Select\\flam_Select_000.png",
	"data\\TEXTURE\\Select\\flam_Select_001.png",
	"data\\TEXTURE\\Select\\flam_back.png",

	//=================
	// �|�[�Y
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
	// �`���[�g���A��
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
	"data\\TEXTURE\\Tutorial\\Keyboard\\A�{�^��.png",
	"data\\TEXTURE\\Tutorial\\Keyboard\\D�{�^��.png",
	"data\\TEXTURE\\Tutorial\\Keyboard\\K�{�^��.png",
	"data\\TEXTURE\\Tutorial\\Keyboard\\L�{�^��.png",
	"data\\TEXTURE\\Tutorial\\Keyboard\\S�{�^��.png",
	"data\\TEXTURE\\Tutorial\\Keyboard\\W�{�^��.png",

	//=================
	// �X�^�[�g���}
	//=================
	"data\\TEXTURE\\game\\start\\HERE.png",
	"data\\TEXTURE\\game\\start\\WE.png",
	"data\\TEXTURE\\game\\start\\GO.png",

	//=================
	// �t�F�[�h
	//=================
	"data\\TEXTURE\\Fade\\NowLoading.png",
	"data\\TEXTURE\\Fade\\LoadingAnim.png",

	//=================
	// ���̑�
	//=================
	"data\\TEXTURE\\bg_fream.png"
};

//**************************************************************
//		�R���X�g���N�^
//**************************************************************
CTexture::CTexture() {}

//**************************************************************
//		�f�X�g���N�^
//**************************************************************
CTexture::~CTexture() {}

//**************************************************************
//		�e�N�X�`���̓ǂݍ���
//**************************************************************
HRESULT CTexture::Load(void)
{
	// �f�o�C�X�̎擾
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
//		�e�N�X�`���̊J��
//**************************************************************
void CTexture::Unload(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{	// NULL�ȊO�Ȃ�
			m_apTexture[nCntTex]->Release();		// �j��
			m_apTexture[nCntTex] = NULL;			// NULL��
		}
	}

	if (m_apTexture != NULL)
	{
		delete m_apTexture;
		m_apTexture = NULL;
	}
}