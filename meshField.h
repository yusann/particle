//*****************************************************************************
//   二重定義防止
//*****************************************************************************
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//*****************************************************************************
//   クラスの定義
//*****************************************************************************
// シーンクラス
class CMeshField : public CSceneMesh
{
public:
	CMeshField();                                   // デフォルトコンストラクタ
	~CMeshField();                                  // デストラクタ
	
	void Init(void);                              // 初期化処理
	void Uninit(void);                            // 終了処理
	void Update(void);                            // 更新処理
	void Draw(void);                              // 描画処理

	static CMeshField *Create(void);                  // 作成
	float GetHeight(D3DXVECTOR3 pos);

private:
	void MakeVex(void);     // 頂点の設定
	void MakeBuff(void);    // バッファの設定
	void LoadFile(void);

	D3DXVECTOR2 m_size;
	D3DXVECTOR2 m_divide;
	D3DXVECTOR2 m_texUV;
	float *m_PosY;
	int m_MaxX, m_MaxY;

};

#endif