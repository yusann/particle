#define _CRT_SECURE_NO_WARNINGS
//*****************************************************************************
//   �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "motionPartsX.h"

//*****************************************************************************
//   �ÓI�����o�ϐ��錾
//*****************************************************************************
CMotionPartsX::MOSIONPARTSX		*CMotionPartsX::m_MotionPartsX[TYPE_MAX] = {};
LPDIRECT3DTEXTURE9		CMotionPartsX::m_pTextureBG = NULL;

// �e�N�X�`���p�X
char CMotionPartsX::m_FileName[TYPE_MAX][128] =
{
	"data/text1.txt",
	"data/MOTION/ENEMY000/motion.txt"
};

//=======================================================================================
//   �R���X�g���N�^
//=======================================================================================
CMotionPartsX::CMotionPartsX()
{
	int nCntType, nCntParts, nCntID;
	for (nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{
		for (nCntParts = 0; nCntParts < MOTIONPARTSX_PARTS_MAX; nCntParts++)
		{
			// ���b�V�����̃����[�X����
			if (m_MotionPartsX[nCntType]->Parts[nCntParts].pMesh != NULL) {
				m_MotionPartsX[nCntType]->Parts[nCntParts].pMesh = NULL;
			}

			// �}�e���A�����̃����[�X����
			if (m_MotionPartsX[nCntType]->Parts[nCntParts].pBuffMat != NULL) {
				m_MotionPartsX[nCntType]->Parts[nCntParts].pBuffMat = NULL;
			}

			// �}�b�s���O�p�e�N�X�`���C���^�[�t�F�[�X�̃����[�X����
			for (int i = 0; i < MOTIONPARTSX_MATERIAL_MAX; i++)
			{
				if (m_MotionPartsX[nCntType]->Parts[nCntParts].pTexture[i] != NULL) {
					m_MotionPartsX[nCntType]->Parts[nCntParts].pTexture[i] = NULL;
				}
			}
		}
	}
	// �}�b�s���O�p�e�N�X�`���i���j�C���^�[�t�F�[�X�̃����[�X����
	if (m_pTextureBG != NULL) {
		m_pTextureBG = NULL;
	}
}

//=======================================================================================
//   �f�X�g���N�^
//=======================================================================================
CMotionPartsX::~CMotionPartsX()
{
}

//=======================================================================================
//   ����������
//=======================================================================================
CMotionPartsX::MOSIONPARTSX *CMotionPartsX::LoadFile(char *pPath)
{
	int nCntModel, nCntParts, nCntMaterial, nCntMotion, nCntKey, nCntID;
	MOSIONPARTSX *MotionPartsX = new MOSIONPARTSX;
	char aWork[128];
	FILE* pFile = NULL;
	D3DXMATERIAL*	pMat = NULL;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;                // �G���[�`�F�b�N�̂���NULL������
	pDevice = CManager::GetRenderer()->GetDevice();                           // �f�o�C�X�̃|�C���^
	if (pDevice == NULL)                            // �G���[�`�F�b�N
	{
		MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return NULL;
	}

	//  �t�@�C���̓ǂݍ���
	pFile = fopen(pPath, "r+");
	if (pFile == NULL)
	{
		//  �G���[�������ꍇ
		MessageBox(NULL, "�t�@�C�����ǂݍ��߂܂���ł����B", "�G���[���b�Z�[�W", MB_OK | MB_ICONWARNING);
		return NULL;
	}
	else
	{
		while (1)
		{
			// �����ǂݍ���
			fscanf(pFile, "%s", &aWork);

			// �����`�F�b�N
			//  ���[�h�J�n
			if (strcmp(aWork, "LOAD_START") == 0) {
				continue;
			}

			// ���f����
			else if (strcmp(aWork, "SET_NUM_MODEL") == 0) {

				//  �t�@�C�����烂�f�������i�[
				fscanf(pFile, "%d", &MotionPartsX->NumParts);

				// �I��
				while (1) {
					// �����ǂݍ���
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "END_NUM_MODEL") == 0) {
						break;
					}
				}
				continue;
			}

			// ���f���̃t�@�C���p�X
			else if (strcmp(aWork, "SET_FILENAME") == 0) {

				for (int i = 0; i < MotionPartsX->NumParts; ++i)
				{
					fscanf(pFile, "%s", &MotionPartsX->Parts[i].FilePath);
				}

				// �I��
				while (1) {
					// �����ǂݍ���
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "END_FILENAME") == 0) {
						break;
					}
				}
				continue;
			}

			// �I�t�Z�b�g���
			else if (strcmp(aWork, "SET_OFFSET") == 0) {

				for (int i = 0; i < MotionPartsX->NumParts; ++i)
				{
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "INDEX") == 0)
					{
						fscanf(pFile, "%d", &MotionPartsX->Parts[i].Index);
					}
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "PARENT") == 0)
					{
						fscanf(pFile, "%d", &MotionPartsX->Parts[i].Parent);
					}
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "POS") == 0)
					{
						fscanf(pFile, "%f", &MotionPartsX->Parts[i].Pos.x);
						fscanf(pFile, "%f", &MotionPartsX->Parts[i].Pos.y);
						fscanf(pFile, "%f", &MotionPartsX->Parts[i].Pos.z);
					}
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "ROT") == 0)
					{
						fscanf(pFile, "%f", &MotionPartsX->Parts[i].Rot.x);
						fscanf(pFile, "%f", &MotionPartsX->Parts[i].Rot.y);
						fscanf(pFile, "%f", &MotionPartsX->Parts[i].Rot.z);
					}
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "SCL") == 0)
					{
						fscanf(pFile, "%f", &MotionPartsX->Parts[i].Scl.x);
						fscanf(pFile, "%f", &MotionPartsX->Parts[i].Scl.y);
						fscanf(pFile, "%f", &MotionPartsX->Parts[i].Scl.z);
					}

					//  x�t�@�C���̓ǂݍ���
					if (FAILED(D3DXLoadMeshFromX(MotionPartsX->Parts[i].FilePath,												//  �t�@�C����
						D3DXMESH_MANAGED,															//  �I�v�V����
						pDevice,																	//  �f�o�C�X
						NULL,																		//  
						&MotionPartsX->Parts[i].pBuffMat,		//  �}�e���A�����
						NULL,																	//  
						&MotionPartsX->Parts[i].NumMat,		//  �}�e���A����
						&MotionPartsX->Parts[i].pMesh)))	//  ���b�V�����
					{
						MessageBox(NULL, "x�t�@�C������肭�擾�o���܂���ł����B", "�G���[���b�Z�[�W", MB_OK);
						return NULL;
					}

					//  �}�e���A�����̐擪�A�h���X���擾
					pMat = (D3DXMATERIAL*)MotionPartsX->Parts[i].pBuffMat->GetBufferPointer();

					//  �}�e���A���̐����̃��[�v
					for (int j = 0; j < (int)MotionPartsX->Parts[i].NumMat; ++j)
					{
						//  �e�N�X�`���|�C���^������ꍇ
						if (pMat[j].pTextureFilename != NULL)
						{
							//  �e�N�X�`���̓ǂݍ���( �n�[�h�f�B�X�N���� )	
							if (FAILED(D3DXCreateTextureFromFile(pDevice,	//  �f�o�C�X
								pMat[j].pTextureFilename,					//  �e�N�X�`����
								&MotionPartsX->Parts[i].pTexture[j])))		//  �Ăэ��ޏꏊ
							{
								//  �G���[�������ꍇ
								MessageBox(NULL, "�e�N�X�`���C���^�t�F�[�X�̃|�C���^������܂���B", "�G���[���b�Z�[�W", MB_OK | MB_ICONWARNING);
								return NULL;
							}
						}
					}
				}

				// �I��
				while (1) {
					// �����ǂݍ���
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "END_OFFSET") == 0) {
						break;
					}
				}
				continue;
			}

			// ���[�V������
			else if (strcmp(aWork, "SET_NUM_MOTION") == 0) {

				//  �t�@�C�����烂�f�������i�[
				fscanf(pFile, "%d", &MotionPartsX->NumMotion);

				// �I��
				while (1) {
					// �����ǂݍ���
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "END_NUM_MOTION") == 0) {
						break;
					}
				}
				continue;
			}

			// �I�t�Z�b�g���
			else if (strcmp(aWork, "SET_MOTION") == 0) {

				for (int i = 0; i < MotionPartsX->NumMotion; ++i)
				{
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "LOOP") == 0)
					{
						fscanf(pFile, "%d", &MotionPartsX->Motion[i].Loop);
					}
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "NUMKEY") == 0)
					{
						fscanf(pFile, "%d", &MotionPartsX->Motion[i].NumKey);
					}
					MotionPartsX->Motion[i].pKey = new KEY[MotionPartsX->Motion[i].NumKey];

					for (int j = 0; j < MotionPartsX->Motion[i].NumKey; ++j)
					{
						fscanf(pFile, "%s", &aWork);
						if (strcmp(aWork, "FRAME") == 0)
						{
							fscanf(pFile, "%d", &MotionPartsX->Motion[i].pKey[j].Frame);
						}
						for (int k = 0; k < MotionPartsX->NumParts; ++k)
						{
							fscanf(pFile, "%s", &aWork);
							if (strcmp(aWork, "POS") == 0)
							{
								fscanf(pFile, "%f", &MotionPartsX->Motion[i].pKey[j].Pos[k].x);
								fscanf(pFile, "%f", &MotionPartsX->Motion[i].pKey[j].Pos[k].y);
								fscanf(pFile, "%f", &MotionPartsX->Motion[i].pKey[j].Pos[k].z);
							}
							fscanf(pFile, "%s", &aWork);
							if (strcmp(aWork, "ROT") == 0)
							{
								fscanf(pFile, "%f", &MotionPartsX->Motion[i].pKey[j].Rot[k].x);
								fscanf(pFile, "%f", &MotionPartsX->Motion[i].pKey[j].Rot[k].y);
								fscanf(pFile, "%f", &MotionPartsX->Motion[i].pKey[j].Rot[k].z);
							}
							fscanf(pFile, "%s", &aWork);
							if (strcmp(aWork, "SCL") == 0)
							{
								fscanf(pFile, "%f", &MotionPartsX->Motion[i].pKey[j].Scl[k].x);
								fscanf(pFile, "%f", &MotionPartsX->Motion[i].pKey[j].Scl[k].y);
								fscanf(pFile, "%f", &MotionPartsX->Motion[i].pKey[j].Scl[k].z);
							}
						}
					}
				}

				// �I��
				while (1) {
					// �����ǂݍ���
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "END_MOTION") == 0) {
						break;
					}
				}
				continue;
			}

			// �I��
			else if (strcmp(aWork, "END_LOAD") == 0) {
				break;
			}
		}
		//  �t�@�C�������
		fclose(pFile);
	}
	return MotionPartsX;
}


//=======================================================================================
//   ����������
//=======================================================================================
void CMotionPartsX::Init()
{
	// ���f���̏�����
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{
		m_MotionPartsX[nCntType] = LoadFile(m_FileName[nCntType]);
	}

	m_pTextureBG = *CTexture::GetTexture(CTexture::TEXTYPE_WHITE);

	// �^�C�v�����[�v
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{
		// �p�[�c�������[�v
		for (int nCntParts = 0; nCntParts < m_MotionPartsX[nCntType]->NumParts; nCntParts++)
		{
			// �f�t�H���g���[�V�����f�[�^�̑��
			m_MotionPartsX[nCntType]->DefaultPos[nCntParts] = m_MotionPartsX[nCntType]->Parts[nCntParts].Pos;
			m_MotionPartsX[nCntType]->DefaultRot[nCntParts] = m_MotionPartsX[nCntType]->Parts[nCntParts].Rot;
			m_MotionPartsX[nCntType]->DefaultScl[nCntParts] = m_MotionPartsX[nCntType]->Parts[nCntParts].Scl;

			// �������[�V�������
			m_MotionPartsX[nCntType]->Parts[nCntParts].Pos += m_MotionPartsX[nCntType]->Motion[0].pKey[0].Pos[nCntParts];
			m_MotionPartsX[nCntType]->Parts[nCntParts].Rot += m_MotionPartsX[nCntType]->Motion[0].pKey[0].Rot[nCntParts];
			m_MotionPartsX[nCntType]->Parts[nCntParts].Scl += m_MotionPartsX[nCntType]->Motion[0].pKey[0].Scl[nCntParts];
		}
	}
}

//=======================================================================================
//   �I������
//=======================================================================================
void CMotionPartsX::Uninit()
{
	int nCntType, nCntParts, nCntID;
	for (nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{
		for (nCntParts = 0; nCntParts < MOTIONPARTSX_PARTS_MAX; nCntParts++)
		{
			// ���b�V�����̃����[�X����
			if (m_MotionPartsX[nCntType]->Parts[nCntParts].pMesh != NULL) {
				m_MotionPartsX[nCntType]->Parts[nCntParts].pMesh = NULL;
			}

			// �}�e���A�����̃����[�X����
			if (m_MotionPartsX[nCntType]->Parts[nCntParts].pBuffMat != NULL) {
				m_MotionPartsX[nCntType]->Parts[nCntParts].pBuffMat = NULL;
			}

			// �}�b�s���O�p�e�N�X�`���C���^�[�t�F�[�X�̃����[�X����
			for (int i = 0; i < MOTIONPARTSX_MATERIAL_MAX; i++)
			{
				if (m_MotionPartsX[nCntType]->Parts[nCntParts].pTexture[i] != NULL) {
					m_MotionPartsX[nCntType]->Parts[nCntParts].pTexture[i] = NULL;
				}
			}
		}
		for (int nCntMotion = 0; nCntMotion < MOTIONPARTSX_MOTION_MAX; nCntMotion++)
		{
			// �L�[�̃����[�X����
			if (m_MotionPartsX[nCntType]->Motion[nCntMotion].pKey != NULL) {
				m_MotionPartsX[nCntType]->Motion[nCntMotion].pKey = NULL;
			}
		}
	}
	// �}�b�s���O�p�e�N�X�`���i���j�C���^�[�t�F�[�X�̃����[�X����
	if (m_pTextureBG != NULL) {
		m_pTextureBG = NULL;
	}
}

CMotionPartsX::MOSIONPARTSX *CMotionPartsX::GetMotionPartsX(TYPE type)
{
	return m_MotionPartsX[type];
}
