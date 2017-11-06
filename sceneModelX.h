//*****************************************************************************
//   二重定義防止
//*****************************************************************************
#ifndef _SCENEMODELX_H_
#define _SCENEMODELX_H_

#define MODEL_MATERIAL_MAX (100)

//*****************************************************************************
//   クラスの定義
//*****************************************************************************
// シーンクラス
class CSceneModelX : public CScene
{
public:
	CSceneModelX(int nPriority = 3);                                   // デフォルトコンストラクタ
	~CSceneModelX();                                  // デストラクタ
	
	void Init(void);                              // 初期化処理
	void Uninit(void);                            // 終了処理
	void Update(void);                            // 更新処理
	void Draw(void);                              // 描画処理

protected:
	D3DXVECTOR3 m_vecUp;    // 上方向
	D3DXVECTOR3 m_rot;      // 回転

private:
	LPD3DXMESH				m_pMeshModel;                    // メッシュ情報
	LPD3DXBUFFER			m_pBuffMatModel;                 // マテリアル情報
	DWORD					m_nNumMatModel;                  // マテリアル情報の数
	LPDIRECT3DTEXTURE9		m_pTextureModel[ MODEL_MATERIAL_MAX ];    // マッピング用テクスチャインターフェース
};

#endif