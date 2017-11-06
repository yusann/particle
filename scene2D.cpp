//*****************************************************************************
//   �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"

#include "scene.h"
#include "scene2D.h"

//=======================================================================================
//   �R���X�g���N�^
//=======================================================================================
CScene2D::CScene2D(int nPriority) :CScene(nPriority)
{
}

//=======================================================================================
//   �f�X�g���N�^
//=======================================================================================
CScene2D::~CScene2D()
{
}

//=======================================================================================
//   ����������
//=======================================================================================
void CScene2D::Init()
{
	// ���_�ݒ�
	MakeVex();
}

//=======================================================================================
//   �I������
//=======================================================================================
void CScene2D::Uninit()
{
	SAFE_RELEASE( m_pVtxBuff );      // ���_�o�b�t�@�̔j��
	CScene::Release();
}

//=======================================================================================
//   �X�V����
//=======================================================================================
void CScene2D::Update()
{
}

//=======================================================================================
//   �`�揈��
//=======================================================================================
void CScene2D::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return;
	}
	
	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource( 0,
							  m_pVtxBuff,              // �X�g���[���̂��ƂɂȂ钸�_�̃o�b�t�@�̎n�_
							  0,                       // �I�t�Z�b�g�i�o�C�g�j
							  sizeof(VERTEX_2D));      // ��̒��_�f�[�^�̃T�C�Y�i�X�g���C�h�ʁj

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF( FVF_VERTEX_2D );

	// �`�撼�O�Ƀe�N�X�`�����Z�b�g�i�e�N�X�`���̐ݒ�j
	pDevice->SetTexture( 0, m_pTexture );

	// �|���S���̕`��
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,        // �v���~�e�B�u�̎��
							0,                          // �I�t�Z�b�g�i���_���j
							NUM_POLYGON );              // �v���~�e�B�u�̐��i�|���S���̐��j
}

//=======================================================================================
// �|���S���̒��_�ݒ�
//=======================================================================================
void CScene2D::MakeVex(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer( sizeof( VERTEX_2D ) * NUM_VERTEX,           // �쐬���������_�o�b�t�@�̃T�C�Y�i��̒��_*���_���j
								 D3DUSAGE_WRITEONLY,                         // �������ނ������Ȃ��i�`�F�b�N���Ȃ��j
								 FVF_VERTEX_2D,                              // �ǂ�Ȓ��_�ŏ����́i0�ɂ��Ă�OK�j
								 D3DPOOL_MANAGED,                            // �������Ǘ������C���ɂ���
								 &m_pVtxBuff,
								 NULL );

	// ���_����ݒ�
	// ���_���i�[�p�^���o�b�t�@�̐錾
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����i0,0���L������ƑS�������b�N�j
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�i 2D���W�E�E��� �j
	pVtx[0].pos = D3DXVECTOR3( m_pos.x          , m_pos.y           , 0.0f);                 // ����̍��W
	pVtx[1].pos = D3DXVECTOR3( m_pos.x + m_scl.x, m_pos.y           , 0.0f);                 // �E��̍��W
	pVtx[2].pos = D3DXVECTOR3( m_pos.x          , m_pos.y + m_scl.y, 0.0f);                 // �����̍��W
	pVtx[3].pos = D3DXVECTOR3( m_pos.x + m_scl.x, m_pos.y + m_scl.y, 0.0f);                 // �E���̍��W

	// rhw�̐ݒ�i�K��1.0f�j
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�i0~255�̐����l�j
	pVtx[0].color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );  // ����̐F
	pVtx[1].color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );  // �E��̐F
	pVtx[2].color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );  // �����̐F
	pVtx[3].color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );  // �E���̐F

	// ���_�f�[�^��UV�f�[�^�̒ǉ�
	pVtx[0].tex = D3DXVECTOR2( 0.0f , 0.0f );                    // �����UV���W
	pVtx[1].tex = D3DXVECTOR2( 1.0f , 0.0f );                    // �E���UV���W
	pVtx[2].tex = D3DXVECTOR2( 0.0f , 1.0f );                    // ������UV���W
	pVtx[3].tex = D3DXVECTOR2( 1.0f , 1.0f );                    // �E����UV���W

	// �����J����
	m_pVtxBuff->Unlock();
}