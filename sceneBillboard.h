//*****************************************************************************
//   ��d��`�h�~
//*****************************************************************************
#ifndef _SCENEBILLBOARD_H_
#define _SCENEBILLBOARD_H_

//*****************************************************************************
//   �N���X�̒�`
//*****************************************************************************
// �V�[���N���X
class CSceneBillboard : public CScene
{
public:
	CSceneBillboard(int nPriority = 3);             // �f�t�H���g�R���X�g���N�^
	~CSceneBillboard();            // �f�X�g���N�^
	
	void Init(void);        // ����������
	void Uninit(void);      // �I������
	void Update(void);      // �X�V����
	void Draw(void);        // �`�揈��

protected:
	float m_fRot;

private:
	void MakeVex(void);     // ���_�̐ݒ�
};

#endif