//*****************************************************************************
//   �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "2D_sample.h"
#include "texture.h"

//*****************************************************************************
//   �萔��`
//*****************************************************************************
#define POLYGON_WIDTH  (250.0f)
#define POLYGON_HEIGHT (400.0f)

//=======================================================================================
//   �R���X�g���N�^
//=======================================================================================
C2D_sample::C2D_sample():CScene2D(7)
{
}

//=======================================================================================
//   �f�X�g���N�^
//=======================================================================================
C2D_sample::~C2D_sample()
{
}

//=======================================================================================
//   ����������
//=======================================================================================
void C2D_sample::Init()
{
	// �����o�ϐ��̏�����
	m_scl = D3DXVECTOR3(POLYGON_WIDTH, POLYGON_HEIGHT, 0.0f);

	// �e�̏�����
	CScene2D::Init();

	// �e�N�X�`���Z�b�g
	m_pTexture = CTexture::GetTexture((int)CTexture::TEXTYPE_PLAYER);

	// �^�C�v�̑��
	CScene::SetObjType(CScene::OBJTYPE_NONE);
}

//=======================================================================================
//   �I������
//=======================================================================================
void C2D_sample::Uninit()
{
	CScene2D::Uninit();
}

//=======================================================================================
//   �X�V����
//=======================================================================================
void C2D_sample::Update()
{
}

//=======================================================================================
//   �`�揈��
//=======================================================================================
void C2D_sample::Draw()
{
	CScene2D::Draw();
}
//=======================================================================================
//   �`�揈��
//=======================================================================================
void C2D_sample::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==================================================================================================================================================
// �쐬����
//==================================================================================================================================================
C2D_sample *C2D_sample::Create(D3DXVECTOR3 pos)
{
	C2D_sample *pScene2D;                            // �ϐ��錾
	pScene2D = new C2D_sample;                       // ���I�m��
	pScene2D->SetPos(pos);
	pScene2D->Init();                            // ������
	return pScene2D;                             // �l��Ԃ�
}