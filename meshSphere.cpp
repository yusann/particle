//*****************************************************************************
//   �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "sceneMesh.h"
#include "meshSphere.h"

//=======================================================================================
//   �R���X�g���N�^
//=======================================================================================
CMeshSphere::CMeshSphere() :CSceneMesh(CScene::OBJTYPE_UI)
{
}

//=======================================================================================
//   �f�X�g���N�^
//=======================================================================================
CMeshSphere::~CMeshSphere()
{
}

//==================================================================================================================================================
// �쐬����
//==================================================================================================================================================
CMeshSphere *CMeshSphere::Create(D3DXVECTOR3 pos, float radius, int numBlockX, int numBlockY, D3DXCOLOR color)
{
	CMeshSphere *pScene3D;                            // �ϐ��錾
	pScene3D = new CMeshSphere;                       // ���I�m��
	pScene3D->Init(pos, radius, numBlockX, numBlockY, color);                            // ������
	return pScene3D;                             // �l��Ԃ�
}

//=======================================================================================
//   ����������
//=======================================================================================
void CMeshSphere::Init(D3DXVECTOR3 pos, float radius, int numBlockX, int numBlockY, D3DXCOLOR color)
{
	// �����o�ϐ��̏�����
	m_pos = pos;

	// �v���C�x�[�g�ϐ��̏�����
	m_pIdxBuff = NULL;
	m_nVexNum = (numBlockY * ((numBlockX + 1) * 2)) + ((numBlockY - 1) * 2);
	m_nPolygonNum = m_nVexNum - 2;

	m_Radius = radius;
	m_NumBlockX = numBlockX;
	m_NumBlockY = numBlockY;
	m_color = color;
	m_Use = false;

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
void CMeshSphere::Uninit()
{
	CSceneMesh::Uninit();
}

//=======================================================================================
//   �X�V����
//=======================================================================================
void CMeshSphere::Update(D3DXVECTOR3 pos, float radius)
{
	m_Use = true;
	m_pos = pos;
	m_Radius = radius;
	// ���_���i�[�p�^���o�b�t�@�̐錾
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����i0,0���L������ƑS�������b�N�j
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���a���L��
	float Radius = m_Radius;

	for (int nCntHeight = 0; nCntHeight < m_NumBlockY + 1; nCntHeight++)
	{
		m_Radius = cos(D3DXToRadian((180 / m_NumBlockY) * nCntHeight - 90)) * Radius;
		for (int nCntWidth = 0; nCntWidth < m_NumBlockX + 1; nCntWidth++, pVtx++)
		{
			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(cos(D3DXToRadian((360 / m_NumBlockX) * nCntWidth)) * m_Radius,      // X���W�̐ݒ�i 3D���W�E�E��� �j
				sin(D3DXToRadian(((-180 / m_NumBlockY) * nCntHeight - 90))) * Radius,                                                                            // Y���W�̐ݒ�
				sin(D3DXToRadian((360 / m_NumBlockX) * nCntWidth)) * m_Radius);    // Z���W�̐ݒ�
			pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);                          // �@���̐ݒ�
			pVtx[0].color = m_color;                       // �J���[�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(nCntWidth * (1.0f / m_NumBlockX),              // �e�N�X�`��U���W�̐ݒ�
				(m_NumBlockY - nCntHeight) * (1.0f / m_NumBlockY));            // �e�N�X�`��V���W�̐ݒ�

		}
	}
	// �����J����
	m_pVtxBuff->Unlock();
}

//=======================================================================================
//   �`�揈��
//=======================================================================================
void CMeshSphere::Draw()
{
	if (m_Use)
	{
		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = NULL;
		pDevice = CManager::GetRenderer()->GetDevice();
		if (pDevice == NULL) {
			MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
			return;
		}
		// ���C�g�̐ݒ�iOFF�j
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		CSceneMesh::Draw();
		// ���C�g�̐ݒ�iON�j
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		m_Use = false;
	}
}

//=======================================================================================
// �|���S���̒��_�ݒ�
//=======================================================================================
void CMeshSphere::MakeVex(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return;
	}

	// ���_�o�b�t�@�����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((m_NumBlockX + 1) * (m_NumBlockY + 1)),           // �쐬���������_�o�b�t�@�̃T�C�Y�i��̒��_*���_���j
		D3DUSAGE_WRITEONLY,                                      // �������ނ������Ȃ��i�`�F�b�N���Ȃ��j
		FVF_VERTEX_3D,                                           // �ǂ�Ȓ��_�ŏ����́i0�ɂ��Ă�OK�j
		D3DPOOL_MANAGED,                                         // �������Ǘ������C���ɂ���
		&m_pVtxBuff,
		NULL);

	// ���_���i�[�p�^���o�b�t�@�̐錾
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����i0,0���L������ƑS�������b�N�j
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���a���L��
	float Radius = m_Radius;

	for (int nCntHeight = 0; nCntHeight < m_NumBlockY + 1; nCntHeight++)
	{
		m_Radius = cos(D3DXToRadian((180 / m_NumBlockY) * nCntHeight - 90)) * Radius;
		for (int nCntWidth = 0; nCntWidth < m_NumBlockX + 1; nCntWidth++, pVtx++)
		{
			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(cos(D3DXToRadian((360 / m_NumBlockX) * nCntWidth)) * m_Radius,      // X���W�̐ݒ�i 3D���W�E�E��� �j
				sin(D3DXToRadian(((-180 / m_NumBlockY) * nCntHeight - 90))) * Radius,                                                                            // Y���W�̐ݒ�
				sin(D3DXToRadian((360 / m_NumBlockX) * nCntWidth)) * m_Radius);    // Z���W�̐ݒ�
			pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);                          // �@���̐ݒ�
			pVtx[0].color = m_color;                       // �J���[�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(nCntWidth * (1.0f / m_NumBlockX),              // �e�N�X�`��U���W�̐ݒ�
				(m_NumBlockY - nCntHeight) * (1.0f / m_NumBlockY));            // �e�N�X�`��V���W�̐ݒ�

		}
	}
	// �����J����
	m_pVtxBuff->Unlock();
}


//=======================================================================================
// �|���S���̃o�b�t�@�ݒ�
//=======================================================================================
void CMeshSphere::MakeBuff(void)
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
	hr = pDevice->CreateIndexBuffer(sizeof(WORD) * m_nVexNum,        // ���_���iWORD��16Bid�j
									D3DUSAGE_WRITEONLY,              // �g�p�p�r�t���O�i�������ނ̂݁j
									D3DFMT_INDEX16,                  // �f�[�^�̃t�H�[�}�b�g�i32Bid�ݒ肵����16��32�ɕύX�j
									D3DPOOL_MANAGED,                 // �������̊Ǘ����@�i���C���j
									&m_pIdxBuff,              // �o�b�t�@�\�C���^�[�t�F�C�X�|�C���^�̃A�h���X
									NULL);

	// �C���f�b�N�X���i�[�p�^���o�b�t�@�̐錾
	WORD* pIdx;

	// ���b�N
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// Z���̃��[�v
	for (int nCntHeight = 0; nCntHeight < m_NumBlockY; nCntHeight++)
	{
		// X���̃��[�v
		for (int nCntWidth = 0; nCntWidth < m_NumBlockX + 1; nCntWidth++, pIdx += 2)                          // ���[�v�����pIdx��2�O�i
		{
			/*
			// �@��������
			// 1��ڈȊO��1�s�ڂ̏���
			if( nCntHeight != 0 && nCntWidth == 0 ) {
			pIdx[0] = ( g_aSphere[ eType ][ nID ].nNumBlockX + 1 ) + ( nCntHeight * ( g_aSphere[ eType ][ nID ].nNumBlockX + 1 ) + nCntWidth );                  // �������ɃC���f�b�N�X������
			pIdx+=1;                                                                               // 1�O�i
			}
			// �d�����Ȃ����_�̏���
			pIdx[0] = ( g_aSphere[ eType ][ nID ].nNumBlockX + 1 ) + ( nCntHeight * ( g_aSphere[ eType ][ nID ].nNumBlockX + 1 ) + nCntWidth );
			pIdx[1] = ( g_aSphere[ eType ][ nID ].nNumBlockX + 1 ) + ( nCntHeight * ( g_aSphere[ eType ][ nID ].nNumBlockX + 1 ) + nCntWidth ) - ( g_aSphere[ eType ][ nID ].nNumBlockX + 1);

			// �ŏI��ڈȊO�̍ŏI�s�ڂ̏���
			if( nCntHeight != g_aSphere[ eType ][ nID ].nNumBlockY-1 && nCntWidth == g_aSphere[ eType ][ nID ].nNumBlockX ) {
			pIdx[2] = ( g_aSphere[ eType ][ nID ].nNumBlockX + 1 ) + ( nCntHeight * ( g_aSphere[ eType ][ nID ].nNumBlockX + 1 ) + nCntWidth ) - ( g_aSphere[ eType ][ nID ].nNumBlockX + 1 );  // �������ɃC���f�b�N�X������
			pIdx+=1;                                                                               // 1�O�i
			}
			*/
			// �@���\����
			// 1��ڈȊO��1�s�ڂ̏���
			if (nCntHeight != 0 && nCntWidth == 0) {
				pIdx[0] = (unsigned short)((m_NumBlockX + 1) + (nCntHeight * (m_NumBlockX + 1) + nCntWidth) - (m_NumBlockX + 1));                   // �������ɃC���f�b�N�X������
				pIdx += 1;                                                                               // 1�O�i
			}
			// �d�����Ȃ����_�̏���
			pIdx[0] = (unsigned short)((m_NumBlockX + 1) + (nCntHeight * (m_NumBlockX + 1) + nCntWidth) - (m_NumBlockX + 1));
			pIdx[1] = (unsigned short)((m_NumBlockX + 1) + (nCntHeight * (m_NumBlockX + 1) + nCntWidth));

			// �ŏI��ڈȊO�̍ŏI�s�ڂ̏���
			if (nCntHeight != m_NumBlockY - 1 && nCntWidth == m_NumBlockX) {
				pIdx[2] = (unsigned short)((m_NumBlockX + 1) + (nCntHeight * (m_NumBlockX + 1) + nCntWidth)); // �������ɃC���f�b�N�X������
				pIdx += 1;                                                                               // 1�O�i
			}
		}
	}

	// ���b�N���
	m_pIdxBuff->Unlock();
}
