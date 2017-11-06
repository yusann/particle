//*****************************************************************************
//   ��d��`�h�~
//*****************************************************************************
#ifndef _MODELX_H_
#define _MODELX_H_

//*****************************************************************************
//   �N���X�̒�`
//*****************************************************************************
// �V�[���N���X
class CModelX
{
public:

	// ���f�����
	typedef enum
	{
		TYPE_PLAYER = 0,
		TYPE_ENEMY,
		TYPE_BLOCK,
		TYPE_MAX
	}TYPE;

	// ���f�����
	struct MODELX
	{
		char*				Name;
		LPD3DXMESH			pMesh;              // ���C�g
		LPD3DXBUFFER		pBuffMat;           // ����
		DWORD				NumMat;
		LPDIRECT3DTEXTURE9	*pTexture;
	};
	
	static void Init(void);                              // ����������
	static void Uninit(void);                            // �I������
	static MODELX *GetModelX(int type) { return &m_modelX[m_FileName[type]]; }	// ���f���擾
	static MODELX *GetModelX(char *FileName);									// ���f���擾

private:
	static MODELX LoadModel(std::string FileName);
	static std::unordered_map<std::string, MODELX>	m_modelX;
	static std::vector<std::string> m_FileName;
};

#endif