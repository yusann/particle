//=============================================================================
//
// レンダラ処理
// Author : YUUSAN KA
//--------------------------
// DATA : 2016/11/09
//
//=============================================================================
//*****************************************************************************
//   二重定義防止
//*****************************************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
// ライブラリファイル
// [構成プロパティ]->[リンカー]->[入力]->[追加の依存ファイル]に記述しても可能
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")         // システム時刻取得に必要
#pragma comment(lib,"dinput8.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_POLYGON       (2)                                  // ポリゴン数
#define NUM_VERTEX        (4)                                  // 頂点数

#define FVF_VERTEX_2D     (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)                // 頂点フォーマット（ 座標変換済み頂点座標=>2D座標 | 頂点カラー | UVを追加 ）
#define FVF_VERTEX_3D     (D3DFVF_XYZ  | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX1)  // 頂点フォーマット      （ 座標変換する頂点座標=>3D座標 | 頂点カラー| 法線| UVを追加）

//*****************************************************************************
// 構造体定義
//*****************************************************************************
//  頂点情報（上の頂点フォーマットと構造を合わせること）
typedef struct
{
	D3DXVECTOR3 pos;        // XYZの宣言
	float rhw;              // RHWの宣言
	D3DCOLOR color;         // 色の宣言
	D3DXVECTOR2 tex;        // 頂点データ構造体にもUV追加（必ず最後に追加）2はXY, 3はXYZ
}VERTEX_2D;

typedef struct
{
	// 順番が必ず守る！！
	D3DXVECTOR3 pos;        // XYZの宣言
	D3DXVECTOR3 normal;     // 法線(必ずpositionの下)
	D3DCOLOR color;         // 色の宣言
	D3DXVECTOR2 tex;        // 頂点データ構造体にもUV追加（必ず最後に追加）2はXY, 3はXYZ
}VERTEX_3D;

//*****************************************************************************
//   クラスの定義
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();                                   // デフォルトコンストラクタ
	~CRenderer();                                  // デストラクタ
	
	HRESULT Init(HWND hWnd, BOOL bWindow);            // 初期化処理
	void Uninit(void);                             // 終了処理
	bool DrawBegin(void);
	void DrawEnd(void);

	LPDIRECT3DDEVICE9 GetDevice(void);             // デバイスの取得処理

private:
	LPDIRECT3D9				m_pD3D;                // Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice;          // Deviceオブジェクト(描画に必要)
	bool					m_bDraw;
};

#endif