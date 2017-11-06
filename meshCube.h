//*****************************************************************************
//   ��d��`�h�~
//*****************************************************************************
#ifndef _MESHCUBE_H_
#define _MESHCUBE_H_

//*****************************************************************************
//   �N���X�̒�`
//*****************************************************************************
// �V�[���N���X
class CMeshCube : public CSceneMesh
{
public:
	CMeshCube();                                   // �f�t�H���g�R���X�g���N�^
	~CMeshCube();                                  // �f�X�g���N�^

	void Init(void);                              // ����������
	void Uninit(void);                            // �I������
	void Update(void);                            // �X�V����
	void Draw(void);                              // �`�揈��

	static CMeshCube *Create(void);                  // �쐬
	
private:
	void MakeVex(void);     // ���_�̐ݒ�
	void MakeBuff(void);    // �o�b�t�@�̐ݒ�

};

#endif