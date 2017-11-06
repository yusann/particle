//*****************************************************************************
//   インクルードファイル
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"

#include "scene.h"
#include "sceneMesh.h"
#include "meshCube.h"

//=======================================================================================
//   コンストラクタ
//=======================================================================================
CMeshCube::CMeshCube() :CSceneMesh(1)
{
}

//=======================================================================================
//   デストラクタ
//=======================================================================================
CMeshCube::~CMeshCube()
{
}

//==================================================================================================================================================
// 作成処理
//==================================================================================================================================================
CMeshCube *CMeshCube::Create(void)
{
	CMeshCube *pScene3D;                            // 変数宣言
	pScene3D = new CMeshCube;                       // 動的確保
	pScene3D->Init();                            // 初期化
	return pScene3D;                             // 値を返す
}

//=======================================================================================
//   初期化処理
//=======================================================================================
void CMeshCube::Init(void)
{
	// メンバ変数の初期化
	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_scl = D3DXVECTOR3(10.0f, 10.0f, 10.0f);

	// プライベート変数の初期化
	m_pIdxBuff = NULL;
	m_nPolygonNum = 16;
	m_nVexNum = 8;

	// 頂点設定
	MakeVex();
	MakeBuff();

	// テクスチャセット
	m_pTexture = NULL;

	// タイプの代入
	CSceneMesh::Init();
}

//=======================================================================================
//   終了処理
//=======================================================================================
void CMeshCube::Uninit()
{
	CSceneMesh::Uninit();
}

//=======================================================================================
//   更新処理
//=======================================================================================
void CMeshCube::Update()
{
	m_rot.x += 0.01f;
}

//=======================================================================================
//   描画処理
//=======================================================================================
void CMeshCube::Draw()
{
	CSceneMesh::Draw();
}

//=======================================================================================
// ポリゴンの頂点設定
//=======================================================================================
void CMeshCube::MakeVex(void)
{
	// 変数宣言
	int nCntHeight;     // 地面ループ用
	float fLength = sqrt((m_scl.x * m_scl.x) + (m_scl.z * m_scl.z)) * 0.5f;
	float fAngle = atan2(m_scl.z, m_scl.x);

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULLチェックしてください！", "エラー", MB_OK | MB_ICONASTERISK);         // エラーメッセージ
		return;
	}

	// 頂点バッファを作る
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVexNum,           // 作成したい頂点バッファのサイズ（一つの頂点*頂点数）
		D3DUSAGE_WRITEONLY,                                      // 書き込むしかしない（チェックしない）
		FVF_VERTEX_3D,                                           // どんな頂点で書くの（0にしてもOK）
		D3DPOOL_MANAGED,                                         // メモリ管理をお任せにする
		&m_pVtxBuff,
		NULL);

	// 頂点情報格納用疑似バッファの宣言
	VERTEX_3D* pVtx;

	// 頂点バッファをロックして、仮想アドレスを取得する（0,0を記入すると全部をロック）
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntHeight = 0; nCntHeight < 2; nCntHeight++, pVtx += 4)
	{
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(cosf(-fAngle + D3DX_PI) * fLength,		// X座標の設定
			nCntHeight * m_scl.y- (m_scl.y*0.5f),						// Y座標の設定
			sinf(-fAngle + D3DX_PI) * fLength);							// Z座標の設定
		pVtx[1].pos = D3DXVECTOR3(cosf(fAngle) * fLength,					// X座標の設定
			nCntHeight * m_scl.y - (m_scl.y*0.5f),					// Y座標の設定
			sinf(fAngle) * fLength);									// Z座標の設定
		pVtx[2].pos = D3DXVECTOR3(cosf(-fAngle) * fLength,					// X座標の設定
			nCntHeight * m_scl.y - (m_scl.y*0.5f),					// Y座標の設定
			sinf(-fAngle) * fLength);									// Z座標の設定
		pVtx[3].pos = D3DXVECTOR3(cosf(fAngle + D3DX_PI) * fLength,			// X座標の設定
			nCntHeight * m_scl.y - (m_scl.y*0.5f),					// Y座標の設定
			sinf(fAngle + D3DX_PI) * fLength);							// Z座標の設定

		pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);                          // 法線の設定
		pVtx[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);                          // 法線の設定
		pVtx[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);                          // 法線の設定
		pVtx[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);                          // 法線の設定

		pVtx[0].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);                       // カラーの設定
		pVtx[1].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);                       // カラーの設定
		pVtx[2].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);                       // カラーの設定
		pVtx[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);                       // カラーの設定

		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);            // テクスチャ座標の設定
		pVtx[1].tex = D3DXVECTOR2(0.0f, 1.0f);            // テクスチャ座標の設定
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);            // テクスチャ座標の設定
		pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);            // テクスチャ座標の設定
	}
	// 鍵を開ける
	m_pVtxBuff->Unlock();
}


//=======================================================================================
// ポリゴンのバッファ設定
//=======================================================================================
void CMeshCube::MakeBuff(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULLチェックしてください！", "エラー", MB_OK | MB_ICONASTERISK);         // エラーメッセージ
		return;
	}

	// インデックスバッファの作成
	HRESULT hr;
	hr = pDevice->CreateIndexBuffer(sizeof(WORD) * 18,        // 頂点数（WORDは16Bid）
									D3DUSAGE_WRITEONLY,              // 使用用途フラグ（書き込むのみ）
									D3DFMT_INDEX16,                  // データのフォーマット（32Bid設定した時16を32に変更）
									D3DPOOL_MANAGED,                 // メモリの管理方法（お任せ）
									&m_pIdxBuff,              // バッファ―インターフェイスポインタのアドレス
									NULL);

	// インデックス情報格納用疑似バッファの宣言
	WORD* pIdx;

	// ロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 4;
	pIdx[1] = 7;
	pIdx[2] = 5;
	pIdx[3] = 6;
	pIdx[4] = 6;
	pIdx[5] = 2;
	pIdx[6] = 5;
	pIdx[7] = 1;
	pIdx[8] = 4;
	pIdx[9] = 0;
	pIdx[10] = 7;
	pIdx[11] = 3;
	pIdx[12] = 6;
	pIdx[13] = 2;
	pIdx[14] = 2;
	pIdx[15] = 3;
	pIdx[16] = 1;
	pIdx[17] = 0;
/*
	pIdx[0] = 4;
	pIdx[1] = 5;
	pIdx[2] = 7;
	pIdx[3] = 6;
	pIdx[4] = 2;
	pIdx[5] = 6;
	pIdx[6] = 1;
	pIdx[7] = 5;
	pIdx[8] = 0;
	pIdx[9] = 4;
	pIdx[10] = 3;
	pIdx[11] = 7;
	pIdx[12] = 2;
	pIdx[13] = 6;
	pIdx[14] = 2;
	pIdx[15] = 1;
	pIdx[16] = 3;
	pIdx[17] = 0;*/

	// ロック解放
	m_pIdxBuff->Unlock();
}
