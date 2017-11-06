#ifndef _MOTIONPARTSX_H_
#define _MOTIONPARTSX_H_

#define MOTIONPARTSX_PARTS_MAX      (32)
#define MOTIONPARTSX_MATERIAL_MAX   (64)
#define MOTIONPARTSX_MOTION_MAX     (16)

//*****************************************************************************
//   クラスの定義
//*****************************************************************************
// シーンクラス
class CMotionPartsX
{
public:

	// モデル種類
	typedef enum
	{
		TYPE_PLAYER = 0,
		TYPE_ENEMY,
		TYPE_MAX
	}TYPE;

	// アニメーション
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
		bool Work;                                    // 補間フラグ
	}MOTION;

	// パーツ
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

	// モデル
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

	CMotionPartsX();						// デフォルトコンストラクタ
	~CMotionPartsX();						// デストラクタ
	
	static void Init(void);                              // 初期化処理
	static void Uninit(void);                            // 終了処理
	static MOSIONPARTSX *GetMotionPartsX(TYPE type);					// モデル取得

private:
	static MOSIONPARTSX *LoadFile(char *pPath);
	static MOSIONPARTSX	*m_MotionPartsX[TYPE_MAX];			// モデル情報
	static char m_FileName[TYPE_MAX][128];										// モデルパス
	static LPDIRECT3DTEXTURE9	m_pTextureBG;
};

#endif