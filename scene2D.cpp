//*****************************************************************************
//   インクルードファイル
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"

#include "scene.h"
#include "scene2D.h"

//=======================================================================================
//   コンストラクタ
//=======================================================================================
CScene2D::CScene2D(int nPriority) :CScene(nPriority)
{
}

//=======================================================================================
//   デストラクタ
//=======================================================================================
CScene2D::~CScene2D()
{
}

//=======================================================================================
//   初期化処理
//=======================================================================================
void CScene2D::Init()
{
	// 頂点設定
	MakeVex();
}

//=======================================================================================
//   終了処理
//=======================================================================================
void CScene2D::Uninit()
{
	SAFE_RELEASE( m_pVtxBuff );      // 頂点バッファの破棄
	CScene::Release();
}

//=======================================================================================
//   更新処理
//=======================================================================================
void CScene2D::Update()
{
}

//=======================================================================================
//   描画処理
//=======================================================================================
void CScene2D::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULLチェックしてください！", "エラー", MB_OK | MB_ICONASTERISK);         // エラーメッセージ
		return;
	}
	
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource( 0,
							  m_pVtxBuff,              // ストリームのもとになる頂点のバッファの始点
							  0,                       // オフセット（バイト）
							  sizeof(VERTEX_2D));      // 一つの頂点データのサイズ（ストライド量）

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_2D );

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
void CScene2D::MakeVex(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULLチェックしてください！", "エラー", MB_OK | MB_ICONASTERISK);         // エラーメッセージ
		return;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer( sizeof( VERTEX_2D ) * NUM_VERTEX,           // 作成したい頂点バッファのサイズ（一つの頂点*頂点数）
								 D3DUSAGE_WRITEONLY,                         // 書き込むしかしない（チェックしない）
								 FVF_VERTEX_2D,                              // どんな頂点で書くの（0にしてもOK）
								 D3DPOOL_MANAGED,                            // メモリ管理をお任せにする
								 &m_pVtxBuff,
								 NULL );

	// 頂点情報を設定
	// 頂点情報格納用疑似バッファの宣言
	VERTEX_2D* pVtx;

	// 頂点バッファをロックして、仮想アドレスを取得する（0,0を記入すると全部をロック）
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定（ 2D座標・右回り ）
	pVtx[0].pos = D3DXVECTOR3( m_pos.x          , m_pos.y           , 0.0f);                 // 左上の座標
	pVtx[1].pos = D3DXVECTOR3( m_pos.x + m_scl.x, m_pos.y           , 0.0f);                 // 右上の座標
	pVtx[2].pos = D3DXVECTOR3( m_pos.x          , m_pos.y + m_scl.y, 0.0f);                 // 左下の座標
	pVtx[3].pos = D3DXVECTOR3( m_pos.x + m_scl.x, m_pos.y + m_scl.y, 0.0f);                 // 右下の座標

	// rhwの設定（必ず1.0f）
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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