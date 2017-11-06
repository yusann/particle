//*****************************************************************************
//   インクルードファイル
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"

#include "scene.h"
#include "sceneMesh.h"

//=======================================================================================
//   コンストラクタ
//=======================================================================================
CSceneMesh::CSceneMesh(int nPriority) :CScene(nPriority)
{
}

//=======================================================================================
//   デストラクタ
//=======================================================================================
CSceneMesh::~CSceneMesh()
{
}

//=======================================================================================
//   初期化処理
//=======================================================================================
void CSceneMesh::Init(void)
{
	// タイプの代入
	CScene::SetObjType(CScene::OBJTYPE_FIELD);
}

//=======================================================================================
//   終了処理
//=======================================================================================
void CSceneMesh::Uninit()
{
	SAFE_RELEASE( m_pVtxBuff );      // 頂点バッファの破棄
	SAFE_RELEASE(m_pIdxBuff);      // インデックスバッファの破棄
	CScene::Release();
}

//=======================================================================================
//   更新処理
//=======================================================================================
void CSceneMesh::Update()
{
}

//=======================================================================================
//   描画処理
//=======================================================================================
void CSceneMesh::Draw()
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
		m_rot.y,
		m_rot.x,
		m_rot.z);

	// ローカル座標の代入
	D3DXMatrixTranslation(&mtxPos,
		m_pos.x,
		m_pos.y,
		m_pos.z);

	// ワールド情報処理
	D3DXMatrixIdentity(&mtxWorld);                       // ワールドの中身を初期化
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);   // ワールド回転の代入
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxPos);   // ワールド座標の代入
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

	// インデックスプリミティブの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,                 // プリミティブの種類
		0,
		0,
		m_nVexNum,   // 頂点数
		0,
		m_nPolygonNum);                       // プリミティブの数（ポリゴンの数）
}
