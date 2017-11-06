//*****************************************************************************
//   ��d��`�h�~
//*****************************************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
//   �N���X�̒�`
//*****************************************************************************
// �V�[���N���X
class CCamera
{
public:
	typedef struct
	{
		D3DXMATRIX  mtxView;     // �r���[�s��
		D3DXMATRIX  mtxProj;     // �r���[�s��
		D3DXVECTOR3 posEye;      // ���W
		D3DXVECTOR3 posAt;       // �����_
		D3DXVECTOR3 vecUp;       // �����
	}CAMERA;

	CCamera();                                   // �f�t�H���g�R���X�g���N�^
	~CCamera();                                  // �f�X�g���N�^
	
	void Init(void);                              // ����������
	void Uninit(void);                            // �I������
	void Update(void);		// �X�V����
	void Set(void);			// �Z�b�g����

	// �J�����̎擾
	static CAMERA GetCamera() {
		return m_pCamera;
	}

private:
	static CAMERA m_pCamera;
	D3DXVECTOR3     m_posMove;     // �ړ���
	D3DXVECTOR3     m_rot;         // �p�x
	float       m_length;
	float       m_rotY;
	float       m_rotX;
	float       m_fZoom;       // �Y�[��
};

#endif