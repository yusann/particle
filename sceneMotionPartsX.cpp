//*****************************************************************************
//   インクルードファイル
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "scene.h"
#include "sceneMotionPartsX.h"
#include "sceneMesh.h"
#include "meshSphere.h"
#include "sound.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
//=======================================================================================
//   コンストラクタ
//=======================================================================================
CSceneMotionPartsX::CSceneMotionPartsX(int nPriority) :CScene(nPriority)
{
	// メンバ変数の初期化
	m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_MotionID = 0;
	m_Frame = 0;
	m_Key = 0;

	motionMode = false;
	offset_window = false;
	model_window = false;
	all_motion_window = false;
	motion_window = false;
	save_data_window = false;
	collision_window = false;
	sound_window = false;
	keyUpdata = false;
	scl_all = 0.0f;
	value = 0.01f;
	parts_id = 0;
	model_id = 0;
	m_Blend = false;
	m_MotionFrame = 0;
	m_MotionFrameAll = 0;
}

//=======================================================================================
//   デストラクタ
//=======================================================================================
CSceneMotionPartsX::~CSceneMotionPartsX()
{
}

//=======================================================================================
//   初期化処理
//=======================================================================================
void CSceneMotionPartsX::Init()
{
	// ファイル読み込み
	LoadDataFile();
	LoadMotionFile((char*)imGui_parts_name.c_str());

	// データ代入
	for (int i = 0; i < m_Part.size(); i++)
	{
		m_SaveData.push_back(new VECTR3);
		m_BlendData.push_back(new VECTR3);
		// 階層決め
		if (i == 0)
		{
			m_Model[i]->pParent = NULL;
		}
		else
		{
			m_Model[i]->pParent = m_Model[m_Part[i]->Parent];
		}

		// データ代入
		m_Model[i]->Pos = m_Part[i]->OffSetPos + m_Motion[0]->KeyFrame[0]->Key[i]->Pos;
		m_Model[i]->Rot = m_Part[i]->OffSetRot + m_Motion[0]->KeyFrame[0]->Key[i]->Rot;
		m_Model[i]->Scl = m_Part[i]->OffSetScl + m_Motion[0]->KeyFrame[0]->Key[i]->Scl;
		m_SaveData[i]->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_SaveData[i]->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_SaveData[i]->Scl = D3DXVECTOR3(0.0f,0.0f,0.0f);

		m_BlendData[i]->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_BlendData[i]->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_BlendData[i]->Scl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	for (int i = 0; i < m_Motion.size(); i++)
	{
		for (int j = 0; j < m_Motion[i]->Collision.size(); j++)
		{
			m_Model[i]->Collision[j]->Pos = m_Motion[i]->Collision[j]->Pos;
			m_Model[i]->Collision[j]->Scl = m_Motion[i]->Collision[j]->Scl;
			m_Model[i]->Collision[j]->Use = false;
			m_Model[i]->Collision[j]->Sphere = CMeshSphere::Create(m_Model[i]->Collision[j]->Pos, m_Model[i]->Collision[j]->Scl, 10, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
		}
	}

	m_LastKye = false;
	m_MotionID = 0;
	m_Frame = 0;
	m_Key = 0;
}

//=======================================================================================
//   終了処理
//=======================================================================================
void CSceneMotionPartsX::Uninit()
{
	// モーション破棄
	for (auto CntMotion = m_Motion.begin(); CntMotion != m_Motion.end(); CntMotion++)
	{
		for (auto CntFrame = (*CntMotion)->KeyFrame.begin(); CntFrame != (*CntMotion)->KeyFrame.end(); CntFrame++)
		{
			(*CntFrame)->Key.clear();
		}
		(*CntMotion)->KeyFrame.clear();
	}
	m_Motion.clear();

	// パーツ破棄
	m_Part.clear();

	// モデル破棄
	m_Model.clear();

	m_SaveData.clear();
	m_BlendData.clear();
}

//=======================================================================================
//   更新処理
//=======================================================================================
void CSceneMotionPartsX::Update()
{
	ImGui();

	// 変換行列の宣言
	D3DXMATRIX mtxRot;             // ローカル回転
	D3DXMATRIX mtxScl;             // ローカルスケール
	D3DXMATRIX mtxPos;             // ローカル座標

	// ローカル回転の代入
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_Rot.y,
		m_Rot.x,
		m_Rot.z);

	// ローカルスケールの代入
	D3DXMatrixScaling(&mtxScl,
		m_scl.x,
		m_scl.y,
		m_scl.z);

	// ローカル座標の代入
	D3DXMatrixTranslation(&mtxPos,
		m_pos.x,
		m_pos.y,
		m_pos.z);

	// ワールド情報処理
	D3DXMatrixIdentity(&m_Matrix);                       // ワールドの中身を初期化
	D3DXMatrixMultiply(&m_Matrix, &m_Matrix, &mtxScl);   // パーツ情報をワールドスケールに代入してワールド情報を計算
	D3DXMatrixMultiply(&m_Matrix, &m_Matrix, &mtxRot);   // ワールド回転の代入
	D3DXMatrixMultiply(&m_Matrix, &m_Matrix, &mtxPos);   // ワールド座標の代入

	if (!offset_window)
	{
		// ループしないモーション処理
		if (m_Motion[m_MotionID]->KeyFrame[m_Key]->Frame <= 0)
		{
			m_LastKye = true;
			return;
		}

		// フレーム更新
		if (motionMode) { m_Frame++; }

		// マトリクスセット処理
		if (!m_Blend) { UpdateMotion(); }
		else { UpdateBlendMotion(); }
	}
	else
	{
		// 変換行列の宣言
		D3DXMATRIX mtxPartRot;             // ローカル回転
		D3DXMATRIX mtxPartScl;             // ローカルスケール
		D3DXMATRIX mtxPartPos;             // ローカル座標
		// データ代入
		for (int i = 0; i < m_Part.size(); i++)
		{
			// データ代入
			m_Model[i]->Pos = m_Part[i]->OffSetPos;
			m_Model[i]->Rot = m_Part[i]->OffSetRot;
			m_Model[i]->Scl = m_Part[i]->OffSetScl;

			// ローカル回転の代入
			D3DXMatrixRotationYawPitchRoll(&mtxPartRot,
				m_Model[i]->Rot.y,
				m_Model[i]->Rot.x,
				m_Model[i]->Rot.z);

			// ローカルスケールの代入
			D3DXMatrixScaling(&mtxPartScl,
				m_Model[i]->Scl.x,
				m_Model[i]->Scl.y,
				m_Model[i]->Scl.z);

			// ローカル座標の代入
			D3DXMatrixTranslation(&mtxPartPos,
				m_Model[i]->Pos.x,
				m_Model[i]->Pos.y,
				m_Model[i]->Pos.z);

			// ワールド情報処理
			D3DXMatrixIdentity(&m_Model[i]->Matrix);                       // ワールドの中身を初期化
			D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &mtxPartScl);   // ワールドスケールの代入
			D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &mtxPartRot);   // ワールド回転の代入
			D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &mtxPartPos);   // ワールド座標の代入

																						 // マトリクスの階層化処理
			if (m_Part[i]->Parent == -1) {
				D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &m_Matrix);   // ワールド座標の代入
			}
			else {
				D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &m_Model[i]->pParent->Matrix);   // ワールド座標の代入
			}
		}
	}

	// モーションフレーム計算
	int FrameOld = 0;
	for (int i = 0; i < m_Key; i++)
	{
		FrameOld += m_Motion[m_MotionID]->KeyFrame[i]->Frame;
	}
	m_MotionFrame = FrameOld + m_Frame;

	m_MotionFrameAll = 0;
	for (int i = 0; i < (int)m_Motion[m_MotionID]->KeyFrame.size(); i++)
	{
		m_MotionFrameAll += m_Motion[m_MotionID]->KeyFrame[i]->Frame;
	}

	// 当たり判定
	for (int i = 0; i < m_Motion[m_MotionID]->Collision.size(); i++)
	{
		if (m_Motion[m_MotionID]->Collision[i]->StartFrame <= m_MotionFrame &&m_Motion[m_MotionID]->Collision[i]->EndFrame >= m_MotionFrame)
		{
			D3DXVec3TransformCoord(&m_Model[m_MotionID]->Collision[i]->Pos,
				&m_Motion[m_MotionID]->Collision[i]->Pos,
				&m_Model[m_Motion[m_MotionID]->Collision[i]->PartID]->Matrix);
			m_Model[m_MotionID]->Collision[i]->Sphere->Update(m_Model[m_MotionID]->Collision[i]->Pos,
				m_Motion[m_MotionID]->Collision[i]->Scl);
			m_Model[m_MotionID]->Collision[i]->Use = true;
		}
		else
		{
			m_Model[m_MotionID]->Collision[i]->Use = false;
		}
	}

	// 効果音
	for (int i = 0; i < m_Motion[m_MotionID]->Sound.size(); i++)
	{
		if( m_MotionFrame == m_Motion[m_MotionID]->Sound[i]->Frame)
		CManager::GetSound()->Play((CSound::SOUND_LABEL)m_Motion[m_MotionID]->Sound[i]->ID);
	}
}
// マトリクスセット処理
void CSceneMotionPartsX::UpdateMotion()
{
	// 変換行列の宣言
	D3DXMATRIX mtxPartRot;             // ローカル回転
	D3DXMATRIX mtxPartScl;             // ローカルスケール
	D3DXMATRIX mtxPartPos;             // ローカル座標
	
	// 次のキーフレーム
	int NextKey = m_Key + 1;

	// 次のキーフレームのクリア
	if (NextKey >= m_Motion[m_MotionID]->KeyFrame.size())
		NextKey = 0;

	// パーツ数分ループして補間処理
	for (int i = 0; i < m_Part.size(); i++)
	{
		float Frame = (float)m_Frame / (float)m_Motion[m_MotionID]->KeyFrame[m_Key]->Frame;

		// 補間量で更新
		m_Model[i]->Pos = (m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos * (1.0f - Frame)) +
			(m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Pos * Frame) +
			m_Part[i]->OffSetPos;
		m_Model[i]->Rot = (m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot * (1.0f - Frame)) +
			(m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Rot * Frame) +
			m_Part[i]->OffSetRot;
		m_Model[i]->Scl = (m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl * (1.0f - Frame)) +
			(m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl * Frame) +
			m_Part[i]->OffSetScl;

		// ローカル回転の代入
		D3DXMatrixRotationYawPitchRoll(&mtxPartRot,
			m_Model[i]->Rot.y,
			m_Model[i]->Rot.x,
			m_Model[i]->Rot.z);

		// ローカルスケールの代入
		D3DXMatrixScaling(&mtxPartScl,
			m_Model[i]->Scl.x,
			m_Model[i]->Scl.y,
			m_Model[i]->Scl.z);

		// ローカル座標の代入
		D3DXMatrixTranslation(&mtxPartPos,
			m_Model[i]->Pos.x,
			m_Model[i]->Pos.y,
			m_Model[i]->Pos.z);

		// ワールド情報処理
		D3DXMatrixIdentity(&m_Model[i]->Matrix);                       // ワールドの中身を初期化
		D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &mtxPartScl);   // ワールドスケールの代入
		D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &mtxPartRot);   // ワールド回転の代入
		D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &mtxPartPos);   // ワールド座標の代入

		// マトリクスの階層化処理
		if (m_Part[i]->Parent == -1) {
			D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &m_Matrix);   // ワールド座標の代入
		}
		else {
			D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &m_Model[i]->pParent->Matrix);   // ワールド座標の代入
		}
	}
	// フレーム数チェック
	if (m_Frame > m_Motion[m_MotionID]->KeyFrame[m_Key]->Frame)
	{
		// フレームをクリア
		m_Frame = 0;

		// 次のキーフレームへ
		m_Key++;

		// ループモーション
		if (m_Key >= m_Motion[m_MotionID]->KeyFrame.size())
		{
			m_Key = 0;
		}
	}
}
// ブレンドモーションのマトリクスセット処理
void CSceneMotionPartsX::UpdateBlendMotion()
{
	// 変換行列の宣言
	D3DXMATRIX mtxPartRot;             // ローカル回転
	D3DXMATRIX mtxPartScl;             // ローカルスケール
	D3DXMATRIX mtxPartPos;             // ローカル座標

	// パーツ数分ループして補間処理
	for (int i = 0; i < m_Part.size(); i++)
	{
		float Frame = (float)m_Frame / (m_Motion[m_MotionID]->KeyFrame[m_Key]->Frame*0.5f);

		// 補間量で更新
		m_Model[i]->Pos = (m_BlendData[i]->Pos * (1.0f - Frame)) +
			(m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos * Frame) +
			m_Part[i]->OffSetPos;
		m_Model[i]->Rot = (m_BlendData[i]->Rot * (1.0f - Frame)) +
			(m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot * Frame) +
			m_Part[i]->OffSetRot;
		m_Model[i]->Scl = (m_BlendData[i]->Scl * (1.0f - Frame)) +
			(m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl * Frame) +
			m_Part[i]->OffSetScl;

		// ローカル回転の代入
		D3DXMatrixRotationYawPitchRoll(&mtxPartRot,
			m_Model[i]->Rot.y,
			m_Model[i]->Rot.x,
			m_Model[i]->Rot.z);

		// ローカルスケールの代入
		D3DXMatrixScaling(&mtxPartScl,
			m_Model[i]->Scl.x,
			m_Model[i]->Scl.y,
			m_Model[i]->Scl.z);

		// ローカル座標の代入
		D3DXMatrixTranslation(&mtxPartPos,
			m_Model[i]->Pos.x,
			m_Model[i]->Pos.y,
			m_Model[i]->Pos.z);

		// ワールド情報処理
		D3DXMatrixIdentity(&m_Model[i]->Matrix);                       // ワールドの中身を初期化
		D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &mtxPartScl);   // ワールドスケールの代入
		D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &mtxPartRot);   // ワールド回転の代入
		D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &mtxPartPos);   // ワールド座標の代入

																					 // マトリクスの階層化処理
		if (m_Part[i]->Parent == -1) {
			D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &m_Matrix);   // ワールド座標の代入
		}
		else {
			D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &m_Model[i]->pParent->Matrix);   // ワールド座標の代入
		}
	}
	// フレーム数チェック
	if (m_Frame > m_Motion[m_MotionID]->KeyFrame[m_Key]->Frame*0.5f)
	{
		// フレームをクリア
		m_Frame = 0;

		// 次のキーフレームへ
		m_Key = 0;

		m_Blend = false;
	}
}

//=======================================================================================
//   描画処理
//=======================================================================================
void CSceneMotionPartsX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = NULL;                // エラーチェックのためNULLを入れる
	pDevice = CManager::GetRenderer()->GetDevice();                           // デバイスのポインタを取得

	if (pDevice == NULL)                            // エラーチェック
	{
		MessageBox(NULL, "InitのpDeveceのNULLチェックしてください！", "エラー", MB_OK | MB_ICONASTERISK);         // エラーメッセージ
		return;
	}
	// 全パーツ分ループ
	for (int i = 0; i < m_Part.size(); i++) {

		// NULLチェック
		if (m_Part[i]->Model->pBuffMat == NULL) { return; }

		D3DXMATERIAL*	pMat = NULL;					//  マテリアル

		// ワールド情報セット
		pDevice->SetTransform(D3DTS_WORLD, &m_Model[i]->Matrix);

		D3DMATERIAL9 matDef;
		pDevice->GetMaterial(&matDef);                 // 現在デバイスに設定されてるアテリアル情報を取得

		pMat = (D3DXMATERIAL*)m_Part[i]->Model->pBuffMat->GetBufferPointer();

		for (int j = 0; j < (int)m_Part[i]->Model->NumMat; j++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[j].MatD3D);


			// テクスチャの描画
			pDevice->SetTexture(0, m_Part[i]->Model->pTexture[j]);

			// メッシュの描画
			m_Part[i]->Model->pMesh->DrawSubset(j);
			pDevice->SetMaterial(&matDef);
		}
	}
}

void CSceneMotionPartsX::SetMotion(int MotionID)
{
	if (!keyUpdata) return;
	if( m_MotionID == MotionID) return;

	for (int i = 0; i < m_Part.size(); i++) {
		m_BlendData[i]->Pos = m_Model[i]->Pos - m_Part[i]->OffSetPos;
		m_BlendData[i]->Rot = m_Model[i]->Rot - m_Part[i]->OffSetRot;
		m_BlendData[i]->Scl = m_Model[i]->Scl - m_Part[i]->OffSetScl;
	}
	m_Blend = true;

	m_MotionID = MotionID;
	m_Key = 0;
	m_Frame = 0;
	m_LastKye = false;
}

//=======================================================================================
//   データファイルの読み込み
//=======================================================================================
void CSceneMotionPartsX::LoadDataFile()
{
	// パーツファイル読み込み
	char aWork[64];
	FILE* pFile = NULL;

	//  ファイルの読み込み
	pFile = fopen("data/motion.txt", "r+");
	if (pFile == NULL)
	{
		//  エラーだった場合
		MessageBox(NULL, "ファイルが読み込めませんでした。", "エラーメッセージ", MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		while (1)
		{
			fscanf(pFile, "%s", &aWork[0]);

			// 終了
			if (strcmp(aWork, "END_LOAD") == 0)
			{
				imGui_parts_name += '\0';
				break;
			}
			// 動的確保
			parts_name.push_back(aWork);
			imGui_parts_name += aWork;
			imGui_parts_name += '\0';
		}
		//  ファイルを閉じる
		fclose(pFile);
	}

	//  ファイルの読み込み
	pFile = fopen("data/model.txt", "r+");
	if (pFile == NULL)
	{
		//  エラーだった場合
		MessageBox(NULL, "ファイルが読み込めませんでした。", "エラーメッセージ", MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		model_name.push_back("\0");
		imGui_model_name = '\0';
		while (1)
		{
			fscanf(pFile, "%s", &aWork[0]);
			if (strcmp(aWork, "SET_PART") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aWork[0]);
					if (strcmp(aWork, "END_PART") == 0) { break; }
					model_name.push_back(aWork);
					imGui_model_name += aWork;
					imGui_model_name += '\0';
				}
			}
			else if (strcmp(aWork, "END_LOAD") == 0)
			{
				imGui_model_name += '\0';
				break;
			}
		}
		//  ファイルを閉じる
		fclose(pFile);
	}
}

//=======================================================================================
//   ファイル読み込み処理
//=======================================================================================
bool CSceneMotionPartsX::LoadMotionFile(char *FileName)
{
	char aWork[128];
	int CntParts = -1;
	int CntMotion = -1;
	int CntKey = -1;
	int CntKeyFrame = -1;
	int CntCollision = -1;
	int CntSound = -1;
	FILE* pFile = NULL;

	//  ファイルの読み込み
	pFile = fopen(FileName, "r+");
	if (pFile == NULL)
	{
		//  エラーだった場合
		MessageBox(NULL, "ファイルが読み込めませんでした。", "エラーメッセージ", MB_OK | MB_ICONWARNING);
		return false;
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

			// オフセット情報
			else if (strcmp(aWork, "SET_OFFSET") == 0) {
				while (1)
				{
					fscanf(pFile, "%s", &aWork);
					// オフセット終了
					if (strcmp(aWork, "END_OFFSET") == 0) {
						break;
					}
					else if (strcmp(aWork, "FILENAME") == 0) {
						// 動的確保
						m_Part.push_back(new PART);
						m_Model.push_back(new MODEL);
						CntParts++;

						char Path[64];
						fscanf(pFile, "%s", &Path[0]);
						m_Part[CntParts]->Model = CModelX::GetModelX(Path);
						int jiji = 3456;
					}
					else if (strcmp(aWork, "PARENT") == 0) {
						fscanf(pFile, "%d", &m_Part[CntParts]->Parent);
					}
					// 座標
					else if (strcmp(aWork, "POS") == 0) {
							fscanf(pFile, "%f %f %f",
								&m_Part[CntParts]->OffSetPos.x,
								&m_Part[CntParts]->OffSetPos.y,
								&m_Part[CntParts]->OffSetPos.z);
					}
					// 回転
					else if (strcmp(aWork, "ROT") == 0) {
						fscanf(pFile, "%f %f %f",
							&m_Part[CntParts]->OffSetRot.x,
							&m_Part[CntParts]->OffSetRot.y,
							&m_Part[CntParts]->OffSetRot.z);
					}
					// スケール
					else if (strcmp(aWork, "SCL") == 0) {
						fscanf(pFile, "%f %f %f",
							&m_Part[CntParts]->OffSetScl.x,
							&m_Part[CntParts]->OffSetScl.y,
							&m_Part[CntParts]->OffSetScl.z);

					}
				}
			}
			// モーション情報
			else if (strcmp(aWork, "SET_MOTION") == 0) {

				while (1)
				{
					fscanf(pFile, "%s", &aWork);
					// ループフラグ
					if (strcmp(aWork, "SET_KEY_FRAME") == 0) {

						// 確保
						m_Motion.push_back(new MOTION);
						CntMotion++;
						CntKeyFrame = -1;
					}
					// サウンド
					else if (strcmp(aWork, "SET_SOUND") == 0)
					{
						while (1)
						{
							fscanf(pFile, "%s", &aWork);
							if (strcmp(aWork, "ID") == 0) {
								m_Motion[CntMotion]->Sound.push_back(new SOUND);
								CntSound++;
								fscanf(pFile, "%d", &m_Motion[CntMotion]->Sound[CntSound]->ID);
							}
							else if (strcmp(aWork, "FRAME") == 0) {
								fscanf(pFile, "%d", &m_Motion[CntMotion]->Sound[CntSound]->Frame);
							}
							else if (strcmp(aWork, "END_SOUND") == 0) {
								CntSound = -1;
								break;
							}
						}
					}
					// 当たり判定
					else if (strcmp(aWork, "SET_COLLISION") == 0)
					{
						while (1)
						{
							fscanf(pFile, "%s", &aWork);
							// 1つのモーション読み込み終了
							if (strcmp(aWork, "PARTS") == 0) {
								m_Motion[CntMotion]->Collision.push_back(new COLLISION_DATA);
								m_Model[CntMotion]->Collision.push_back(new COLLISION_SPHERE);
								CntCollision++;
								fscanf(pFile, "%d", &m_Motion[CntMotion]->Collision[CntCollision]->PartID);
							}
							else if (strcmp(aWork, "POS") == 0) {
								fscanf(pFile, "%f %f %f\n", &m_Motion[CntMotion]->Collision[CntCollision]->Pos.x,
									&m_Motion[CntMotion]->Collision[CntCollision]->Pos.y,
									&m_Motion[CntMotion]->Collision[CntCollision]->Pos.z);
							}
							else if (strcmp(aWork, "SCL") == 0) {
								fscanf(pFile, "%f", &m_Motion[CntMotion]->Collision[CntCollision]->Scl);
							}
							else if (strcmp(aWork, "START") == 0) {
								fscanf(pFile, "%d", &m_Motion[CntMotion]->Collision[CntCollision]->StartFrame);
							}
							else if (strcmp(aWork, "END") == 0) {
								fscanf(pFile, "%d", &m_Motion[CntMotion]->Collision[CntCollision]->EndFrame);
							}
							else if (strcmp(aWork, "END_COLLISION") == 0) {
								CntCollision = -1;
								break;
							}
						}
					}
					// フレーム数
					else if (strcmp(aWork, "FRAME") == 0)
					{
						// キーフレームカウント
						CntKeyFrame++;
						// キーカウンタの初期化
						CntKey = -1;

						// 確保
						m_Motion[CntMotion]->KeyFrame.push_back(new KEY_FRAME);
						fscanf(pFile, "%d", &m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Frame);
					}
					// 座標
					else if (strcmp(aWork, "POS") == 0)
					{
						// 確保
						m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key.push_back(new KEY);
						// キーカウント
						CntKey++;

						fscanf(pFile, "%f %f %f\n", &m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key[CntKey]->Pos.x,
							&m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key[CntKey]->Pos.y,
							&m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key[CntKey]->Pos.z);
					}
					// 回転
					else if (strcmp(aWork, "ROT") == 0)
					{
						fscanf(pFile, "%f %f %f\n", &m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key[CntKey]->Rot.x,
							&m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key[CntKey]->Rot.y,
							&m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key[CntKey]->Rot.z);
					}
					// スケール
					else if (strcmp(aWork, "SCL") == 0)
					{
						fscanf(pFile, "%f %f %f\n", &m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key[CntKey]->Scl.x,
							&m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key[CntKey]->Scl.y,
							&m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key[CntKey]->Scl.z);
					}
					// 1つのモーション読み込み終了
					else if (strcmp(aWork, "END_MOTION") == 0) {
						break;
					}
				}
			}
			// 終了
			else if (strcmp(aWork, "END_LOAD") == 0) {
				break;
			}
		}
		//  ファイルを閉じる
		fclose(pFile);
	}
	return true;
}

void CSceneMotionPartsX::ChangeFile(char *FileName)
{
	FILE* pFile = NULL;

	//  ファイルの読み込み
	pFile = fopen(FileName, "r+");
	if (pFile == NULL)
	{
		//  エラーだった場合
		MessageBox(NULL, "ファイルがない！（ChangeFile）", "エラーメッセージ", MB_OK | MB_ICONWARNING);
		return;
	}
	//  ファイルを閉じる
	fclose(pFile);


	Uninit();

	// ファイル読み込み
	LoadMotionFile(FileName);

	// データ代入
	for (int i = 0; i < m_Part.size(); i++)
	{
		m_SaveData.push_back(new VECTR3);
		m_BlendData.push_back(new VECTR3);
		// 階層決め
		if (i == 0)
		{
			m_Model[i]->pParent = NULL;
		}
		else
		{
			m_Model[i]->pParent = m_Model[m_Part[i]->Parent];
		}

		// データ代入
		m_Model[i]->Pos = m_Part[i]->OffSetPos + m_Motion[0]->KeyFrame[0]->Key[i]->Pos;
		m_Model[i]->Rot = m_Part[i]->OffSetRot + m_Motion[0]->KeyFrame[0]->Key[i]->Rot;
		m_Model[i]->Scl = m_Part[i]->OffSetScl + m_Motion[0]->KeyFrame[0]->Key[i]->Scl;

		m_SaveData[i]->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_SaveData[i]->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_SaveData[i]->Scl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		m_BlendData[i]->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_BlendData[i]->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_BlendData[i]->Scl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_LastKye = false;
	m_MotionID = 0;
	m_Frame = 0;
	m_Key = 0;

	motionMode = false;
	offset_window = false;
	model_window = false;
	all_motion_window = false;
	motion_window = false;
}

//=======================================================================================
//   モデル追加処理
//=======================================================================================
void CSceneMotionPartsX::CreateModel(void)
{
	m_Part.push_back(new PART);
	m_Model.push_back(new MODEL);
	m_Part.back()->Model = CModelX::GetModelX( "data/MODEL/block00.x");
	m_Part.back()->OffSetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Part.back()->OffSetRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Part.back()->OffSetScl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Part.back()->Parent = 0;
	m_Model.back()->pParent = m_Model[m_Part.back()->Parent];
	m_Model.back()->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Model.back()->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Model.back()->Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	KeyAdd();
}

//=======================================================================================
//   モデル破棄処理
//=======================================================================================
void CSceneMotionPartsX::DelModel()
{
	m_Part.pop_back();

	m_Model.pop_back();

	KeySub();
}

//=======================================================================================
//   モデル破棄処理
//=======================================================================================
void CSceneMotionPartsX::ChangeModel(PART *pPart, char *FileName)
{
	pPart->Model = CModelX::GetModelX(FileName);
}

//=======================================================================================
//   書き出し処理
//=======================================================================================
void CSceneMotionPartsX::SaveMotion(char *FileName)
{
	// 書き出し処理
	FILE *pFile;

	//  ファイルの読み込み
	pFile = fopen(FileName, "r+");
	if (pFile != NULL)
	{
		UINT nID = MessageBox(NULL, "同じファイルが既に存在します。\n上書きしますか？", "上書き", MB_YESNO | MB_DEFBUTTON2);
		//  ファイルを閉じる
		fclose(pFile);
		if (nID == IDNO) return;
	}

	fopen_s(&pFile, FileName, "w");

	// NULLチェック
	if (pFile != NULL)
	{
		// セーブ開始
		fprintf(pFile, "LOAD_START\n\n");

		// モデルのオフセット情報
		fprintf(pFile, "オフセット情報\n");
		fprintf(pFile, "SET_OFFSET\n");
		for (int i = 0; i < m_Part.size(); ++i)
		{
			fprintf(pFile, "	FILENAME %s\n", m_Part[i]->Model->Name);
			fprintf(pFile, "	PARENT %d\n", m_Part[i]->Parent);
			fprintf(pFile, "	POS %.2f %.2f %.2f\n"  , m_Part[i]->OffSetPos.x, m_Part[i]->OffSetPos.y, m_Part[i]->OffSetPos.z);
			fprintf(pFile, "	ROT %.2f %.2f %.2f\n"  , m_Part[i]->OffSetRot.x, m_Part[i]->OffSetRot.y, m_Part[i]->OffSetRot.z);
			fprintf(pFile, "	SCL %.2f %.2f %.2f\n", m_Part[i]->OffSetScl.x, m_Part[i]->OffSetScl.y, m_Part[i]->OffSetScl.z);
			if (i < m_Part.size()-1) { fprintf(pFile, "\n"); }
		}
		fprintf(pFile, "END_OFFSET\n\n");

		// モーション情報
		fprintf(pFile, "モーション情報\n");
		fprintf(pFile, "SET_MOTION\n");
		for (int i = 0; i < m_Motion.size(); ++i)
		{
			fprintf(pFile, "	SET_KEY_FRAME\n");
			if (!m_Motion[i]->Sound.empty())
			{
				fprintf(pFile, "		SET_SOUND\n");
				for (int j = 0; j < m_Motion[i]->Sound.size(); j++)
				{
					fprintf(pFile, "			ID    %d\n", m_Motion[i]->Sound[j]->ID);
					fprintf(pFile, "			FRAME %d\n", m_Motion[i]->Sound[j]->Frame);
					if (j < m_Motion[i]->Sound.size() - 1) { fprintf(pFile, "\n"); }
				}
				fprintf(pFile, "		END_SOUND\n\n");
			}

			if (!m_Motion[i]->Collision.empty())
			{
				fprintf(pFile, "		SET_COLLISION\n");
				for (int j = 0; j < m_Motion[i]->Collision.size(); j++)
				{
					fprintf(pFile, "			PARTS %d\n", m_Motion[i]->Collision[j]->PartID);
					fprintf(pFile, "			POS %.2f %.2f %.2f\n", m_Motion[i]->Collision[j]->Pos.x, m_Motion[i]->Collision[j]->Pos.y, m_Motion[i]->Collision[j]->Pos.z);
					fprintf(pFile, "			SCL %.2f\n", m_Motion[i]->Collision[j]->Scl);
					fprintf(pFile, "			START %d\n", m_Motion[i]->Collision[j]->StartFrame);
					fprintf(pFile, "			END %d\n", m_Motion[i]->Collision[j]->EndFrame);
					if (j < m_Motion[i]->Collision.size() - 1) { fprintf(pFile, "\n"); }
				}
				fprintf(pFile, "		END_COLLISION\n\n");
			}
			for (int j = 0; j < m_Motion[i]->KeyFrame.size(); ++j)
			{
				fprintf(pFile, "		FRAME %d\n", m_Motion[i]->KeyFrame[j]->Frame);

				for (int k = 0; k < m_Motion[i]->KeyFrame[j]->Key.size(); ++k)
				{
					fprintf(pFile, "			POS %.2f %.2f %.2f\n", m_Motion[i]->KeyFrame[j]->Key[k]->Pos.x, m_Motion[i]->KeyFrame[j]->Key[k]->Pos.y, m_Motion[i]->KeyFrame[j]->Key[k]->Pos.z);
					fprintf(pFile, "			ROT %.2f %.2f %.2f\n", m_Motion[i]->KeyFrame[j]->Key[k]->Rot.x, m_Motion[i]->KeyFrame[j]->Key[k]->Rot.y, m_Motion[i]->KeyFrame[j]->Key[k]->Rot.z);
					fprintf(pFile, "			SCL %.2f %.2f %.2f\n", m_Motion[i]->KeyFrame[j]->Key[k]->Scl.x, m_Motion[i]->KeyFrame[j]->Key[k]->Scl.y, m_Motion[i]->KeyFrame[j]->Key[k]->Scl.z);
					if (k < m_Motion[i]->KeyFrame[j]->Key.size() - 1) { fprintf(pFile, "\n"); }
				}
			}
		}
		fprintf(pFile, "END_MOTION\n");

		// セーブ終了
		fprintf(pFile, "END_LOAD");
		fclose(pFile);				// ファイル閉じ
		MessageBox(NULL, "保存しました。", "終了", MB_DEFBUTTON2);
	}
	else
	{
		assert(!"書き出しエラー！");          // エラーメッセージ
	}
}

//=======================================================================================
//   モーションを増減処理
//=======================================================================================
void CSceneMotionPartsX::MotionAdd()
{
	m_Motion.push_back(new MOTION);
	for (int i = 0; i < m_Motion[0]->KeyFrame.size(); i++)
	{
		m_Motion.back()->KeyFrame.push_back(new KEY_FRAME);
		m_Motion.back()->KeyFrame[i]->Frame = 30;
		for (int j = 0; j < m_Part.size(); j++)
		{
			m_Motion.back()->KeyFrame[i]->Key.push_back(new KEY);
			m_Motion.back()->KeyFrame[i]->Key[j]->Pos.x = 0.0f;
			m_Motion.back()->KeyFrame[i]->Key[j]->Pos.y = 0.0f;
			m_Motion.back()->KeyFrame[i]->Key[j]->Pos.z = 0.0f;

			m_Motion.back()->KeyFrame[i]->Key[j]->Rot.x = 0.0f;
			m_Motion.back()->KeyFrame[i]->Key[j]->Rot.y = 0.0f;
			m_Motion.back()->KeyFrame[i]->Key[j]->Rot.z = 0.0f;

			m_Motion.back()->KeyFrame[i]->Key[j]->Scl.x = 0.0f;
			m_Motion.back()->KeyFrame[i]->Key[j]->Scl.y = 0.0f;
			m_Motion.back()->KeyFrame[i]->Key[j]->Scl.z = 0.0f;
		}
	}
}
void CSceneMotionPartsX::MotionSub()
{
	if (m_Motion.size() <= 1)
		return;

	m_Motion.pop_back();

	if (m_MotionID >= m_Motion.size())
		m_MotionID--;
}

//=======================================================================================
//   キーフレームを増減処理
//=======================================================================================
void CSceneMotionPartsX::KeyFrameAdd()
{
	m_Motion[m_MotionID]->KeyFrame.push_back(new KEY_FRAME);
	m_Motion[m_MotionID]->KeyFrame.back()->Frame = 30;
	for (int i = 0; i < m_Part.size(); i++)
	{
		m_Motion[m_MotionID]->KeyFrame.back()->Key.push_back(new KEY);
		m_Motion[m_MotionID]->KeyFrame.back()->Key[i]->Pos.x = 0.0f;
		m_Motion[m_MotionID]->KeyFrame.back()->Key[i]->Pos.y = 0.0f;
		m_Motion[m_MotionID]->KeyFrame.back()->Key[i]->Pos.z = 0.0f;

		m_Motion[m_MotionID]->KeyFrame.back()->Key[i]->Rot.x = 0.0f;
		m_Motion[m_MotionID]->KeyFrame.back()->Key[i]->Rot.y = 0.0f;
		m_Motion[m_MotionID]->KeyFrame.back()->Key[i]->Rot.z = 0.0f;

		m_Motion[m_MotionID]->KeyFrame.back()->Key[i]->Scl.x = 0.0f;
		m_Motion[m_MotionID]->KeyFrame.back()->Key[i]->Scl.y = 0.0f;
		m_Motion[m_MotionID]->KeyFrame.back()->Key[i]->Scl.z = 0.0f;
	}
}
void CSceneMotionPartsX::KeyFrameSub()
{
	if (m_Motion[m_MotionID]->KeyFrame.size() <= 1)
		return;

	m_Motion[m_MotionID]->KeyFrame.pop_back();

	if (m_Key >= m_Motion[m_MotionID]->KeyFrame.size())
		m_Key--;
}

//=======================================================================================
//   キーを増減処理
//=======================================================================================
void CSceneMotionPartsX::KeyAdd()
{
	for (int i = 0; i < m_Motion.size(); i++)
	{
		for (int j = 0; j < m_Motion[i]->KeyFrame.size(); j++)
		{
			m_Motion[i]->KeyFrame[j]->Key.push_back(new KEY);
			m_Motion[i]->KeyFrame[j]->Key.back()->Pos.x = 0.0f;
			m_Motion[i]->KeyFrame[j]->Key.back()->Pos.y = 0.0f;
			m_Motion[i]->KeyFrame[j]->Key.back()->Pos.z = 0.0f;

			m_Motion[i]->KeyFrame[j]->Key.back()->Rot.x = 0.0f;
			m_Motion[i]->KeyFrame[j]->Key.back()->Rot.y = 0.0f;
			m_Motion[i]->KeyFrame[j]->Key.back()->Rot.z = 0.0f;

			m_Motion[i]->KeyFrame[j]->Key.back()->Scl.x = 0.0f;
			m_Motion[i]->KeyFrame[j]->Key.back()->Scl.y = 0.0f;
			m_Motion[i]->KeyFrame[j]->Key.back()->Scl.z = 0.0f;
		}
	}
	m_SaveData.push_back(new VECTR3);
	m_SaveData.back()->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_SaveData.back()->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_SaveData.back()->Scl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_BlendData.push_back(new VECTR3);
	m_BlendData.back()->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_BlendData.back()->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_BlendData.back()->Scl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
void CSceneMotionPartsX::KeySub()
{
	for (int i = 0; i < m_Motion.size(); i++)
	{
		for (int j = 0; j < m_Motion[i]->KeyFrame.size(); j++)
		{
			m_Motion[i]->KeyFrame[j]->Key.pop_back();
		}
	}
	m_SaveData.pop_back();
	m_BlendData.pop_back();
}

//=======================================================================================
//   当たり判定を増減処理
//=======================================================================================
void CSceneMotionPartsX::CollisionAdd(int MotionID)
{
	m_Motion[MotionID]->Collision.push_back(new COLLISION_DATA);
	m_Motion[MotionID]->Collision.back()->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Motion[MotionID]->Collision.back()->Scl = 1.0f;
	m_Motion[MotionID]->Collision.back()->PartID = 0;
	m_Motion[MotionID]->Collision.back()->StartFrame = 0;
	m_Motion[MotionID]->Collision.back()->EndFrame = 60;

	m_Model[MotionID]->Collision.push_back(new COLLISION_SPHERE);
	m_Model[MotionID]->Collision.back()->Pos = m_Motion[MotionID]->Collision.back()->Pos;
	m_Model[MotionID]->Collision.back()->Scl = m_Motion[MotionID]->Collision.back()->Scl;
	m_Model[MotionID]->Collision.back()->Use = false;
	m_Model[MotionID]->Collision.back()->Sphere = CMeshSphere::Create(m_Model[MotionID]->Collision.back()->Pos, m_Model[MotionID]->Collision.back()->Scl, 10, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
}
void CSceneMotionPartsX::CollisionSub(int MotionID)
{
	if (!m_Motion[MotionID]->Collision.empty())
	{
		m_Motion[MotionID]->Collision.pop_back();
		m_Model[MotionID]->Collision.pop_back();
	}
}

//=======================================================================================
//   効果音を増減処理
//=======================================================================================
void CSceneMotionPartsX::SoundAdd()
{
	m_Motion[m_MotionID]->Sound.push_back(new SOUND);
	m_Motion[m_MotionID]->Sound.back()->Frame = -1;
	m_Motion[m_MotionID]->Sound.back()->ID = CSound::SE_SELECT000;
}
void CSceneMotionPartsX::SoundSub()
{
	if (!m_Motion[m_MotionID]->Sound.empty())
	{
		CManager::GetSound()->Stop((CSound::SOUND_LABEL)m_Motion[m_MotionID]->Sound.back()->ID);
		m_Motion[m_MotionID]->Sound.pop_back();
	}
}

//=======================================================================================
//   デバッグ表示（imgui）
//=======================================================================================
void CSceneMotionPartsX::ImGui(void)
{
	static char FileName_Save[128] = "data/text.txt";
	int MotionIDOld = m_MotionID;

	// メインメニュー
	ImGui::Checkbox("Updata Key", &keyUpdata);
	ImGui::Checkbox("Updata Motion", &motionMode);
	ImGui::Checkbox("    Model  Window", &model_window);
	ImGui::Checkbox("    OffSet Window", &offset_window);
	ImGui::Checkbox("All Motion Window", &all_motion_window);
	ImGui::Checkbox(" Save Data Window", &save_data_window);

	if (offset_window)
	{
		motionMode = false;
		model_window = false;
		all_motion_window = false;
		motion_window = false;
	}
	// モーション設定
	if (ImGui::CollapsingHeader("Motion Data"))
	{
		ImGui::Checkbox("    Motion Window", &motion_window);
		ImGui::Checkbox(" Collision Window", &collision_window);
		bool check = sound_window;
		ImGui::Checkbox("     Sound Window", &sound_window);
		if (check != sound_window) { CManager::GetSound()->Stop(); }
		ImGui::SliderInt("MotionID", &m_MotionID, 0, m_Motion.size() - 1);
		if (ImGui::Button("Motion+"))  MotionAdd();
		if (ImGui::Button("Motion-"))  MotionSub();
		if (m_MotionID != MotionIDOld)
		{
			m_Key = 0;
			m_Frame = 0;
		}
		ImGui::SliderInt("KeyID", &m_Key, 0, m_Motion[m_MotionID]->KeyFrame.size()-1);
		if (ImGui::Button("Key+"))  KeyFrameAdd();
		if (ImGui::Button("Key-"))  KeyFrameSub();
		ImGui::SliderInt("Frame", &m_Frame, 0, m_Motion[m_MotionID]->KeyFrame[m_Key]->Frame);
		ImGui::Text("MotionFrame %d / %d", m_MotionFrame, m_MotionFrameAll);
	}

	// セーブ処理
	if (ImGui::CollapsingHeader("Save"))
	{
		ImGui::InputText("FilePath", FileName_Save, sizeof(FileName_Save));
		if (ImGui::Button("SaveFile")) SaveMotion(FileName_Save);
	}

	// ファイル変更
	if (ImGui::CollapsingHeader("Change"))
	{
		ImGui::Combo("FilePath", &parts_id, imGui_parts_name.c_str(), IM_ARRAYSIZE(imGui_parts_name.c_str()));   // Combo using proper array. You can also pass a callback to retrieve array value, no need to create/copy an array just for that.
		if (ImGui::Button("ChangeFile")) ChangeFile((char*)parts_name[parts_id].c_str());
	}

	// モデルパーツウィンドウ
	if (model_window) imGuiModel();

	// オフセット設定
	if (offset_window) imGuiOffset();

	// モーションウィンドウ
	if (all_motion_window) imGuiAllMotion();

	// 当たり判定ウィンドウ
	if (collision_window) imGuiCollision();

	// サウンドウィンドウ
	if (sound_window) imGuiSound();

	// 現在のモーションウィンドウ
	if (motion_window) imGuiMotion();

	// 一時保存データウィンドウ
	if (save_data_window)
	{
		ImGui::SetNextWindowPos(ImVec2(350, 20), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Save Data Window", &model_window);

		// バーツの情報
		for (int i = 0; i < m_Part.size(); i++) {

				ImGui::Text("pos%d( %.3f, %.3f, %.3f)",i, m_SaveData[i]->Pos.x, m_SaveData[i]->Pos.y, m_SaveData[i]->Pos.z);
				ImGui::Text("rot%d( %.3f, %.3f, %.3f)",i, m_SaveData[i]->Rot.x, m_SaveData[i]->Rot.y, m_SaveData[i]->Rot.z);
				ImGui::Text("scl%d( %.3f, %.3f, %.3f)",i, m_SaveData[i]->Scl.x, m_SaveData[i]->Scl.y, m_SaveData[i]->Scl.z);
				ImGui::Text(" ");
		}
		ImGui::End();
	}
}

void CSceneMotionPartsX::imGuiModel()
{
	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Model Window", &model_window);
	char BoxName[16];
	// オフセットキー情報保存
	if (ImGui::Button("SaveKey"))
	{
		for (int i = 0; i < m_Part.size(); i++) {
			m_SaveData[i]->Pos = m_Part[i]->OffSetPos;
			m_SaveData[i]->Rot = m_Part[i]->OffSetRot;
			m_SaveData[i]->Scl = m_Part[i]->OffSetScl;
		}
	}

	// バーツの情報
	ImGui::Text("Parts Num %d", m_Part.size());
	for (int i = 0; i < m_Part.size(); i++) {
		ImGui::Text("Index  %d(%s)", i, m_Part[i]->Model->Name);
		ImGui::Text("Parent %d", m_Part[i]->Parent);
		sprintf(BoxName, "pos %d", i);
		ImGui::DragFloat3(BoxName, &m_Model[i]->Pos[0], 0.01f);
		sprintf(BoxName, "rot %d", i);
		ImGui::DragFloat3(BoxName, &m_Model[i]->Rot[0], 0.01f);
		sprintf(BoxName, "scl %d", i);
		ImGui::DragFloat3(BoxName, &m_Model[i]->Scl[0], 0.01f);
	}
	ImGui::End();
}
void CSceneMotionPartsX::imGuiOffset()
{
	char BoxName[16];
	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Model Window", &offset_window);
	ImGui::InputFloat("Value", &value, 0.001f);

	// オフセットキー情報保存
	if (ImGui::Button("SaveKey"))
	{
		for (int i = 0; i < m_Part.size(); i++) {
			m_SaveData[i]->Pos = m_Part[i]->OffSetPos;
			m_SaveData[i]->Rot = m_Part[i]->OffSetRot;
			m_SaveData[i]->Scl = m_Part[i]->OffSetScl;
		}
	}
	// オフセットキー情報保存
	if (ImGui::Button("LoadKey"))
	{
		for (int i = 0; i < m_Part.size(); i++) {
			m_Part[i]->OffSetPos = m_SaveData[i]->Pos;
			m_Part[i]->OffSetRot = m_SaveData[i]->Rot;
			m_Part[i]->OffSetScl = m_SaveData[i]->Scl;
		}
	}

	if (ImGui::Button("Add Model"))
	{
		CreateModel();
	}
	if (ImGui::Button("Delete Model"))
	{
		if (m_Part.size() > 1)
		{
			DelModel();
		}
	}

	// バーツの情報
	ImGui::Text("Parts Num %d", m_Part.size());
	for (int i = 0; i < m_Part.size(); i++) {

		// インデックス
		sprintf(BoxName, "PartsID %d", i);
		if (ImGui::TreeNode(BoxName))
		{
			// パーツモデルの変更
			sprintf(BoxName, "File %d", i);
			std::string imGuiModelName = m_Part[i]->Model->Name + imGui_model_name;
			ImGui::Combo(BoxName, &model_id, imGuiModelName.c_str(), 10);
			if (model_id != 0)
			{
				char* name = new char[model_name[model_id].size() + 1]; // メモリ確保
				std::strcpy(name, model_name[model_id].c_str());        // コピー
				ChangeModel(m_Part[i], name);
				model_id = 0;
				delete[] name; // 解放
			}

			// 親情報変更
			if (m_Part[i]->Parent < 0)
			{
				ImGui::Text("Parent %d", m_Part[i]->Parent);
			}
			else
			{
				sprintf(BoxName, "Parent %d", i);
				ImGui::InputInt(BoxName, &m_Part[i]->Parent);
				if (m_Part[i]->Parent < 0)
				{
					m_Part[i]->Parent = 0;
				}
				if (m_Part[i]->Parent > m_Part.size()-1)
				{
					m_Part[i]->Parent = m_Part.size() - 1;
				}
				m_Model[i]->pParent = m_Model[m_Part[i]->Parent];
			}

			sprintf(BoxName, "POS %d", i);
			if (ImGui::TreeNode(BoxName))
			{
				sprintf(BoxName, "posX  %d", i);
				ImGui::InputFloat(BoxName, &m_Part[i]->OffSetPos.x, value);
				sprintf(BoxName, " posX %d", i);
				ImGui::DragFloat(BoxName, &m_Part[i]->OffSetPos.x, value);

				sprintf(BoxName, "posY  %d", i);
				ImGui::InputFloat(BoxName, &m_Part[i]->OffSetPos.y, value);
				sprintf(BoxName, " posY %d", i);
				ImGui::DragFloat(BoxName, &m_Part[i]->OffSetPos.y, value);

				sprintf(BoxName, "posZ  %d", i);
				ImGui::InputFloat(BoxName, &m_Part[i]->OffSetPos.z, value);
				sprintf(BoxName, " posZ %d", i);
				ImGui::DragFloat(BoxName, &m_Part[i]->OffSetPos.z, value);
				ImGui::TreePop();
			}

			sprintf(BoxName, "ROT %d", i);
			if (ImGui::TreeNode(BoxName))
			{
				sprintf(BoxName, "rotX  %d", i);
				ImGui::InputFloat(BoxName, &m_Part[i]->OffSetRot.x, value);
				sprintf(BoxName, " rotX %d", i);
				ImGui::DragFloat(BoxName, &m_Part[i]->OffSetRot.x, value);

				sprintf(BoxName, "rotY  %d", i);
				ImGui::InputFloat(BoxName, &m_Part[i]->OffSetRot.y, value);
				sprintf(BoxName, " rotY %d", i);
				ImGui::DragFloat(BoxName, &m_Part[i]->OffSetRot.y, value);

				sprintf(BoxName, "rotZ  %d", i);
				ImGui::InputFloat(BoxName, &m_Part[i]->OffSetRot.z, value);
				sprintf(BoxName, " rotZ %d", i);
				ImGui::DragFloat(BoxName, &m_Part[i]->OffSetRot.y, value);
				ImGui::TreePop();
			}

			sprintf(BoxName, "SCL %d", i);
			if (ImGui::TreeNode(BoxName))
			{
				ImGui::InputFloat("Scl  ALL", &scl_all, value);
				ImGui::DragFloat(" Scl ALL", &scl_all, value);
				m_Part[i]->OffSetScl.x += scl_all;
				m_Part[i]->OffSetScl.y += scl_all;
				m_Part[i]->OffSetScl.z += scl_all;
				scl_all = 0.0f;
				sprintf(BoxName, "sclX  %d", i);
				ImGui::InputFloat(BoxName, &m_Part[i]->OffSetScl.x, value);
				sprintf(BoxName, " sclX %d", i);
				ImGui::DragFloat(BoxName, &m_Part[i]->OffSetScl.x, value);

				sprintf(BoxName, "sclY  %d", i);
				ImGui::InputFloat(BoxName, &m_Part[i]->OffSetScl.y, value);
				sprintf(BoxName, " sclY %d", i);
				ImGui::DragFloat(BoxName, &m_Part[i]->OffSetScl.y, value);

				sprintf(BoxName, "sclZ  %d", i);
				ImGui::InputFloat(BoxName, &m_Part[i]->OffSetScl.z, value);
				sprintf(BoxName, " sclZ %d", i);
				ImGui::DragFloat(BoxName, &m_Part[i]->OffSetScl.z, value);

				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
}
void CSceneMotionPartsX::imGuiAllMotion()
{
	char BoxName[16];
	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Motion Window", &model_window);
	ImGui::InputFloat("Value", &value, 0.001f);

	// パーツ数表示
	ImGui::Text("Parts Num %d", m_Part.size());

	// キーフレーム情報保存
	if (ImGui::Button("SaveKey"))
	{
		for (int i = 0; i < m_Part.size(); i++) {
			m_SaveData[i]->Pos = m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos;
			m_SaveData[i]->Rot = m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot;
			m_SaveData[i]->Scl = m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl;
		}
	}
	// キーフレーム情報読込
	if (ImGui::Button("LoadKey"))
	{
		for (int i = 0; i < m_Part.size(); i++) {
			m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos = m_SaveData[i]->Pos;
			m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot = m_SaveData[i]->Rot;
			m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl = m_SaveData[i]->Scl;
		}
	}

	// 各モーション情報
	for (int i = 0; i < m_Motion.size(); i++)
	{
		sprintf(BoxName, "MotionID %d", i);
		if (ImGui::CollapsingHeader(BoxName))
		{
			// 各キーフレーム情報
			for (int j = 0; j < m_Motion[i]->KeyFrame.size(); j++)
			{
				sprintf(BoxName, "KeyID %d", j);
				if (ImGui::TreeNode(BoxName))
				{
					sprintf(BoxName, "Frame %d", j);
					ImGui::DragInt(BoxName, &m_Motion[i]->KeyFrame[j]->Frame);

					// 各パーツ情報
					for (int k = 0; k < m_Part.size(); k++) {
						ImGui::Text("Num %d(%s)", k, m_Part[k]->Model->Name);

						sprintf(BoxName, "POS %d-%d", j, k);
						if (ImGui::TreeNode(BoxName))
						{
							sprintf(BoxName, "posX  %d-%d", j, k);
							ImGui::InputFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Pos.x, value);
							sprintf(BoxName, " posX %d-%d", j, k);
							ImGui::DragFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Pos.x, value);

							sprintf(BoxName, "posY  %d-%d", j, k);
							ImGui::InputFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Pos.y, value);
							sprintf(BoxName, " posY %d-%d", j, k);
							ImGui::DragFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Pos.y, value);

							sprintf(BoxName, "posZ  %d-%d", j, k);
							ImGui::InputFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Pos.z, value);
							sprintf(BoxName, " posZ %d-%d", j, k);
							ImGui::DragFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Pos.z, value);
							ImGui::TreePop();
						}

						sprintf(BoxName, "ROT %d-%d", j, k);
						if (ImGui::TreeNode(BoxName))
						{
							sprintf(BoxName, "rotX  %d-%d", j, k);
							ImGui::InputFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Rot.x, value);
							sprintf(BoxName, " rotX %d-%d", j, k);
							ImGui::DragFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Rot.x, value);

							sprintf(BoxName, "rotY  %d-%d", j, k);
							ImGui::InputFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Rot.y, value);
							sprintf(BoxName, " rotY %d-%d", j, k);
							ImGui::DragFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Rot.y, value);

							sprintf(BoxName, "rotZ  %d-%d", j, k);
							ImGui::InputFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Rot.z, value);
							sprintf(BoxName, " rotZ %d-%d", j, k);
							ImGui::DragFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Rot.z, value);
							ImGui::TreePop();
						}

						sprintf(BoxName, "SCL %d-%d", j, k);
						if (ImGui::TreeNode(BoxName))
						{
							ImGui::InputFloat("Scl  ALL", &scl_all, value);
							ImGui::DragFloat(" Scl ALL", &scl_all, value);
							m_Motion[i]->KeyFrame[j]->Key[k]->Scl.x += scl_all;
							m_Motion[i]->KeyFrame[j]->Key[k]->Scl.y += scl_all;
							m_Motion[i]->KeyFrame[j]->Key[k]->Scl.z += scl_all;
							scl_all = 0.0f;
							sprintf(BoxName, "sclX  %d-%d", j, k);
							ImGui::InputFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Scl.x, value);
							sprintf(BoxName, " sclX %d-%d", j, k);
							ImGui::DragFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Scl.x, value);

							sprintf(BoxName, "sclY  %d-%d", j, k);
							ImGui::InputFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Scl.y, value);
							sprintf(BoxName, " sclY %d-%d", j, k);
							ImGui::DragFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Scl.y, value);

							sprintf(BoxName, "sclZ  %d-%d", j, k);
							ImGui::InputFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Scl.z, value);
							sprintf(BoxName, " sclZ %d-%d", j, k);
							ImGui::DragFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Scl.z, value);
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
			}
		}
	}
	ImGui::End();
}
void CSceneMotionPartsX::imGuiCollision()
{
	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Collision Window", &collision_window);
	std::string BoxName;
	ImGui::Text("MotionID %d", m_MotionID);
	ImGui::InputFloat("Value", &value, 0.001f);

	if (ImGui::Button("Collision++")) CollisionAdd(m_MotionID);
	if (ImGui::Button("Collision--")) CollisionSub(m_MotionID);
	for (int j = 0; j < m_Motion[m_MotionID]->Collision.size(); j++)
	{
		BoxName = "CollisionID " + std::to_string(j);
		if (ImGui::CollapsingHeader(BoxName.c_str()))
		{
			BoxName = "PartsID " + std::to_string(j);
			ImGui::InputInt(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->PartID);
			if (m_Motion[m_MotionID]->Collision[j]->PartID < 0) m_Motion[m_MotionID]->Collision[j]->PartID = 0;
			if (m_Motion[m_MotionID]->Collision[j]->PartID >= (int)m_Part.size()) m_Motion[m_MotionID]->Collision[j]->PartID = (int)m_Part.size()-1;

			BoxName = "POS " + std::to_string(j);
			if (ImGui::TreeNode(BoxName.c_str()))
			{
				BoxName = "posX  " + std::to_string(j);
				ImGui::InputFloat(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->Pos.x, value);
				BoxName = " posX " + std::to_string(j);
				ImGui::DragFloat(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->Pos.x, value);

				BoxName = "posY  " + std::to_string(j);
				ImGui::InputFloat(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->Pos.y, value);
				BoxName = " posY " + std::to_string(j);
				ImGui::DragFloat(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->Pos.y, value);

				BoxName = "posZ  " + std::to_string(j);
				ImGui::InputFloat(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->Pos.z, value);
				BoxName = " posZ " + std::to_string(j);
				ImGui::DragFloat(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->Pos.z, value);
				ImGui::TreePop();
			}

			BoxName = "SCL " + std::to_string(j);
			if (ImGui::TreeNode(BoxName.c_str()))
			{
				BoxName = "scl  " + std::to_string(j);
				ImGui::InputFloat(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->Scl, value);
				BoxName = " scl " + std::to_string(j);
				ImGui::DragFloat(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->Scl, value);
				ImGui::TreePop();
			}

			BoxName = "START FRAME " + std::to_string(j);
			ImGui::InputInt(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->StartFrame);
			if (m_Motion[m_MotionID]->Collision[j]->StartFrame < 0) m_Motion[m_MotionID]->Collision[j]->StartFrame = 0;

			BoxName = "  END FRAME " + std::to_string(j);
			ImGui::InputInt(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->EndFrame);
			if (m_Motion[m_MotionID]->Collision[j]->StartFrame > m_Motion[m_MotionID]->Collision[j]->EndFrame)
				m_Motion[m_MotionID]->Collision[j]->EndFrame = m_Motion[m_MotionID]->Collision[j]->StartFrame;
		}
	}
	ImGui::End();
}
void CSceneMotionPartsX::imGuiSound()
{
	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Sound Window", &sound_window);
	std::string BoxName;
	ImGui::Text("MotionID %d", m_MotionID);

	if (ImGui::Button("Sound++")) SoundAdd();
	if (ImGui::Button("Sound--")) SoundSub();

	for (int i = 0; i < m_Motion[m_MotionID]->Sound.size(); i++)
	{
		BoxName = "SoundID " + std::to_string(i);
		if (ImGui::CollapsingHeader(BoxName.c_str()))
		{
			BoxName = "Frame " + std::to_string(i);
			ImGui::InputInt(BoxName.c_str(), &m_Motion[m_MotionID]->Sound[i]->Frame);
			if (m_Motion[m_MotionID]->Sound[i]->Frame < -1) { m_Motion[m_MotionID]->Sound[i]->Frame = -1; }
			if (m_Motion[m_MotionID]->Sound[i]->Frame >= m_MotionFrameAll) { m_Motion[m_MotionID]->Sound[i]->Frame = m_MotionFrameAll - 1; }

			int IDold = m_Motion[m_MotionID]->Sound[i]->ID;
			BoxName = "Sound " + std::to_string(i);
			ImGui::InputInt(BoxName.c_str(), &m_Motion[m_MotionID]->Sound[i]->ID);
			if (m_Motion[m_MotionID]->Sound[i]->ID < 0) { m_Motion[m_MotionID]->Sound[i]->ID = 0; }
			if (m_Motion[m_MotionID]->Sound[i]->ID >= CSound::SOUND_MAX) { m_Motion[m_MotionID]->Sound[i]->ID = CSound::SOUND_MAX - 1; }
			if (IDold != m_Motion[m_MotionID]->Sound[i]->ID)
			{
				CManager::GetSound()->Stop((CSound::SOUND_LABEL)IDold);
			}
			ImGui::Text("%d-(%s)",i, CManager::GetSound()->m_aParam[m_Motion[m_MotionID]->Sound[i]->ID].pFilename);
			BoxName = "Play Sound " + std::to_string(i);
			if (ImGui::Button(BoxName.c_str()))
			{
				CManager::GetSound()->Play((CSound::SOUND_LABEL)m_Motion[m_MotionID]->Sound[i]->ID);
			}
		}
	}
	ImGui::End();
}
void CSceneMotionPartsX::imGuiMotion()
{
	char BoxName[32];
	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Now Motion Window", &model_window);
	ImGui::InputFloat("Value", &value, 0.001f);

	// パーツ数表示
	ImGui::Text("Parts Num %d", m_Part.size());

	sprintf(BoxName, "Start KeyID %d", m_Key);
	if (ImGui::CollapsingHeader(BoxName))
	{
		// キーフレーム情報保存
		if (ImGui::Button("SaveStartKey"))
		{
			for (int i = 0; i < m_Part.size(); i++) {
				m_SaveData[i]->Pos = m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos;
				m_SaveData[i]->Rot = m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot;
				m_SaveData[i]->Scl = m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl;
			}
		}
		// キーフレーム情報読込
		if (ImGui::Button("LoadStartKey"))
		{
			for (int i = 0; i < m_Part.size(); i++) {
				m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos = m_SaveData[i]->Pos;
				m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot = m_SaveData[i]->Rot;
				m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl = m_SaveData[i]->Scl;
			}
		}

		// フレーム情報
		sprintf(BoxName, "Frame %d", m_Key);
		ImGui::DragInt(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Frame);

		// 各パーツ情報
		for (int i = 0; i < m_Part.size(); i++) {
			ImGui::Text("Num %d(%s)", i, m_Part[i]->Model->Name);

			sprintf(BoxName, "POS %d-%d", m_Key, i);
			if (ImGui::TreeNode(BoxName))
			{
				sprintf(BoxName, "posX  %d-%d", m_Key, i);
				ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos.x, value);
				sprintf(BoxName, " posX %d-%d", m_Key, i);
				ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos.x, value);

				sprintf(BoxName, "posY  %d-%d", m_Key, i);
				ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos.y, value);
				sprintf(BoxName, " posY %d-%d", m_Key, i);
				ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos.y, value);

				sprintf(BoxName, "posZ  %d-%d", m_Key, i);
				ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos.z, value);
				sprintf(BoxName, " posZ %d-%d", m_Key, i);
				ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos.z, value);
				ImGui::TreePop();
			}

			sprintf(BoxName, "ROT %d-%d", m_Key, i);
			if (ImGui::TreeNode(BoxName))
			{
				sprintf(BoxName, "rotX  %d-%d", m_Key, i);
				ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot.x, value);
				sprintf(BoxName, " rotX %d-%d", m_Key, i);
				ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot.x, value);

				sprintf(BoxName, "rotY  %d-%d", m_Key, i);
				ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot.y, value);
				sprintf(BoxName, " rotY %d-%d", m_Key, i);
				ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot.y, value);

				sprintf(BoxName, "rotZ  %d-%d", m_Key, i);
				ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot.z, value);
				sprintf(BoxName, " rotZ %d-%d", m_Key, i);
				ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot.z, value);
				ImGui::TreePop();
			}

			sprintf(BoxName, "SCL %d-%d", m_Key, i);
			if (ImGui::TreeNode(BoxName))
			{
				ImGui::InputFloat("Scl  ALL", &scl_all, value);
				ImGui::DragFloat(" Scl ALL", &scl_all, value);
				m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl.x += scl_all;
				m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl.y += scl_all;
				m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl.z += scl_all;
				scl_all = 0.0f;
				sprintf(BoxName, "sclX  %d-%d", m_Key, i);
				ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl.x, value);
				sprintf(BoxName, " sclX %d-%d", m_Key, i);
				ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl.x, value);

				sprintf(BoxName, "sclY  %d-%d", m_Key, i);
				ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl.y, value);
				sprintf(BoxName, " sclY %d-%d", m_Key, i);
				ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl.y, value);

				sprintf(BoxName, "sclZ  %d-%d", m_Key, i);
				ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl.z, value);
				sprintf(BoxName, " sclZ %d-%d", m_Key, i);
				ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl.z, value);
				ImGui::TreePop();
			}
		}
	}

	if (m_Motion[m_MotionID]->KeyFrame[m_Key]->Frame > 0)
	{
		int NextKey = m_Key + 1;

		// 次のキーフレームのクリア
		if (NextKey >= m_Motion[m_MotionID]->KeyFrame.size())
			NextKey = 0;

		sprintf(BoxName, "  End KeyID %d", NextKey);
		if (ImGui::CollapsingHeader(BoxName))
		{
			// キーフレーム情報保存
			if (ImGui::Button("SaveEndKey"))
			{
				for (int i = 0; i < m_Part.size(); i++) {
					m_SaveData[i]->Pos = m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Pos;
					m_SaveData[i]->Rot = m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Rot;
					m_SaveData[i]->Scl = m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl;
				}
			}
			// キーフレーム情報読込
			if (ImGui::Button("LoadEndKey"))
			{
				for (int i = 0; i < m_Part.size(); i++) {
					m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Pos = m_SaveData[i]->Pos;
					m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Rot = m_SaveData[i]->Rot;
					m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl = m_SaveData[i]->Scl;
				}
			}

			// フレーム情報
			sprintf(BoxName, "Frame %d", NextKey);
			ImGui::DragInt(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Frame);

			// 各パーツ情報
			for (int i = 0; i < m_Part.size(); i++) {
				ImGui::Text("Num %d(%s)", i, m_Part[i]->Model->Name);

				sprintf(BoxName, "POS %d-%d", NextKey, i);
				if (ImGui::TreeNode(BoxName))
				{
					sprintf(BoxName, "posX  %d-%d", NextKey, i);
					ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Pos.x, value);
					sprintf(BoxName, " posX %d-%d", NextKey, i);
					ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Pos.x, value);

					sprintf(BoxName, "posY  %d-%d", NextKey, i);
					ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Pos.y, value);
					sprintf(BoxName, " posY %d-%d", NextKey, i);
					ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Pos.y, value);

					sprintf(BoxName, "posZ  %d-%d", NextKey, i);
					ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Pos.z, value);
					sprintf(BoxName, " posZ %d-%d", NextKey, i);
					ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Pos.z, value);
					ImGui::TreePop();
				}

				sprintf(BoxName, "ROT %d-%d", NextKey, i);
				if (ImGui::TreeNode(BoxName))
				{
					sprintf(BoxName, "rotX  %d-%d", NextKey, i);
					ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Rot.x, value);
					sprintf(BoxName, " rotX %d-%d", NextKey, i);
					ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Rot.x, value);

					sprintf(BoxName, "rotY  %d-%d", NextKey, i);
					ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Rot.y, value);
					sprintf(BoxName, " rotY %d-%d", NextKey, i);
					ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Rot.y, value);

					sprintf(BoxName, "rotZ  %d-%d", NextKey, i);
					ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Rot.z, value);
					sprintf(BoxName, " rotZ %d-%d", NextKey, i);
					ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Rot.z, value);
					ImGui::TreePop();
				}

				sprintf(BoxName, "SCL %d-%d", NextKey, i);
				if (ImGui::TreeNode(BoxName))
				{
					ImGui::InputFloat("Scl  ALL", &scl_all, value);
					ImGui::DragFloat(" Scl ALL", &scl_all, value);
					m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl.x += scl_all;
					m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl.y += scl_all;
					m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl.z += scl_all;
					scl_all = 0.0f;
					sprintf(BoxName, "sclX  %d-%d", NextKey, i);
					ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl.x, value);
					sprintf(BoxName, " sclX %d-%d", NextKey, i);
					ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl.x, value);

					sprintf(BoxName, "sclY  %d-%d", NextKey, i);
					ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl.y, value);
					sprintf(BoxName, " sclY %d-%d", NextKey, i);
					ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl.y, value);

					sprintf(BoxName, "sclZ  %d-%d", NextKey, i);
					ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl.z, value);
					sprintf(BoxName, " sclZ %d-%d", NextKey, i);
					ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl.z, value);
					ImGui::TreePop();
				}
			}
		}
	}

	ImGui::End();
}