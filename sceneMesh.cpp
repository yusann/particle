//*****************************************************************************
//   �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"

#include "scene.h"
#include "sceneMesh.h"

//=======================================================================================
//   �R���X�g���N�^
//=======================================================================================
CSceneMesh::CSceneMesh(int nPriority) :CScene(nPriority)
{
}

//=======================================================================================
//   �f�X�g���N�^
//=======================================================================================
CSceneMesh::~CSceneMesh()
{
}

//=======================================================================================
//   ����������
//=======================================================================================
void CSceneMesh::Init(void)
{
	// �^�C�v�̑��
	CScene::SetObjType(CScene::OBJTYPE_FIELD);
}

//=======================================================================================
//   �I������
//=======================================================================================
void CSceneMesh::Uninit()
{
	SAFE_RELEASE( m_pVtxBuff );      // ���_�o�b�t�@�̔j��
	SAFE_RELEASE(m_pIdxBuff);      // �C���f�b�N�X�o�b�t�@�̔j��
	CScene::Release();
}

//=======================================================================================
//   �X�V����
//=======================================================================================
void CSceneMesh::Update()
{
}

//=======================================================================================
//   �`�揈��
//=======================================================================================
void CSceneMesh::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return;
	}

	// �ϊ��s��̐錾
	D3DXMATRIX mtxRot;             // ���[�J����]
	D3DXMATRIX mtxScl;             // ���[�J���X�P�[��
	D3DXMATRIX mtxPos;             // ���[�J�����W
	D3DXMATRIX mtxWorld;           // ���[���h���

	// ���[�J����]�̑��
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);

	// ���[�J�����W�̑��
	D3DXMatrixTranslation(&mtxPos,
		m_pos.x,
		m_pos.y,
		m_pos.z);

	// ���[���h��񏈗�
	D3DXMatrixIdentity(&mtxWorld);                       // ���[���h�̒��g��������
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);   // ���[���h��]�̑��
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxPos);   // ���[���h���W�̑��
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

	// �C���f�b�N�X�v���~�e�B�u�̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,                 // �v���~�e�B�u�̎��
		0,
		0,
		m_nVexNum,   // ���_��
		0,
		m_nPolygonNum);                       // �v���~�e�B�u�̐��i�|���S���̐��j
}
