//*****************************************************************************
//   �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

#include "scene.h"
#include "sceneModelX.h"

//*****************************************************************************
//   �萔��`
//*****************************************************************************
#define MODEL_NAME001        "data/MODEL/cursor000.x"

//=======================================================================================
//   �R���X�g���N�^
//=======================================================================================
CSceneModelX::CSceneModelX(int nPriority) :CScene(nPriority)
{
	// �����o�ϐ��̏�����
	m_vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//=======================================================================================
//   �f�X�g���N�^
//=======================================================================================
CSceneModelX::~CSceneModelX()
{
}

//=======================================================================================
//   ����������
//=======================================================================================
void CSceneModelX::Init()
{
	char aFileName[128] = {
		MODEL_NAME001
	};

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return;
	}

	// �}�b�s���O�p�e�N�X�`���C���^�[�t�F�[�X�̃����[�X����
	for( int i = 0; i < MODEL_MATERIAL_MAX; i++ )
	{
		m_pTextureModel[i] = NULL;
	}

	// ���f���̃G���[�`�F�b�N
	if( FAILED( D3DXLoadMeshFromX( aFileName,              // �t�@�C����
							D3DXMESH_MANAGED,        // 
							pDevice,                 // �f�o�C�X
							NULL,
							&m_pBuffMatModel,        // ���f���̃}�e���A�����
							NULL,
							&m_nNumMatModel,         // �}�e���A����
							&m_pMeshModel )))          // ���b�V�����
	{
		MessageBox( NULL , "���f��������܂���I" , "�G���[" , MB_OK | MB_ICONASTERISK );         // �I�����郁�b�Z�[�W
	}
	D3DXMATERIAL* pMat;
	pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();
	
	for( int i = 0; i < (int)m_nNumMatModel; i++ )
	{
		// �e�N�X�`���̐ݒ�
		if( pMat[i].pTextureFilename != NULL )
		{
			// �e�N�X�`���̓ǂݍ���(�f�B�X�N����)
			if( FAILED( D3DXCreateTextureFromFile( pDevice, pMat[i].pTextureFilename, &m_pTextureModel[i] )))
			{
				MessageBox( NULL , "�摜������܂���I" , "�G���[" , MB_OK | MB_ICONASTERISK );         // �I�����郁�b�Z�[�W
			}
		}
	}

	// �e�N�X�`���Z�b�g
	m_pTexture = *CTexture::GetTexture((int)CTexture::TEXTYPE_WHITE);

	// �v���W�F�N�V�����s��
	D3DXMATRIX mtxProj;
	D3DXMatrixPerspectiveFovLH( &mtxProj,                               // Fov�͉�p
								D3DX_PI / 3.0f,                         // ��p�i����p�j60�x�ɂ��邽�߃΁��R
								(float)SCREEN_WIDTH / SCREEN_HEIGHT,    // �A�X�y�N�g��ifloat�Ōv�Z����Afloat>int�Ȃ̂ŕЕ���OK�j
								1.0f,                                   // near �K��0.0f���傫������  �ǂ�����`�悷�邩
								100.0f);                                // far �ǂ��܂ŕ`�悷�邩
	pDevice->SetTransform( D3DTS_PROJECTION, &mtxProj );
}

//=======================================================================================
//   �I������
//=======================================================================================
void CSceneModelX::Uninit()
{
	// ���b�V�����̃����[�X����
	SAFE_RELEASE(m_pMeshModel);

	// �}�e���A�����̃����[�X����
	SAFE_RELEASE(m_pBuffMatModel);

	// �}�b�s���O�p�e�N�X�`���C���^�[�t�F�[�X�̃����[�X����
	for( int i = 0; i < MODEL_MATERIAL_MAX; i++ )
	{
		SAFE_RELEASE(m_pTextureModel[i]);
	}
	CScene::Release();
}

//=======================================================================================
//   �X�V����
//=======================================================================================
void CSceneModelX::Update()
{
}

//=======================================================================================
//   �`�揈��
//=======================================================================================
void CSceneModelX::Draw()
{
	if (m_pBuffMatModel == NULL) { return; }

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
	D3DXMatrixRotationYawPitchRoll( &mtxRot,
									m_rot.y,
									m_rot.x,
									m_rot.z );

	// ���[�J���X�P�[���̑��
	D3DXMatrixScaling( &mtxScl,
					   m_scl.x,
					   m_scl.y,
					   m_scl.z );

	// ���[�J�����W�̑��
	D3DXMatrixTranslation( &mtxPos,
						   m_pos.x,
						   m_pos.y,
						   m_pos.z );
	
	// ���[���h��񏈗�
	D3DXMatrixIdentity( &mtxWorld );                       // ���[���h�̒��g��������
	D3DXMatrixMultiply( &mtxWorld, &mtxWorld, &mtxRot );   // ���[���h��]�̑��
	D3DXMatrixMultiply( &mtxWorld, &mtxWorld, &mtxScl );   // ���[���h�X�P�[���̑��
	D3DXMatrixMultiply( &mtxWorld, &mtxWorld, &mtxPos );   // ���[���h���W�̑��
	pDevice->SetTransform( D3DTS_WORLD, &mtxWorld );       // ���[���h���Z�b�g

	D3DMATERIAL9 matDef;
	pDevice->GetMaterial( &matDef );                 // ���݃f�o�C�X�ɐݒ肳��Ă�A�e���A�������擾

	D3DXMATERIAL* pMat;
	pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();

	for( int i = 0; i < (int)m_nNumMatModel; i++ )
	{
		//  �g�U���̐ݒ�
		pMat[i].MatD3D.Diffuse = m_color;

		// �����̐ݒ�
		pMat[i].MatD3D.Ambient = m_color;

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial( &pMat[i].MatD3D );

		// �e�N�X�`���̕`��
		if( pMat[i].pTextureFilename != NULL )
		{
			pDevice->SetTexture( 0, m_pTextureModel[i] );      // �e�N�X�`���}�b�s���O
		}
		else
		{
			pDevice->SetTexture( 0, m_pTexture );         // �e�N�X�`���}�b�s���O(��)
		}
		// ���b�V���̕`��
		m_pMeshModel->DrawSubset(i);
		pDevice->SetMaterial( &matDef );
	}
}
