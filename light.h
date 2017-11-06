//*****************************************************************************
//   二重定義防止
//*****************************************************************************
#ifndef _LIGHT_H_
#define _LIGHT_H_

//*****************************************************************************
//   クラスの定義
//*****************************************************************************
// シーンクラス
class CLight
{
public:
	typedef struct
	{
		D3DLIGHT9 light;              // ライト
		D3DXVECTOR3 vecDir;           // 方向
	}LIGHT;

	CLight();                                   // デフォルトコンストラクタ
	~CLight();                                  // デストラクタ
	
	void Init(void);                              // 初期化処理
	void Uninit(void);                            // 終了処理
	void Update(void);
	void Set(void);

	// カメラの取得
	static LIGHT GetLight() {
		return m_light;
	}

private:
	static LIGHT m_light;
};

#endif