//*****************************************************************************
//   二重定義防止
//*****************************************************************************
#ifndef _2D_SAMPLE_H_
#define _2D_SAMPLE_H_

//*****************************************************************************
//   クラスの定義
//*****************************************************************************
// シーンクラス
class C2D_sample : public CScene2D
{
public:
	C2D_sample();             // デフォルトコンストラクタ
	~C2D_sample();                             // デストラクタ
	
	void Init(void);        // 初期化処理
	void Uninit(void);      // 終了処理
	void Update(void);      // 更新処理
	void Draw(void);        // 描画処理

	static C2D_sample *Create(D3DXVECTOR3 pos);                  // 作成

private:
	void SetPos(D3DXVECTOR3 pos);        // 描画処理
};

#endif