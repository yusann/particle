//*****************************************************************************
//   インクルードファイル
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "paticle.h"
#include "texture.h"
#include "camera.h"

//=======================================================================================
//   初期化処理
//=======================================================================================
void CPaticle::Init(void)
{
	m_nPolygonNum = 8;
	m_nVexNum     = 8;
	m_pos         = D3DXVECTOR3(0.0f,0.0f,0.0f);       // 座標
	m_scl         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // サイズ
	m_rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_color       = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);// 色

	// 頂点設定
	MakeVex();
	MakeBuff();
	// テクスチャセット
	m_pTexture = *CTexture::GetTexture((int)CTexture::TEXTYPE_FIELD);
}

//=======================================================================================
//   終了処理
//=======================================================================================
void CPaticle::Uninit()
{
	SAFE_RELEASE( m_pVtxBuff );      // 頂点バッファの破棄
	SAFE_RELEASE(m_pIdxBuff);      // インデックスバッファの破棄
	SAFE_RELEASE(m_pTexture);      // インデックスバッファの破棄
}

//=======================================================================================
//   更新処理
//=======================================================================================
void CPaticle::Update()
{
	// 頂点情報を設定
	// 頂点情報格納用疑似バッファの宣言
	VERTEX_3D* pVtx;

	// 頂点バッファをロックして、仮想アドレスを取得する（0,0を記入すると全部をロック）
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報の設定
	pVtx[0].pos = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);				// Z座標の設定
	pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// 法線の設定
	pVtx[0].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// カラーの設定（0~255の整数値）
	pVtx[0].tex = D3DXVECTOR2(0.0f,1.0f);						// テクスチャV座標の設定

	pVtx[1].pos = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);				// Z座標の設定
	pVtx[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// 法線の設定
	pVtx[1].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// カラーの設定（0~255の整数値）
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);						// テクスチャV座標の設定

	pVtx[2].pos = D3DXVECTOR3(1.0f, 1.0f, 0.0f);				// Z座標の設定
	pVtx[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// 法線の設定
	pVtx[2].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// カラーの設定（0~255の整数値）
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);						// テクスチャV座標の設定

	pVtx[3].pos = D3DXVECTOR3(1.0f, -1.0f, 0.0f);				// Z座標の設定
	pVtx[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// 法線の設定
	pVtx[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// カラーの設定（0~255の整数値）
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);						// テクスチャV座標の設定

																// 頂点情報の設定
	pVtx[4].pos = D3DXVECTOR3(3-1.0f, 3+1.0f, 0.0f);				// Z座標の設定
	pVtx[4].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// 法線の設定
	pVtx[4].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// カラーの設定（0~255の整数値）
	pVtx[4].tex = D3DXVECTOR2(0.0f, 1.0f);						// テクスチャV座標の設定

	pVtx[5].pos = D3DXVECTOR3(3-1.0f, 3-1.0f, 0.0f);				// Z座標の設定
	pVtx[5].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// 法線の設定
	pVtx[5].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// カラーの設定（0~255の整数値）
	pVtx[5].tex = D3DXVECTOR2(0.0f, 0.0f);						// テクスチャV座標の設定

	pVtx[6].pos = D3DXVECTOR3(3 + 1.0f, 3 + 1.0f, 0.0f);				// Z座標の設定
	pVtx[6].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// 法線の設定
	pVtx[6].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// カラーの設定（0~255の整数値）
	pVtx[6].tex = D3DXVECTOR2(1.0f, 1.0f);						// テクスチャV座標の設定

	pVtx[7].pos = D3DXVECTOR3(3 + 1.0f, 3-1.0f, 0.0f);				// Z座標の設定
	pVtx[7].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// 法線の設定
	pVtx[7].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// カラーの設定（0~255の整数値）
	pVtx[7].tex = D3DXVECTOR2(1.0f, 0.0f);						// テクスチャV座標の設定

	// 鍵を開ける
	m_pVtxBuff->Unlock();
}

//=======================================================================================
//   描画処理
//=======================================================================================
void CPaticle::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULLチェックしてください！", "エラー", MB_OK | MB_ICONASTERISK);         // エラーメッセージ
		return;
	}

	// カメラの取得
	CCamera::CAMERA camera;
	camera = CCamera::GetCamera();

	// 変換行列の宣言
	D3DXMATRIX mtxPos;             // ローカル座標
	D3DXMATRIX mtxWorld;           // ワールド情報
	D3DXMATRIX mtxViewInverse;     // 転置行列
	D3DXMATRIX mtxViewCamera = camera.mtxView;      // カメラ行列の取得

	// ローカル座標の代入
	D3DXMatrixTranslation(&mtxPos,
		m_pos.x,
		m_pos.y,
		m_pos.z);

	// 逆行列の作成
	D3DXMatrixTranspose(&mtxViewInverse, &mtxViewCamera);

	// 平行移動成分を除去（転置前）
	mtxViewInverse._14 = 0.0f;
	mtxViewInverse._24 = 0.0f;
	mtxViewInverse._34 = 0.0f;

	// ワールド情報処理
	D3DXMatrixIdentity(&mtxWorld);                       // ワールドの中身を初期化
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxPos);   // ワールド座標の代入
	mtxWorld = mtxViewInverse * mtxWorld;

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);       // ワールド情報セット

	// パイプライン
	pDevice->SetStreamSource(0,
		m_pVtxBuff,     // ストリームのもとになる頂点のバッファの始点
		0,                       // オフセット（バイト）
		sizeof(VERTEX_3D));      // 一つの頂点データのサイズ（ストライド量）

								 // デバイスにインデックスバッファの設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// 描画直前にテクスチャをセット（テクスチャの設定）
	pDevice->SetTexture(0, m_pTexture);

	// ライトの設定（OFF）
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// インデックスプリミティブの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,                 // プリミティブの種類
		0,
		0,
		m_nVexNum,   // 頂点数
		0,
		m_nPolygonNum+1);                       // プリミティブの数（ポリゴンの数）

	// ライトの設定（ON）
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}


//=======================================================================================
// ポリゴンの頂点設定
//=======================================================================================
void CPaticle::MakeVex(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULLチェックしてください！", "エラー", MB_OK | MB_ICONASTERISK);         // エラーメッセージ
		return;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4*2,           // 作成したい頂点バッファのサイズ（一つの頂点*頂点数）
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

	// 頂点情報の設定
	pVtx[0].pos = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);				// Z座標の設定
	pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// 法線の設定
	pVtx[0].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// カラーの設定（0~255の整数値）
	pVtx[0].tex = D3DXVECTOR2(0.0f,1.0f);						// テクスチャV座標の設定

	pVtx[1].pos = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);				// Z座標の設定
	pVtx[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// 法線の設定
	pVtx[1].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// カラーの設定（0~255の整数値）
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);						// テクスチャV座標の設定

	pVtx[2].pos = D3DXVECTOR3(1.0f, 1.0f, 0.0f);				// Z座標の設定
	pVtx[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// 法線の設定
	pVtx[2].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// カラーの設定（0~255の整数値）
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);						// テクスチャV座標の設定

	pVtx[3].pos = D3DXVECTOR3(1.0f, -1.0f, 0.0f);				// Z座標の設定
	pVtx[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// 法線の設定
	pVtx[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// カラーの設定（0~255の整数値）
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);						// テクスチャV座標の設定

																// 頂点情報の設定
	pVtx[4].pos = D3DXVECTOR3(3-1.0f, 3+1.0f, 0.0f);				// Z座標の設定
	pVtx[4].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// 法線の設定
	pVtx[4].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// カラーの設定（0~255の整数値）
	pVtx[4].tex = D3DXVECTOR2(0.0f, 1.0f);						// テクスチャV座標の設定

	pVtx[5].pos = D3DXVECTOR3(3-1.0f, 3-1.0f, 0.0f);				// Z座標の設定
	pVtx[5].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// 法線の設定
	pVtx[5].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// カラーの設定（0~255の整数値）
	pVtx[5].tex = D3DXVECTOR2(0.0f, 0.0f);						// テクスチャV座標の設定

	pVtx[6].pos = D3DXVECTOR3(3 + 1.0f, 3 + 1.0f, 0.0f);				// Z座標の設定
	pVtx[6].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// 法線の設定
	pVtx[6].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// カラーの設定（0~255の整数値）
	pVtx[6].tex = D3DXVECTOR2(1.0f, 1.0f);						// テクスチャV座標の設定

	pVtx[7].pos = D3DXVECTOR3(3 + 1.0f, 3-1.0f, 0.0f);				// Z座標の設定
	pVtx[7].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			// 法線の設定
	pVtx[7].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// カラーの設定（0~255の整数値）
	pVtx[7].tex = D3DXVECTOR2(1.0f, 0.0f);						// テクスチャV座標の設定

	// 鍵を開ける
	m_pVtxBuff->Unlock();
}


//=======================================================================================
// ポリゴンのバッファ設定
//=======================================================================================
void CPaticle::MakeBuff(void)
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
	hr = pDevice->CreateIndexBuffer(sizeof(WORD) * 12,        // 頂点数（WORDは16Bid）
		D3DUSAGE_WRITEONLY,              // 使用用途フラグ（書き込むのみ）
		D3DFMT_INDEX16,                  // データのフォーマット（32Bid設定した時16を32に変更）
		D3DPOOL_MANAGED,                 // メモリの管理方法（お任せ）
		&m_pIdxBuff,              // バッファ―インターフェイスポインタのアドレス
		NULL);

	// インデックス情報格納用疑似バッファの宣言
	WORD* pIdx;

	// ロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 0;
	pIdx[1] = 0;
	pIdx[2] = 1;
	pIdx[3] = 2;
	pIdx[4] = 3;
	pIdx[5] = 3;

	pIdx[6] = 4;
	pIdx[7] = 4;
	pIdx[8] = 5;
	pIdx[9] = 6;
	pIdx[10] = 7;
	pIdx[11] = 7;

	// ロック解放
	m_pIdxBuff->Unlock();
}
