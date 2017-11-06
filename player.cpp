//*****************************************************************************
//   インクルードファイル
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "sceneModelX.h"
#include "sceneMotionPartsX.h"
#include "sceneBillboard.h"
#include "player.h"
#include "sceneMesh.h"
#include "meshField.h"
#include "input.h"

#include "camera.h"

//*****************************************************************************
//   定数定義
//*****************************************************************************
#define MOVE (0.5f)

//=======================================================================================
//   コンストラクタ
//=======================================================================================
CPlayer::CPlayer():CSceneMotionPartsX(CScene::OBJTYPE_PLAYER)
{
}

//=======================================================================================
//   デストラクタ
//=======================================================================================
CPlayer::~CPlayer()
{
}

//==================================================================================================================================================
// 作成処理
//==================================================================================================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *pSceneMD;			// 変数宣言
	pSceneMD = new CPlayer;		// 動的確保
	pSceneMD->Init(pos);		// 初期化
	return pSceneMD;			// 値を返す
}

//=======================================================================================
//   初期化処理
//=======================================================================================
void CPlayer::Init(D3DXVECTOR3 pos)
{
	// 変数の初期化
	m_Mode = MODE_NORMAL;
	m_pos = pos;
	m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Jump = 3.0f;
	m_Speed = 0.5f;


	// 親の初期化（頂点生成）
	CSceneMotionPartsX::Init();

	// タイプの代入
	CScene::SetObjType(CScene::OBJTYPE_PLAYER);
}

//=======================================================================================
//   終了処理
//=======================================================================================
void CPlayer::Uninit()
{
	CSceneMotionPartsX::Uninit();
}

//=======================================================================================
//   更新処理
//=======================================================================================
void CPlayer::Update()
{
	ImGui();
	// キー判定
	InputKey();

	m_Move.y -= 0.2f;

	switch (m_Mode)
	{
	case MODE_NORMAL: ModeNormal(); break;
	case MODE_MOVE:   ModeMove();   break;
	case MODE_ATTACK: ModeAttack(); break;
	case MODE_JUMP:   ModeJump();   break;
	default:
		assert("プレイヤーモードエラー！");
		break;
	}

	// 移動処理
	m_pos += m_Move;
	m_Move.x = 0.0f;
	m_Move.z = 0.0f;
	CMeshField *pMeshField = CManager::GetMeshField();
	if (pMeshField == NULL) { return; }
	float PosY = pMeshField->GetHeight(m_pos);
	if (m_pos.y < PosY)
	{
		m_Move.y = 0.0f;
		m_pos.y = PosY;
		if (m_Mode != MODE_ATTACK && m_Mode != MODE_MOVE)
		{
			m_Mode = MODE_NORMAL;
		}
	}
	// モーションのラストキーに達した と ジャンプモードではない と 攻撃モードではない時
	if (m_LastKye)
	{
		// ニュートラルモード変更
		m_Mode = MODE_NORMAL;

		// モーションのラストキーのフラグクリア
		m_LastKye = false;
	}
	CSceneMotionPartsX::SetMotion((int)m_Mode);
	CSceneMotionPartsX::Update();

	// モードの切り替え
	if (m_Mode == MODE_MOVE)
	{
		m_Mode = MODE_NORMAL;
	}


}

//=======================================================================================
//   描画処理
//=======================================================================================
void CPlayer::Draw()
{

	LPDIRECT3DDEVICE9 pDevice = NULL;                // エラーチェックのためNULLを入れる
	pDevice = CManager::GetRenderer()->GetDevice();                           // デバイスのポインタを取得

	if (pDevice == NULL)                            // エラーチェック
	{
		MessageBox(NULL, "InitのpDeveceのNULLチェックしてください！", "エラー", MB_OK | MB_ICONASTERISK);         // エラーメッセージ
		return;
	}
	CSceneMotionPartsX::Draw();
}

//=======================================================================================
//   キー判定処理
//=======================================================================================
void CPlayer::InputKey(void)
{
	// 攻撃状態時実行しない
	if (m_Mode == MODE_ATTACK) { return; }

	// カメラ情報取得
	CCamera* pCamera = CManager::GetCamera();
	if (pCamera == NULL) { return; }
	CCamera::CAMERA Camera = pCamera->GetCamera();

	// カメラとのベクトル
	D3DXVECTOR3 Vector;
	Vector.x = Camera.posEye.x - m_pos.x;
	Vector.z = Camera.posEye.z - m_pos.z;
	Vector.y = 0.0f;

	// カメラとの角度
	float Angle = -atan2(Vector.z, Vector.x) + D3DX_PI*0.5f;

	// 前進キー判定
	bool Input_MoveUP = false;

	// 前進
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_S))
	{
		m_Mode = MODE_MOVE;
		Angle += D3DX_PI;

		// 近いルードで回す
		if (m_Rot.y - Angle >  D3DX_PI) { m_Rot.y -= D3DX_PI * 2; }
		if (m_Rot.y - Angle < -D3DX_PI) { m_Rot.y += D3DX_PI * 2; }

		// 角度計算
		m_Rot.y += (Angle - m_Rot.y)*0.1f;

		// 前進キーフラグ
		Input_MoveUP = true;
	}
	// 後退
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_W))
	{
		m_Mode = MODE_MOVE;
		if (m_Rot.y - Angle >  D3DX_PI) { m_Rot.y -= D3DX_PI * 2; }
		if (m_Rot.y - Angle < -D3DX_PI) { m_Rot.y += D3DX_PI * 2; }
		m_Rot.y += (Angle - m_Rot.y)*0.1f;
	}
	// 左移動
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_D))
	{
		m_Mode = MODE_MOVE;

		// 前進キーを押しているとき
		if (Input_MoveUP)
		{
			Angle -= D3DX_PI*0.5f;
		}
		else
		{
			Angle += D3DX_PI*0.5f;
		}
		if (m_Rot.y - Angle >  D3DX_PI) { m_Rot.y -= D3DX_PI * 2; }
		if (m_Rot.y - Angle < -D3DX_PI) { m_Rot.y += D3DX_PI * 2; }
		m_Rot.y += (Angle - m_Rot.y)*0.1f;
	}
	// 右移動
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_A))
	{
		m_Mode = MODE_MOVE;
		if (Input_MoveUP)
		{
			Angle += D3DX_PI*0.5f;
		}
		else
		{
			Angle -= D3DX_PI*0.5f;
		}
		if (m_Rot.y - Angle >  D3DX_PI) { m_Rot.y -= D3DX_PI * 2; }
		if (m_Rot.y - Angle < -D3DX_PI) { m_Rot.y += D3DX_PI * 2; }
		m_Rot.y += (Angle - m_Rot.y)*0.1f;
	}

	if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_B))
	{
		m_Mode = MODE_ATTACK;
	}

	// 左移動
	if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_SPACE))
	{
		m_Mode = MODE_JUMP;
		m_Move.y = m_Jump;
	}
}

//=======================================================================================
//   ノーマルモード処理
//=======================================================================================
void CPlayer::ModeNormal()
{

}

//=======================================================================================
//   移動モード処理
//=======================================================================================
void CPlayer::ModeMove()
{
	m_Move.x = (float)cos(-m_Rot.y + D3DXToRadian(-90))*m_Speed;
	m_Move.z = (float)sin(-m_Rot.y + D3DXToRadian(-90))*m_Speed;
}

//=======================================================================================
//   攻撃モード処理
//=======================================================================================
void CPlayer::ModeAttack()
{

}

//=======================================================================================
//   ジャンプモード処理
//=======================================================================================
void CPlayer::ModeJump()
{

}

void CPlayer::ImGui()
{
	// セーブ処理
	if (ImGui::CollapsingHeader("Player Data"))
	{
		ImGui::Text("Mode %d", (int)m_Mode);
		ImGui::InputFloat("MoveSpeed", &m_Speed, 0.01f);
		ImGui::InputFloat("Jump", &m_Jump, 0.01f);
	}
}