//*****************************************************************************
//   二重定義防止
//*****************************************************************************
#ifndef _MESHCUBE_H_
#define _MESHCUBE_H_

//*****************************************************************************
//   クラスの定義
//*****************************************************************************
// シーンクラス
class CMeshCube : public CSceneMesh
{
public:
	CMeshCube();                                   // デフォルトコンストラクタ
	~CMeshCube();                                  // デストラクタ

	void Init(void);                              // 初期化処理
	void Uninit(void);                            // 終了処理
	void Update(void);                            // 更新処理
	void Draw(void);                              // 描画処理

	static CMeshCube *Create(void);                  // 作成
	
private:
	void MakeVex(void);     // 頂点の設定
	void MakeBuff(void);    // バッファの設定

};

#endif