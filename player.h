//*****************************************************************************
//   ��d��`�h�~
//*****************************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

class CObject2D;

//*****************************************************************************
//   �N���X�̒�`
//*****************************************************************************
// �V�[���N���X
class CPlayer : public CSceneMotionPartsX
{
public:

	// ���
	typedef enum
	{
		MODE_NORMAL = 0,
		MODE_MOVE,
		MODE_ATTACK,
		MODE_JUMP,
		MODE_MAX
	}MODE;

	CPlayer();             // �f�t�H���g�R���X�g���N�^
	~CPlayer();                             // �f�X�g���N�^

	static CPlayer *Create(D3DXVECTOR3 pos);                  // �쐬
	void Init(D3DXVECTOR3 pos);        // ����������
	void Uninit(void);      // �I������
	void Update(void);      // �X�V����
	void Draw(void);        // �`�揈��

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