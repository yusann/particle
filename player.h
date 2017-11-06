//*****************************************************************************
//   二重定義防止
//*****************************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

class CObject2D;

//*****************************************************************************
//   クラスの定義
//*****************************************************************************
// シーンクラス
class CPlayer : public CSceneMotionPartsX
{
public:

	// 状態
	typedef enum
	{
		MODE_NORMAL = 0,
		MODE_MOVE,
		MODE_ATTACK,
		MODE_JUMP,
		MODE_MAX
	}MODE;

	CPlayer();             // デフォルトコンストラクタ
	~CPlayer();                             // デストラクタ

	static CPlayer *Create(D3DXVECTOR3 pos);                  // 作成
	void Init(D3DXVECTOR3 pos);        // 初期化処理
	void Uninit(void);      // 終了処理
	void Update(void);      // 更新処理
	void Draw(void);        // 描画処理

private:
	void InputKey(void);
	void ModeNormal(void);
	void ModeMove(void);
	void ModeAttack(void);
	void ModeJump(void);

	void ImGui(void);

	MODE m_Mode;
	D3DXVECTOR3 m_Move;
};

#endif