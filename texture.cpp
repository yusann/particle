//*****************************************************************************
//   インクルードファイル
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//*****************************************************************************
//   定数定義
//*****************************************************************************
#define TEXTURENAME000 "data/TEXTURE/bg.jpg"
#define TEXTURENAME001 "data/TEXTURE/field000.jpg"

//*****************************************************************************
//   静的メンバ変数
//*****************************************************************************
std::unordered_map<std::string, LPDIRECT3DTEXTURE9> CTexture::m_Texture;
std::vector<std::string> CTexture::m_FileName;

//=======================================================================================
//   初期化処理
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
//   終了処理
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
//   テクスチャ読み込み処理
//=======================================================================================
LPDIRECT3DTEXTURE9 CTexture::LoadTex(std::string FileName)
{
	LPDIRECT3DTEXTURE9 tex;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULLチェックしてください！", "エラー", MB_OK | MB_ICONASTERISK);         // エラーメッセージ
		return NULL;
	}

	// テクスチャのエラーチェック
	for (int i = 0; i < TEXTYPE_MAX; i++) {
		if (FAILED(D3DXCreateTextureFromFile(pDevice, FileName.c_str(), &tex))) {
			MessageBox(NULL, "画像がありません！", "エラー", MB_OK | MB_ICONASTERISK);         // 終了するメッセージ
			return NULL;
		}
	}
	return tex;
}

//=======================================================================================
//   テクスチャ取得処理
//=======================================================================================
LPDIRECT3DTEXTURE9 *CTexture::GetTexture(char *FileName)
{
	// 検索
	auto itr = m_Texture.find(FileName);

	// 存在しない場合生成
	if (itr == m_Texture.end()) {
		m_Texture[FileName] = LoadTex(FileName);
	}
	return &m_Texture[FileName];
}