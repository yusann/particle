//*****************************************************************************
//   ��d��`�h�~
//*****************************************************************************
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
//   �萔��`
//*****************************************************************************
#define SCENE_PRIORITY_MAX  (8)						// �D��x�̍ő吔

//*****************************************************************************
//   �N���X�̒�`
//*****************************************************************************
// �V�[���N���X
class CScene
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_BACKGROUND,

		OBJTYPE_FIELD,
		OBJTYPE_PLAYER,
		OBJTYPE_ENEMY,
		OBJTYPE_BULLET,
		OBJTYPE_EXPLOSION,

		OBJTYPE_UI,
		OBJTYPE_MAX
	}OBJTYPE;

	CScene(int nPriority = 3);                        // �f�t�H���g�R���X�g���N�^
	virtual ~CScene();                       // �f�X�g���N�^
	
	virtual void Init(void)   =0;      // ����������
	virtual void Uninit(void) =0;      // �I������
	virtual void Update(void) =0;      // �X�V����
	virtual void Draw(void)   =0;      // �`�揈��

	static void UpdateAll(void);                // ���ׂĂ̍X�V
	static void DrawAll(void);                  // ���ׂĂ̕`��
	static void ReleaseAll(void);               // ���ׂẴ����[�X

	static CScene *GetScene(int nPriority);       // �V�[���̎擾����
	void SetObjType(OBJTYPE objType);                            // �V�[���^�C�v�̑������
	OBJTYPE GetObjType(void);                                      // �V�[���^�C�v�̎擾����

	// ���W�̎擾
	D3DXVECTOR3 GetPos(void);                          // 

protected:
	void Release(void);                         // �����[�X�����i�q�N���X�������Ă���e�N���X���������ߗp�j
	D3DXVECTOR3             m_pos;       // ���W
	D3DXVECTOR3             m_scl;      // �T�C�Y
	D3DXCOLOR               m_color;    // �F
	LPDIRECT3DTEXTURE9      m_pTexture;  // �e�N�X�`��
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;  // ���_�o�b�t�@�ւ̃|�C���^

private:
	static CScene *m_Top[SCENE_PRIORITY_MAX];      // �V�[���̐錾
	CScene *m_Prev;
	CScene *m_Next;
	static int m_nNumScene;                     // �V�[���̑���
	int m_nPriority;
	OBJTYPE m_objType;
	bool m_bDelete;

};

#endif