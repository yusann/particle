//*****************************************************************************
//   �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "paticle.h"
#include "texture.h"
#include "camera.h"

//=======================================================================================
//   ����������
//=======================================================================================
void CPaticle::Init(void)
{
	m_nPolygonNum = 8;
	m_nVexNum     = 8;
	m_pos         = D3DXVECTOR3(0.0f,0.0f,0.0f);       // ���W
	m_scl         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // �T�C�Y
	m_rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_color       = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);// �F

	// ���_�ݒ�
	MakeVex();
	MakeBuff();
	// �e�N�X�`���Z�b�g
	m_pTexture = *CTexture::GetTexture((int)CTexture::TEXTYPE_FIELD);
}

//=======================================================================================
//   �I������
//=======================================================================================
void CPaticle::Uninit()
{
	SAFE_RELEASE( m_pVtxBuff );      // ���_�o�b�t�@�̔j��
	SAFE_RELEASE(m_pIdxBuff);      // �C���f�b�N�X�o�b�t�@�̔j��
	SAFE_RELEASE(m_pTexture);      // �C���f�b�N�X�o�b�t�@�̔j��
}

//=======================================================================================
//   �X�V����
//=======================================================================================
void CPaticle::Update()
{
	// ���_����ݒ�
	// ���_���i�[�p�^���o�b�t�@�̐錾
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����i0,0���L������ƑS�������b�N�j
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);				// Z���W�̐ݒ�
	pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// �@���̐ݒ�
	pVtx[0].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �J���[�̐ݒ�i0~255�̐����l�j
	pVtx[0].tex = D3DXVECTOR2(0.0f,1.0f);						// �e�N�X�`��V���W�̐ݒ�

	pVtx[1].pos = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);				// Z���W�̐ݒ�
	pVtx[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// �@���̐ݒ�
	pVtx[1].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �J���[�̐ݒ�i0~255�̐����l�j
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);						// �e�N�X�`��V���W�̐ݒ�

	pVtx[2].pos = D3DXVECTOR3(1.0f, 1.0f, 0.0f);				// Z���W�̐ݒ�
	pVtx[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// �@���̐ݒ�
	pVtx[2].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �J���[�̐ݒ�i0~255�̐����l�j
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);						// �e�N�X�`��V���W�̐ݒ�

	pVtx[3].pos = D3DXVECTOR3(1.0f, -1.0f, 0.0f);				// Z���W�̐ݒ�
	pVtx[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// �@���̐ݒ�
	pVtx[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �J���[�̐ݒ�i0~255�̐����l�j
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);						// �e�N�X�`��V���W�̐ݒ�

																// ���_���̐ݒ�
	pVtx[4].pos = D3DXVECTOR3(3-1.0f, 3+1.0f, 0.0f);				// Z���W�̐ݒ�
	pVtx[4].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// �@���̐ݒ�
	pVtx[4].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �J���[�̐ݒ�i0~255�̐����l�j
	pVtx[4].tex = D3DXVECTOR2(0.0f, 1.0f);						// �e�N�X�`��V���W�̐ݒ�

	pVtx[5].pos = D3DXVECTOR3(3-1.0f, 3-1.0f, 0.0f);				// Z���W�̐ݒ�
	pVtx[5].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// �@���̐ݒ�
	pVtx[5].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �J���[�̐ݒ�i0~255�̐����l�j
	pVtx[5].tex = D3DXVECTOR2(0.0f, 0.0f);						// �e�N�X�`��V���W�̐ݒ�

	pVtx[6].pos = D3DXVECTOR3(3 + 1.0f, 3 + 1.0f, 0.0f);				// Z���W�̐ݒ�
	pVtx[6].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// �@���̐ݒ�
	pVtx[6].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �J���[�̐ݒ�i0~255�̐����l�j
	pVtx[6].tex = D3DXVECTOR2(1.0f, 1.0f);						// �e�N�X�`��V���W�̐ݒ�

	pVtx[7].pos = D3DXVECTOR3(3 + 1.0f, 3-1.0f, 0.0f);				// Z���W�̐ݒ�
	pVtx[7].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// �@���̐ݒ�
	pVtx[7].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �J���[�̐ݒ�i0~255�̐����l�j
	pVtx[7].tex = D3DXVECTOR2(1.0f, 0.0f);						// �e�N�X�`��V���W�̐ݒ�

	// �����J����
	m_pVtxBuff->Unlock();
}

//=======================================================================================
//   �`�揈��
//=======================================================================================
void CPaticle::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return;
	}

	// �J�����̎擾
	CCamera::CAMERA camera;
	camera = CCamera::GetCamera();

	// �ϊ��s��̐錾
	D3DXMATRIX mtxPos;             // ���[�J�����W
	D3DXMATRIX mtxWorld;           // ���[���h���
	D3DXMATRIX mtxViewInverse;     // �]�u�s��
	D3DXMATRIX mtxViewCamera = camera.mtxView;      // �J�����s��̎擾

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
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxPos);   // ���[���h���W�̑��
	mtxWorld = mtxViewInverse * mtxWorld;

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);       // ���[���h���Z�b�g

	// �p�C�v���C��
	pDevice->SetStreamSource(0,
		m_pVtxBuff,     // �X�g���[���̂��ƂɂȂ钸�_�̃o�b�t�@�̎n�_
		0,                       // �I�t�Z�b�g�i�o�C�g�j
		sizeof(VERTEX_3D));      // ��̒��_�f�[�^�̃T�C�Y�i�X�g���C�h�ʁj

								 // �f�o�C�X�ɃC���f�b�N�X�o�b�t�@�̐ݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �`�撼�O�Ƀe�N�X�`�����Z�b�g�i�e�N�X�`���̐ݒ�j
	pDevice->SetTexture(0, m_pTexture);

	// ���C�g�̐ݒ�iOFF�j
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �C���f�b�N�X�v���~�e�B�u�̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,                 // �v���~�e�B�u�̎��
		0,
		0,
		m_nVexNum,   // ���_��
		0,
		m_nPolygonNum+1);                       // �v���~�e�B�u�̐��i�|���S���̐��j

	// ���C�g�̐ݒ�iON�j
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}


//=======================================================================================
// �|���S���̒��_�ݒ�
//=======================================================================================
void CPaticle::MakeVex(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4*2,           // �쐬���������_�o�b�t�@�̃T�C�Y�i��̒��_*���_���j
		D3DUSAGE_WRITEONLY,                                      // �������ނ������Ȃ��i�`�F�b�N���Ȃ��j
		FVF_VERTEX_3D,                                           // �ǂ�Ȓ��_�ŏ����́i0�ɂ��Ă�OK�j
		D3DPOOL_MANAGED,                                         // �������Ǘ������C���ɂ���
		&m_pVtxBuff,
		NULL);

	// ���_����ݒ�
	// ���_���i�[�p�^���o�b�t�@�̐錾
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����i0,0���L������ƑS�������b�N�j
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);				// Z���W�̐ݒ�
	pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// �@���̐ݒ�
	pVtx[0].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �J���[�̐ݒ�i0~255�̐����l�j
	pVtx[0].tex = D3DXVECTOR2(0.0f,1.0f);						// �e�N�X�`��V���W�̐ݒ�

	pVtx[1].pos = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);				// Z���W�̐ݒ�
	pVtx[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// �@���̐ݒ�
	pVtx[1].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �J���[�̐ݒ�i0~255�̐����l�j
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);						// �e�N�X�`��V���W�̐ݒ�

	pVtx[2].pos = D3DXVECTOR3(1.0f, 1.0f, 0.0f);				// Z���W�̐ݒ�
	pVtx[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// �@���̐ݒ�
	pVtx[2].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �J���[�̐ݒ�i0~255�̐����l�j
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);						// �e�N�X�`��V���W�̐ݒ�

	pVtx[3].pos = D3DXVECTOR3(1.0f, -1.0f, 0.0f);				// Z���W�̐ݒ�
	pVtx[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// �@���̐ݒ�
	pVtx[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �J���[�̐ݒ�i0~255�̐����l�j
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);						// �e�N�X�`��V���W�̐ݒ�

																// ���_���̐ݒ�
	pVtx[4].pos = D3DXVECTOR3(3-1.0f, 3+1.0f, 0.0f);				// Z���W�̐ݒ�
	pVtx[4].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// �@���̐ݒ�
	pVtx[4].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �J���[�̐ݒ�i0~255�̐����l�j
	pVtx[4].tex = D3DXVECTOR2(0.0f, 1.0f);						// �e�N�X�`��V���W�̐ݒ�

	pVtx[5].pos = D3DXVECTOR3(3-1.0f, 3-1.0f, 0.0f);				// Z���W�̐ݒ�
	pVtx[5].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// �@���̐ݒ�
	pVtx[5].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �J���[�̐ݒ�i0~255�̐����l�j
	pVtx[5].tex = D3DXVECTOR2(0.0f, 0.0f);						// �e�N�X�`��V���W�̐ݒ�

	pVtx[6].pos = D3DXVECTOR3(3 + 1.0f, 3 + 1.0f, 0.0f);				// Z���W�̐ݒ�
	pVtx[6].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// �@���̐ݒ�
	pVtx[6].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �J���[�̐ݒ�i0~255�̐����l�j
	pVtx[6].tex = D3DXVECTOR2(1.0f, 1.0f);						// �e�N�X�`��V���W�̐ݒ�

	pVtx[7].pos = D3DXVECTOR3(3 + 1.0f, 3-1.0f, 0.0f);				// Z���W�̐ݒ�
	pVtx[7].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// �@���̐ݒ�
	pVtx[7].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �J���[�̐ݒ�i0~255�̐����l�j
	pVtx[7].tex = D3DXVECTOR2(1.0f, 0.0f);						// �e�N�X�`��V���W�̐ݒ�

	// �����J����
	m_pVtxBuff->Unlock();
}


//=======================================================================================
// �|���S���̃o�b�t�@�ݒ�
//=======================================================================================
void CPaticle::MakeBuff(void)
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
	hr = pDevice->CreateIndexBuffer(sizeof(WORD) * 12,        // ���_���iWORD��16Bid�j
		D3DUSAGE_WRITEONLY,              // �g�p�p�r�t���O�i�������ނ̂݁j
		D3DFMT_INDEX16,                  // �f�[�^�̃t�H�[�}�b�g�i32Bid�ݒ肵����16��32�ɕύX�j
		D3DPOOL_MANAGED,                 // �������̊Ǘ����@�i���C���j
		&m_pIdxBuff,              // �o�b�t�@�\�C���^�[�t�F�C�X�|�C���^�̃A�h���X
		NULL);

	// �C���f�b�N�X���i�[�p�^���o�b�t�@�̐錾
	WORD* pIdx;

	// ���b�N
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 0;
	pIdx[1] = 0;
	pIdx[2] = 1;
	pIdx[3] = 2;
	pIdx[4] = 3;
	pIdx[5] = 3;

	pIdx[6] = 4;
	pIdx[7] = 4;
	pIdx[8] = 5;
	pIdx[9] = 6;
	pIdx[10] = 7;
	pIdx[11] = 7;

	// ���b�N���
	m_pIdxBuff->Unlock();
}
