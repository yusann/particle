//*****************************************************************************
//   ��d��`�h�~
//*****************************************************************************
#ifndef _2D_SAMPLE_H_
#define _2D_SAMPLE_H_

//*****************************************************************************
//   �N���X�̒�`
//*****************************************************************************
// �V�[���N���X
class C2D_sample : public CScene2D
{
public:
	C2D_sample();             // �f�t�H���g�R���X�g���N�^
	~C2D_sample();                             // �f�X�g���N�^
	
	void Init(void);        // ����������
	void Uninit(void);      // �I������
	void Update(void);      // �X�V����
	void Draw(void);        // �`�揈��

	static C2D_sample *Create(D3DXVECTOR3 pos);                  // �쐬

private:
	void SetPos(D3DXVECTOR3 pos);        // �`�揈��
};

#endif