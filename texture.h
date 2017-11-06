//*****************************************************************************
//   二重定義防止
//*****************************************************************************
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//*****************************************************************************
//   クラスの定義
//*****************************************************************************
// シーンクラス
class CTexture
{
public:

	typedef enum
	{
		TEXTYPE_WHITE = 0,
		TEXTYPE_FIELD,
		TEXTYPE_MAX
	}TEXTYPE;
	
	static void Init(void);                              // 初期化処理
	static void Uninit(void);                            // 終了処理

	static LPDIRECT3DTEXTURE9 *GetTexture(int nNo) { return &m_Texture[m_FileName[nNo]]; }
	static LPDIRECT3DTEXTURE9 *GetTexture(char *FileName);

private:
	static LPDIRECT3DTEXTURE9 LoadTex(std::string FileName);

	static std::unordered_map<std::string, LPDIRECT3DTEXTURE9>	m_Texture;
	static std::vector<std::string> m_FileName;
};

#endif