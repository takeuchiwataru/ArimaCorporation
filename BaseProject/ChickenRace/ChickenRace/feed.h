//=============================================================================
//
// �a�̏��� [feed.h]
// Author : ���R���
//
//=============================================================================
#ifndef _FEED_H_
#define _FEED_H_

#include "model3D.h"
#include "camerabace.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FEED_PRIOTITY				(3)			// �H�ו��̗D�揇��
#define MAX_FEED					(3)			// �H�ו��̃��f����
#define MAX_FEED_TEXTURE			(3)			// �H�ו��̃e�N�X�`����

//*****************************************************************************
// �O���錾
//*****************************************************************************
class C3DPolygon;
class C3DAnim;
class CPlayer;
//=====================
//  CModel3D�̔h���N���X
//=====================
class CFeed : public CModel3D
{
public:
	typedef enum
	{// �a�̎��
		FEEDTYPE_ATTACK = 0,	// �U��
		FEEDTYPE_ANNOY,			// �W�Q
		FEEDTYPE_SPEED,			// ����
		FEEDTYPE_MAX,			//�ő吔
	}FEEDTYPE;

	CFeed();
	~CFeed();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CFeed *Create(D3DXVECTOR3 pos, int nZone, int nType);
	bool CollisionFeed(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);
	bool TargetFeed(CPlayer *pPlayer, float &fValue);
	FEEDTYPE GetFeedType(void) { return m_feedType; }

private:
	static D3DXVECTOR3 m_VtxMaxModel[FEEDTYPE_MAX];		//���f���̍ő�l
	static D3DXVECTOR3 m_VtxMinModel[FEEDTYPE_MAX];		//���f���̍ŏ��l

	bool				m_bGet;							// ���邩�ǂ���
	bool				m_bEffect;						// �G�t�F�N�g���o�邩�ǂ���
	int					m_nCntGetTimer;					// ����悤�ɂȂ�܂ł̎���
	int					m_nCntEffectTimer;				// �G�t�F�N�g���o�鎞��
	int					m_nZone;						// �]�[��
	FEEDTYPE			m_feedType;						// �a�̎��
	D3DXVECTOR3			m_scale;						// �傫��
	D3DXVECTOR3			m_rot;							// ��]
	CShadow				*m_pShadow;						// �e�̃|�C���^
	C3DPolygon			*m_pPin;						//�s���̃|���S��
	C3DAnim			*m_pLight;						//���C�g�̃|���S��
};
#endif