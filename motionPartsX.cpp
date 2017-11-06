#define _CRT_SECURE_NO_WARNINGS
//*****************************************************************************
//   インクルードファイル
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "motionPartsX.h"

//*****************************************************************************
//   静的メンバ変数宣言
//*****************************************************************************
CMotionPartsX::MOSIONPARTSX		*CMotionPartsX::m_MotionPartsX[TYPE_MAX] = {};
LPDIRECT3DTEXTURE9		CMotionPartsX::m_pTextureBG = NULL;

// テクスチャパス
char CMotionPartsX::m_FileName[TYPE_MAX][128] =
{
	"data/text1.txt",
	"data/MOTION/ENEMY000/motion.txt"
};

//=======================================================================================
//   コンストラクタ
//=======================================================================================
CMotionPartsX::CMotionPartsX()
{
	int nCntType, nCntParts, nCntID;
	for (nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{
		for (nCntParts = 0; nCntParts < MOTIONPARTSX_PARTS_MAX; nCntParts++)
		{
			// メッシュ情報のリリース処理
			if (m_MotionPartsX[nCntType]->Parts[nCntParts].pMesh != NULL) {
				m_MotionPartsX[nCntType]->Parts[nCntParts].pMesh = NULL;
			}

			// マテリアル情報のリリース処理
			if (m_MotionPartsX[nCntType]->Parts[nCntParts].pBuffMat != NULL) {
				m_MotionPartsX[nCntType]->Parts[nCntParts].pBuffMat = NULL;
			}

			// マッピング用テクスチャインターフェースのリリース処理
			for (int i = 0; i < MOTIONPARTSX_MATERIAL_MAX; i++)
			{
				if (m_MotionPartsX[nCntType]->Parts[nCntParts].pTexture[i] != NULL) {
					m_MotionPartsX[nCntType]->Parts[nCntParts].pTexture[i] = NULL;
				}
			}
		}
	}
	// マッピング用テクスチャ（白）インターフェースのリリース処理
	if (m_pTextureBG != NULL) {
		m_pTextureBG = NULL;
	}
}

//=======================================================================================
//   デストラクタ
//=======================================================================================
CMotionPartsX::~CMotionPartsX()
{
}

//=======================================================================================
//   初期化処理
//=======================================================================================
CMotionPartsX::MOSIONPARTSX *CMotionPartsX::LoadFile(char *pPath)
{
	int nCntModel, nCntParts, nCntMaterial, nCntMotion, nCntKey, nCntID;
	MOSIONPARTSX *MotionPartsX = new MOSIONPARTSX;
	char aWork[128];
	FILE* pFile = NULL;
	D3DXMATERIAL*	pMat = NULL;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = NULL;                // エラーチェックのためNULLを入れる
	pDevice = CManager::GetRenderer()->GetDevice();                           // デバイスのポインタ
	if (pDevice == NULL)                            // エラーチェック
	{
		MessageBox(NULL, "NULLチェックしてください！", "エラー", MB_OK | MB_ICONASTERISK);         // エラーメッセージ
		return NULL;
	}

	//  ファイルの読み込み
	pFile = fopen(pPath, "r+");
	if (pFile == NULL)
	{
		//  エラーだった場合
		MessageBox(NULL, "ファイルが読み込めませんでした。", "エラーメッセージ", MB_OK | MB_ICONWARNING);
		return NULL;
	}
	else
	{
		while (1)
		{
			// 文字読み込み
			fscanf(pFile, "%s", &aWork);

			// 文字チェック
			//  ロード開始
			if (strcmp(aWork, "LOAD_START") == 0) {
				continue;
			}

			// モデル数
			else if (strcmp(aWork, "SET_NUM_MODEL") == 0) {

				//  ファイルからモデル数を格納
				fscanf(pFile, "%d", &MotionPartsX->NumParts);

				// 終了
				while (1) {
					// 文字読み込み
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "END_NUM_MODEL") == 0) {
						break;
					}
				}
				continue;
			}

			// モデルのファイルパス
			else if (strcmp(aWork, "SET_FILENAME") == 0) {

				for (int i = 0; i < MotionPartsX->NumParts; ++i)
				{
					fscanf(pFile, "%s", &MotionPartsX->Parts[i].FilePath);
				}

				// 終了
				while (1) {
					// 文字読み込み
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "END_FILENAME") == 0) {
						break;
					}
				}
				continue;
			}

			// オフセット情報
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

					//  xファイルの読み込み
					if (FAILED(D3DXLoadMeshFromX(MotionPartsX->Parts[i].FilePath,												//  ファイル名
						D3DXMESH_MANAGED,															//  オプション
						pDevice,																	//  デバイス
						NULL,																		//  
						&MotionPartsX->Parts[i].pBuffMat,		//  マテリアル情報
						NULL,																	//  
						&MotionPartsX->Parts[i].NumMat,		//  マテリアル数
						&MotionPartsX->Parts[i].pMesh)))	//  メッシュ情報
					{
						MessageBox(NULL, "xファイルを上手く取得出来ませんでした。", "エラーメッセージ", MB_OK);
						return NULL;
					}

					//  マテリアル情報の先頭アドレスを取得
					pMat = (D3DXMATERIAL*)MotionPartsX->Parts[i].pBuffMat->GetBufferPointer();

					//  マテリアルの数分のループ
					for (int j = 0; j < (int)MotionPartsX->Parts[i].NumMat; ++j)
					{
						//  テクスチャポインタがある場合
						if (pMat[j].pTextureFilename != NULL)
						{
							//  テクスチャの読み込み( ハードディスクから )	
							if (FAILED(D3DXCreateTextureFromFile(pDevice,	//  デバイス
								pMat[j].pTextureFilename,					//  テクスチャ名
								&MotionPartsX->Parts[i].pTexture[j])))		//  呼び込む場所
							{
								//  エラーだった場合
								MessageBox(NULL, "テクスチャインタフェースのポインタがありません。", "エラーメッセージ", MB_OK | MB_ICONWARNING);
								return NULL;
							}
						}
					}
				}

				// 終了
				while (1) {
					// 文字読み込み
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "END_OFFSET") == 0) {
						break;
					}
				}
				continue;
			}

			// モーション数
			else if (strcmp(aWork, "SET_NUM_MOTION") == 0) {

				//  ファイルからモデル数を格納
				fscanf(pFile, "%d", &MotionPartsX->NumMotion);

				// 終了
				while (1) {
					// 文字読み込み
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "END_NUM_MOTION") == 0) {
						break;
					}
				}
				continue;
			}

			// オフセット情報
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

				// 終了
				while (1) {
					// 文字読み込み
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "END_MOTION") == 0) {
						break;
					}
				}
				continue;
			}

			// 終了
			else if (strcmp(aWork, "END_LOAD") == 0) {
				break;
			}
		}
		//  ファイルを閉じる
		fclose(pFile);
	}
	return MotionPartsX;
}


//=======================================================================================
//   初期化処理
//=======================================================================================
void CMotionPartsX::Init()
{
	// モデルの初期化
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{
		m_MotionPartsX[nCntType] = LoadFile(m_FileName[nCntType]);
	}

	m_pTextureBG = *CTexture::GetTexture(CTexture::TEXTYPE_WHITE);

	// タイプ数ループ
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{
		// パーツ数分ループ
		for (int nCntParts = 0; nCntParts < m_MotionPartsX[nCntType]->NumParts; nCntParts++)
		{
			// デフォルトモーションデータの代入
			m_MotionPartsX[nCntType]->DefaultPos[nCntParts] = m_MotionPartsX[nCntType]->Parts[nCntParts].Pos;
			m_MotionPartsX[nCntType]->DefaultRot[nCntParts] = m_MotionPartsX[nCntType]->Parts[nCntParts].Rot;
			m_MotionPartsX[nCntType]->DefaultScl[nCntParts] = m_MotionPartsX[nCntType]->Parts[nCntParts].Scl;

			// 初期モーション代入
			m_MotionPartsX[nCntType]->Parts[nCntParts].Pos += m_MotionPartsX[nCntType]->Motion[0].pKey[0].Pos[nCntParts];
			m_MotionPartsX[nCntType]->Parts[nCntParts].Rot += m_MotionPartsX[nCntType]->Motion[0].pKey[0].Rot[nCntParts];
			m_MotionPartsX[nCntType]->Parts[nCntParts].Scl += m_MotionPartsX[nCntType]->Motion[0].pKey[0].Scl[nCntParts];
		}
	}
}

//=======================================================================================
//   終了処理
//=======================================================================================
void CMotionPartsX::Uninit()
{
	int nCntType, nCntParts, nCntID;
	for (nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{
		for (nCntParts = 0; nCntParts < MOTIONPARTSX_PARTS_MAX; nCntParts++)
		{
			// メッシュ情報のリリース処理
			if (m_MotionPartsX[nCntType]->Parts[nCntParts].pMesh != NULL) {
				m_MotionPartsX[nCntType]->Parts[nCntParts].pMesh = NULL;
			}

			// マテリアル情報のリリース処理
			if (m_MotionPartsX[nCntType]->Parts[nCntParts].pBuffMat != NULL) {
				m_MotionPartsX[nCntType]->Parts[nCntParts].pBuffMat = NULL;
			}

			// マッピング用テクスチャインターフェースのリリース処理
			for (int i = 0; i < MOTIONPARTSX_MATERIAL_MAX; i++)
			{
				if (m_MotionPartsX[nCntType]->Parts[nCntParts].pTexture[i] != NULL) {
					m_MotionPartsX[nCntType]->Parts[nCntParts].pTexture[i] = NULL;
				}
			}
		}
		for (int nCntMotion = 0; nCntMotion < MOTIONPARTSX_MOTION_MAX; nCntMotion++)
		{
			// キーのリリース処理
			if (m_MotionPartsX[nCntType]->Motion[nCntMotion].pKey != NULL) {
				m_MotionPartsX[nCntType]->Motion[nCntMotion].pKey = NULL;
			}
		}
	}
	// マッピング用テクスチャ（白）インターフェースのリリース処理
	if (m_pTextureBG != NULL) {
		m_pTextureBG = NULL;
	}
}

CMotionPartsX::MOSIONPARTSX *CMotionPartsX::GetMotionPartsX(TYPE type)
{
	return m_MotionPartsX[type];
}
