//=============================================================================
//
// �}�l�[�W���[
// Author : YUUSAN KA
//--------------------------
// DATA : 2016/11/16
// UPDATA : 2016/11/30		�L�[�{�[�h���͂̒ǉ�
//
//=============================================================================
//*****************************************************************************
//   ��d��`�h�~
//*****************************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include <tchar.h>

//*****************************************************************************
//   �O���錾
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
//   �N���X�̒�`
//*****************************************************************************
class CManager
{
public:
	CManager();                                   // �f�t�H���g�R���X�g���N�^
	~CManager();                                  // �f�X�g���N�^
	
	HRESULT Init( HINSTANCE hInstance,             // ����������
			   HWND      hWnd,
			   BOOL      bWindow);              // �t���X�N���[����ONOFF
	void Uninit(void);                             // �I������
	void Update(void);                             // �X�V����
	void Draw(void);                               // �`�揈��

	static void WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
		if (ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return;
	}

	static CRenderer *GetRenderer(void);               // �����_���[�̎擾
	static CInputKeyboard *GetInputKeyboard(void);     // �L�[�{�[�h���͂̎擾
	static CInputMouse *GetInputMouse(void);
	static CCamera *GetCamera(void);
	static CLight *GetLight(void);
	static CMeshField *GetMeshField(void);
	static CSound    *GetSound() { return m_pSound; }

private:
	static CRenderer *m_pRenderer;            // �����_���[
	static CInputKeyboard *m_pInputKeyboard;  // �L�[�{�[�h����
	static CInputMouse *m_pInputMouse;
	static CCamera *m_pCamera;
	static CLight *m_pLight;
	static CMeshField *m_pMeshField;
	static CSound *m_pSound;
};

#endif