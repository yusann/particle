//*****************************************************************************
//   二重定義防止
//*****************************************************************************
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
//   定数定義
//*****************************************************************************
#define SCENE_PRIORITY_MAX  (8)						// 優先度の最大数

//*****************************************************************************
//   クラスの定義
//*****************************************************************************
// シーンクラス
class CScene
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_BACKGROUND,

		OBJTYPE_FIELD,
		OBJTYPE_PLAYER,
		OBJTYPE_ENEMY,
		OBJTYPE_BULLET,
		OBJTYPE_EXPLOSION,

		OBJTYPE_UI,
		OBJTYPE_MAX
	}OBJTYPE;

	CScene(int nPriority = 3);                        // デフォルトコンストラクタ
	virtual ~CScene();                       // デストラクタ
	
	virtual void Init(void)   =0;      // 初期化処理
	virtual void Uninit(void) =0;      // 終了処理
	virtual void Update(void) =0;      // 更新処理
	virtual void Draw(void)   =0;      // 描画処理

	static void UpdateAll(void);                // すべての更新
	static void DrawAll(void);                  // すべての描画
	static void ReleaseAll(void);               // すべてのリリース

	static CScene *GetScene(int nPriority);       // シーンの取得処理
	void SetObjType(OBJTYPE objType);                            // シーンタイプの代入処理
	OBJTYPE GetObjType(void);                                      // シーンタイプの取得処理

	// 座標の取得
	D3DXVECTOR3 GetPos(void);                          // 

protected:
	void Release(void);                         // リリース処理（子クラスを消してから親クラスを消すため用）
	D3DXVECTOR3             m_pos;       // 座標
	D3DXVECTOR3             m_scl;      // サイズ
	D3DXCOLOR               m_color;    // 色
	LPDIRECT3DTEXTURE9      m_pTexture;  // テクスチャ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;  // 頂点バッファへのポインタ

private:
	static CScene *m_Top[SCENE_PRIORITY_MAX];      // シーンの宣言
	CScene *m_Prev;
	CScene *m_Next;
	static int m_nNumScene;                     // シーンの総数
	int m_nPriority;
	OBJTYPE m_objType;
	bool m_bDelete;

};

#endif