#ifndef _MOTIONPARTSX_H_
#define _MOTIONPARTSX_H_

#define MOTIONPARTSX_PARTS_MAX      (32)
#define MOTIONPARTSX_MATERIAL_MAX   (64)
#define MOTIONPARTSX_MOTION_MAX     (16)

//*****************************************************************************
//   �N���X�̒�`
//*****************************************************************************
// �V�[���N���X
class CMotionPartsX
{
public:

	// ���f�����
	typedef enum
	{
		TYPE_PLAYER = 0,
		TYPE_ENEMY,
		TYPE_MAX
	}TYPE;

	// �A�j���[�V����
	typedef struct
	{
		int				Frame;
		D3DXVECTOR3		Pos[MOTIONPARTSX_PARTS_MAX];
		D3DXVECTOR3		Rot[MOTIONPARTSX_PARTS_MAX];
		D3DXVECTOR3		Scl[MOTIONPARTSX_PARTS_MAX];
	}KEY;

	typedef struct
	{
		int Loop;
		int NumKey;
		KEY *pKey;
		bool Work;                                    // ��ԃt���O
	}MOTION;

	// �p�[�c
	typedef struct
	{
		char				FilePath[128];
		LPD3DXMESH			pMesh;
		LPD3DXBUFFER		pBuffMat;
		DWORD				NumMat;
		LPDIRECT3DTEXTURE9  pTexture[MOTIONPARTSX_MATERIAL_MAX];
		D3DXVECTOR3			Pos;
		D3DXVECTOR3			Rot;
		D3DXVECTOR3			Scl;
		D3DXMATRIX			Matrix;
		int					Index;
		int					Parent;
	}PARTS;

	// ���f��
	typedef struct
	{
		MOTION				Motion[MOTIONPARTSX_MOTION_MAX];
		PARTS				Parts[MOTIONPARTSX_PARTS_MAX];
		D3DXVECTOR3			DefaultPos[MOTIONPARTSX_MOTION_MAX];
		D3DXVECTOR3			DefaultRot[MOTIONPARTSX_MOTION_MAX];
		D3DXVECTOR3			DefaultScl[MOTIONPARTSX_MOTION_MAX];
		int					NumParts;
		int					NumMotion;
	}MOSIONPARTSX;

	CMotionPartsX();						// �f�t�H���g�R���X�g���N�^
	~CMotionPartsX();						// �f�X�g���N�^
	
	static void Init(void);                              // ����������
	static void Uninit(void);                            // �I������
	static MOSIONPARTSX *GetMotionPartsX(TYPE type);					// ���f���擾

private:
	static MOSIONPARTSX *LoadFile(char *pPath);
	static MOSIONPARTSX	*m_MotionPartsX[TYPE_MAX];			// ���f�����
	static char m_FileName[TYPE_MAX][128];										// ���f���p�X
	static LPDIRECT3DTEXTURE9	m_pTextureBG;
};

#endif