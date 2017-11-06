//*****************************************************************************
//   インクルードファイル
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"

#include "scene.h"
#include "scene3D.h"

//=======================================================================================
//   コンストラクタ
//=======================================================================================
CScene3D::CScene3D(int nPriority) :CScene(nPriority)
{
}

//=======================================================================================
//   デストラクタ
//=======================================================================================
CScene3D::~CScene3D()
{
}

//=======================================================================================
//   初期化処理
//=======================================================================================
void CScene3D::Init()
{
	// 頂点設定
	MakeVex();
}

//=======================================================================================
//   終了処理
//=======================================================================================
void CScene3D::Uninit()
{
	SAFE_RELEASE( m_pVtxBuff );      // 頂点バッファの破棄
	CScene::Release();
}

//=======================================================================================
//   更新処理
//=======================================================================================
void CScene3D::Update()
{
}

//=======================================================================================
//   描画処理
//=======================================================================================
void CScene3D::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULLチェックしてください！", "エラー", MB_OK | MB_ICONASTERISK);         // エラーメッセージ
		return;
	}

	// 変換行列の宣言
	D3DXMATRIX mtxRot;             // ローカル回転
	D3DXMATRIX mtxScl;             // ローカルスケール
	D3DXMATRIX mtxPos;             // ローカル座標
	D3DXMATRIX mtxWorld;           // ワールド情報

	// ローカル回転の代入
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		fRot,
		0.0f,
		0.0f);

	// ローカル座標の代入
	D3DXMatrixTranslation(&mtxPos,
		m_pos.x,
		m_pos.z,
		m_pos.y);

	// ワールド情報処理
	D3DXMatrixIdentity(&mtxWorld);                       // ワールドの中身を初期化
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);   // ワールド回転の代入
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxPos);   // ワールド座標の代入
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);       // ワールド情報セット

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource( 0,
							  m_pVtxBuff,              // ストリームのもとになる頂点のバッファの始点
							  0,                       // オフセット（バイト）
							  sizeof(VERTEX_3D));      // 一つの頂点データのサイズ（ストライド量）

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_3D );

	// 描画直前にテクスチャをセット（テクスチャの設定）
	pDevice->SetTexture( 0, m_pTexture );

	// ポリゴンの描画
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,        // プリミティブの種類
							0,                          // オフセット（頂点数）
							NUM_POLYGON );              // プリミティブの数（ポリゴンの数）
}

//=======================================================================================
// ポリゴンの頂点設定
//=======================================================================================
void CScene3D::MakeVex(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULLチェックしてください！", "エラー", MB_OK | MB_ICONASTERISK);         // エラーメッセージ
		return;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * NUM_VERTEX,           // 作成したい頂点バッファのサイズ（一つの頂点*頂点数）
								 D3DUSAGE_WRITEONLY,                         // 書き込むしかしない（チェックしない）
								 FVF_VERTEX_3D,                              // どんな頂点で書くの（0にしてもOK）
								 D3DPOOL_MANAGED,                            // メモリ管理をお任せにする
								 &m_pVtxBuff,
								 NULL );

	// 頂点情報を設定
	// 頂点情報格納用疑似バッファの宣言
	VERTEX_3D* pVtx;

	// 頂点バッファをロックして、仮想アドレスを取得する（0,0を記入すると全部をロック）
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定（ 2D座標・右回り ）
	pVtx[0].pos = D3DXVECTOR3( m_pos.x - m_scl.x, m_scl.y, m_pos.y + m_scl.z);                 // 左上の座標
	pVtx[1].pos = D3DXVECTOR3( m_pos.x + m_scl.x, m_scl.y, m_pos.y + m_scl.z);                 // 右上の座標
	pVtx[2].pos = D3DXVECTOR3( m_pos.x - m_scl.x, m_scl.y, m_pos.y - m_scl.z);                 // 左下の座標
	pVtx[3].pos = D3DXVECTOR3( m_pos.x + m_scl.x, m_scl.y, m_pos.y - m_scl.z);                 // 右下の座標
	
	// 頂点法線の設定
	pVtx[0].normal = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
	pVtx[1].normal = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
	pVtx[2].normal = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
	pVtx[3].normal = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);


	// 頂点カラーの設定（0~255の整数値）
	pVtx[0].color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );  // 左上の色
	pVtx[1].color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );  // 右上の色
	pVtx[2].color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );  // 左下の色
	pVtx[3].color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );  // 右下の色

	// 頂点データへUVデータの追加
	pVtx[0].tex = D3DXVECTOR2( 0.0f , 0.0f );                    // 左上のUV座標
	pVtx[1].tex = D3DXVECTOR2( 1.0f , 0.0f );                    // 右上のUV座標
	pVtx[2].tex = D3DXVECTOR2( 0.0f , 1.0f );                    // 左下のUV座標
	pVtx[3].tex = D3DXVECTOR2( 1.0f , 1.0f );                    // 右下のUV座標

	// 鍵を開ける
	m_pVtxBuff->Unlock();
}
