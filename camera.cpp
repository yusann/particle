//*****************************************************************************
//   インクルードファイル
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"

#include "camera.h"
#include "input.h"

//*****************************************************************************
//   定数定義
//*****************************************************************************
#define CAMERA_MOVE     (0.2f)                    // 移動量
#define CAMERA_ANGLE    (0.05f)                   // 回転量
#define CAMERA_POSY     (15.0f)                   // 初期Y座標
#define CAMERA_POSZ     (-20.0f)                  // 初期Z座標
#define CAMERA_ZOOM     (0.01f)                   // ズーム速度

//*****************************************************************************
//   静的メンバ変数
//*****************************************************************************
CCamera::CAMERA CCamera::m_pCamera;

//=======================================================================================
//   コンストラクタ
//=======================================================================================
CCamera::CCamera()
{
}

//=======================================================================================
//   デストラクタ
//=======================================================================================
CCamera::~CCamera()
{
}

//=======================================================================================
//   初期化処理
//=======================================================================================
void CCamera::Init()
{
	// カメラの初期化
	m_pCamera.vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);                 // 上方向（Y軸の上）
	m_posMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);                 // 移動量
	m_rot = D3DXVECTOR3(0.3f, -1.5f, 0.0f);                         // 角度
	m_rotX = 0.0f;
	m_rotY = 0.0f;
	m_length = -CAMERA_POSZ;
	m_fZoom = D3DX_PI / 3.0f;                                  // 視野角（60°）
	m_pCamera.posEye.x = cos(m_rot.y) * cos(m_rot.x) * m_length + m_posMove.x;
	m_pCamera.posEye.z = sin(m_rot.y) * cos(m_rot.x) * m_length + m_posMove.z;
	m_pCamera.posEye.y = sin(m_rot.x) * m_length + m_posMove.y;
	m_pCamera.posAt = m_posMove;
	D3DXMatrixIdentity(&m_pCamera.mtxView);                            // ワールドの中身を初期化
}

//=======================================================================================
//   終了処理
//=======================================================================================
void CCamera::Uninit()
{
	// カメラの初期化
	m_pCamera.vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);                 // 上方向（Y軸の上）
	m_posMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);                 // 移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);                 // 角度
	m_length = CAMERA_POSZ;
	m_fZoom = D3DX_PI / 3.0f;                                  // 視野角（60°）
	m_pCamera.posEye.x = cos(m_rot.y) * cos(m_rot.x) * m_length + m_posMove.x;
	m_pCamera.posEye.z = sin(m_rot.y) * cos(m_rot.x) * m_length + m_posMove.z;
	m_pCamera.posEye.y = sin(m_rot.x) * m_length + m_posMove.y;
	m_pCamera.posAt = m_posMove;
	D3DXMatrixIdentity(&m_pCamera.mtxView);                            // ワールドの中身を初期化
}

//=======================================================================================
//   更新処理
//=======================================================================================
void CCamera::Update()
{
	if (ImGui::IsMouseHoveringAnyWindow())
	{
		return;
	}
	// ベクトルデータの宣言
	float fRad = sqrt(((m_pCamera.posAt.x - m_pCamera.posEye.x) * (m_pCamera.posAt.x - m_pCamera.posEye.x)) +
		((m_pCamera.posAt.z - m_pCamera.posEye.z) * (m_pCamera.posAt.z - m_pCamera.posEye.z)));   // 距離の宣言及び算出

	// 移動量のベクトル宣言
	D3DXVECTOR3 work;
	D3DXVECTOR3 move;

	// Y軸無視の長さ（カメラを軸とした長さ）
	work.x = m_pCamera.posAt.x - m_pCamera.posEye.x;    // X軸
	work.y = m_pCamera.posAt.y - m_pCamera.posEye.y;                                      // Y軸
	work.z = m_pCamera.posAt.z - m_pCamera.posEye.z;    // Z軸

	// 移動量ノーマライズ（初期化：方向はそのまま、大きさを１にする）
	D3DXVec3Normalize(&move, &work);

	// キー判定 ///////////////////////////////////////////////////////////////////////////////
	m_length -= CManager::GetInputMouse()->GetAxisZ() * 0.02f;
	if (CManager::GetInputMouse()->GetCenterPress())
	{
		m_length += CManager::GetInputMouse()->GetAxisZ() * 0.01f;
		float speed = CManager::GetInputMouse()->GetAxisX()*0.1f;

		m_posMove.x -= move.z * speed;
		m_posMove.z += move.x * speed;

		speed = CManager::GetInputMouse()->GetAxisY()*0.1f;
		m_posMove.y -= move.y * speed;
	}
	if (m_length < 0.0f)
	{
		m_length = 0.0f;
	}
	if (CManager::GetInputMouse()->GetLeftPress())
	{
		m_rot.y -= CManager::GetInputMouse()->GetAxisX() * 0.01f;
		m_rot.x += CManager::GetInputMouse()->GetAxisY() * 0.01f;
		if (m_rot.x > 3.141592f*0.5f)
		{
			m_rot.x = 3.141592f*0.5f;
		}
		if (m_rot.x < -3.141592f*0.5f)
		{
			m_rot.x = -3.141592f*0.5f;
		}
	}
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_X))
	{
		m_length = -CAMERA_POSZ;
		m_rot = D3DXVECTOR3(1.0f, 0.0f, 0.0f);                         // 角度
		m_posMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_pCamera.posEye.x = cos(m_rot.y) * cos(m_rot.x) * m_length + m_posMove.x;
	m_pCamera.posEye.z = sin(m_rot.y) * cos(m_rot.x) * m_length + m_posMove.z;
	m_pCamera.posEye.y = sin(m_rot.x) * m_length + m_posMove.y;
	m_pCamera.posAt = m_posMove;
}

//=======================================================================================
//   セット処理
//=======================================================================================
void CCamera::Set(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULLチェックしてください！", "エラー", MB_OK | MB_ICONASTERISK);         // エラーメッセージ
		return;
	}

	// プロジェクション行列
	D3DXMATRIX mtxProj;
	D3DXMatrixPerspectiveFovLH(&mtxProj,                               // Fovは画角
		m_fZoom,                          // 画角（視野角）60度にするためπ÷３
		(float)SCREEN_WIDTH / SCREEN_HEIGHT,    // アスペクト比（floatで計算する、float>intなので片方でOK）
		1.0f,                                   // near 必ず0.0fより大きいこと  どこから描画するか
		10000.0f);                                // far どこまで描画するか
	m_pCamera.mtxProj = mtxProj;
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);

	// ビュー行列の作成 LH左手座標系
	D3DXMatrixLookAtLH(&m_pCamera.mtxView, &m_pCamera.posEye, &m_pCamera.posAt, &m_pCamera.vecUp);    // Eye,At,Upの情報からビュー行列(mtxView)を作る関数

	// ビュー行列セット
	pDevice->SetTransform(D3DTS_VIEW, &m_pCamera.mtxView);
}