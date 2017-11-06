//*****************************************************************************
//   ��d��`�h�~
//*****************************************************************************
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//*****************************************************************************
//   �N���X�̒�`
//*****************************************************************************
// �V�[���N���X
class CTexture
{
public:

	typedef enum
	{
		TEXTYPE_WHITE = 0,
		TEXTYPE_FIELD,
		TEXTYPE_MAX
	}TEXTYPE;
	
	static void Init(void);                              // ����������
	static void Uninit(void);                            // �I������

	static LPDIRECT3DTEXTURE9 *GetTexture(int nNo) { return &m_Texture[m_FileName[nNo]]; }
	static LPDIRECT3DTEXTURE9 *GetTexture(char *FileName);

private:
	static LPDIRECT3DTEXTURE9 LoadTex(std::string FileName);

	static std::unordered_map<std::string, LPDIRECT3DTEXTURE9>	m_Texture;
	static std::vector<std::string> m_FileName;
};

#endif