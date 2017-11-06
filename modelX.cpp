//*****************************************************************************
//   インクルードファイル
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "modelX.h"

//*****************************************************************************
//   静的メンバ変数宣言
//*****************************************************************************
std::unordered_map<std::string, CModelX::MODELX>	CModelX::m_modelX;
std::vector<std::string>	CModelX::m_FileName;

//=======================================================================================
//   初期化処理
//=======================================================================================
void CModelX::Init()
{
	// パーツファイル読み込み
	char aWork[64];
	FILE* pFile = NULL;

	//  ファイルの読み込み
	pFile = fopen("data/model.txt", "r+");
	if (pFile == NULL)
	{
		//  エラーだった場合
		MessageBox(NULL, "ファイルが読み込めませんでした。", "エラーメッセージ", MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		while (1)
		{
			// データ読み込み
			fscanf(pFile, "%s", &aWork[0]);

			// モデルのファイルパス読み込み
			if (strcmp(aWork, "SET_MODEL") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aWork[0]);
					if (strcmp(aWork, "END_MODEL") == 0) { break; }
					m_FileName.push_back(aWork);
				}
			}
			// パーツモデルの読み込み
			else if(strcmp(aWork, "SET_PART") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aWork[0]);
					if (strcmp(aWork, "END_PART") == 0) { break; }
					m_modelX[aWork] = LoadModel(aWork);
				}
			}
			// 終了
			else if(strcmp(aWork, "END_LOAD") == 0) { break; }
		}
		//  ファイルを閉じる
		fclose(pFile);
	}

	// モデルの読み込み
	for (int i = 0; i < m_FileName.size(); i++)
	{
		m_modelX[m_FileName[i]] = LoadModel(m_FileName[i]);
	}
}

//=======================================================================================
//   終了処理
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
//   モデル読み込み処理
//=======================================================================================
CModelX::MODELX CModelX::LoadModel(std::string FileName)
{
	MODELX Model;
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = NULL;                // エラーチェックのためNULLを入れる
	pDevice = CManager::GetRenderer()->GetDevice();                           // デバイスのポインタ
	if (pDevice == NULL)                            // エラーチェック
	{
		MessageBox(NULL, "NULLチェックしてください！", "エラー", MB_OK | MB_ICONASTERISK);         // エラーメッセージ
	}
	Model.Name = new char[FileName.size() + 1];
	std::strcpy(Model.Name, FileName.c_str());        // コピー
	//  xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(FileName.c_str(),				//  ファイル名
		D3DXMESH_MANAGED,															//  オプション
		pDevice,																	//  デバイス
		NULL,
		&Model.pBuffMat,		//  マテリアル情報
		NULL,
		&Model.NumMat,			//  マテリアル数
		&Model.pMesh)))			//  メッシュ情報
	{
		MessageBox(NULL, "xファイルを上手く取得出来ませんでした。", "エラーメッセージ", MB_OK);
	}

	//  テクスチャ情報取得
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)Model.pBuffMat->GetBufferPointer();
	Model.pTexture = new LPDIRECT3DTEXTURE9[(int)Model.NumMat];

	//  マテリアルの数分のループ
	for (int j = 0; j < (int)Model.NumMat; ++j)
	{
		//  テクスチャポインタがある場合
		if (pMat[j].pTextureFilename != NULL)
		{
			//  テクスチャの読み込み( ハードディスクから )	
			if (FAILED(D3DXCreateTextureFromFile(pDevice,	//  デバイス
				pMat[j].pTextureFilename,					//  テクスチャ名
				&Model.pTexture[j])))		//  呼び込む場所
			{
				//  エラーだった場合
				MessageBox(NULL, "テクスチャインタフェースのポインタがありません。", "エラーメッセージ", MB_OK | MB_ICONWARNING);
			}
		}
		else
		{
			// ない時に白いテクスチャを代入
			Model.pTexture[j] = *CTexture::GetTexture((int)CTexture::TEXTYPE_WHITE);
		}
	}
	return Model;
}

//=======================================================================================
//   モデル取得処理
//=======================================================================================
CModelX::MODELX *CModelX::GetModelX(char *FileName)
{
	// 検索
	auto itr = m_modelX.find(FileName);

	// 存在しない場合生成
	if (itr == m_modelX.end()) {
		m_modelX[FileName] = LoadModel(FileName);
	}
	return &m_modelX[FileName]; 
}