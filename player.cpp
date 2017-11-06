//*****************************************************************************
//   �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "sceneModelX.h"
#include "sceneMotionPartsX.h"
#include "sceneBillboard.h"
#include "player.h"
#include "sceneMesh.h"
#include "meshField.h"
#include "input.h"

#include "camera.h"

//*****************************************************************************
//   �萔��`
//*****************************************************************************
#define MOVE (0.5f)

//=======================================================================================
//   �R���X�g���N�^
//=======================================================================================
CPlayer::CPlayer():CSceneMotionPartsX(CScene::OBJTYPE_PLAYER)
{
}

//=======================================================================================
//   �f�X�g���N�^
//=======================================================================================
CPlayer::~CPlayer()
{
}

//==================================================================================================================================================
// �쐬����
//==================================================================================================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *pSceneMD;			// �ϐ��錾
	pSceneMD = new CPlayer;		// ���I�m��
	pSceneMD->Init(pos);		// ������
	return pSceneMD;			// �l��Ԃ�
}

//=======================================================================================
//   ����������
//=======================================================================================
void CPlayer::Init(D3DXVECTOR3 pos)
{
	// �ϐ��̏�����
	m_Mode = MODE_NORMAL;
	m_pos = pos;
	m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Jump = 3.0f;
	m_Speed = 0.5f;


	// �e�̏������i���_�����j
	CSceneMotionPartsX::Init();

	// �^�C�v�̑��
	CScene::SetObjType(CScene::OBJTYPE_PLAYER);
}

//=======================================================================================
//   �I������
//=======================================================================================
void CPlayer::Uninit()
{
	CSceneMotionPartsX::Uninit();
}

//=======================================================================================
//   �X�V����
//=======================================================================================
void CPlayer::Update()
{
	ImGui();
	// �L�[����
	InputKey();

	m_Move.y -= 0.2f;

	switch (m_Mode)
	{
	case MODE_NORMAL: ModeNormal(); break;
	case MODE_MOVE:   ModeMove();   break;
	case MODE_ATTACK: ModeAttack(); break;
	case MODE_JUMP:   ModeJump();   break;
	default:
		assert("�v���C���[���[�h�G���[�I");
		break;
	}

	// �ړ�����
	m_pos += m_Move;
	m_Move.x = 0.0f;
	m_Move.z = 0.0f;
	CMeshField *pMeshField = CManager::GetMeshField();
	if (pMeshField == NULL) { return; }
	float PosY = pMeshField->GetHeight(m_pos);
	if (m_pos.y < PosY)
	{
		m_Move.y = 0.0f;
		m_pos.y = PosY;
		if (m_Mode != MODE_ATTACK && m_Mode != MODE_MOVE)
		{
			m_Mode = MODE_NORMAL;
		}
	}
	// ���[�V�����̃��X�g�L�[�ɒB���� �� �W�����v���[�h�ł͂Ȃ� �� �U�����[�h�ł͂Ȃ���
	if (m_LastKye)
	{
		// �j���[�g�������[�h�ύX
		m_Mode = MODE_NORMAL;

		// ���[�V�����̃��X�g�L�[�̃t���O�N���A
		m_LastKye = false;
	}
	CSceneMotionPartsX::SetMotion((int)m_Mode);
	CSceneMotionPartsX::Update();

	// ���[�h�̐؂�ւ�
	if (m_Mode == MODE_MOVE)
	{
		m_Mode = MODE_NORMAL;
	}


}

//=======================================================================================
//   �`�揈��
//=======================================================================================
void CPlayer::Draw()
{

	LPDIRECT3DDEVICE9 pDevice = NULL;                // �G���[�`�F�b�N�̂���NULL������
	pDevice = CManager::GetRenderer()->GetDevice();                           // �f�o�C�X�̃|�C���^���擾

	if (pDevice == NULL)                            // �G���[�`�F�b�N
	{
		MessageBox(NULL, "Init��pDevece��NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return;
	}
	CSceneMotionPartsX::Draw();
}

//=======================================================================================
//   �L�[���菈��
//=======================================================================================
void CPlayer::InputKey(void)
{
	// �U����Ԏ����s���Ȃ�
	if (m_Mode == MODE_ATTACK) { return; }

	// �J�������擾
	CCamera* pCamera = CManager::GetCamera();
	if (pCamera == NULL) { return; }
	CCamera::CAMERA Camera = pCamera->GetCamera();

	// �J�����Ƃ̃x�N�g��
	D3DXVECTOR3 Vector;
	Vector.x = Camera.posEye.x - m_pos.x;
	Vector.z = Camera.posEye.z - m_pos.z;
	Vector.y = 0.0f;

	// �J�����Ƃ̊p�x
	float Angle = -atan2(Vector.z, Vector.x) + D3DX_PI*0.5f;

	// �O�i�L�[����
	bool Input_MoveUP = false;

	// �O�i
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_S))
	{
		m_Mode = MODE_MOVE;
		Angle += D3DX_PI;

		// �߂����[�h�ŉ�
		if (m_Rot.y - Angle >  D3DX_PI) { m_Rot.y -= D3DX_PI * 2; }
		if (m_Rot.y - Angle < -D3DX_PI) { m_Rot.y += D3DX_PI * 2; }

		// �p�x�v�Z
		m_Rot.y += (Angle - m_Rot.y)*0.1f;

		// �O�i�L�[�t���O
		Input_MoveUP = true;
	}
	// ���
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_W))
	{
		m_Mode = MODE_MOVE;
		if (m_Rot.y - Angle >  D3DX_PI) { m_Rot.y -= D3DX_PI * 2; }
		if (m_Rot.y - Angle < -D3DX_PI) { m_Rot.y += D3DX_PI * 2; }
		m_Rot.y += (Angle - m_Rot.y)*0.1f;
	}
	// ���ړ�
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_D))
	{
		m_Mode = MODE_MOVE;

		// �O�i�L�[�������Ă���Ƃ�
		if (Input_MoveUP)
		{
			Angle -= D3DX_PI*0.5f;
		}
		else
		{
			Angle += D3DX_PI*0.5f;
		}
		if (m_Rot.y - Angle >  D3DX_PI) { m_Rot.y -= D3DX_PI * 2; }
		if (m_Rot.y - Angle < -D3DX_PI) { m_Rot.y += D3DX_PI * 2; }
		m_Rot.y += (Angle - m_Rot.y)*0.1f;
	}
	// �E�ړ�
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_A))
	{
		m_Mode = MODE_MOVE;
		if (Input_MoveUP)
		{
			Angle += D3DX_PI*0.5f;
		}
		else
		{
			Angle -= D3DX_PI*0.5f;
		}
		if (m_Rot.y - Angle >  D3DX_PI) { m_Rot.y -= D3DX_PI * 2; }
		if (m_Rot.y - Angle < -D3DX_PI) { m_Rot.y += D3DX_PI * 2; }
		m_Rot.y += (Angle - m_Rot.y)*0.1f;
	}

	if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_B))
	{
		m_Mode = MODE_ATTACK;
	}

	// ���ړ�
	if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_SPACE))
	{
		m_Mode = MODE_JUMP;
		m_Move.y = m_Jump;
	}
}

//=======================================================================================
//   �m�[�}�����[�h����
//=======================================================================================
void CPlayer::ModeNormal()
{

}

//=======================================================================================
//   �ړ����[�h����
//=======================================================================================
void CPlayer::ModeMove()
{
	m_Move.x = (float)cos(-m_Rot.y + D3DXToRadian(-90))*m_Speed;
	m_Move.z = (float)sin(-m_Rot.y + D3DXToRadian(-90))*m_Speed;
}

//=======================================================================================
//   �U�����[�h����
//=======================================================================================
void CPlayer::ModeAttack()
{

}

//=======================================================================================
//   �W�����v���[�h����
//=======================================================================================
void CPlayer::ModeJump()
{

}

void CPlayer::ImGui()
{
	// �Z�[�u����
	if (ImGui::CollapsingHeader("Player Data"))
	{
		ImGui::Text("Mode %d", (int)m_Mode);
		ImGui::InputFloat("MoveSpeed", &m_Speed, 0.01f);
		ImGui::InputFloat("Jump", &m_Jump, 0.01f);
	}
}