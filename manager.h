//=============================================================================
//
// マネージャー
// Author : YUUSAN KA
//--------------------------
// DATA : 2016/11/16
// UPDATA : 2016/11/30		キーボード入力の追加
//
//=============================================================================
//*****************************************************************************
//   二重定義防止
//*****************************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include <tchar.h>

//*****************************************************************************
//   前方宣言
//*****************************************************************************
class CRenderer;
class CSound;
class CInputKeyboard;
class CInputMouse;
class CTexture;
class CCamera;
class CLight;
class CMeshField;

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//*****************************************************************************
//   クラスの定義
//*****************************************************************************
class CManager
{
public:
	CManager();                                   // デフォルトコンストラクタ
	~CManager();                                  // デストラクタ
	
	HRESULT Init( HINSTANCE hInstance,             // 初期化処理
			   HWND      hWnd,
			   BOOL      bWindow);              // フルスクリーンのONOFF
	void Uninit(void);                             // 終了処理
	void Update(void);                             // 更新処理
	void Draw(void);                               // 描画処理

	static void WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
		if (ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return;
	}

	static CRenderer *GetRenderer(void);               // レンダラーの取得
	static CInputKeyboard *GetInputKeyboard(void);     // キーボード入力の取得
	static CInputMouse *GetInputMouse(void);
	static CCamera *GetCamera(void);
	static CLight *GetLight(void);
	static CMeshField *GetMeshField(void);
	static CSound    *GetSound() { return m_pSound; }

private:
	static CRenderer *m_pRenderer;            // レンダラー
	static CInputKeyboard *m_pInputKeyboard;  // キーボード入力
	static CInputMouse *m_pInputMouse;
	static CCamera *m_pCamera;
	static CLight *m_pLight;
	static CMeshField *m_pMeshField;
	static CSound *m_pSound;
};

#endif