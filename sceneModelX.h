//*****************************************************************************
//   ��d��`�h�~
//*****************************************************************************
#ifndef _SCENEMODELX_H_
#define _SCENEMODELX_H_

#define MODEL_MATERIAL_MAX (100)

//*****************************************************************************
//   �N���X�̒�`
//*****************************************************************************
// �V�[���N���X
class CSceneModelX : public CScene
{
public:
	CSceneModelX(int nPriority = 3);                                   // �f�t�H���g�R���X�g���N�^
	~CSceneModelX();                                  // �f�X�g���N�^
	
	void Init(void);                              // ����������
	void Uninit(void);                            // �I������
	void Update(void);                            // �X�V����
	void Draw(void);                              // �`�揈��

protected:
	D3DXVECTOR3 m_vecUp;    // �����
	D3DXVECTOR3 m_rot;      // ��]

private:
	LPD3DXMESH				m_pMeshModel;                    // ���b�V�����
	LPD3DXBUFFER			m_pBuffMatModel;                 // �}�e���A�����
	DWORD					m_nNumMatModel;                  // �}�e���A�����̐�
	LPDIRECT3DTEXTURE9		m_pTextureModel[ MODEL_MATERIAL_MAX ];    // �}�b�s���O�p�e�N�X�`���C���^�[�t�F�[�X
};

#endif