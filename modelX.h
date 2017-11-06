//*****************************************************************************
//   二重定義防止
//*****************************************************************************
#ifndef _MODELX_H_
#define _MODELX_H_

//*****************************************************************************
//   クラスの定義
//*****************************************************************************
// シーンクラス
class CModelX
{
public:

	// モデル種類
	typedef enum
	{
		TYPE_PLAYER = 0,
		TYPE_ENEMY,
		TYPE_BLOCK,
		TYPE_MAX
	}TYPE;

	// モデル情報
	struct MODELX
	{
		char*				Name;
		LPD3DXMESH			pMesh;              // ライト
		LPD3DXBUFFER		pBuffMat;           // 方向
		DWORD				NumMat;
		LPDIRECT3DTEXTURE9	*pTexture;
	};
	
	static void Init(void);                              // 初期化処理
	static void Uninit(void);                            // 終了処理
	static MODELX *GetModelX(int type) { return &m_modelX[m_FileName[type]]; }	// モデル取得
	static MODELX *GetModelX(char *FileName);									// モデル取得

private:
	static MODELX LoadModel(std::string FileName);
	static std::unordered_map<std::string, MODELX>	m_modelX;
	static std::vector<std::string> m_FileName;
};

#endif