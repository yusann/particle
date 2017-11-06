//=============================================================================
//
// �����_������
// Author : YUUSAN KA
//--------------------------
// DATA : 2016/11/09
//
//=============================================================================
//*****************************************************************************
//   ��d��`�h�~
//*****************************************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
// ���C�u�����t�@�C��
// [�\���v���p�e�B]->[�����J�[]->[����]->[�ǉ��̈ˑ��t�@�C��]�ɋL�q���Ă��\
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")         // �V�X�e�������擾�ɕK�v
#pragma comment(lib,"dinput8.lib")

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_POLYGON       (2)                                  // �|���S����
#define NUM_VERTEX        (4)                                  // ���_��

#define FVF_VERTEX_2D     (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)                // ���_�t�H�[�}�b�g�i ���W�ϊ��ςݒ��_���W=>2D���W | ���_�J���[ | UV��ǉ� �j
#define FVF_VERTEX_3D     (D3DFVF_XYZ  | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX1)  // ���_�t�H�[�}�b�g      �i ���W�ϊ����钸�_���W=>3D���W | ���_�J���[| �@��| UV��ǉ��j

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
//  ���_���i��̒��_�t�H�[�}�b�g�ƍ\�������킹�邱�Ɓj
typedef struct
{
	D3DXVECTOR3 pos;        // XYZ�̐錾
	float rhw;              // RHW�̐錾
	D3DCOLOR color;         // �F�̐錾
	D3DXVECTOR2 tex;        // ���_�f�[�^�\���̂ɂ�UV�ǉ��i�K���Ō�ɒǉ��j2��XY, 3��XYZ
}VERTEX_2D;

typedef struct
{
	// ���Ԃ��K�����I�I
	D3DXVECTOR3 pos;        // XYZ�̐錾
	D3DXVECTOR3 normal;     // �@��(�K��position�̉�)
	D3DCOLOR color;         // �F�̐錾
	D3DXVECTOR2 tex;        // ���_�f�[�^�\���̂ɂ�UV�ǉ��i�K���Ō�ɒǉ��j2��XY, 3��XYZ
}VERTEX_3D;

//*****************************************************************************
//   �N���X�̒�`
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();                                   // �f�t�H���g�R���X�g���N�^
	~CRenderer();                                  // �f�X�g���N�^
	
	HRESULT Init(HWND hWnd, BOOL bWindow);            // ����������
	void Uninit(void);                             // �I������
	bool DrawBegin(void);
	void DrawEnd(void);

	LPDIRECT3DDEVICE9 GetDevice(void);             // �f�o�C�X�̎擾����

private:
	LPDIRECT3D9				m_pD3D;                // Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3DDevice;          // Device�I�u�W�F�N�g(�`��ɕK�v)
	bool					m_bDraw;
};

#endif