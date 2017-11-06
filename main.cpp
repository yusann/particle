//-------------------------------------------------------------------------------------------------------------------------------------------------------
//   �w�b�_�t�@�C��
//-------------------------------------------------------------------------------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME        "�G�f�B�^�["            // �E�B���h�E�N���X�̖��O
#define WINDOW_NAME       "���[�V�����G�f�B�^�["  // �E�B���h�E�̖��O�i�^�C�g�����j

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------------------------------------------------------------------------------------------
//   ���C���֐�
//-----------------------------------------------------------------------------------------------------------------------------------------------------
int APIENTRY WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow )
{
	// �ϐ��錾
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

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�B���h�E���W���v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�̍쐬
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

	// �}�l�[�W���[�̐���
	pManager = new CManager;

	// �}�l�[�W���[�̏����������i�f�o�C�X�Ȃǁj
	if (FAILED(pManager->Init(hInstance, hWnd, true)))
	{
		return -1;
	}

	// ����\��ݒ�
	timeBeginPeriod(1);

	// �t���[���J�E���g������
	dwCurrentTime =
		dwFrameCount = 0;
	dwExecLastTime =
		dwFPSLastTime = timeGetTime();

	// �E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ���b�Z�[�W���[�v
	for(;;)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				// �}�l�[�W���[�̍X�V����
				pManager->Update();

				// �}�l�[�W���[�̕`�揈��
				pManager->Draw();

				dwFrameCount++;
			}
		}
	}

	// �}�l�[�W���[�̔j��
	if (pManager != NULL) {         // �����_��
		pManager->Uninit();
		pManager = NULL;
	}

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// ����\��߂�
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
//  �E�B���h�E�v���V�[�W��
//  (CALLBACK�FWin32API�֐����Ăяo�����̋K��)
//  hWnd     �F�E�B���h�E�̃n���h��
//  uMsg     �F���b�Z�[�W�̎��ʎq
//  wParam   �F���b�Z�[�W�̍ŏ��̃p�����[�^
//  lParam   �F���b�Z�[�W�̂Q�Ԗڂ̃p�����[�^
//-------------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CManager::WndProc(hWnd, uMsg, wParam, lParam);
	switch( uMsg )
	{
	case WM_DESTROY:                   // �E�B���h�E�j��
		PostQuitMessage(0);            // "WM_QUIT"���b�Z�[�W
		break;

	case WM_KEYDOWN:                                            // �I������
		if( wParam == VK_ESCAPE )                               // ESC�L�[������
		{
			UINT nID = MessageBox( NULL , "�I�����܂����H" , "�I��" , MB_YESNO | MB_DEFBUTTON2 );         // �I�����郁�b�Z�[�W
			if( nID == IDYES )
			{
				DestroyWindow( hWnd );                                                                    // �I������
			}
		}
		break;

	default:
		break;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}