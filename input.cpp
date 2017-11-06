//=============================================================================
//
// 入力処理 [input.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	COUNT_WAIT_REPEAT	(20)	// リピート待ち時間

//*****************************************************************************
// 静的変数
//*****************************************************************************
LPDIRECTINPUT8	CInput::m_pDInput = NULL;	// DirectInputオブジェクト

//=============================================================================
// CInputコンストラスタ
//=============================================================================
CInput::CInput()
{
	m_pDIDevice = NULL;
}

//=============================================================================
// CInputデストラスタ
//=============================================================================
CInput::~CInput()
{
}

//=============================================================================
// 入力処理の初期化処理
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr = S_FALSE;

	// DirectInputオブジェクトの作成
	if(!m_pDInput)
	{
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&m_pDInput, NULL);
	}

	return hr;
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
#if 0	// ---> 純粋仮想関数化
HRESULT CInput::Update(void)
{
	return S_OK;
}
#endif

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void CInput::Uninit(void)
{
	if(m_pDIDevice)
	{// デバイスオブジェクトの開放
		m_pDIDevice->Unacquire();

		m_pDIDevice->Release();
		m_pDIDevice = NULL;
	}

	if(m_pDInput)
	{// DirectInputオブジェクトの開放
		m_pDInput->Release();
		m_pDInput = NULL;
	}
}

//=============================================================================
// CInputKeyboardコンストラスタ
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	// 各ワークのクリア
	ZeroMemory(m_aKeyState, sizeof m_aKeyState);
	ZeroMemory(m_aKeyStateTrigger, sizeof m_aKeyStateTrigger);
	ZeroMemory(m_aKeyStateRelease, sizeof m_aKeyStateRelease);
	ZeroMemory(m_aKeyStateRepeat, sizeof m_aKeyStateRepeat);
	ZeroMemory(m_aKeyStateRepeatCnt, sizeof m_aKeyStateRepeatCnt);
}

//=============================================================================
// CInputKeyboardデストラスタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{
}

//=============================================================================
// キーボードの初期化処理
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// 入力処理の初期化
	CInput::Init(hInst, hWnd);

	// デバイスオブジェクトを作成
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevice, NULL);
	if(FAILED(hr) || m_pDIDevice == NULL)
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = m_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// m_pDIDeviceアクセス権を獲得(入力制御開始)
	m_pDIDevice->Acquire();

	return hr;
}

//=============================================================================
// キーボード入力処理の開放
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	// 入力処理の開放
	CInput::Uninit();
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
void CInputKeyboard::Update(void)
{
	HRESULT hr;
	BYTE aKeyState[NUM_KEY_MAX];

	if(!m_pDIDevice)
	{
		return;
	}

	// デバイスからデータを取得
	hr = m_pDIDevice->GetDeviceState(sizeof(aKeyState), aKeyState);
	if(SUCCEEDED(hr))
	{
		for(int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];
			m_aKeyStateRepeat[nCntKey] = m_aKeyStateTrigger[nCntKey];

			if(aKeyState[nCntKey])
			{
				m_aKeyStateRepeatCnt[nCntKey]++;
				if(m_aKeyStateRepeatCnt[nCntKey] >= COUNT_WAIT_REPEAT)
				{
					m_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
				}
			}
			else
			{
				m_aKeyStateRepeatCnt[nCntKey] = 0;
				m_aKeyStateRepeat[nCntKey] = 0;
			}

			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		m_pDIDevice->Acquire();
	}
}

//=============================================================================
// キーボードデータ取得(プレス)
//=============================================================================
BOOL CInputKeyboard::GetKeyPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? TRUE: FALSE;
}

//=============================================================================
// キーボードデータ取得(トリガー)
//=============================================================================
BOOL CInputKeyboard::GetKeyTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? TRUE: FALSE;
}

//=============================================================================
// キーボードデータ取得(リリース)
//=============================================================================
BOOL CInputKeyboard::GetKeyRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? TRUE: FALSE;
}

//=============================================================================
// キーボードデータ取得(リピート)
//=============================================================================
BOOL CInputKeyboard::GetKeyRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) ? TRUE: FALSE;
}

//=============================================================================
// キーボードデータ削除(トリガー)
//=============================================================================
void CInputKeyboard::FlushKeyTrigger(int nKey)
{
	m_aKeyStateTrigger[nKey] = 0;
}



//*****************************************************************************
//   マウスクラス処理
//*****************************************************************************
//=======================================================================================
//   コンストラクタ
//=======================================================================================
CInputMouse::CInputMouse()
{
}

//=======================================================================================
//   デストラクタ
//=======================================================================================
CInputMouse::~CInputMouse()
{
}

//=======================================================================================
//   マウスの初期化処理
//=======================================================================================
HRESULT CInputMouse::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// 入力処理の初期化
	CInput::Init(hInst, hWnd);

	// デバイスオブジェクトを作成
	hr = m_pDInput->CreateDevice(GUID_SysMouse, &m_pDIDevice, NULL);
	if (FAILED(hr) || m_pDIDevice == NULL)
	{
		MessageBox(hWnd, "マウスがねぇ！", "警告!", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = m_pDIDevice->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マウスのデータフォーマットがねぇ！", "警告!", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他）
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マウスの協調モードを設定がねぇ！", "警告!", MB_ICONWARNING);
		return hr;
	}

	// デバイスの設定
	{
		DIPROPDWORD dipdw;

		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = DIPROPAXISMODE_REL;

		hr = m_pDIDevice->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
	}

	// マウスへのアクセス権を獲得（入力制御開始）
	m_pDIDevice->Acquire();

	return hr;
}

//=======================================================================================
//   マウスの開放処理
//=======================================================================================
void CInputMouse::Uninit(void)
{
	// 入力処理の開放
	CInput::Uninit();
}

//=======================================================================================
//   マウスの更新処理
//=======================================================================================
void CInputMouse::Update(void)
{
	HRESULT hr;
	DIMOUSESTATE2 mouseState;

	if (!m_pDIDevice)
	{
		return;
	}

	// デバイスからデータを取得
	hr = m_pDIDevice->GetDeviceState(sizeof(mouseState), &mouseState);
	if (SUCCEEDED(hr))
	{
		m_mouseStateTrigger.lX = ((m_mouseState.lX ^ mouseState.lX) & mouseState.lX);
		m_mouseStateTrigger.lY = ((m_mouseState.lY ^ mouseState.lY) & mouseState.lY);
		m_mouseStateTrigger.lZ = ((m_mouseState.lZ ^ mouseState.lZ) & mouseState.lZ);

		for (int nCntKey = 0; nCntKey < 8; nCntKey++)
		{
			m_mouseStateTrigger.rgbButtons[nCntKey] =
				((m_mouseState.rgbButtons[nCntKey] ^ m_mouseState.rgbButtons[nCntKey]) & m_mouseState.rgbButtons[nCntKey]);
		}

		m_mouseState = mouseState;

		// スクリーン座標を取得
		GetCursorPos(&m_posMouseWorld);
	}
	else
	{
		m_pDIDevice->Acquire();
	}
}

//=======================================================================================
//   マウスデータ取得（左プレス）
//=======================================================================================
BOOL CInputMouse::GetLeftPress(void)
{
	return (m_mouseState.rgbButtons[0] & 0x80) ? TRUE : FALSE;
}

//=======================================================================================
//   マウスデータ取得（左トリガー）
//=======================================================================================
BOOL CInputMouse::GetLeftTrigger(void)
{
	return (m_mouseStateTrigger.rgbButtons[0] & 0x80) ? TRUE : FALSE;
}

//=======================================================================================
//   マウスデータ取得（右プレス）
//=======================================================================================
BOOL CInputMouse::GetRightPress(void)
{
	return (m_mouseState.rgbButtons[1] & 0x80) ? TRUE : FALSE;
}

//=======================================================================================
//   マウスデータ取得（右トリガー）
//=======================================================================================
BOOL CInputMouse::GetRightTrigger(void)
{
	return (m_mouseStateTrigger.rgbButtons[1] & 0x80) ? TRUE : FALSE;
}

//=======================================================================================
//   マウスデータ取得（中央プレス）
//=======================================================================================
BOOL CInputMouse::GetCenterPress(void)
{
	return (m_mouseState.rgbButtons[2] & 0x80) ? TRUE : FALSE;
}


//=======================================================================================
//   マウスデータ取得（中央トリガー）
//=======================================================================================
BOOL CInputMouse::GetCenterTrigger(void)
{
	return (m_mouseStateTrigger.rgbButtons[2] & 0x80) ? TRUE : FALSE;
}

//=======================================================================================
//   マウスデータ取得（X軸移動）
//=======================================================================================
LONG CInputMouse::GetAxisX(void)
{
	return m_mouseState.lX;
}

//=======================================================================================
//   マウスデータ取得（Y軸移動）
//=======================================================================================
LONG CInputMouse::GetAxisY(void)
{
	return m_mouseState.lY;
}

//=======================================================================================
//   マウスデータ取得（Z軸移動）(マウスホイル)
//=======================================================================================
LONG CInputMouse::GetAxisZ(void)
{
	return m_mouseState.lZ;
}

//=======================================================================================
//   デストラクタ
//=======================================================================================
POINT *CInputMouse::GetPosWorld(void)
{
	return &m_posMouseWorld;
}