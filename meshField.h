//*****************************************************************************
//   ��d��`�h�~
//*****************************************************************************
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//*****************************************************************************
//   �N���X�̒�`
//*****************************************************************************
// �V�[���N���X
class CMeshField : public CSceneMesh
{
public:
	CMeshField();                                   // �f�t�H���g�R���X�g���N�^
	~CMeshField();                                  // �f�X�g���N�^
	
	void Init(void);                              // ����������
	void Uninit(void);                            // �I������
	void Update(void);                            // �X�V����
	void Draw(void);                              // �`�揈��

	static CMeshField *Create(void);                  // �쐬
	float GetHeight(D3DXVECTOR3 pos);

private:
	void MakeVex(void);     // ���_�̐ݒ�
	void MakeBuff(void);    // �o�b�t�@�̐ݒ�
	void LoadFile(void);

	D3DXVECTOR2 m_size;
	D3DXVECTOR2 m_divide;
	D3DXVECTOR2 m_texUV;
	float *m_PosY;
	int m_MaxX, m_MaxY;

};

#endif