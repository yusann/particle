//*****************************************************************************
//   二重定義防止
//*****************************************************************************
#ifndef _SCENEBILLBOARD_H_
#define _SCENEBILLBOARD_H_

//*****************************************************************************
//   クラスの定義
//*****************************************************************************
// シーンクラス
class CSceneBillboard : public CScene
{
public:
	CSceneBillboard(int nPriority = 3);             // デフォルトコンストラクタ
	~CSceneBillboard();            // デストラクタ
	
	void Init(void);        // 初期化処理
	void Uninit(void);      // 終了処理
	void Update(void);      // 更新処理
	void Draw(void);        // 描画処理

protected:
	float m_fRot;

private:
	void MakeVex(void);     // 頂点の設定
};

#endif