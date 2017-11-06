//*****************************************************************************
//   �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//*****************************************************************************
//   �萔��`
//*****************************************************************************
#define TEXTURENAME000 "data/TEXTURE/bg.jpg"
#define TEXTURENAME001 "data/TEXTURE/field000.jpg"

//*****************************************************************************
//   �ÓI�����o�ϐ�
//*****************************************************************************
std::unordered_map<std::string, LPDIRECT3DTEXTURE9> CTexture::m_Texture;
std::vector<std::string> CTexture::m_FileName;

//=======================================================================================
//   ����������
//=======================================================================================
void CTexture::Init()
{
	m_FileName.push_back(TEXTURENAME000);
	m_FileName.push_back(TEXTURENAME001);
	for (int i = 0; i < m_FileName.size(); i++)
	{
		m_Texture[m_FileName[i]] = LoadTex(m_FileName[i]);
	}
}

//=======================================================================================
//   �I������
//=======================================================================================
void CTexture::Uninit()
{
	m_FileName.clear();
	for (auto CntPartsX = m_Texture.begin(); CntPartsX != m_Texture.end(); CntPartsX++)
	{
		SAFE_RELEASE((*CntPartsX).second);
	}
	m_Texture.clear();
}

//=======================================================================================
//   �e�N�X�`���ǂݍ��ݏ���
//=======================================================================================
LPDIRECT3DTEXTURE9 CTexture::LoadTex(std::string FileName)
{
	LPDIRECT3DTEXTURE9 tex;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return NULL;
	}

	// �e�N�X�`���̃G���[�`�F�b�N
	for (int i = 0; i < TEXTYPE_MAX; i++) {
		if (FAILED(D3DXCreateTextureFromFile(pDevice, FileName.c_str(), &tex))) {
			MessageBox(NULL, "�摜������܂���I", "�G���[", MB_OK | MB_ICONASTERISK);         // �I�����郁�b�Z�[�W
			return NULL;
		}
	}
	return tex;
}

//=======================================================================================
//   �e�N�X�`���擾����
//=======================================================================================
LPDIRECT3DTEXTURE9 *CTexture::GetTexture(char *FileName)
{
	// ����
	auto itr = m_Texture.find(FileName);

	// ���݂��Ȃ��ꍇ����
	if (itr == m_Texture.end()) {
		m_Texture[FileName] = LoadTex(FileName);
	}
	return &m_Texture[FileName];
}