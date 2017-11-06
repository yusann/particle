//*****************************************************************************
//   インクルードファイル
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "sceneMesh.h"
#include "meshField.h"
#include "texture.h"

//*****************************************************************************
//   定数定義
//*****************************************************************************
#define FIELD_WIDTH       (12.0f)                                      // 幅
#define FIELD_DEPTH       (12.0f)                                      // 奥行
#define FIELD_TEXU        (0.1f)                                     // テクスチャ横分割数
#define FIELD_TEXV        (0.1f)                                     // テクスチャ縦分割数
#define FIELD_HEIGHT      (10)

//=======================================================================================
//   コンストラクタ
//=======================================================================================
CMeshField::CMeshField() :CSceneMesh(1)
{
}

//=======================================================================================
//   デストラクタ
//=======================================================================================
CMeshField::~CMeshField()
{
}

//==================================================================================================================================================
// 作成処理
//==================================================================================================================================================
CMeshField *CMeshField::Create(void)
{
	CMeshField *pScene3D;                            // 変数宣言
	pScene3D = new CMeshField;                       // 動的確保
	pScene3D->Init();                            // 初期化
	return pScene3D;                             // 値を返す
}

//=======================================================================================
//   初期化処理
//=======================================================================================
void CMeshField::Init()
{
	LoadFile();

	// メンバ変数の初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(FIELD_WIDTH, FIELD_DEPTH);

	// プライベート変数の初期化
	m_pIdxBuff = NULL;
	m_divide = D3DXVECTOR2((float)m_MaxX + 1, (float)m_MaxY);
	m_texUV = D3DXVECTOR2(FIELD_TEXU, FIELD_TEXV);
	m_nPolygonNum = (int)((m_divide.y*(m_divide.x * 2)) + ((m_divide.y - 1) * 2) - 2);
	m_nVexNum = (int)(m_divide.x * (m_divide.y + 1));

	// 頂点設定
	MakeVex();
	MakeBuff();

	// テクスチャセット
	m_pTexture = *CTexture::GetTexture((int)CTexture::TEXTYPE_FIELD);

	// タイプの代入
	CSceneMesh::Init();
}

//=======================================================================================
//   終了処理
//=======================================================================================
void CMeshField::Uninit()
{
	CSceneMesh::Uninit();
}

//=======================================================================================
//   更新処理
//=======================================================================================
void CMeshField::Update()
{
}

//=======================================================================================
//   描画処理
//=======================================================================================
void CMeshField::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULLチェックしてください！", "エラー", MB_OK | MB_ICONASTERISK);         // エラーメッセージ
		return;
	}
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	CSceneMesh::Draw();
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

//=======================================================================================
// ポリゴンの頂点設定
//=======================================================================================
void CMeshField::MakeVex(void)
{
	// 変数宣言
	int nCntHeight;     // 地面ループ用
	int nCntWidth;      // 地面ループ用

						// 法線の計算用変数
	D3DXVECTOR3 m_vtxPos[128 + 1][128 + 1];
	D3DXVECTOR3 directionX, directionZ, normalX, normalZ, normal;

	const float g_fieldPosX = m_MaxX*FIELD_WIDTH*-0.5f;                            // X座標
	const float g_fieldPosZ = m_MaxY*FIELD_DEPTH*0.5f;                                 // Z座標

	// 座標代入
	for (nCntHeight = 0; nCntHeight < (int)m_divide.y + 1; nCntHeight++)
	{
		for (nCntWidth = 0; nCntWidth < (int)m_divide.x; nCntWidth++)
		{
			m_vtxPos[nCntHeight][nCntWidth].x = g_fieldPosX + (nCntWidth * m_size.x);
			m_vtxPos[nCntHeight][nCntWidth].y = m_PosY[nCntHeight*(int)m_divide.x+nCntWidth];
			m_vtxPos[nCntHeight][nCntWidth].z = g_fieldPosZ - (nCntHeight * m_size.y);
		}
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULLチェックしてください！", "エラー", MB_OK | MB_ICONASTERISK);         // エラーメッセージ
		return;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVexNum,           // 作成したい頂点バッファのサイズ（一つの頂点*頂点数）
		D3DUSAGE_WRITEONLY,                                      // 書き込むしかしない（チェックしない）
		FVF_VERTEX_3D,                                           // どんな頂点で書くの（0にしてもOK）
		D3DPOOL_MANAGED,                                         // メモリ管理をお任せにする
		&m_pVtxBuff,
		NULL);

	// 頂点情報を設定
	// 頂点情報格納用疑似バッファの宣言
	VERTEX_3D* pVtx;

	// 頂点バッファをロックして、仮想アドレスを取得する（0,0を記入すると全部をロック）
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntHeight = 0; nCntHeight < m_divide.y + 1; nCntHeight++)
	{
		for (nCntWidth = 0; nCntWidth < m_divide.x; nCntWidth++, pVtx++)
		{
			// 頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(g_fieldPosX + (nCntWidth * m_size.x),      // X座標の設定（ 3D座標・右回り ）
				m_PosY[nCntHeight*(int)m_divide.x + nCntWidth],                                           // Y座標の設定
				g_fieldPosZ - (nCntHeight * m_size.y));    // Z座標の設定

														   // 外側の頂点実行しない
			if (nCntHeight != 0 && nCntHeight != m_divide.y &&
				nCntWidth != 0 && nCntWidth != m_divide.x - 1)
			{
				// X軸の法線計算
				directionX = m_vtxPos[nCntHeight][nCntWidth + 1] - m_vtxPos[nCntHeight][nCntWidth - 1];
				normalX.x = -directionX.y;
				normalX.y = directionX.x;
				normalX.z = 0.0f;

				// Z軸の法線計算
				directionZ = m_vtxPos[nCntHeight + 1][nCntWidth] - m_vtxPos[nCntHeight - 1][nCntWidth];
				normalZ.x = 0.0f;
				normalZ.y = directionZ.x;
				normalZ.z = -directionZ.y;

				// X軸とZ軸の法線計算
				normal = normalX + normalZ;

				// 1にノーマライズ
				D3DXVec3Normalize(&normal, &normal);
			}
			// 外側の頂点法線設定
			else
			{
				normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			}

			pVtx[0].normal = D3DXVECTOR3(normal.x, normal.y, normal.z);					// 法線の設定
			pVtx[0].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);						// カラーの設定（0~255の整数値）
			pVtx[0].tex = D3DXVECTOR2(0.0f + (nCntWidth * m_texUV.x),					// テクスチャU座標の設定
				0.0f + (nCntHeight * m_texUV.y));									// テクスチャV座標の設定
		}
	}

	// 鍵を開ける
	m_pVtxBuff->Unlock();
}


//=======================================================================================
// ポリゴンのバッファ設定
//=======================================================================================
void CMeshField::MakeBuff(void)
{
	// 変数宣言
	int nCntHeight;     // 地面ループ用
	int nCntWidth;      // 地面ループ用

						// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULLチェックしてください！", "エラー", MB_OK | MB_ICONASTERISK);         // エラーメッセージ
		return;
	}

	// インデックスバッファの作成
	HRESULT hr;
	hr = pDevice->CreateIndexBuffer(sizeof(WORD) * (m_nPolygonNum + 2),        // 頂点数（WORDは16Bid）
		D3DUSAGE_WRITEONLY,              // 使用用途フラグ（書き込むのみ）
		D3DFMT_INDEX16,                  // データのフォーマット（32Bid設定した時16を32に変更）
		D3DPOOL_MANAGED,                 // メモリの管理方法（お任せ）
		&m_pIdxBuff,              // バッファ―インターフェイスポインタのアドレス
		NULL);

	// インデックス情報格納用疑似バッファの宣言
	WORD* pIdx;

	// ロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// Z軸のループ
	for (nCntHeight = 0; nCntHeight < m_divide.y; nCntHeight++)
	{
		// X軸のループ
		for (nCntWidth = 0; nCntWidth < m_divide.x; nCntWidth++, pIdx += 2)                          // ループするとpIdxを2つ前進
		{
			// 1列目以外の1行目の処理
			if (nCntHeight != 0 && nCntWidth == 0) {
				pIdx[0] = (unsigned short)(m_divide.x + (nCntHeight * m_divide.x + nCntWidth));                  // 同じ処にインデックスをうつ
				pIdx += 1;                                                                               // 1つ前進
			}
			// 重複しない頂点の処理
			pIdx[0] = (unsigned short)(m_divide.x + (nCntHeight * m_divide.x + nCntWidth));
			pIdx[1] = (unsigned short)(m_divide.x + (nCntHeight * m_divide.x + nCntWidth) - m_divide.x);

			// 最終列目以外の最終行目の処理
			if (nCntHeight != m_divide.y - 1 && nCntWidth == m_divide.x - 1) {
				pIdx[2] = (unsigned short)(m_divide.x + (nCntHeight * m_divide.x + nCntWidth) - m_divide.x);  // 同じ処にインデックスをうつ
				pIdx += 1;                                                                               // 1つ前進
			}
		}
	}
	// ロック解放
	m_pIdxBuff->Unlock();
}

float CMeshField::GetHeight(D3DXVECTOR3 pos)
{
	// 頂点情報を設定
	// 頂点情報格納用疑似バッファの宣言
	VERTEX_3D* pVtx;

	// 頂点バッファをロックして、仮想アドレスを取得する（0,0を記入すると全部をロック）
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 座標変換
	float posZ = -pos.z + (FIELD_DEPTH*m_MaxY*0.5f);
	float posX = pos.x + (FIELD_WIDTH*m_MaxX*0.5f);

	// 座標からインデックス変換
	int indexZ = (int)(posZ / FIELD_DEPTH);
	int indexX = (int)(posX / FIELD_WIDTH);

	if (indexX < 0 || indexZ < 0)
	{
		return pos.y;
	}

	// 計算用変数
	D3DXVECTOR3 normal, work;
	D3DXVECTOR3 vecF, vecT;
	D3DXVECTOR3 pos0, pos1, pos2;
	D3DXVECTOR3 posT = pos;

	// 頂点座標代入
	pos0 = pVtx[indexZ*(m_MaxX + 1) + indexX + 1].pos;
	pos1 = pVtx[(indexZ + 1)*(m_MaxX + 1) + indexX + 1].pos;
	pos2 = pVtx[indexZ*(m_MaxX + 1) + indexX].pos;

	// ベクトル計算
	vecF = pos1 - pos0;
	vecT = posT - pos0;

	// 内積計算
	D3DXVec3Cross(&vecF, &vecF, &vecT);

	// 範囲内チェック
	if (vecF.y >= 0.0f)
	{
		vecF = pos2 - pos1;
		vecT = posT - pos1;
		D3DXVec3Cross(&vecF, &vecF, &vecT);
		if (vecF.y >= 0.0f)
		{
			vecF = pos0 - pos2;
			vecT = posT - pos2;
			D3DXVec3Cross(&vecF, &vecF, &vecT);
			if (vecF.y >= 0.0f)
			{
				// 高さ計算（法線）
				vecF = pos1 - pos0;
				vecT = pos2 - pos0;
				D3DXVec3Cross(&normal, &vecF, &vecT);

				// ノーマライズ（単位１にする）
				D3DXVec3Normalize(&normal, &normal);

				// 鍵を開ける
				m_pVtxBuff->Unlock();

				// 高さを返す
				return pos0.y - ((pos.x - pos0.x) * normal.x + (pos.z - pos0.z) * normal.z) / normal.y;
			}
		}
	}
	pos0 = pVtx[(indexZ + 1)*(m_MaxX + 1) + indexX].pos;
	pos1 = pVtx[indexZ*(m_MaxX + 1) + indexX].pos;
	pos2 = pVtx[(indexZ + 1)*(m_MaxX + 1) + indexX + 1].pos;

	// 鍵を開ける
	m_pVtxBuff->Unlock();

	vecF = pos1 - pos0;
	vecT = posT - pos0;
	D3DXVec3Cross(&vecF, &vecF, &vecT);
	if (vecF.y >= 0.0f)
	{
		vecF = pos2 - pos1;
		vecT = posT - pos1;
		D3DXVec3Cross(&vecF, &vecF, &vecT);
		if (vecF.y >= 0.0f)
		{
			vecF = pos0 - pos2;
			vecT = posT - pos2;
			D3DXVec3Cross(&vecF, &vecF, &vecT);
			if (vecF.y >= 0.0f)
			{
				vecF = pos1 - pos0;
				vecT = pos2 - pos0;
				D3DXVec3Cross(&normal, &vecF, &vecT);
				D3DXVec3Normalize(&normal, &normal);
				return pos0.y - ((pos.x - pos0.x) * normal.x + (pos.z - pos0.z) * normal.z) / normal.y;
			}
		}
	}
	return pos.y;
}

void CMeshField::LoadFile(void)
{
	char aWork[64];
	// 書き出し処理
	FILE *pFile;
	fopen_s(&pFile, "data/field.txt", "r+");

	if (pFile == NULL)
	{
		assert(!("ロードするファイル（LOAD_NAME）がありません！"));
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
			else if (strcmp(aWork, "LOAD_SIZE") == 0) {

				//  
				fscanf(pFile, "%d", &m_MaxX);
				fscanf(pFile, "%d", &m_MaxY);
				m_PosY = new float[(m_MaxY + 1)*(m_MaxX + 1)];

				// 終了
				while (1) {
					// 文字読み込み
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "END_SIZE") == 0) {
						break;
					}
				}
				continue;
			}
			else if (strcmp(aWork, "LOAD_FIELD") == 0) {

				for (int i = 0; i < m_MaxY + 1; i++)
				{
					for (int j = 0; j < m_MaxX + 1; j++)
					{
						// 座標
						fscanf(pFile, "%f ", &m_PosY[i*(m_MaxX + 1) + j]);
					}
				}

				// 終了
				while (1) {
					// 文字読み込み
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "END_FIELD") == 0) {
						break;
					}
				}
				continue;
			}

			//  ロード終了
			else if (strcmp(aWork, "END_LOAD") == 0) {
				break;
			}

			// エラー
			assert(!("セーブ内容が間違っている！！"));
		}
		//  ファイルを閉じる
		fclose(pFile);
	}

}