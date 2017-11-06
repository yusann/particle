//*****************************************************************************
//   �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"

#include "camera.h"
#include "input.h"

//*****************************************************************************
//   �萔��`
//*****************************************************************************
#define CAMERA_MOVE     (0.2f)                    // �ړ���
#define CAMERA_ANGLE    (0.05f)                   // ��]��
#define CAMERA_POSY     (15.0f)                   // ����Y���W
#define CAMERA_POSZ     (-20.0f)                  // ����Z���W
#define CAMERA_ZOOM     (0.01f)                   // �Y�[�����x

//*****************************************************************************
//   �ÓI�����o�ϐ�
//*****************************************************************************
CCamera::CAMERA CCamera::m_pCamera;

//=======================================================================================
//   �R���X�g���N�^
//=======================================================================================
CCamera::CCamera()
{
}

//=======================================================================================
//   �f�X�g���N�^
//=======================================================================================
CCamera::~CCamera()
{
}

//=======================================================================================
//   ����������
//=======================================================================================
void CCamera::Init()
{
	// �J�����̏�����
	m_pCamera.vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);                 // ������iY���̏�j
	m_posMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);                 // �ړ���
	m_rot = D3DXVECTOR3(0.3f, -1.5f, 0.0f);                         // �p�x
	m_rotX = 0.0f;
	m_rotY = 0.0f;
	m_length = -CAMERA_POSZ;
	m_fZoom = D3DX_PI / 3.0f;                                  // ����p�i60���j
	m_pCamera.posEye.x = cos(m_rot.y) * cos(m_rot.x) * m_length + m_posMove.x;
	m_pCamera.posEye.z = sin(m_rot.y) * cos(m_rot.x) * m_length + m_posMove.z;
	m_pCamera.posEye.y = sin(m_rot.x) * m_length + m_posMove.y;
	m_pCamera.posAt = m_posMove;
	D3DXMatrixIdentity(&m_pCamera.mtxView);                            // ���[���h�̒��g��������
}

//=======================================================================================
//   �I������
//=======================================================================================
void CCamera::Uninit()
{
	// �J�����̏�����
	m_pCamera.vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);                 // ������iY���̏�j
	m_posMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);                 // �ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);                 // �p�x
	m_length = CAMERA_POSZ;
	m_fZoom = D3DX_PI / 3.0f;                                  // ����p�i60���j
	m_pCamera.posEye.x = cos(m_rot.y) * cos(m_rot.x) * m_length + m_posMove.x;
	m_pCamera.posEye.z = sin(m_rot.y) * cos(m_rot.x) * m_length + m_posMove.z;
	m_pCamera.posEye.y = sin(m_rot.x) * m_length + m_posMove.y;
	m_pCamera.posAt = m_posMove;
	D3DXMatrixIdentity(&m_pCamera.mtxView);                            // ���[���h�̒��g��������
}

//=======================================================================================
//   �X�V����
//=======================================================================================
void CCamera::Update()
{
	if (ImGui::IsMouseHoveringAnyWindow())
	{
		return;
	}
	// �x�N�g���f�[�^�̐錾
	float fRad = sqrt(((m_pCamera.posAt.x - m_pCamera.posEye.x) * (m_pCamera.posAt.x - m_pCamera.posEye.x)) +
		((m_pCamera.posAt.z - m_pCamera.posEye.z) * (m_pCamera.posAt.z - m_pCamera.posEye.z)));   // �����̐錾�y�юZ�o

	// �ړ��ʂ̃x�N�g���錾
	D3DXVECTOR3 work;
	D3DXVECTOR3 move;

	// Y�������̒����i�J���������Ƃ��������j
	work.x = m_pCamera.posAt.x - m_pCamera.posEye.x;    // X��
	work.y = m_pCamera.posAt.y - m_pCamera.posEye.y;                                      // Y��
	work.z = m_pCamera.posAt.z - m_pCamera.posEye.z;    // Z��

	// �ړ��ʃm�[�}���C�Y�i�������F�����͂��̂܂܁A�傫�����P�ɂ���j
	D3DXVec3Normalize(&move, &work);

	// �L�[���� ///////////////////////////////////////////////////////////////////////////////
	m_length -= CManager::GetInputMouse()->GetAxisZ() * 0.02f;
	if (CManager::GetInputMouse()->GetCenterPress())
	{
		m_length += CManager::GetInputMouse()->GetAxisZ() * 0.01f;
		float speed = CManager::GetInputMouse()->GetAxisX()*0.1f;

		m_posMove.x -= move.z * speed;
		m_posMove.z += move.x * speed;

		speed = CManager::GetInputMouse()->GetAxisY()*0.1f;
		m_posMove.y -= move.y * speed;
	}
	if (m_length < 0.0f)
	{
		m_length = 0.0f;
	}
	if (CManager::GetInputMouse()->GetLeftPress())
	{
		m_rot.y -= CManager::GetInputMouse()->GetAxisX() * 0.01f;
		m_rot.x += CManager::GetInputMouse()->GetAxisY() * 0.01f;
		if (m_rot.x > 3.141592f*0.5f)
		{
			m_rot.x = 3.141592f*0.5f;
		}
		if (m_rot.x < -3.141592f*0.5f)
		{
			m_rot.x = -3.141592f*0.5f;
		}
	}
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_X))
	{
		m_length = -CAMERA_POSZ;
		m_rot = D3DXVECTOR3(1.0f, 0.0f, 0.0f);                         // �p�x
		m_posMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_pCamera.posEye.x = cos(m_rot.y) * cos(m_rot.x) * m_length + m_posMove.x;
	m_pCamera.posEye.z = sin(m_rot.y) * cos(m_rot.x) * m_length + m_posMove.z;
	m_pCamera.posEye.y = sin(m_rot.x) * m_length + m_posMove.y;
	m_pCamera.posAt = m_posMove;
}

//=======================================================================================
//   �Z�b�g����
//=======================================================================================
void CCamera::Set(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return;
	}

	// �v���W�F�N�V�����s��
	D3DXMATRIX mtxProj;
	D3DXMatrixPerspectiveFovLH(&mtxProj,                               // Fov�͉�p
		m_fZoom,                          // ��p�i����p�j60�x�ɂ��邽�߃΁��R
		(float)SCREEN_WIDTH / SCREEN_HEIGHT,    // �A�X�y�N�g��ifloat�Ōv�Z����Afloat>int�Ȃ̂ŕЕ���OK�j
		1.0f,                                   // near �K��0.0f���傫������  �ǂ�����`�悷�邩
		10000.0f);                                // far �ǂ��܂ŕ`�悷�邩
	m_pCamera.mtxProj = mtxProj;
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);

	// �r���[�s��̍쐬 LH������W�n
	D3DXMatrixLookAtLH(&m_pCamera.mtxView, &m_pCamera.posEye, &m_pCamera.posAt, &m_pCamera.vecUp);    // Eye,At,Up�̏�񂩂�r���[�s��(mtxView)�����֐�

	// �r���[�s��Z�b�g
	pDevice->SetTransform(D3DTS_VIEW, &m_pCamera.mtxView);
}