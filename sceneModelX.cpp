//*****************************************************************************
//   インクルードファイル
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

#include "scene.h"
#include "sceneModelX.h"

//*****************************************************************************
//   定数定義
//*****************************************************************************
#define MODEL_NAME001        "data/MODEL/cursor000.x"

//=======================================================================================
//   コンストラクタ
//=======================================================================================
CSceneModelX::CSceneModelX(int nPriority) :CScene(nPriority)
{
	// メンバ変数の初期化
	m_vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//=======================================================================================
//   デストラクタ
//=======================================================================================
CSceneModelX::~CSceneModelX()
{
}

//=======================================================================================
//   初期化処理
//=======================================================================================
void CSceneModelX::Init()
{
	char aFileName[128] = {
		MODEL_NAME001
	};

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULLチェックしてください！", "エラー", MB_OK | MB_ICONASTERISK);         // エラーメッセージ
		return;
	}

	// マッピング用テクスチャインターフェースのリリース処理
	for( int i = 0; i < MODEL_MATERIAL_MAX; i++ )
	{
		m_pTextureModel[i] = NULL;
	}

	// モデルのエラーチェック
	if( FAILED( D3DXLoadMeshFromX( aFileName,              // ファイル名
							D3DXMESH_MANAGED,        // 
							pDevice,                 // デバイス
							NULL,
							&m_pBuffMatModel,        // モデルのマテリアル情報
							NULL,
							&m_nNumMatModel,         // マテリアル数
							&m_pMeshModel )))          // メッシュ情報
	{
		MessageBox( NULL , "モデルがありません！" , "エラー" , MB_OK | MB_ICONASTERISK );         // 終了するメッセージ
	}
	D3DXMATERIAL* pMat;
	pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();
	
	for( int i = 0; i < (int)m_nNumMatModel; i++ )
	{
		// テクスチャの設定
		if( pMat[i].pTextureFilename != NULL )
		{
			// テクスチャの読み込み(ディスクから)
			if( FAILED( D3DXCreateTextureFromFile( pDevice, pMat[i].pTextureFilename, &m_pTextureModel[i] )))
			{
				MessageBox( NULL , "画像がありません！" , "エラー" , MB_OK | MB_ICONASTERISK );         // 終了するメッセージ
			}
		}
	}

	// テクスチャセット
	m_pTexture = *CTexture::GetTexture((int)CTexture::TEXTYPE_WHITE);

	// プロジェクション行列
	D3DXMATRIX mtxProj;
	D3DXMatrixPerspectiveFovLH( &mtxProj,                               // Fovは画角
								D3DX_PI / 3.0f,                         // 画角（視野角）60度にするためπ÷３
								(float)SCREEN_WIDTH / SCREEN_HEIGHT,    // アスペクト比（floatで計算する、float>intなので片方でOK）
								1.0f,                                   // near 必ず0.0fより大きいこと  どこから描画するか
								100.0f);                                // far どこまで描画するか
	pDevice->SetTransform( D3DTS_PROJECTION, &mtxProj );
}

//=======================================================================================
//   終了処理
//=======================================================================================
void CSceneModelX::Uninit()
{
	// メッシュ情報のリリース処理
	SAFE_RELEASE(m_pMeshModel);

	// マテリアル情報のリリース処理
	SAFE_RELEASE(m_pBuffMatModel);

	// マッピング用テクスチャインターフェースのリリース処理
	for( int i = 0; i < MODEL_MATERIAL_MAX; i++ )
	{
		SAFE_RELEASE(m_pTextureModel[i]);
	}
	CScene::Release();
}

//=======================================================================================
//   更新処理
//=======================================================================================
void CSceneModelX::Update()
{
}

//=======================================================================================
//   描画処理
//=======================================================================================
void CSceneModelX::Draw()
{
	if (m_pBuffMatModel == NULL) { return; }

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
	D3DXMatrixRotationYawPitchRoll( &mtxRot,
									m_rot.y,
									m_rot.x,
									m_rot.z );

	// ローカルスケールの代入
	D3DXMatrixScaling( &mtxScl,
					   m_scl.x,
					   m_scl.y,
					   m_scl.z );

	// ローカル座標の代入
	D3DXMatrixTranslation( &mtxPos,
						   m_pos.x,
						   m_pos.y,
						   m_pos.z );
	
	// ワールド情報処理
	D3DXMatrixIdentity( &mtxWorld );                       // ワールドの中身を初期化
	D3DXMatrixMultiply( &mtxWorld, &mtxWorld, &mtxRot );   // ワールド回転の代入
	D3DXMatrixMultiply( &mtxWorld, &mtxWorld, &mtxScl );   // ワールドスケールの代入
	D3DXMatrixMultiply( &mtxWorld, &mtxWorld, &mtxPos );   // ワールド座標の代入
	pDevice->SetTransform( D3DTS_WORLD, &mtxWorld );       // ワールド情報セット

	D3DMATERIAL9 matDef;
	pDevice->GetMaterial( &matDef );                 // 現在デバイスに設定されてるアテリアル情報を取得

	D3DXMATERIAL* pMat;
	pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();

	for( int i = 0; i < (int)m_nNumMatModel; i++ )
	{
		//  拡散光の設定
		pMat[i].MatD3D.Diffuse = m_color;

		// 環境光の設定
		pMat[i].MatD3D.Ambient = m_color;

		// マテリアルの設定
		pDevice->SetMaterial( &pMat[i].MatD3D );

		// テクスチャの描画
		if( pMat[i].pTextureFilename != NULL )
		{
			pDevice->SetTexture( 0, m_pTextureModel[i] );      // テクスチャマッピング
		}
		else
		{
			pDevice->SetTexture( 0, m_pTexture );         // テクスチャマッピング(白)
		}
		// メッシュの描画
		m_pMeshModel->DrawSubset(i);
		pDevice->SetMaterial( &matDef );
	}
}
