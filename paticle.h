//*****************************************************************************
//   二重定義防止
//*****************************************************************************
#ifndef _PATICLE_H_
#define _PATICLE_H_

//*****************************************************************************
//   クラスの定義
//*****************************************************************************
class CPaticle
{
public:
	void Init(void);                              // 初期化処理
	void Uninit(void);                            // 終了処理
	void Update(void);                            // 更新処理
	void Draw(void);                              // 描画処理
	
private:
	void MakeVex(void);     // 頂点の設定
	void MakeBuff(void);    // バッファの設定

	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;     // インデックスバッファ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;  // 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9      m_pTexture;  // テクスチャ
	int                     m_nPolygonNum;
	int                     m_nVexNum;
	D3DXVECTOR3             m_pos;       // 座標
	D3DXVECTOR3             m_scl;      // サイズ
	D3DXVECTOR3             m_rot;
	D3DXCOLOR               m_color;    // 色
};

#endif