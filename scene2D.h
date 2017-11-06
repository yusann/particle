//*****************************************************************************
//   二重定義防止
//*****************************************************************************
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*****************************************************************************
//   クラスの定義
//*****************************************************************************
// シーンクラス
class CScene2D : public CScene
{
public:
	CScene2D(int nPriority = 7);             // デフォルトコンストラクタ
	~CScene2D();                             // デストラクタ
	
	void Init(void);        // 初期化処理
	void Uninit(void);      // 終了処理
	void Update(void);      // 更新処理
	void Draw(void);        // 描画処理

private:
	void MakeVex(void);     // 頂点の設定

};

#endif