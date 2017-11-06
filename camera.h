//*****************************************************************************
//   二重定義防止
//*****************************************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
//   クラスの定義
//*****************************************************************************
// シーンクラス
class CCamera
{
public:
	typedef struct
	{
		D3DXMATRIX  mtxView;     // ビュー行列
		D3DXMATRIX  mtxProj;     // ビュー行列
		D3DXVECTOR3 posEye;      // 座標
		D3DXVECTOR3 posAt;       // 注視点
		D3DXVECTOR3 vecUp;       // 上方向
	}CAMERA;

	CCamera();                                   // デフォルトコンストラクタ
	~CCamera();                                  // デストラクタ
	
	void Init(void);                              // 初期化処理
	void Uninit(void);                            // 終了処理
	void Update(void);		// 更新処理
	void Set(void);			// セット処理

	// カメラの取得
	static CAMERA GetCamera() {
		return m_pCamera;
	}

private:
	static CAMERA m_pCamera;
	D3DXVECTOR3     m_posMove;     // 移動量
	D3DXVECTOR3     m_rot;         // 角度
	float       m_length;
	float       m_rotY;
	float       m_rotX;
	float       m_fZoom;       // ズーム
};

#endif