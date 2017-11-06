//*****************************************************************************
//   �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"

#include "scene.h"
#include "sceneMesh.h"
#include "meshCube.h"

//=======================================================================================
//   �R���X�g���N�^
//=======================================================================================
CMeshCube::CMeshCube() :CSceneMesh(1)
{
}

//=======================================================================================
//   �f�X�g���N�^
//=======================================================================================
CMeshCube::~CMeshCube()
{
}

//==================================================================================================================================================
// �쐬����
//==================================================================================================================================================
CMeshCube *CMeshCube::Create(void)
{
	CMeshCube *pScene3D;                            // �ϐ��錾
	pScene3D = new CMeshCube;                       // ���I�m��
	pScene3D->Init();                            // ������
	return pScene3D;                             // �l��Ԃ�
}

//=======================================================================================
//   ����������
//=======================================================================================
void CMeshCube::Init(void)
{
	// �����o�ϐ��̏�����
	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_scl = D3DXVECTOR3(10.0f, 10.0f, 10.0f);

	// �v���C�x�[�g�ϐ��̏�����
	m_pIdxBuff = NULL;
	m_nPolygonNum = 16;
	m_nVexNum = 8;

	// ���_�ݒ�
	MakeVex();
	MakeBuff();

	// �e�N�X�`���Z�b�g
	m_pTexture = NULL;

	// �^�C�v�̑��
	CSceneMesh::Init();
}

//=======================================================================================
//   �I������
//=======================================================================================
void CMeshCube::Uninit()
{
	CSceneMesh::Uninit();
}

//=======================================================================================
//   �X�V����
//=======================================================================================
void CMeshCube::Update()
{
	m_rot.x += 0.01f;
}

//=======================================================================================
//   �`�揈��
//=======================================================================================
void CMeshCube::Draw()
{
	CSceneMesh::Draw();
}

//=======================================================================================
// �|���S���̒��_�ݒ�
//=======================================================================================
void CMeshCube::MakeVex(void)
{
	// �ϐ��錾
	int nCntHeight;     // �n�ʃ��[�v�p
	float fLength = sqrt((m_scl.x * m_scl.x) + (m_scl.z * m_scl.z)) * 0.5f;
	float fAngle = atan2(m_scl.z, m_scl.x);

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return;
	}

	// ���_�o�b�t�@�����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVexNum,           // �쐬���������_�o�b�t�@�̃T�C�Y�i��̒��_*���_���j
		D3DUSAGE_WRITEONLY,                                      // �������ނ������Ȃ��i�`�F�b�N���Ȃ��j
		FVF_VERTEX_3D,                                           // �ǂ�Ȓ��_�ŏ����́i0�ɂ��Ă�OK�j
		D3DPOOL_MANAGED,                                         // �������Ǘ������C���ɂ���
		&m_pVtxBuff,
		NULL);

	// ���_���i�[�p�^���o�b�t�@�̐錾
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����i0,0���L������ƑS�������b�N�j
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntHeight = 0; nCntHeight < 2; nCntHeight++, pVtx += 4)
	{
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(cosf(-fAngle + D3DX_PI) * fLength,		// X���W�̐ݒ�
			nCntHeight * m_scl.y- (m_scl.y*0.5f),						// Y���W�̐ݒ�
			sinf(-fAngle + D3DX_PI) * fLength);							// Z���W�̐ݒ�
		pVtx[1].pos = D3DXVECTOR3(cosf(fAngle) * fLength,					// X���W�̐ݒ�
			nCntHeight * m_scl.y - (m_scl.y*0.5f),					// Y���W�̐ݒ�
			sinf(fAngle) * fLength);									// Z���W�̐ݒ�
		pVtx[2].pos = D3DXVECTOR3(cosf(-fAngle) * fLength,					// X���W�̐ݒ�
			nCntHeight * m_scl.y - (m_scl.y*0.5f),					// Y���W�̐ݒ�
			sinf(-fAngle) * fLength);									// Z���W�̐ݒ�
		pVtx[3].pos = D3DXVECTOR3(cosf(fAngle + D3DX_PI) * fLength,			// X���W�̐ݒ�
			nCntHeight * m_scl.y - (m_scl.y*0.5f),					// Y���W�̐ݒ�
			sinf(fAngle + D3DX_PI) * fLength);							// Z���W�̐ݒ�

		pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);                          // �@���̐ݒ�
		pVtx[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);                          // �@���̐ݒ�
		pVtx[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);                          // �@���̐ݒ�
		pVtx[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);                          // �@���̐ݒ�

		pVtx[0].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);                       // �J���[�̐ݒ�
		pVtx[1].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);                       // �J���[�̐ݒ�
		pVtx[2].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);                       // �J���[�̐ݒ�
		pVtx[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);                       // �J���[�̐ݒ�

		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);            // �e�N�X�`�����W�̐ݒ�
		pVtx[1].tex = D3DXVECTOR2(0.0f, 1.0f);            // �e�N�X�`�����W�̐ݒ�
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);            // �e�N�X�`�����W�̐ݒ�
		pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);            // �e�N�X�`�����W�̐ݒ�
	}
	// �����J����
	m_pVtxBuff->Unlock();
}


//=======================================================================================
// �|���S���̃o�b�t�@�ݒ�
//=======================================================================================
void CMeshCube::MakeBuff(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return;
	}

	// �C���f�b�N�X�o�b�t�@�̍쐬
	HRESULT hr;
	hr = pDevice->CreateIndexBuffer(sizeof(WORD) * 18,        // ���_���iWORD��16Bid�j
									D3DUSAGE_WRITEONLY,              // �g�p�p�r�t���O�i�������ނ̂݁j
									D3DFMT_INDEX16,                  // �f�[�^�̃t�H�[�}�b�g�i32Bid�ݒ肵����16��32�ɕύX�j
									D3DPOOL_MANAGED,                 // �������̊Ǘ����@�i���C���j
									&m_pIdxBuff,              // �o�b�t�@�\�C���^�[�t�F�C�X�|�C���^�̃A�h���X
									NULL);

	// �C���f�b�N�X���i�[�p�^���o�b�t�@�̐錾
	WORD* pIdx;

	// ���b�N
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 4;
	pIdx[1] = 7;
	pIdx[2] = 5;
	pIdx[3] = 6;
	pIdx[4] = 6;
	pIdx[5] = 2;
	pIdx[6] = 5;
	pIdx[7] = 1;
	pIdx[8] = 4;
	pIdx[9] = 0;
	pIdx[10] = 7;
	pIdx[11] = 3;
	pIdx[12] = 6;
	pIdx[13] = 2;
	pIdx[14] = 2;
	pIdx[15] = 3;
	pIdx[16] = 1;
	pIdx[17] = 0;
/*
	pIdx[0] = 4;
	pIdx[1] = 5;
	pIdx[2] = 7;
	pIdx[3] = 6;
	pIdx[4] = 2;
	pIdx[5] = 6;
	pIdx[6] = 1;
	pIdx[7] = 5;
	pIdx[8] = 0;
	pIdx[9] = 4;
	pIdx[10] = 3;
	pIdx[11] = 7;
	pIdx[12] = 2;
	pIdx[13] = 6;
	pIdx[14] = 2;
	pIdx[15] = 1;
	pIdx[16] = 3;
	pIdx[17] = 0;*/

	// ���b�N���
	m_pIdxBuff->Unlock();
}
