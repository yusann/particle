//*****************************************************************************
//   二重定義防止
//*****************************************************************************
#ifndef _SCENEMESH_H_
#define _SCENEMESH_H_

//*****************************************************************************
//   クラスの定義
//*****************************************************************************
// シーンクラス
class CSceneMesh : public CScene
{
public:
	CSceneMesh(int nPriority = 1);                                   // デフォルトコンストラクタ
	~CSceneMesh();                                  // デストラクタ

	void Init(void);                              // 初期化処理
	void Uninit(void);                            // 終了処理
	void Update(void);                            // 更新処理
	void Draw(void);                              // 描画処理
	
protected:
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;     // インデックスバッファ
	D3DXVECTOR3 m_rot;
	int   m_nPolygonNum;
	int   m_nVexNum;

};

#endif