//*****************************************************************************
//   �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "modelX.h"

//*****************************************************************************
//   �ÓI�����o�ϐ��錾
//*****************************************************************************
std::unordered_map<std::string, CModelX::MODELX>	CModelX::m_modelX;
std::vector<std::string>	CModelX::m_FileName;

//=======================================================================================
//   ����������
//=======================================================================================
void CModelX::Init()
{
	// �p�[�c�t�@�C���ǂݍ���
	char aWork[64];
	FILE* pFile = NULL;

	//  �t�@�C���̓ǂݍ���
	pFile = fopen("data/model.txt", "r+");
	if (pFile == NULL)
	{
		//  �G���[�������ꍇ
		MessageBox(NULL, "�t�@�C�����ǂݍ��߂܂���ł����B", "�G���[���b�Z�[�W", MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		while (1)
		{
			// �f�[�^�ǂݍ���
			fscanf(pFile, "%s", &aWork[0]);

			// ���f���̃t�@�C���p�X�ǂݍ���
			if (strcmp(aWork, "SET_MODEL") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aWork[0]);
					if (strcmp(aWork, "END_MODEL") == 0) { break; }
					m_FileName.push_back(aWork);
				}
			}
			// �p�[�c���f���̓ǂݍ���
			else if(strcmp(aWork, "SET_PART") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aWork[0]);
					if (strcmp(aWork, "END_PART") == 0) { break; }
					m_modelX[aWork] = LoadModel(aWork);
				}
			}
			// �I��
			else if(strcmp(aWork, "END_LOAD") == 0) { break; }
		}
		//  �t�@�C�������
		fclose(pFile);
	}

	// ���f���̓ǂݍ���
	for (int i = 0; i < m_FileName.size(); i++)
	{
		m_modelX[m_FileName[i]] = LoadModel(m_FileName[i]);
	}
}

//=======================================================================================
//   �I������
//=======================================================================================
void CModelX::Uninit()
{
	m_FileName.clear();
	for (auto CntPartsX = m_modelX.begin(); CntPartsX != m_modelX.end(); CntPartsX++)
	{
		SAFE_RELEASE((*CntPartsX).second.pMesh);
		SAFE_RELEASE((*CntPartsX).second.pBuffMat);
		if ((*CntPartsX).second.pTexture != NULL)
		{
			delete[] (*CntPartsX).second.pTexture;
		}
	}
	m_modelX.clear();
}

//=======================================================================================
//   ���f���ǂݍ��ݏ���
//=======================================================================================
CModelX::MODELX CModelX::LoadModel(std::string FileName)
{
	MODELX Model;
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;                // �G���[�`�F�b�N�̂���NULL������
	pDevice = CManager::GetRenderer()->GetDevice();                           // �f�o�C�X�̃|�C���^
	if (pDevice == NULL)                            // �G���[�`�F�b�N
	{
		MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
	}
	Model.Name = new char[FileName.size() + 1];
	std::strcpy(Model.Name, FileName.c_str());        // �R�s�[
	//  x�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(FileName.c_str(),				//  �t�@�C����
		D3DXMESH_MANAGED,															//  �I�v�V����
		pDevice,																	//  �f�o�C�X
		NULL,
		&Model.pBuffMat,		//  �}�e���A�����
		NULL,
		&Model.NumMat,			//  �}�e���A����
		&Model.pMesh)))			//  ���b�V�����
	{
		MessageBox(NULL, "x�t�@�C������肭�擾�o���܂���ł����B", "�G���[���b�Z�[�W", MB_OK);
	}

	//  �e�N�X�`�����擾
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)Model.pBuffMat->GetBufferPointer();
	Model.pTexture = new LPDIRECT3DTEXTURE9[(int)Model.NumMat];

	//  �}�e���A���̐����̃��[�v
	for (int j = 0; j < (int)Model.NumMat; ++j)
	{
		//  �e�N�X�`���|�C���^������ꍇ
		if (pMat[j].pTextureFilename != NULL)
		{
			//  �e�N�X�`���̓ǂݍ���( �n�[�h�f�B�X�N���� )	
			if (FAILED(D3DXCreateTextureFromFile(pDevice,	//  �f�o�C�X
				pMat[j].pTextureFilename,					//  �e�N�X�`����
				&Model.pTexture[j])))		//  �Ăэ��ޏꏊ
			{
				//  �G���[�������ꍇ
				MessageBox(NULL, "�e�N�X�`���C���^�t�F�[�X�̃|�C���^������܂���B", "�G���[���b�Z�[�W", MB_OK | MB_ICONWARNING);
			}
		}
		else
		{
			// �Ȃ����ɔ����e�N�X�`������
			Model.pTexture[j] = *CTexture::GetTexture((int)CTexture::TEXTYPE_WHITE);
		}
	}
	return Model;
}

//=======================================================================================
//   ���f���擾����
//=======================================================================================
CModelX::MODELX *CModelX::GetModelX(char *FileName)
{
	// ����
	auto itr = m_modelX.find(FileName);

	// ���݂��Ȃ��ꍇ����
	if (itr == m_modelX.end()) {
		m_modelX[FileName] = LoadModel(FileName);
	}
	return &m_modelX[FileName]; 
}