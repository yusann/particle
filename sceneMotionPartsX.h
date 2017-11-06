//=============================================================================
//
// �p�[�c���f��.cpp
// Author : YUUSAN KA
//--------------------------
// DATA : 2017/01/12
//=============================================================================
//*****************************************************************************
//   ��d��`�h�~
//*****************************************************************************
#ifndef _SCENEMOTIONPARTSX_H_
#define _SCENEMOTIONPARTSX_H_

#include "modelX.h"
//#include "motionPartsX.h"
class CMeshSphere;

//*****************************************************************************
//   �N���X�̒�`
//*****************************************************************************
// �V�[���N���X
class CSceneMotionPartsX : public CScene
{
public:
	CSceneMotionPartsX(int nPriority = 3);                                   // �f�t�H���g�R���X�g���N�^
	~CSceneMotionPartsX();                                  // �f�X�g���N�^
	
	// �L�[���
	struct SOUND
	{
		int ID;
		int Frame;
	};

	// ���̓����蔻����
	struct COLLISION_SPHERE
	{
		CMeshSphere			*Sphere;
		D3DXVECTOR3			Pos;
		float				Scl;
		bool				Use;
	};

	// �����蔻����
	struct COLLISION_DATA
	{
		int					PartID;
		D3DXVECTOR3			Pos;
		float				Scl;
		int					StartFrame;
		int					EndFrame;
	};
	
	// �p�[�c�������
	struct PART
	{
		CModelX::MODELX		*Model;
		D3DXVECTOR3			OffSetPos;
		D3DXVECTOR3			OffSetRot;
		D3DXVECTOR3			OffSetScl;
		int					Parent;
	};

	// �L�[���
	struct KEY
	{
		D3DXVECTOR3			Pos;
		D3DXVECTOR3			Rot;
		D3DXVECTOR3			Scl;
	};

	// �L�[�t���[�����
	struct KEY_FRAME
	{
		int Frame;
		std::vector<KEY*> Key;
	};

	// ���[�V�������
	struct MOTION
	{
		std::vector<KEY_FRAME*> KeyFrame;
		std::vector<COLLISION_DATA*>	Collision;
		std::vector<SOUND*>		Sound;
	};

	// ���f��
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

	void Init(void);                              // ����������
	void Uninit(void);                            // �I������
	void Update(void);                            // �X�V����
	void Draw(void);                              // �`�揈��
	D3DXVECTOR3 GetRot(void) { return m_Rot; }
	void SetMotion(int MotionID);

protected:
	std::vector<MOTION*>	m_Motion;		// ���[�V����
	D3DXVECTOR3				m_Rot;			// ��]
	float					m_Speed;		// �ړ����x
	float					m_Jump;			// �W�����v��
	bool m_LastKye;

private:
	void UpdateMotion(void);				// �}�g���N�X�Z�b�g����
	void UpdateBlendMotion(void);			// �قȂ郂�[�V�����̃}�g���N�X�Z�b�g����
	void LoadDataFile(void);				// �t�@�C���p�X�̓ǂݍ���
	void MotionAdd(void);					// ���[�V�����̒ǉ�
	void MotionSub(void);					// ����
	void KeyFrameAdd(void);					// �L�[�t���[���̒ǉ�
	void KeyFrameSub(void);					// ����
	void KeyAdd(void);						// �L�[�̒ǉ�
	void KeySub(void);						// ����
	bool LoadMotionFile(char *FileName);	// ���[�V�����t�@�C���̓ǂݍ���
	void ChangeFile(char *FileName);		// �L�����`�F���W
	void CreateModel(void);					// �ǉ�
	void DelModel(void);					// ����
	void CollisionAdd(int MotionID);		// �����蔻��ǉ�
	void CollisionSub(int MotionID);		// ����
	void SoundAdd(void);					// ���ʉ��ǉ�
	void SoundSub(void);					// ����
	void ChangeModel(PART *pPart,char *FileName);			// �`�F���W
	void SaveMotion(char *FileName);		// �Z�[�u���[�V����
	void ImGui(void);						// �f�o�b�O�\��

	// �L�������
	std::vector<PART*>		m_Part;
	std::vector<MODEL*>		m_Model;
	D3DXMATRIX				m_Matrix;
	int m_MotionID;
	int m_Frame;
	int m_Key;
	int m_MotionFrame;
	int m_MotionFrameAll;

	// �ꎞ�L���p
	std::vector<VECTR3*>	m_SaveData;

	// �قȂ郂�[�V�����̕�ԏ����p
	std::vector<VECTR3*>	m_BlendData;
	bool					m_Blend;

	// �f�o�b�O�\���p�E�ҏW
	void imGuiOffset(void);			// �I�t�Z�b�g�ҏW
	void imGuiModel(void);			// ���f�����ꗗ
	void imGuiAllMotion(void);		// ���[�V�����ҏW
	void imGuiCollision(void);		// �����蔻��ҏW
	void imGuiSound(void);			// ���ʉ��ҏW
	void imGuiMotion(void);			// ���݂̃��[�V�������ꗗ

	std::string imGui_parts_name;			// ���[�V�����̃t�@�C���p�X�i�\���p�j
	std::vector<std::string> parts_name;	// ����p
	int parts_id;							// ID

	std::string imGui_model_name;			// ���f���̃t�@�C���p�X�i�\���p�j
	std::vector<std::string> model_name;	// ����p
	int model_id;							// ID

	bool keyUpdata;					// �L�[�������t���O
	bool motionMode;				// �L�����X�V�����t���O
	bool offset_window;				// �I�t�Z�b�g�E�B���h�E
	bool model_window;				// ���f���E�B���h�E
	bool all_motion_window;			// ���[�V�����E�B���h�E
	bool save_data_window;			// �ꎞ�ۑ��p���E�B���h�E
	bool motion_window;				// ���݂̃��[�V�����E�B���h�E
	bool collision_window;			// �����蔻��f�[�^�E�B���h�E
	bool sound_window;				// �T�E���h�E�B���h�E
	float scl_all;					// �X�P�[���̈ꊇ����p
	float value;					// �ω��ʒ����p
};

#endif