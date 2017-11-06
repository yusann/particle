//=============================================================================
//
// パーツモデル.cpp
// Author : YUUSAN KA
//--------------------------
// DATA : 2017/01/12
//=============================================================================
//*****************************************************************************
//   二重定義防止
//*****************************************************************************
#ifndef _SCENEMOTIONPARTSX_H_
#define _SCENEMOTIONPARTSX_H_

#include "modelX.h"
//#include "motionPartsX.h"
class CMeshSphere;

//*****************************************************************************
//   クラスの定義
//*****************************************************************************
// シーンクラス
class CSceneMotionPartsX : public CScene
{
public:
	CSceneMotionPartsX(int nPriority = 3);                                   // デフォルトコンストラクタ
	~CSceneMotionPartsX();                                  // デストラクタ
	
	// キー情報
	struct SOUND
	{
		int ID;
		int Frame;
	};

	// 球の当たり判定情報
	struct COLLISION_SPHERE
	{
		CMeshSphere			*Sphere;
		D3DXVECTOR3			Pos;
		float				Scl;
		bool				Use;
	};

	// 当たり判定情報
	struct COLLISION_DATA
	{
		int					PartID;
		D3DXVECTOR3			Pos;
		float				Scl;
		int					StartFrame;
		int					EndFrame;
	};
	
	// パーツ初期情報
	struct PART
	{
		CModelX::MODELX		*Model;
		D3DXVECTOR3			OffSetPos;
		D3DXVECTOR3			OffSetRot;
		D3DXVECTOR3			OffSetScl;
		int					Parent;
	};

	// キー情報
	struct KEY
	{
		D3DXVECTOR3			Pos;
		D3DXVECTOR3			Rot;
		D3DXVECTOR3			Scl;
	};

	// キーフレーム情報
	struct KEY_FRAME
	{
		int Frame;
		std::vector<KEY*> Key;
	};

	// モーション情報
	struct MOTION
	{
		std::vector<KEY_FRAME*> KeyFrame;
		std::vector<COLLISION_DATA*>	Collision;
		std::vector<SOUND*>		Sound;
	};

	// モデル
	struct MODEL
	{
		MODEL				*pParent;
		D3DXVECTOR3			Pos;
		D3DXVECTOR3			Rot;
		D3DXVECTOR3			Scl;
		D3DXMATRIX			Matrix;
		std::vector<COLLISION_SPHERE*>	Collision;
	};

	// D3DXVECTR3
	struct VECTR3
	{
		D3DXVECTOR3			Pos;
		D3DXVECTOR3			Rot;
		D3DXVECTOR3			Scl;
	};

	void Init(void);                              // 初期化処理
	void Uninit(void);                            // 終了処理
	void Update(void);                            // 更新処理
	void Draw(void);                              // 描画処理
	D3DXVECTOR3 GetRot(void) { return m_Rot; }
	void SetMotion(int MotionID);

protected:
	std::vector<MOTION*>	m_Motion;		// モーション
	D3DXVECTOR3				m_Rot;			// 回転
	float					m_Speed;		// 移動速度
	float					m_Jump;			// ジャンプ力
	bool m_LastKye;

private:
	void UpdateMotion(void);				// マトリクスセット処理
	void UpdateBlendMotion(void);			// 異なるモーションのマトリクスセット処理
	void LoadDataFile(void);				// ファイルパスの読み込み
	void MotionAdd(void);					// モーションの追加
	void MotionSub(void);					// 消去
	void KeyFrameAdd(void);					// キーフレームの追加
	void KeyFrameSub(void);					// 消去
	void KeyAdd(void);						// キーの追加
	void KeySub(void);						// 消去
	bool LoadMotionFile(char *FileName);	// モーションファイルの読み込み
	void ChangeFile(char *FileName);		// キャラチェンジ
	void CreateModel(void);					// 追加
	void DelModel(void);					// 消去
	void CollisionAdd(int MotionID);		// 当たり判定追加
	void CollisionSub(int MotionID);		// 消去
	void SoundAdd(void);					// 効果音追加
	void SoundSub(void);					// 消去
	void ChangeModel(PART *pPart,char *FileName);			// チェンジ
	void SaveMotion(char *FileName);		// セーブモーション
	void ImGui(void);						// デバッグ表示

	// キャラ情報
	std::vector<PART*>		m_Part;
	std::vector<MODEL*>		m_Model;
	D3DXMATRIX				m_Matrix;
	int m_MotionID;
	int m_Frame;
	int m_Key;
	int m_MotionFrame;
	int m_MotionFrameAll;

	// 一時記憶用
	std::vector<VECTR3*>	m_SaveData;

	// 異なるモーションの補間処理用
	std::vector<VECTR3*>	m_BlendData;
	bool					m_Blend;

	// デバッグ表示用・編集
	void imGuiOffset(void);			// オフセット編集
	void imGuiModel(void);			// モデル情報一覧
	void imGuiAllMotion(void);		// モーション編集
	void imGuiCollision(void);		// 当たり判定編集
	void imGuiSound(void);			// 効果音編集
	void imGuiMotion(void);			// 現在のモーション情報一覧

	std::string imGui_parts_name;			// モーションのファイルパス（表示用）
	std::vector<std::string> parts_name;	// 代入用
	int parts_id;							// ID

	std::string imGui_model_name;			// モデルのファイルパス（表示用）
	std::vector<std::string> model_name;	// 代入用
	int model_id;							// ID

	bool keyUpdata;					// キー押し許可フラグ
	bool motionMode;				// キャラ更新処理フラグ
	bool offset_window;				// オフセットウィンドウ
	bool model_window;				// モデルウィンドウ
	bool all_motion_window;			// モーションウィンドウ
	bool save_data_window;			// 一時保存用情報ウィンドウ
	bool motion_window;				// 現在のモーションウィンドウ
	bool collision_window;			// 当たり判定データウィンドウ
	bool sound_window;				// サウンドウィンドウ
	float scl_all;					// スケールの一括操作用
	float value;					// 変化量調整用
};

#endif