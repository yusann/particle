//*****************************************************************************
//   ��d��`�h�~
//*****************************************************************************
#ifndef _LIGHT_H_
#define _LIGHT_H_

//*****************************************************************************
//   �N���X�̒�`
//*****************************************************************************
// �V�[���N���X
class CLight
{
public:
	typedef struct
	{
		D3DLIGHT9 light;              // ���C�g
		D3DXVECTOR3 vecDir;           // ����
	}LIGHT;

	CLight();                                   // �f�t�H���g�R���X�g���N�^
	~CLight();                                  // �f�X�g���N�^
	
	void Init(void);                              // ����������
	void Uninit(void);                            // �I������
	void Update(void);
	void Set(void);

	// �J�����̎擾
	static LIGHT GetLight() {
		return m_light;
	}

private:
	static LIGHT m_light;
};

#endif