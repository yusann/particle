//*****************************************************************************
//   ��d��`�h�~
//*****************************************************************************
#ifndef _MESHSPHERE_H_
#define _MESHSPHERE_H_

//*****************************************************************************
//   �N���X�̒�`
//*****************************************************************************
// �V�[���N���X
class CMeshSphere : public CSceneMesh
{
private:
	void MakeVex(void);     // ���_�̐ݒ�
	void MakeBuff(void);    // �o�b�t�@�̐ݒ�
	float m_Radius;
	int m_NumBlockX;
	int m_NumBlockY;
	bool m_Use;

public:
	CMeshSphere();                                   // �f�t�H���g�R���X�g���N�^
	~CMeshSphere();                                  // �f�X�g���N�^

	void Init(D3DXVECTOR3 pos, float radius, int numBlockX, int numBlockY, D3DXCOLOR color);                              // ����������
	void Uninit(void);                            // �I������
	void Update(D3DXVECTOR3 pos, float radius);   // �X�V����
	void Draw(void);                              // �`�揈��

	static CMeshSphere *Create(D3DXVECTOR3 pos,
		float radius,
		int numBlockX,
		int numBlockY,
		D3DXCOLOR color = D3DXCOLOR(1.0f,0.0f,0.0f,0.1f));                  // �쐬
};

#endif