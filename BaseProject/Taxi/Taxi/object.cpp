//=============================================================================
//
// �I�u�W�F�N�g�̏��� [object.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "scene.h"
#include "model3D.h"
#include "manager.h"
#include "object.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"
#include "shadow.h"
#include "effectmgr.h"
#include "loadEffect.h"
#include "tutorial.h"
#include "objbillboad.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define MODEL_NAME_1	"data\\MODEL\\�������\\���̋@.x"			// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_2	"data\\MODEL\\�������\\grass.x"			// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_3	"data\\MODEL\\���Ȃ�����\\TreeU.x"		// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_4	"data\\MODEL\\���Ȃ�����\\TreeD.x"		// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_5	"data\\MODEL\\��΂������\\�i�{�[��.x"		// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_6	"data\\MODEL\\��΂������\\cone.x"			// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_7	"data\\MODEL\\���Ȃ�����\\�r��200.x"		// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_8	"data\\MODEL\\���Ȃ�����\\�r��400.x"		// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_9	"data\\MODEL\\���Ȃ�����\\�r��600.x"		// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_10	"data\\MODEL\\�������\\�X��.x"			// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_11	"data\\MODEL\\�������\\�M���@2.x"		// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_12	"data\\MODEL\\�������\\�M���@3.x"		// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_13	"data\\MODEL\\��΂������\\�x���`.x"		// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_14	"data\\MODEL\\���Ȃ�����\\�e���r��.x"		// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_15	"data\\MODEL\\�������\\tree.x"			// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_16	"data\\MODEL\\�������\\���O�d�b.x"		// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_17	"data\\MODEL\\�������\\���d.x"			// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_18	"data\\MODEL\\���Ȃ�����\\�Ԓd.x"			// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_19	"data\\MODEL\\���Ȃ�����\\�K���H����.x"	// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_20	"data\\MODEL\\�����蔻��p\\box.x"			// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_21	"data\\MODEL\\��΂������\\�Ŕ�.x"			// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_22	"data\\MODEL\\��΂������\\�r�[���P�[�X.x"	// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_23	"data\\MODEL\\���Ȃ�����\\���v��.x"		// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_24	"data\\MODEL\\���Ȃ�����\\�ԃ����K.x"		// �ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_25	"data\\MODEL\\���Ȃ�����\\�R�[��.x"		//�ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_26	"data\\MODEL\\���Ȃ�����\\�D�y�w.x"		//�ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_27	"data\\MODEL\\���Ȃ�����\\�G�X�^.x"		//�ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_28	"data\\MODEL\\���Ȃ�����\\���.x"			//�ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_29	"data\\MODEL\\���Ȃ�����\\�A�s�A�h�[��.x"	//�ǂݍ��ރe�N�X�`���t�@�C��
#define MODEL_NAME_30	"data\\MODEL\\���Ȃ�����\\JR�^���[.x"		//�ǂݍ��ރe�N�X�`���t�@�C��
#define MODEL_NAME_31	"data\\MODEL\\���Ȃ�����\\����.x"			//�ǂݍ��ރe�N�X�`���t�@�C��
#define MODEL_NAME_32	"data\\MODEL\\�������\\�w���Ŕ�.x"		//�ǂݍ��ރe�N�X�`���t�@�C��
#define MODEL_NAME_33	"data\\MODEL\\��΂������\\��.x"			//�ǂݍ��ރe�N�X�`���t�@�C��
#define MODEL_NAME_34	"data\\MODEL\\�������\\fire_hydrant.x"	//�ǂݍ��ރe�N�X�`���t�@�C��
#define MODEL_NAME_35	"data\\MODEL\\���Ȃ�����\\�ϗ���.x"		//�ǂݍ��ރe�N�X�`���t�@�C��
#define MODEL_NAME_36	"data\\MODEL\\���Ȃ�����\\�^�s�I�J�J�[.x"	//�ǂݍ��ރe�N�X�`���t�@�C��
#define MODEL_NAME_37	"data\\MODEL\\���Ȃ�����\\�k��a�@.x"		//�ǂݍ��ރe�N�X�`���t�@�C��
#define MODEL_NAME_38	"data\\MODEL\\���Ȃ�����\\LCDpanel.x"		//�ǂݍ��ރe�N�X�`���t�@�C��

#define TEXTURE_NAME_1	"data\\TEXTURE\\modeltex\\bender.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_2	"data\\TEXTURE\\modeltex\\grass.png"		// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_3	"data\\TEXTURE\\modeltex\\�t����.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_4	"data\\TEXTURE\\modeltex\\tree.jpg"			// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_5	"data\\TEXTURE\\modeltex\\�r��200����.png"	// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_6	"data\\TEXTURE\\modeltex\\�r��200����.png"	// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_7	"data\\TEXTURE\\modeltex\\�r��400����.png"	// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_8	"data\\TEXTURE\\modeltex\\�r��400����.png"	// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_9	"data\\TEXTURE\\modeltex\\�r��600����.png"	// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_10	"data\\TEXTURE\\modeltex\\�r��600����.png"	// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_11	"data\\TEXTURE\\modeltex\\�X��.jpg"			// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_12	"data\\TEXTURE\\modeltex\\�M���@2.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_13	"data\\TEXTURE\\modeltex\\�M���@3.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_14	"data\\TEXTURE\\modeltex\\�x���`.png"		// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_15	"data\\TEXTURE\\modeltex\\�e���r��.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_16	"data\\TEXTURE\\modeltex\\leaf.png"			// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_17 "data\\TEXTURE\\modeltex\\���O�d�b.png"		// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_18 "data\\TEXTURE\\modeltex\\���d.jpg"			// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_19 "data\\TEXTURE\\modeltex\\�Ԓd.jpg"			// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_20 "data\\TEXTURE\\modeltex\\�K���H.png"		// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_21 "data\\TEXTURE\\modeltex\\�Ŕ�.jpg"			// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_22 "data\\TEXTURE\\modeltex\\�r�[���P�[�X.png"	// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_23 "data\\TEXTURE\\modeltex\\���v��.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_24 "data\\TEXTURE\\modeltex\\�ԃ����K.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_25 "data\\TEXTURE\\modeltex\\�R�[��.jpg"		//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_26 "data\\TEXTURE\\modeltex\\�D�y�w.jpg"		//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_27 "data\\TEXTURE\\modeltex\\�G�X�^.jpg"		//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_28 "data\\TEXTURE\\modeltex\\���.jpg"			//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_29 "data\\TEXTURE\\modeltex\\�A�s�A�h�[��.jpg"	//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_30 "data\\TEXTURE\\modeltex\\JR�^���[.jpg"		//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_31 "data\\TEXTURE\\modeltex\\����.jpg"			//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_32 "data\\TEXTURE\\modeltex\\�w���Ŕ�.jpg"		//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_33 "data\\TEXTURE\\modeltex\\��.png"			//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_34 "data\\TEXTURE\\modeltex\\.jpg"				//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_35 "data\\TEXTURE\\modeltex\\�ϗ���2.png"		//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_36 "data\\TEXTURE\\modeltex\\�Z�[�R�[�}�[�g�r��.jpg"	//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_37 "data\\TEXTURE\\modeltex\\���[�\���r��.jpg"			//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_38 "data\\TEXTURE\\modeltex\\�Z�u���C���u���r��.jpg"	//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_39 "data\\TEXTURE\\modeltex\\�t�@�~���[�}�[�g�r��.jpg"	//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_40 "data\\TEXTURE\\modeltex\\�^�s�I�J�J�[.png"	//�ǂݍ��ރe�N�X�`���t�@�C��
#define TEXTURE_NAME_41 "data\\TEXTURE\\modeltex\\�k��a�@.jpg"		//�ǂݍ��ރe�N�X�`���t�@�C��

#define MODEL_SPEED				(5.0f)
#define PLAYER_DEPTH			(50)		// �v���C���[�̕������p
#define PLAYER_HEIGHT			(100.0f)	// �v���C���[�̔w�̍���
#define OBJCT_ANGLE_REVISION	(0.2f)		// �p�x�␳
#define EFFECT_HIGHT			(250.0f)	// �G�~�b�^�[�̍���
#define FOUNTAIN_UP				(20.0f)		// �����̏㏸������l

//�X�V�͈�
#define FOUNTAIN_LENGTH			(15000)		//�����̍X�V�͈�
#define LEAF_LENGTH				(10500)		//�t�̍X�V����
#define WOOD_LENGTH				(8000)		//�؂��r���{�[�h�ɓ���ւ��鋗��
#define FLOWER_LENGTH			(5500)		//�Ԓd�̕`��͈�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPD3DXMESH CObject::m_pMeshModel[MAX_OBJECT] = {};						//���b�V�����ւ̃|�C���^
LPD3DXBUFFER CObject::m_pBuffMatModel[MAX_OBJECT] = {};					//�}�e���A���̏��ւ̃|�C���^
DWORD CObject::m_nNumMatModel[MAX_OBJECT] = {};							//�}�e���A���̏��
LPDIRECT3DTEXTURE9 CObject::m_pMeshTextures[MAX_OBJECT_TEXTURE] = {};
D3DXVECTOR3 CObject::m_LoadVtxMaxModel[MAX_OBJECT] = {};
D3DXVECTOR3 CObject::m_LoadVtxMinModel[MAX_OBJECT] = {};

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CObject::CObject() : CModel3D(OBJECT_PRIOTITY, CScene::OBJTYPE_OBJECT)
{
	m_ModelMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Spin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nStageCount = 0;
	m_bStageNext = false;
	m_pLoadEffect = NULL;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CObject::~CObject(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CObject::Init(void)
{
	//�A�b�v�f�[�g��ʂ��Ȃ��I�u�W�F�N�g�̃^�C�v
	int anUpdateType[UPDATE_TYPE_NUM] = { TYPE_TREE00, TYPE_TREE01, TYPE_BILL00, TYPE_BILL01,
										  TYPE_BILL02, TYPE_TVBILL, TYPE_TANUKI,
										  TYPE_OCLOCK, TYPE_REDBILL, TYPE_CORN2,
										  TYPE_STATION, TYPE_ESTA, TYPE_DAIMAL, TYPE_APIA,
										  TYPE_TOWER, TYPE_FOUNTAIN, TYPE_FERRISWGEEL, TYPE_TAPIOCA, TYPE_HOSPITAL, TYPE_LCDPANEL };

	//�������I�u�W�F�N�g�̃^�C�v
	int anSmallObjType[SMALL_OBJ_NUM] = { TYPE_DRINKMACHINE, TYPE_GRASS, TYPE_CARDBORD, TYPE_CORN,
										  TYPE_BENCH, TYPE_PHONEBOX, TYPE_LEAF, TYPE_SIGNBOARD, TYPE_BEERBOX, TYPE_FENCE};

	//�����I�u�W�F�N�g�̃^�C�v
	int anHightObjType[HIGHT_OBJ_NUM] = { TYPE_STREETLIGHT, TYPE_TRAFFICLIGHT00, TYPE_TRAFFICLIGHT01, TYPE_ROAD};

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//3D���f����Init
	CModel3D::Init();

	// �ʒu�̏�����
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�e�N�X�`���ԍ��̏�����
	m_nTex = -1;

	//�ʒu�̑��
	CModel3D::SetPosition(pos);

	// �e����̑��
	CModel3D::SetScale(m_scale);

	//�A�b�v�f�[�g��ʂ����ǂ����̔���
	bool bUpdate = false;
	for (int nCntType = 0; nCntType < UPDATE_TYPE_NUM; nCntType++)
	{
		if (m_nType == anUpdateType[nCntType])
		{
			CModel3D::SetUpdate(false);
			bUpdate = true;
			break;
		}
	}

	//�������������߂�^�C�v
	if (m_nType == TYPE_TREE02 || m_nType == TYPE_FLOWER)
	{
		CModel3D::SetOnlyLength(true);
	}

	if (!bUpdate)
	{
		bool bSmallObj = false;
		for (int nCntType = 0; nCntType < SMALL_OBJ_NUM; nCntType++)
		{// �������I�u�W�F�N�g���ǂ����̔���
			if (m_nType == anSmallObjType[nCntType])
			{
				CModel3D::SetSmallObj(true);
				bSmallObj = true;
				break;
			}
		}

		for (int nCntType = 0; nCntType < HIGHT_OBJ_NUM; nCntType++)
		{// �����I�u�W�F�N�g�̃^�C�v���ǂ����̔���
			if(m_nType == anHightObjType[nCntType])
			{
				CModel3D::SetHightObj(true);
				break;
			}
		}
	}

	if (m_nType == 30 && CManager::GetMode() == CManager::MODE_GAME)
	{// �����ɃG�t�F�N�g������
		m_pLoadEffect = CLoadEffect::Create(6, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 4);
	}
	else if (m_nType == 33 && CManager::GetMode() == CManager::MODE_GAME)
	{// ���ΐ��G�t�F�N�g
		CModel3D::SetBoolRoll(false);
		m_pLoadEffect = CLoadEffect::Create(5, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 4);
	}

	//�ϐ��̏�����
	m_pEffectMgr = NULL;
	m_pObjBill = NULL;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CObject::Uninit(void)
{
	//�G�t�F�N�g�}�l�[�W���[�̔j��
	if (m_pEffectMgr != NULL)
	{
		m_pEffectMgr->Uninit();
		delete m_pEffectMgr;
		m_pEffectMgr = NULL;
	}

	//�c�[���ō쐬�����G�t�F�N�g��j��
	if (m_pLoadEffect != NULL)
	{
		m_pLoadEffect->Uninit();
		delete m_pLoadEffect;
		m_pLoadEffect = NULL;
	}

	//�I�u�W�F�N�g�r���{�[�h
	m_pObjBill = NULL;

	//3D���f����Uninit
	CModel3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CObject::Update(void)
{
	CModel3D::Update();

	//�����̎擾
	float fLength = CModel3D::GetLength();

	// �����̂Ƃ��G�t�F�N�g���o��
	FountainEffect();

	//�t�̃G�t�F�N�g�̍X�V����
	LeafEffect(fLength);

	//�Ԓd�̕`��͈͔���
	if ((m_nType == TYPE_FLOWER) && (CModel3D::GetDraw() != false))
	{
		if (fLength > FLOWER_LENGTH)
		{//�`�悵�Ȃ�
			SetDraw(false);
		}
		else
		{
			SetDraw(true);
		}
	}

	if (CModel3D::GetDelete() == true) { Uninit(); }
}
//=============================================================================
// �`�揈��
//=============================================================================
void CObject::Draw(void)
{
	if (m_nType == 19) { return; }

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�ʒu�̏�����
	D3DXVECTOR3 Modelpos = CModel3D::GetPosition();
	//�v���C���[�̈ʒu���
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	//�Q�[���̏��
	CManager::MODE pMode = CManager::GetMode();

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	//�`�揈��
	CModel3D::Draw();

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CObject * CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float move, int nTexType, int nObjectType, MOVETYPE nMovetype, int nCollision)
{
	CObject *pObject = NULL;

	// NULL�`�F�b�N
	if (pObject == NULL)
	{// �������̓��I�m��

		pObject = new CObject;

		if (pObject != NULL)
		{
			// ��ނ̐ݒ�
			pObject->BindModel(m_pMeshModel[nObjectType], m_pBuffMatModel[nObjectType], m_nNumMatModel[nObjectType], m_pMeshTextures[nTexType],
							   m_LoadVtxMaxModel[nObjectType], m_LoadVtxMinModel[nObjectType]);
			// �I�u�W�F�N�g���Ƃ̐ݒ�p�^�C�v
			pObject->m_nType = nObjectType;
			// �T�C�Y����
			pObject->m_scale = scale;
			// �T�C�Y��e�N���X�ɑ��
			pObject->SetScale(scale);
			// �I�u�W�F�N�g�N���X�̐���
			pObject->Init();
			//�e�N�X�`���ԍ�����
			pObject->m_nTex = nTexType;
			// �ʒu����
			pObject->SetPosition(pos);
			// ��]�𔽉f
			pObject->SetRot(rot);
			// �����̎�ނƈړ��ʂ�ݒ�
			pObject->SetMoveType(nMovetype);
			//pObject->m_move = D3DXVECTOR3(move, move, move);
			// �R���W������ONOFF
			pObject->m_nCollision = nCollision;

			//�G�t�F�N�g�}�l�[�W���[�̐���
			pObject->CreateEffectMgr(pos);
		}
	}

	return pObject;
}
//===============================================================================
// X�t�@�C���̓ǂݍ���
//===============================================================================
HRESULT CObject::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL *pMat;

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODEL_NAME_1, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[0], NULL, &m_nNumMatModel[0], &m_pMeshModel[0]);
	D3DXLoadMeshFromX(MODEL_NAME_2, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[1], NULL, &m_nNumMatModel[1], &m_pMeshModel[1]);
	D3DXLoadMeshFromX(MODEL_NAME_3, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[2], NULL, &m_nNumMatModel[2], &m_pMeshModel[2]);
	D3DXLoadMeshFromX(MODEL_NAME_4, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[3], NULL, &m_nNumMatModel[3], &m_pMeshModel[3]);
	D3DXLoadMeshFromX(MODEL_NAME_5, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[4], NULL, &m_nNumMatModel[4], &m_pMeshModel[4]);
	D3DXLoadMeshFromX(MODEL_NAME_6, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[5], NULL, &m_nNumMatModel[5], &m_pMeshModel[5]);
	D3DXLoadMeshFromX(MODEL_NAME_7, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[6], NULL, &m_nNumMatModel[6], &m_pMeshModel[6]);
	D3DXLoadMeshFromX(MODEL_NAME_8, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[7], NULL, &m_nNumMatModel[7], &m_pMeshModel[7]);
	D3DXLoadMeshFromX(MODEL_NAME_9, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[8], NULL, &m_nNumMatModel[8], &m_pMeshModel[8]);
	D3DXLoadMeshFromX(MODEL_NAME_10, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[9], NULL, &m_nNumMatModel[9], &m_pMeshModel[9]);
	D3DXLoadMeshFromX(MODEL_NAME_11, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[10], NULL, &m_nNumMatModel[10], &m_pMeshModel[10]);
	D3DXLoadMeshFromX(MODEL_NAME_12, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[11], NULL, &m_nNumMatModel[11], &m_pMeshModel[11]);
	D3DXLoadMeshFromX(MODEL_NAME_13, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[12], NULL, &m_nNumMatModel[12], &m_pMeshModel[12]);
	D3DXLoadMeshFromX(MODEL_NAME_14, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[13], NULL, &m_nNumMatModel[13], &m_pMeshModel[13]);
	D3DXLoadMeshFromX(MODEL_NAME_15, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[14], NULL, &m_nNumMatModel[14], &m_pMeshModel[14]);
	D3DXLoadMeshFromX(MODEL_NAME_16, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[15], NULL, &m_nNumMatModel[15], &m_pMeshModel[15]);
	D3DXLoadMeshFromX(MODEL_NAME_17, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[16], NULL, &m_nNumMatModel[16], &m_pMeshModel[16]);
	D3DXLoadMeshFromX(MODEL_NAME_18, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[17], NULL, &m_nNumMatModel[17], &m_pMeshModel[17]);
	D3DXLoadMeshFromX(MODEL_NAME_19, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[18], NULL, &m_nNumMatModel[18], &m_pMeshModel[18]);
	D3DXLoadMeshFromX(MODEL_NAME_20, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[19], NULL, &m_nNumMatModel[19], &m_pMeshModel[19]);
	D3DXLoadMeshFromX(MODEL_NAME_21, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[20], NULL, &m_nNumMatModel[20], &m_pMeshModel[20]);
	D3DXLoadMeshFromX(MODEL_NAME_22, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[21], NULL, &m_nNumMatModel[21], &m_pMeshModel[21]);
	D3DXLoadMeshFromX(MODEL_NAME_23, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[22], NULL, &m_nNumMatModel[22], &m_pMeshModel[22]);
	D3DXLoadMeshFromX(MODEL_NAME_24, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[23], NULL, &m_nNumMatModel[23], &m_pMeshModel[23]);
	D3DXLoadMeshFromX(MODEL_NAME_25, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[24], NULL, &m_nNumMatModel[24], &m_pMeshModel[24]);
	D3DXLoadMeshFromX(MODEL_NAME_26, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[25], NULL, &m_nNumMatModel[25], &m_pMeshModel[25]);
	D3DXLoadMeshFromX(MODEL_NAME_27, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[26], NULL, &m_nNumMatModel[26], &m_pMeshModel[26]);
	D3DXLoadMeshFromX(MODEL_NAME_28, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[27], NULL, &m_nNumMatModel[27], &m_pMeshModel[27]);
	D3DXLoadMeshFromX(MODEL_NAME_29, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[28], NULL, &m_nNumMatModel[28], &m_pMeshModel[28]);
	D3DXLoadMeshFromX(MODEL_NAME_30, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[29], NULL, &m_nNumMatModel[29], &m_pMeshModel[29]);
	D3DXLoadMeshFromX(MODEL_NAME_31, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[30], NULL, &m_nNumMatModel[30], &m_pMeshModel[30]);
	D3DXLoadMeshFromX(MODEL_NAME_32, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[31], NULL, &m_nNumMatModel[31], &m_pMeshModel[31]);
	D3DXLoadMeshFromX(MODEL_NAME_33, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[32], NULL, &m_nNumMatModel[32], &m_pMeshModel[32]);
	D3DXLoadMeshFromX(MODEL_NAME_34, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[33], NULL, &m_nNumMatModel[33], &m_pMeshModel[33]);
	D3DXLoadMeshFromX(MODEL_NAME_35, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[34], NULL, &m_nNumMatModel[34], &m_pMeshModel[34]);
	D3DXLoadMeshFromX(MODEL_NAME_36, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[35], NULL, &m_nNumMatModel[35], &m_pMeshModel[35]);
	D3DXLoadMeshFromX(MODEL_NAME_37, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[36], NULL, &m_nNumMatModel[36], &m_pMeshModel[36]);
	D3DXLoadMeshFromX(MODEL_NAME_38, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[37], NULL, &m_nNumMatModel[37], &m_pMeshModel[37]);

	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		//�}�e���A����񂩂�e�N�X�`���̎擾
		pMat = (D3DXMATERIAL*)m_pBuffMatModel[nCount]->GetBufferPointer();
	}

	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	//���f���̍ő�l�E�ŏ��l���擾����
	for (int nCntModel = 0; nCntModel < MAX_OBJECT; nCntModel++)
	{
		m_LoadVtxMaxModel[nCntModel] = D3DXVECTOR3(-10000, -10000, -10000);	//�ő�l
		m_LoadVtxMinModel[nCntModel] = D3DXVECTOR3(10000, 10000, 10000);	//�ŏ��l

		//���_�����擾
		nNumVtx = m_pMeshModel[nCntModel]->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(m_pMeshModel[nCntModel]->GetFVF());

		//���_�o�b�t�@�̃��b�N
		m_pMeshModel[nCntModel]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//���_���W�̑��

			//�ő�l
			if (vtx.x > m_LoadVtxMaxModel[nCntModel].x)
			{
				m_LoadVtxMaxModel[nCntModel].x = vtx.x;
			}
			if (vtx.y > m_LoadVtxMaxModel[nCntModel].y)
			{
				m_LoadVtxMaxModel[nCntModel].y = vtx.y;
			}
			if (vtx.z > m_LoadVtxMaxModel[nCntModel].z)
			{
				m_LoadVtxMaxModel[nCntModel].z = vtx.z;
			}
			//�ŏ��l
			if (vtx.x < m_LoadVtxMinModel[nCntModel].x)
			{
				m_LoadVtxMinModel[nCntModel].x = vtx.x;
			}
			if (vtx.y < m_LoadVtxMinModel[nCntModel].y)
			{
				m_LoadVtxMinModel[nCntModel].y = vtx.y;
			}
			if (vtx.z < m_LoadVtxMinModel[nCntModel].z)
			{
				m_LoadVtxMinModel[nCntModel].z = vtx.z;
			}

			//�T�C�Y���̃|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}

		//���_�o�b�t�@�̃A�����b�N
		m_pMeshModel[nCntModel]->UnlockVertexBuffer();
	}

	//�g���Ă���e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pMeshTextures[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_2, &m_pMeshTextures[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_3, &m_pMeshTextures[2]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_4, &m_pMeshTextures[3]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_5, &m_pMeshTextures[4]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_6, &m_pMeshTextures[5]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_7, &m_pMeshTextures[6]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_8, &m_pMeshTextures[7]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_9, &m_pMeshTextures[8]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_10, &m_pMeshTextures[9]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_11, &m_pMeshTextures[10]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_12, &m_pMeshTextures[11]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_13, &m_pMeshTextures[12]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_14, &m_pMeshTextures[13]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_15, &m_pMeshTextures[14]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_16, &m_pMeshTextures[15]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_17, &m_pMeshTextures[16]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_18, &m_pMeshTextures[17]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_19, &m_pMeshTextures[18]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_20, &m_pMeshTextures[19]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_21, &m_pMeshTextures[20]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_22, &m_pMeshTextures[21]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_23, &m_pMeshTextures[22]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_24, &m_pMeshTextures[23]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_25, &m_pMeshTextures[24]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_26, &m_pMeshTextures[25]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_27, &m_pMeshTextures[26]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_28, &m_pMeshTextures[27]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_29, &m_pMeshTextures[28]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_30, &m_pMeshTextures[29]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_31, &m_pMeshTextures[30]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_32, &m_pMeshTextures[31]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_33, &m_pMeshTextures[32]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_34, &m_pMeshTextures[33]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_35, &m_pMeshTextures[34]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_36, &m_pMeshTextures[35]);//�Z�R�}
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_37, &m_pMeshTextures[36]);//���[�\��
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_38, &m_pMeshTextures[37]);//�Z�u��
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_39, &m_pMeshTextures[38]);//�t�@�~�}
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_40, &m_pMeshTextures[39]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_41, &m_pMeshTextures[40]);

	return S_OK;
}
//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CObject::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		// ���b�V���̊J��
		if (m_pMeshModel[nCount] != NULL)
		{
			m_pMeshModel[nCount]->Release();
			m_pMeshModel[nCount] = NULL;
		}
		// �}�e���A���̊J��
		if (m_pBuffMatModel[nCount] != NULL)
		{
			m_pBuffMatModel[nCount]->Release();
			m_pBuffMatModel[nCount] = NULL;
		}
	}
	//�e�N�X�`��
	for (int nCntTex = 0; nCntTex < MAX_OBJECT_TEXTURE; nCntTex++)
	{
		if (m_pMeshTextures[nCntTex] != NULL)
		{
			m_pMeshTextures[nCntTex]->Release();
			m_pMeshTextures[nCntTex] = NULL;
		}
	}
}
//=============================================================================
//	�X�e�[�W�ړ����ɏ��������邽��
//=============================================================================
void CObject::DeleteObject(void)
{
	CScene *pScene;

	//�v���C�I���e�B�[�`�F�b�N
	pScene = CScene::GetTop(OBJECT_PRIOTITY);

	//NULL�`�F�b�N
	while (pScene != NULL)
	{
		//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{//�^�C�v���ǂ�������
			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{
				pScene->Uninit();
			}
		}
		//Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}

//===============================================================================
// �G�t�F�N�g�}�l�[�W���[�̐���
//===============================================================================
void CObject::CreateEffectMgr(D3DXVECTOR3 pos)
{
	if ((m_nType != TYPE_TREE00) && (m_nType != TYPE_TREE01) && (m_nType != TYPE_TREE02)) { return; }

	if ((m_pEffectMgr == NULL) && (CManager::MODE_GAME == CManager::GetMode()))
	{ //�G�t�F�N�g�}�l�[�W���[�̐���
		m_pEffectMgr = CEffectMgr::Create(D3DXVECTOR3(pos.x, pos.y + EFFECT_HIGHT, pos.z), CEffectMgr::TYPE_SUMMER);
	}
}

//=============================================================================
// �����ɂ��ړ��ʂ̕ω�
//=============================================================================
D3DXVECTOR3 CObject::Fountain(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	D3DXVECTOR3 posObj = CModel3D::GetPosition();	// �ʒu�̎擾
	D3DXVECTOR3 vec = pos - posObj;					// �������瑊������ւ̃x�N�g��

	// �x�N�g���̒��������߂�
	float fLengthVec = D3DXVec3Length(&vec);		// �������瑊������ւ̃x�N�g���̒���
	float fLengthMove = D3DXVec3Length(&move);		// ����̈ړ��ʂ̒���

	D3DXVECTOR3 vecMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �p�x�����߂�
	float fAngle = acosf(move.x / sqrt(move.x * move.x + move.z * move.z));
	if (move.z < 0.0f)fAngle = 2.0f * D3DX_PI - fAngle;
	if (isnan(fAngle))return vecMove;

	D3DXVECTOR3 vtxMax = CModel3D::GetVtxMax();		// ���_���W�̍ő�l�̎擾

	int nDigit = (int)log10f(vtxMax.x) + 1;
	if (6 < nDigit)return move;

	nDigit = (int)log10f(vtxMax.z) + 1;
	if (6 < nDigit)return move;

	// �ړ��ʂ̌v�Z
	if (m_nType == 33)
	{// ���ΐ�
		vecMove = D3DXVECTOR3(0.8f * (move.x + (cosf(fAngle) * vtxMax.x)), FOUNTAIN_UP * 0.4f, 0.8f * (move.z + (sinf(fAngle) * vtxMax.z)));
	}
	else
	{// ����
		vecMove = D3DXVECTOR3(0.05f * (move.x + (cosf(fAngle) * vtxMax.x)), FOUNTAIN_UP, 0.05f * (move.z + (sinf(fAngle) * vtxMax.z)));
	}

	return vecMove;
}

//=============================================================================
// �����̃G�t�F�N�g�̐���
//=============================================================================
void CObject::FountainEffect(void)
{
	if ((m_nType == 30 || (m_nType == 33 && CModel3D::GetBoolBlow())) && NULL != m_pLoadEffect)
	{// �����ɃG�t�F�N�g������
		CManager::MODE mode = CManager::GetMode();
		D3DXVECTOR3 PlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//���[�h���ƂɎ擾
		if (mode == CManager::MODE_GAME)
		{
			PlayerPos = CGame::GetPlayer()->GetPos();
		}
		else if (mode == CManager::MODE_TUTORIAL)
		{
			PlayerPos = CTutorial::GetPlayer()->GetPos();
		}

		//���������߂�
		D3DXVECTOR3 Distance = PlayerPos - CModel3D::GetPosition();
		float fLength = sqrtf((Distance.x * Distance.x) + (Distance.z * Distance.z));

		if (fLength < FOUNTAIN_LENGTH)
		{//�����̍X�V�͈�
			if(m_nType == 30){ m_pLoadEffect->SetPos(CModel3D::GetPosition() + D3DXVECTOR3(0.0f, CModel3D::GetVtxMax().y, 0.0f)); }
			else if (m_nType == 33) { m_pLoadEffect->SetPos(CModel3D::GetPosition()); }

			m_pLoadEffect->Update();
		}
	}
}

//=============================================================================
// �t�̃G�t�F�N�g�X�V����
//=============================================================================
void CObject::LeafEffect(float fLength)
{
	if (m_pEffectMgr != NULL)
	{
		//�t���ς̃G�t�F�N�g�������X�V
		if(fLength < LEAF_LENGTH)
		{
			m_pEffectMgr->Update();
		}

		if (fLength > WOOD_LENGTH)
		{
			//�r���{�[�h�̐���
			if (m_pObjBill == NULL)
			{
				m_pObjBill = CObjBillboad::Create(CModel3D::GetPosition(), m_nType);
				m_pObjBill->SetModelInfo(m_scale, m_nTex, m_nCollision);
			}
			else if (m_pObjBill != NULL)
			{
				m_pObjBill->SetDraw(true);
				CModel3D::SetDraw(false);
			}
		}
		else
		{
			CModel3D::SetDraw(true);
		}
	}
}

//===============================================================================
// �����蔻��
//===============================================================================
bool CObject::CollisionObject(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * pMove)
{
	//19�ȊO�͂��̏����ɓ���Ȃ�
	if (m_nType != 19) return false;

	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//�������Ă��邩�������ĂȂ���
	bool bLand = false;

	switch (m_nCollision)
	{
	case 1:

		// �e����̎擾
		D3DXVECTOR3 ModelPos = CModel3D::GetPosition();		// �ʒu
		D3DXVECTOR3 ModelMove = CModel3D::GetMove();		// �ړ���
		MOVETYPE ModelMoveType = CModel3D::GetMoveType();	// �����̃^�C�v
		D3DXVECTOR3 VtxMax = CModel3D::GetVtxMax();			// ���f���̍ő�l
		D3DXVECTOR3 VtxMin = CModel3D::GetVtxMin();			// ���f���̍ŏ��l
		D3DXVECTOR3 rot = CModel3D::GetRot();

		D3DXVECTOR3 ModelMax = CModel3D::GetPosition() + CModel3D::GetVtxMax();	// �ʒu���݂̍ő�l
		D3DXVECTOR3 ModelMin = CModel3D::GetPosition() + CModel3D::GetVtxMin();	// �ʒu���݂̍ŏ��l

		// �ړ��ʂ̕ێ�
		if (ModelMove.x == 0.0f) ModelMove.x = m_ModelMove.x;
		if (ModelMove.y == 0.0f) ModelMove.y = m_ModelMove.y;
		if (ModelMove.z == 0.0f) ModelMove.z = m_ModelMove.z;

		if (pPos->x >= ModelMin.x - PLAYER_DEPTH && pPos->x <= ModelMax.x + PLAYER_DEPTH)
		{// Z�͈͓̔��ɂ���
			if (pPos->z >= ModelMin.z - PLAYER_DEPTH && pPos->z <= ModelMax.z + PLAYER_DEPTH)
			{// X�͈͓̔��ɂ���
				if (pPosOld->y >= ModelMax.y && pPos->y <= ModelMax.y)
				{// �I�u�W�F�N�g�̏ォ�瓖����ꍇ
					pPos->y = ModelMax.y + abs(ModelMove.y);
					pMove->y = 0.0f;

					if (ModelMove.x != 0.0f)
					{// X�ړ�����
						pPos->x += ModelMove.x;
					}

					if (ModelMove.z != 0.0f)
					{// Z�ړ�����
						pPos->z += ModelMove.z;
					}

					bLand = true;
				}
				else if (pPosOld->y + PLAYER_HEIGHT <= ModelMin.y && pPos->y + PLAYER_HEIGHT >= ModelMin.y)
				{// �I�u�W�F�N�g�̉����瓖����ꍇ
					pPos->y = ModelMin.y - PLAYER_HEIGHT;
					pMove->y = 0.0f;
				}

				if (!(pPos->y >= ModelMax.y) && !(pPos->y + PLAYER_HEIGHT <= ModelMin.y))
				{// �I�u�W�F�N�g���Ƃ̓����蔻��
					if (pPosOld->x - PLAYER_DEPTH >= ModelMax.x + rot.x && pPos->x - PLAYER_DEPTH < ModelMax.x + rot.x)
					{// �E���甲�����ꍇ
						pPos->x = ModelMax.x + PLAYER_DEPTH;
					}
					else if (pPosOld->x + PLAYER_DEPTH <= ModelMin.x && pPos->x + PLAYER_DEPTH > ModelMin.x)
					{// �����甲�����ꍇ
						pPos->x = ModelMin.x - PLAYER_DEPTH;
					}

					if (pPosOld->z - PLAYER_DEPTH >= ModelMax.z && pPos->z - PLAYER_DEPTH < ModelMax.z)
					{// �����甲�����ꍇ
						pPos->z = ModelMax.z + PLAYER_DEPTH + 0.1f;
					}
					else if (pPosOld->z + PLAYER_DEPTH <= ModelMin.z && pPos->z + PLAYER_DEPTH > ModelMin.z)
					{// ��O���甲�����ꍇ
						pPos->z = ModelMin.z - PLAYER_DEPTH - 0.1f;
					}
				}
			}
		}

		// �ʒu�̑��
		CModel3D::SetPosition(ModelPos);
		// �ړ��ʂ̑��
		m_ModelMove = ModelMove;

		break;
	}

	return bLand;
}
