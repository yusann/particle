//-------------------------------------------------------------------------------------------------------------------------------------------------------
//   ヘッダファイル
//-------------------------------------------------------------------------------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME        "エディター"            // ウィンドウクラスの名前
#define WINDOW_NAME       "モーションエディター"  // ウィンドウの名前（タイトル名）

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------------------------------------------------------------------------------------------
//   メイン関数
//-----------------------------------------------------------------------------------------------------------------------------------------------------
int APIENTRY WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow )
{
	// 変数宣言
	CManager *pManager = NULL;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	HWND hWnd;
	MSG msg;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		NULL,
		NULL,
		hInstance,
		NULL);

	// マネージャーの生成
	pManager = new CManager;

	// マネージャーの初期化処理（デバイスなど）
	if (FAILED(pManager->Init(hInstance, hWnd, true)))
	{
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);

	// フレームカウント初期化
	dwCurrentTime =
		dwFrameCount = 0;
	dwExecLastTime =
		dwFPSLastTime = timeGetTime();

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	for(;;)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				// マネージャーの更新処理
				pManager->Update();

				// マネージャーの描画処理
				pManager->Draw();

				dwFrameCount++;
			}
		}
	}

	// マネージャーの破棄
	if (pManager != NULL) {         // レンダラ
		pManager->Uninit();
		pManager = NULL;
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 分解能を戻す
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
//  ウィンドウプロシージャ
//  (CALLBACK：Win32API関数を呼び出す時の規約)
//  hWnd     ：ウィンドウのハンドル
//  uMsg     ：メッセージの識別子
//  wParam   ：メッセージの最初のパラメータ
//  lParam   ：メッセージの２番目のパラメータ
//-------------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CManager::WndProc(hWnd, uMsg, wParam, lParam);
	switch( uMsg )
	{
	case WM_DESTROY:                   // ウィンドウ破壊
		PostQuitMessage(0);            // "WM_QUIT"メッセージ
		break;

	case WM_KEYDOWN:                                            // 終了処理
		if( wParam == VK_ESCAPE )                               // ESCキー押す時
		{
			UINT nID = MessageBox( NULL , "終了しますか？" , "終了" , MB_YESNO | MB_DEFBUTTON2 );         // 終了するメッセージ
			if( nID == IDYES )
			{
				DestroyWindow( hWnd );                                                                    // 終了する
			}
		}
		break;

	default:
		break;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}