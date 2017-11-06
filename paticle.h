//*****************************************************************************
//   ��d��`�h�~
//*****************************************************************************
#ifndef _PATICLE_H_
#define _PATICLE_H_

//*****************************************************************************
//   �N���X�̒�`
//*****************************************************************************
class CPaticle
{
public:
	void Init(void);                              // ����������
	void Uninit(void);                            // �I������
	void Update(void);                            // �X�V����
	void Draw(void);                              // �`�揈��
	
private:
	void MakeVex(void);     // ���_�̐ݒ�
	void MakeBuff(void);    // �o�b�t�@�̐ݒ�

	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;     // �C���f�b�N�X�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;  // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9      m_pTexture;  // �e�N�X�`��
	int                     m_nPolygonNum;
	int                     m_nVexNum;
	D3DXVECTOR3             m_pos;       // ���W
	D3DXVECTOR3             m_scl;      // �T�C�Y
	D3DXVECTOR3             m_rot;
	D3DXCOLOR               m_color;    // �F
};

#endif