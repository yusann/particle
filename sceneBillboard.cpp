//*****************************************************************************
//   �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"

#include "scene.h"
#include "sceneBillboard.h"
#include "camera.h"

//=======================================================================================
//   �R���X�g���N�^
//=======================================================================================
CSceneBillboard::CSceneBillboard(int nPriority) :CScene(nPriority)
{
}

//=======================================================================================
//   �f�X�g���N�^
//=======================================================================================
CSceneBillboard::~CSceneBillboard()
{
}

//=======================================================================================
//   ����������
//=======================================================================================
void CSceneBillboard::Init()
{
	// ���_�ݒ�
	MakeVex();
}

//=======================================================================================
//   �I������
//=======================================================================================
void CSceneBillboard::Uninit()
{
	SAFE_RELEASE( m_pVtxBuff );      // ���_�o�b�t�@�̔j��
	CScene::Release();
}

//=======================================================================================
//   �X�V����
//=======================================================================================
void CSceneBillboard::Update()
{
}

//=======================================================================================
//   �`�揈��
//=======================================================================================
void CSceneBillboard::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return;
	}

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		m_pVtxBuff,              // �X�g���[���̂��ƂɂȂ钸�_�̃o�b�t�@�̎n�_
		0,                       // �I�t�Z�b�g�i�o�C�g�j
		sizeof(VERTEX_3D));      // ��̒��_�f�[�^�̃T�C�Y�i�X�g���C�h�ʁj

								 // ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �`�撼�O�Ƀe�N�X�`�����Z�b�g�i�e�N�X�`���̐ݒ�j
	pDevice->SetTexture(0, m_pTexture);

	// �J�����̎擾
	CCamera::CAMERA camera;
	camera = CCamera::GetCamera();

	// �ϊ��s��̐錾
	D3DXMATRIX mtxRot;             // ���[�J����]
	D3DXMATRIX mtxPos;             // ���[�J�����W
	D3DXMATRIX mtxWorld;           // ���[���h���
	D3DXMATRIX mtxViewInverse;     // �]�u�s��
	D3DXMATRIX mtxViewCamera = camera.mtxView;      // �J�����s��̎擾

													  // ���[�J����]�̑��
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		0.0f,
		0.0f,
		0.0f);

	// ���[�J�����W�̑��
	D3DXMatrixTranslation(&mtxPos,
		m_pos.x,
		m_pos.y,
		m_pos.z);

	// �t�s��̍쐬
	D3DXMatrixTranspose(&mtxViewInverse, &mtxViewCamera);

	// ���s�ړ������������i�]�u�O�j
	mtxViewInverse._14 = 0.0f;
	mtxViewInverse._24 = 0.0f;
	mtxViewInverse._34 = 0.0f;

	// ���[���h��񏈗�
	D3DXMatrixIdentity(&mtxWorld);                       // ���[���h�̒��g��������

	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);   // ���[���h��]�̑��
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxPos);   // ���[���h���W�̑��

	mtxWorld = mtxViewInverse * mtxWorld;
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);       // ���[���h���Z�b�g

	// ���C�g�̐ݒ�iOFF�j
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,        // �v���~�e�B�u�̎��
		0,                          // �I�t�Z�b�g�i���_���j
		NUM_POLYGON);              // �v���~�e�B�u�̐��i�|���S���̐��j

	// ���C�g�̐ݒ�iON�j
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=======================================================================================
// �|���S���̒��_�ݒ�
//=======================================================================================
void CSceneBillboard::MakeVex(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,           // �쐬���������_�o�b�t�@�̃T�C�Y�i��̒��_*���_���j
		D3DUSAGE_WRITEONLY,                         // �������ނ������Ȃ��i�`�F�b�N���Ȃ��j
		FVF_VERTEX_3D,                              // �ǂ�Ȓ��_�ŏ����́i0�ɂ��Ă�OK�j
		D3DPOOL_MANAGED,                            // �������Ǘ������C���ɂ���
		&m_pVtxBuff,
		NULL);

	// ���_����ݒ�
	// ���_���i�[�p�^���o�b�t�@�̐錾
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����i0,0���L������ƑS�������b�N�j
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�i 2D���W�E�E��� �j
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_scl.x, m_pos.y + m_scl.y, 0.0f);                 // ����̍��W
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_scl.x, m_pos.y + m_scl.y, 0.0f);                 // �E��̍��W
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_scl.x, m_pos.y - m_scl.y, 0.0f);                 // �����̍��W
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_scl.x, m_pos.y - m_scl.y, 0.0f);                 // �E���̍��W

	// ���_�@���̐ݒ�
	pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);


	// ���_�J���[�̐ݒ�i0~255�̐����l�j
	pVtx[0].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // ����̐F
	pVtx[1].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // �E��̐F
	pVtx[2].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // �����̐F
	pVtx[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // �E���̐F

														// ���_�f�[�^��UV�f�[�^�̒ǉ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);                    // �����UV���W
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);                    // �E���UV���W
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);                    // ������UV���W
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);                    // �E����UV���W

															  // �����J����
	m_pVtxBuff->Unlock();
}