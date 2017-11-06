//*****************************************************************************
//   �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "scene.h"
#include "sceneMotionPartsX.h"
#include "sceneMesh.h"
#include "meshSphere.h"
#include "sound.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
//=======================================================================================
//   �R���X�g���N�^
//=======================================================================================
CSceneMotionPartsX::CSceneMotionPartsX(int nPriority) :CScene(nPriority)
{
	// �����o�ϐ��̏�����
	m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_MotionID = 0;
	m_Frame = 0;
	m_Key = 0;

	motionMode = false;
	offset_window = false;
	model_window = false;
	all_motion_window = false;
	motion_window = false;
	save_data_window = false;
	collision_window = false;
	sound_window = false;
	keyUpdata = false;
	scl_all = 0.0f;
	value = 0.01f;
	parts_id = 0;
	model_id = 0;
	m_Blend = false;
	m_MotionFrame = 0;
	m_MotionFrameAll = 0;
}

//=======================================================================================
//   �f�X�g���N�^
//=======================================================================================
CSceneMotionPartsX::~CSceneMotionPartsX()
{
}

//=======================================================================================
//   ����������
//=======================================================================================
void CSceneMotionPartsX::Init()
{
	// �t�@�C���ǂݍ���
	LoadDataFile();
	LoadMotionFile((char*)imGui_parts_name.c_str());

	// �f�[�^���
	for (int i = 0; i < m_Part.size(); i++)
	{
		m_SaveData.push_back(new VECTR3);
		m_BlendData.push_back(new VECTR3);
		// �K�w����
		if (i == 0)
		{
			m_Model[i]->pParent = NULL;
		}
		else
		{
			m_Model[i]->pParent = m_Model[m_Part[i]->Parent];
		}

		// �f�[�^���
		m_Model[i]->Pos = m_Part[i]->OffSetPos + m_Motion[0]->KeyFrame[0]->Key[i]->Pos;
		m_Model[i]->Rot = m_Part[i]->OffSetRot + m_Motion[0]->KeyFrame[0]->Key[i]->Rot;
		m_Model[i]->Scl = m_Part[i]->OffSetScl + m_Motion[0]->KeyFrame[0]->Key[i]->Scl;
		m_SaveData[i]->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_SaveData[i]->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_SaveData[i]->Scl = D3DXVECTOR3(0.0f,0.0f,0.0f);

		m_BlendData[i]->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_BlendData[i]->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_BlendData[i]->Scl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	for (int i = 0; i < m_Motion.size(); i++)
	{
		for (int j = 0; j < m_Motion[i]->Collision.size(); j++)
		{
			m_Model[i]->Collision[j]->Pos = m_Motion[i]->Collision[j]->Pos;
			m_Model[i]->Collision[j]->Scl = m_Motion[i]->Collision[j]->Scl;
			m_Model[i]->Collision[j]->Use = false;
			m_Model[i]->Collision[j]->Sphere = CMeshSphere::Create(m_Model[i]->Collision[j]->Pos, m_Model[i]->Collision[j]->Scl, 10, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
		}
	}

	m_LastKye = false;
	m_MotionID = 0;
	m_Frame = 0;
	m_Key = 0;
}

//=======================================================================================
//   �I������
//=======================================================================================
void CSceneMotionPartsX::Uninit()
{
	// ���[�V�����j��
	for (auto CntMotion = m_Motion.begin(); CntMotion != m_Motion.end(); CntMotion++)
	{
		for (auto CntFrame = (*CntMotion)->KeyFrame.begin(); CntFrame != (*CntMotion)->KeyFrame.end(); CntFrame++)
		{
			(*CntFrame)->Key.clear();
		}
		(*CntMotion)->KeyFrame.clear();
	}
	m_Motion.clear();

	// �p�[�c�j��
	m_Part.clear();

	// ���f���j��
	m_Model.clear();

	m_SaveData.clear();
	m_BlendData.clear();
}

//=======================================================================================
//   �X�V����
//=======================================================================================
void CSceneMotionPartsX::Update()
{
	ImGui();

	// �ϊ��s��̐錾
	D3DXMATRIX mtxRot;             // ���[�J����]
	D3DXMATRIX mtxScl;             // ���[�J���X�P�[��
	D3DXMATRIX mtxPos;             // ���[�J�����W

	// ���[�J����]�̑��
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_Rot.y,
		m_Rot.x,
		m_Rot.z);

	// ���[�J���X�P�[���̑��
	D3DXMatrixScaling(&mtxScl,
		m_scl.x,
		m_scl.y,
		m_scl.z);

	// ���[�J�����W�̑��
	D3DXMatrixTranslation(&mtxPos,
		m_pos.x,
		m_pos.y,
		m_pos.z);

	// ���[���h��񏈗�
	D3DXMatrixIdentity(&m_Matrix);                       // ���[���h�̒��g��������
	D3DXMatrixMultiply(&m_Matrix, &m_Matrix, &mtxScl);   // �p�[�c�������[���h�X�P�[���ɑ�����ă��[���h�����v�Z
	D3DXMatrixMultiply(&m_Matrix, &m_Matrix, &mtxRot);   // ���[���h��]�̑��
	D3DXMatrixMultiply(&m_Matrix, &m_Matrix, &mtxPos);   // ���[���h���W�̑��

	if (!offset_window)
	{
		// ���[�v���Ȃ����[�V��������
		if (m_Motion[m_MotionID]->KeyFrame[m_Key]->Frame <= 0)
		{
			m_LastKye = true;
			return;
		}

		// �t���[���X�V
		if (motionMode) { m_Frame++; }

		// �}�g���N�X�Z�b�g����
		if (!m_Blend) { UpdateMotion(); }
		else { UpdateBlendMotion(); }
	}
	else
	{
		// �ϊ��s��̐錾
		D3DXMATRIX mtxPartRot;             // ���[�J����]
		D3DXMATRIX mtxPartScl;             // ���[�J���X�P�[��
		D3DXMATRIX mtxPartPos;             // ���[�J�����W
		// �f�[�^���
		for (int i = 0; i < m_Part.size(); i++)
		{
			// �f�[�^���
			m_Model[i]->Pos = m_Part[i]->OffSetPos;
			m_Model[i]->Rot = m_Part[i]->OffSetRot;
			m_Model[i]->Scl = m_Part[i]->OffSetScl;

			// ���[�J����]�̑��
			D3DXMatrixRotationYawPitchRoll(&mtxPartRot,
				m_Model[i]->Rot.y,
				m_Model[i]->Rot.x,
				m_Model[i]->Rot.z);

			// ���[�J���X�P�[���̑��
			D3DXMatrixScaling(&mtxPartScl,
				m_Model[i]->Scl.x,
				m_Model[i]->Scl.y,
				m_Model[i]->Scl.z);

			// ���[�J�����W�̑��
			D3DXMatrixTranslation(&mtxPartPos,
				m_Model[i]->Pos.x,
				m_Model[i]->Pos.y,
				m_Model[i]->Pos.z);

			// ���[���h��񏈗�
			D3DXMatrixIdentity(&m_Model[i]->Matrix);                       // ���[���h�̒��g��������
			D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &mtxPartScl);   // ���[���h�X�P�[���̑��
			D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &mtxPartRot);   // ���[���h��]�̑��
			D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &mtxPartPos);   // ���[���h���W�̑��

																						 // �}�g���N�X�̊K�w������
			if (m_Part[i]->Parent == -1) {
				D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &m_Matrix);   // ���[���h���W�̑��
			}
			else {
				D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &m_Model[i]->pParent->Matrix);   // ���[���h���W�̑��
			}
		}
	}

	// ���[�V�����t���[���v�Z
	int FrameOld = 0;
	for (int i = 0; i < m_Key; i++)
	{
		FrameOld += m_Motion[m_MotionID]->KeyFrame[i]->Frame;
	}
	m_MotionFrame = FrameOld + m_Frame;

	m_MotionFrameAll = 0;
	for (int i = 0; i < (int)m_Motion[m_MotionID]->KeyFrame.size(); i++)
	{
		m_MotionFrameAll += m_Motion[m_MotionID]->KeyFrame[i]->Frame;
	}

	// �����蔻��
	for (int i = 0; i < m_Motion[m_MotionID]->Collision.size(); i++)
	{
		if (m_Motion[m_MotionID]->Collision[i]->StartFrame <= m_MotionFrame &&m_Motion[m_MotionID]->Collision[i]->EndFrame >= m_MotionFrame)
		{
			D3DXVec3TransformCoord(&m_Model[m_MotionID]->Collision[i]->Pos,
				&m_Motion[m_MotionID]->Collision[i]->Pos,
				&m_Model[m_Motion[m_MotionID]->Collision[i]->PartID]->Matrix);
			m_Model[m_MotionID]->Collision[i]->Sphere->Update(m_Model[m_MotionID]->Collision[i]->Pos,
				m_Motion[m_MotionID]->Collision[i]->Scl);
			m_Model[m_MotionID]->Collision[i]->Use = true;
		}
		else
		{
			m_Model[m_MotionID]->Collision[i]->Use = false;
		}
	}

	// ���ʉ�
	for (int i = 0; i < m_Motion[m_MotionID]->Sound.size(); i++)
	{
		if( m_MotionFrame == m_Motion[m_MotionID]->Sound[i]->Frame)
		CManager::GetSound()->Play((CSound::SOUND_LABEL)m_Motion[m_MotionID]->Sound[i]->ID);
	}
}
// �}�g���N�X�Z�b�g����
void CSceneMotionPartsX::UpdateMotion()
{
	// �ϊ��s��̐錾
	D3DXMATRIX mtxPartRot;             // ���[�J����]
	D3DXMATRIX mtxPartScl;             // ���[�J���X�P�[��
	D3DXMATRIX mtxPartPos;             // ���[�J�����W
	
	// ���̃L�[�t���[��
	int NextKey = m_Key + 1;

	// ���̃L�[�t���[���̃N���A
	if (NextKey >= m_Motion[m_MotionID]->KeyFrame.size())
		NextKey = 0;

	// �p�[�c�������[�v���ĕ�ԏ���
	for (int i = 0; i < m_Part.size(); i++)
	{
		float Frame = (float)m_Frame / (float)m_Motion[m_MotionID]->KeyFrame[m_Key]->Frame;

		// ��ԗʂōX�V
		m_Model[i]->Pos = (m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos * (1.0f - Frame)) +
			(m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Pos * Frame) +
			m_Part[i]->OffSetPos;
		m_Model[i]->Rot = (m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot * (1.0f - Frame)) +
			(m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Rot * Frame) +
			m_Part[i]->OffSetRot;
		m_Model[i]->Scl = (m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl * (1.0f - Frame)) +
			(m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl * Frame) +
			m_Part[i]->OffSetScl;

		// ���[�J����]�̑��
		D3DXMatrixRotationYawPitchRoll(&mtxPartRot,
			m_Model[i]->Rot.y,
			m_Model[i]->Rot.x,
			m_Model[i]->Rot.z);

		// ���[�J���X�P�[���̑��
		D3DXMatrixScaling(&mtxPartScl,
			m_Model[i]->Scl.x,
			m_Model[i]->Scl.y,
			m_Model[i]->Scl.z);

		// ���[�J�����W�̑��
		D3DXMatrixTranslation(&mtxPartPos,
			m_Model[i]->Pos.x,
			m_Model[i]->Pos.y,
			m_Model[i]->Pos.z);

		// ���[���h��񏈗�
		D3DXMatrixIdentity(&m_Model[i]->Matrix);                       // ���[���h�̒��g��������
		D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &mtxPartScl);   // ���[���h�X�P�[���̑��
		D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &mtxPartRot);   // ���[���h��]�̑��
		D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &mtxPartPos);   // ���[���h���W�̑��

		// �}�g���N�X�̊K�w������
		if (m_Part[i]->Parent == -1) {
			D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &m_Matrix);   // ���[���h���W�̑��
		}
		else {
			D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &m_Model[i]->pParent->Matrix);   // ���[���h���W�̑��
		}
	}
	// �t���[�����`�F�b�N
	if (m_Frame > m_Motion[m_MotionID]->KeyFrame[m_Key]->Frame)
	{
		// �t���[�����N���A
		m_Frame = 0;

		// ���̃L�[�t���[����
		m_Key++;

		// ���[�v���[�V����
		if (m_Key >= m_Motion[m_MotionID]->KeyFrame.size())
		{
			m_Key = 0;
		}
	}
}
// �u�����h���[�V�����̃}�g���N�X�Z�b�g����
void CSceneMotionPartsX::UpdateBlendMotion()
{
	// �ϊ��s��̐錾
	D3DXMATRIX mtxPartRot;             // ���[�J����]
	D3DXMATRIX mtxPartScl;             // ���[�J���X�P�[��
	D3DXMATRIX mtxPartPos;             // ���[�J�����W

	// �p�[�c�������[�v���ĕ�ԏ���
	for (int i = 0; i < m_Part.size(); i++)
	{
		float Frame = (float)m_Frame / (m_Motion[m_MotionID]->KeyFrame[m_Key]->Frame*0.5f);

		// ��ԗʂōX�V
		m_Model[i]->Pos = (m_BlendData[i]->Pos * (1.0f - Frame)) +
			(m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos * Frame) +
			m_Part[i]->OffSetPos;
		m_Model[i]->Rot = (m_BlendData[i]->Rot * (1.0f - Frame)) +
			(m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot * Frame) +
			m_Part[i]->OffSetRot;
		m_Model[i]->Scl = (m_BlendData[i]->Scl * (1.0f - Frame)) +
			(m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl * Frame) +
			m_Part[i]->OffSetScl;

		// ���[�J����]�̑��
		D3DXMatrixRotationYawPitchRoll(&mtxPartRot,
			m_Model[i]->Rot.y,
			m_Model[i]->Rot.x,
			m_Model[i]->Rot.z);

		// ���[�J���X�P�[���̑��
		D3DXMatrixScaling(&mtxPartScl,
			m_Model[i]->Scl.x,
			m_Model[i]->Scl.y,
			m_Model[i]->Scl.z);

		// ���[�J�����W�̑��
		D3DXMatrixTranslation(&mtxPartPos,
			m_Model[i]->Pos.x,
			m_Model[i]->Pos.y,
			m_Model[i]->Pos.z);

		// ���[���h��񏈗�
		D3DXMatrixIdentity(&m_Model[i]->Matrix);                       // ���[���h�̒��g��������
		D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &mtxPartScl);   // ���[���h�X�P�[���̑��
		D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &mtxPartRot);   // ���[���h��]�̑��
		D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &mtxPartPos);   // ���[���h���W�̑��

																					 // �}�g���N�X�̊K�w������
		if (m_Part[i]->Parent == -1) {
			D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &m_Matrix);   // ���[���h���W�̑��
		}
		else {
			D3DXMatrixMultiply(&m_Model[i]->Matrix, &m_Model[i]->Matrix, &m_Model[i]->pParent->Matrix);   // ���[���h���W�̑��
		}
	}
	// �t���[�����`�F�b�N
	if (m_Frame > m_Motion[m_MotionID]->KeyFrame[m_Key]->Frame*0.5f)
	{
		// �t���[�����N���A
		m_Frame = 0;

		// ���̃L�[�t���[����
		m_Key = 0;

		m_Blend = false;
	}
}

//=======================================================================================
//   �`�揈��
//=======================================================================================
void CSceneMotionPartsX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = NULL;                // �G���[�`�F�b�N�̂���NULL������
	pDevice = CManager::GetRenderer()->GetDevice();                           // �f�o�C�X�̃|�C���^���擾

	if (pDevice == NULL)                            // �G���[�`�F�b�N
	{
		MessageBox(NULL, "Init��pDevece��NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return;
	}
	// �S�p�[�c�����[�v
	for (int i = 0; i < m_Part.size(); i++) {

		// NULL�`�F�b�N
		if (m_Part[i]->Model->pBuffMat == NULL) { return; }

		D3DXMATERIAL*	pMat = NULL;					//  �}�e���A��

		// ���[���h���Z�b�g
		pDevice->SetTransform(D3DTS_WORLD, &m_Model[i]->Matrix);

		D3DMATERIAL9 matDef;
		pDevice->GetMaterial(&matDef);                 // ���݃f�o�C�X�ɐݒ肳��Ă�A�e���A�������擾

		pMat = (D3DXMATERIAL*)m_Part[i]->Model->pBuffMat->GetBufferPointer();

		for (int j = 0; j < (int)m_Part[i]->Model->NumMat; j++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[j].MatD3D);


			// �e�N�X�`���̕`��
			pDevice->SetTexture(0, m_Part[i]->Model->pTexture[j]);

			// ���b�V���̕`��
			m_Part[i]->Model->pMesh->DrawSubset(j);
			pDevice->SetMaterial(&matDef);
		}
	}
}

void CSceneMotionPartsX::SetMotion(int MotionID)
{
	if (!keyUpdata) return;
	if( m_MotionID == MotionID) return;

	for (int i = 0; i < m_Part.size(); i++) {
		m_BlendData[i]->Pos = m_Model[i]->Pos - m_Part[i]->OffSetPos;
		m_BlendData[i]->Rot = m_Model[i]->Rot - m_Part[i]->OffSetRot;
		m_BlendData[i]->Scl = m_Model[i]->Scl - m_Part[i]->OffSetScl;
	}
	m_Blend = true;

	m_MotionID = MotionID;
	m_Key = 0;
	m_Frame = 0;
	m_LastKye = false;
}

//=======================================================================================
//   �f�[�^�t�@�C���̓ǂݍ���
//=======================================================================================
void CSceneMotionPartsX::LoadDataFile()
{
	// �p�[�c�t�@�C���ǂݍ���
	char aWork[64];
	FILE* pFile = NULL;

	//  �t�@�C���̓ǂݍ���
	pFile = fopen("data/motion.txt", "r+");
	if (pFile == NULL)
	{
		//  �G���[�������ꍇ
		MessageBox(NULL, "�t�@�C�����ǂݍ��߂܂���ł����B", "�G���[���b�Z�[�W", MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		while (1)
		{
			fscanf(pFile, "%s", &aWork[0]);

			// �I��
			if (strcmp(aWork, "END_LOAD") == 0)
			{
				imGui_parts_name += '\0';
				break;
			}
			// ���I�m��
			parts_name.push_back(aWork);
			imGui_parts_name += aWork;
			imGui_parts_name += '\0';
		}
		//  �t�@�C�������
		fclose(pFile);
	}

	//  �t�@�C���̓ǂݍ���
	pFile = fopen("data/model.txt", "r+");
	if (pFile == NULL)
	{
		//  �G���[�������ꍇ
		MessageBox(NULL, "�t�@�C�����ǂݍ��߂܂���ł����B", "�G���[���b�Z�[�W", MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		model_name.push_back("\0");
		imGui_model_name = '\0';
		while (1)
		{
			fscanf(pFile, "%s", &aWork[0]);
			if (strcmp(aWork, "SET_PART") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aWork[0]);
					if (strcmp(aWork, "END_PART") == 0) { break; }
					model_name.push_back(aWork);
					imGui_model_name += aWork;
					imGui_model_name += '\0';
				}
			}
			else if (strcmp(aWork, "END_LOAD") == 0)
			{
				imGui_model_name += '\0';
				break;
			}
		}
		//  �t�@�C�������
		fclose(pFile);
	}
}

//=======================================================================================
//   �t�@�C���ǂݍ��ݏ���
//=======================================================================================
bool CSceneMotionPartsX::LoadMotionFile(char *FileName)
{
	char aWork[128];
	int CntParts = -1;
	int CntMotion = -1;
	int CntKey = -1;
	int CntKeyFrame = -1;
	int CntCollision = -1;
	int CntSound = -1;
	FILE* pFile = NULL;

	//  �t�@�C���̓ǂݍ���
	pFile = fopen(FileName, "r+");
	if (pFile == NULL)
	{
		//  �G���[�������ꍇ
		MessageBox(NULL, "�t�@�C�����ǂݍ��߂܂���ł����B", "�G���[���b�Z�[�W", MB_OK | MB_ICONWARNING);
		return false;
	}
	else
	{
		while (1)
		{
			// �����ǂݍ���
			fscanf(pFile, "%s", &aWork);

			// �����`�F�b�N
			//  ���[�h�J�n
			if (strcmp(aWork, "LOAD_START") == 0) {
				continue;
			}

			// �I�t�Z�b�g���
			else if (strcmp(aWork, "SET_OFFSET") == 0) {
				while (1)
				{
					fscanf(pFile, "%s", &aWork);
					// �I�t�Z�b�g�I��
					if (strcmp(aWork, "END_OFFSET") == 0) {
						break;
					}
					else if (strcmp(aWork, "FILENAME") == 0) {
						// ���I�m��
						m_Part.push_back(new PART);
						m_Model.push_back(new MODEL);
						CntParts++;

						char Path[64];
						fscanf(pFile, "%s", &Path[0]);
						m_Part[CntParts]->Model = CModelX::GetModelX(Path);
						int jiji = 3456;
					}
					else if (strcmp(aWork, "PARENT") == 0) {
						fscanf(pFile, "%d", &m_Part[CntParts]->Parent);
					}
					// ���W
					else if (strcmp(aWork, "POS") == 0) {
							fscanf(pFile, "%f %f %f",
								&m_Part[CntParts]->OffSetPos.x,
								&m_Part[CntParts]->OffSetPos.y,
								&m_Part[CntParts]->OffSetPos.z);
					}
					// ��]
					else if (strcmp(aWork, "ROT") == 0) {
						fscanf(pFile, "%f %f %f",
							&m_Part[CntParts]->OffSetRot.x,
							&m_Part[CntParts]->OffSetRot.y,
							&m_Part[CntParts]->OffSetRot.z);
					}
					// �X�P�[��
					else if (strcmp(aWork, "SCL") == 0) {
						fscanf(pFile, "%f %f %f",
							&m_Part[CntParts]->OffSetScl.x,
							&m_Part[CntParts]->OffSetScl.y,
							&m_Part[CntParts]->OffSetScl.z);

					}
				}
			}
			// ���[�V�������
			else if (strcmp(aWork, "SET_MOTION") == 0) {

				while (1)
				{
					fscanf(pFile, "%s", &aWork);
					// ���[�v�t���O
					if (strcmp(aWork, "SET_KEY_FRAME") == 0) {

						// �m��
						m_Motion.push_back(new MOTION);
						CntMotion++;
						CntKeyFrame = -1;
					}
					// �T�E���h
					else if (strcmp(aWork, "SET_SOUND") == 0)
					{
						while (1)
						{
							fscanf(pFile, "%s", &aWork);
							if (strcmp(aWork, "ID") == 0) {
								m_Motion[CntMotion]->Sound.push_back(new SOUND);
								CntSound++;
								fscanf(pFile, "%d", &m_Motion[CntMotion]->Sound[CntSound]->ID);
							}
							else if (strcmp(aWork, "FRAME") == 0) {
								fscanf(pFile, "%d", &m_Motion[CntMotion]->Sound[CntSound]->Frame);
							}
							else if (strcmp(aWork, "END_SOUND") == 0) {
								CntSound = -1;
								break;
							}
						}
					}
					// �����蔻��
					else if (strcmp(aWork, "SET_COLLISION") == 0)
					{
						while (1)
						{
							fscanf(pFile, "%s", &aWork);
							// 1�̃��[�V�����ǂݍ��ݏI��
							if (strcmp(aWork, "PARTS") == 0) {
								m_Motion[CntMotion]->Collision.push_back(new COLLISION_DATA);
								m_Model[CntMotion]->Collision.push_back(new COLLISION_SPHERE);
								CntCollision++;
								fscanf(pFile, "%d", &m_Motion[CntMotion]->Collision[CntCollision]->PartID);
							}
							else if (strcmp(aWork, "POS") == 0) {
								fscanf(pFile, "%f %f %f\n", &m_Motion[CntMotion]->Collision[CntCollision]->Pos.x,
									&m_Motion[CntMotion]->Collision[CntCollision]->Pos.y,
									&m_Motion[CntMotion]->Collision[CntCollision]->Pos.z);
							}
							else if (strcmp(aWork, "SCL") == 0) {
								fscanf(pFile, "%f", &m_Motion[CntMotion]->Collision[CntCollision]->Scl);
							}
							else if (strcmp(aWork, "START") == 0) {
								fscanf(pFile, "%d", &m_Motion[CntMotion]->Collision[CntCollision]->StartFrame);
							}
							else if (strcmp(aWork, "END") == 0) {
								fscanf(pFile, "%d", &m_Motion[CntMotion]->Collision[CntCollision]->EndFrame);
							}
							else if (strcmp(aWork, "END_COLLISION") == 0) {
								CntCollision = -1;
								break;
							}
						}
					}
					// �t���[����
					else if (strcmp(aWork, "FRAME") == 0)
					{
						// �L�[�t���[���J�E���g
						CntKeyFrame++;
						// �L�[�J�E���^�̏�����
						CntKey = -1;

						// �m��
						m_Motion[CntMotion]->KeyFrame.push_back(new KEY_FRAME);
						fscanf(pFile, "%d", &m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Frame);
					}
					// ���W
					else if (strcmp(aWork, "POS") == 0)
					{
						// �m��
						m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key.push_back(new KEY);
						// �L�[�J�E���g
						CntKey++;

						fscanf(pFile, "%f %f %f\n", &m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key[CntKey]->Pos.x,
							&m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key[CntKey]->Pos.y,
							&m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key[CntKey]->Pos.z);
					}
					// ��]
					else if (strcmp(aWork, "ROT") == 0)
					{
						fscanf(pFile, "%f %f %f\n", &m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key[CntKey]->Rot.x,
							&m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key[CntKey]->Rot.y,
							&m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key[CntKey]->Rot.z);
					}
					// �X�P�[��
					else if (strcmp(aWork, "SCL") == 0)
					{
						fscanf(pFile, "%f %f %f\n", &m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key[CntKey]->Scl.x,
							&m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key[CntKey]->Scl.y,
							&m_Motion[CntMotion]->KeyFrame[CntKeyFrame]->Key[CntKey]->Scl.z);
					}
					// 1�̃��[�V�����ǂݍ��ݏI��
					else if (strcmp(aWork, "END_MOTION") == 0) {
						break;
					}
				}
			}
			// �I��
			else if (strcmp(aWork, "END_LOAD") == 0) {
				break;
			}
		}
		//  �t�@�C�������
		fclose(pFile);
	}
	return true;
}

void CSceneMotionPartsX::ChangeFile(char *FileName)
{
	FILE* pFile = NULL;

	//  �t�@�C���̓ǂݍ���
	pFile = fopen(FileName, "r+");
	if (pFile == NULL)
	{
		//  �G���[�������ꍇ
		MessageBox(NULL, "�t�@�C�����Ȃ��I�iChangeFile�j", "�G���[���b�Z�[�W", MB_OK | MB_ICONWARNING);
		return;
	}
	//  �t�@�C�������
	fclose(pFile);


	Uninit();

	// �t�@�C���ǂݍ���
	LoadMotionFile(FileName);

	// �f�[�^���
	for (int i = 0; i < m_Part.size(); i++)
	{
		m_SaveData.push_back(new VECTR3);
		m_BlendData.push_back(new VECTR3);
		// �K�w����
		if (i == 0)
		{
			m_Model[i]->pParent = NULL;
		}
		else
		{
			m_Model[i]->pParent = m_Model[m_Part[i]->Parent];
		}

		// �f�[�^���
		m_Model[i]->Pos = m_Part[i]->OffSetPos + m_Motion[0]->KeyFrame[0]->Key[i]->Pos;
		m_Model[i]->Rot = m_Part[i]->OffSetRot + m_Motion[0]->KeyFrame[0]->Key[i]->Rot;
		m_Model[i]->Scl = m_Part[i]->OffSetScl + m_Motion[0]->KeyFrame[0]->Key[i]->Scl;

		m_SaveData[i]->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_SaveData[i]->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_SaveData[i]->Scl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		m_BlendData[i]->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_BlendData[i]->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_BlendData[i]->Scl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_LastKye = false;
	m_MotionID = 0;
	m_Frame = 0;
	m_Key = 0;

	motionMode = false;
	offset_window = false;
	model_window = false;
	all_motion_window = false;
	motion_window = false;
}

//=======================================================================================
//   ���f���ǉ�����
//=======================================================================================
void CSceneMotionPartsX::CreateModel(void)
{
	m_Part.push_back(new PART);
	m_Model.push_back(new MODEL);
	m_Part.back()->Model = CModelX::GetModelX( "data/MODEL/block00.x");
	m_Part.back()->OffSetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Part.back()->OffSetRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Part.back()->OffSetScl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Part.back()->Parent = 0;
	m_Model.back()->pParent = m_Model[m_Part.back()->Parent];
	m_Model.back()->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Model.back()->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Model.back()->Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	KeyAdd();
}

//=======================================================================================
//   ���f���j������
//=======================================================================================
void CSceneMotionPartsX::DelModel()
{
	m_Part.pop_back();

	m_Model.pop_back();

	KeySub();
}

//=======================================================================================
//   ���f���j������
//=======================================================================================
void CSceneMotionPartsX::ChangeModel(PART *pPart, char *FileName)
{
	pPart->Model = CModelX::GetModelX(FileName);
}

//=======================================================================================
//   �����o������
//=======================================================================================
void CSceneMotionPartsX::SaveMotion(char *FileName)
{
	// �����o������
	FILE *pFile;

	//  �t�@�C���̓ǂݍ���
	pFile = fopen(FileName, "r+");
	if (pFile != NULL)
	{
		UINT nID = MessageBox(NULL, "�����t�@�C�������ɑ��݂��܂��B\n�㏑�����܂����H", "�㏑��", MB_YESNO | MB_DEFBUTTON2);
		//  �t�@�C�������
		fclose(pFile);
		if (nID == IDNO) return;
	}

	fopen_s(&pFile, FileName, "w");

	// NULL�`�F�b�N
	if (pFile != NULL)
	{
		// �Z�[�u�J�n
		fprintf(pFile, "LOAD_START\n\n");

		// ���f���̃I�t�Z�b�g���
		fprintf(pFile, "�I�t�Z�b�g���\n");
		fprintf(pFile, "SET_OFFSET\n");
		for (int i = 0; i < m_Part.size(); ++i)
		{
			fprintf(pFile, "	FILENAME %s\n", m_Part[i]->Model->Name);
			fprintf(pFile, "	PARENT %d\n", m_Part[i]->Parent);
			fprintf(pFile, "	POS %.2f %.2f %.2f\n"  , m_Part[i]->OffSetPos.x, m_Part[i]->OffSetPos.y, m_Part[i]->OffSetPos.z);
			fprintf(pFile, "	ROT %.2f %.2f %.2f\n"  , m_Part[i]->OffSetRot.x, m_Part[i]->OffSetRot.y, m_Part[i]->OffSetRot.z);
			fprintf(pFile, "	SCL %.2f %.2f %.2f\n", m_Part[i]->OffSetScl.x, m_Part[i]->OffSetScl.y, m_Part[i]->OffSetScl.z);
			if (i < m_Part.size()-1) { fprintf(pFile, "\n"); }
		}
		fprintf(pFile, "END_OFFSET\n\n");

		// ���[�V�������
		fprintf(pFile, "���[�V�������\n");
		fprintf(pFile, "SET_MOTION\n");
		for (int i = 0; i < m_Motion.size(); ++i)
		{
			fprintf(pFile, "	SET_KEY_FRAME\n");
			if (!m_Motion[i]->Sound.empty())
			{
				fprintf(pFile, "		SET_SOUND\n");
				for (int j = 0; j < m_Motion[i]->Sound.size(); j++)
				{
					fprintf(pFile, "			ID    %d\n", m_Motion[i]->Sound[j]->ID);
					fprintf(pFile, "			FRAME %d\n", m_Motion[i]->Sound[j]->Frame);
					if (j < m_Motion[i]->Sound.size() - 1) { fprintf(pFile, "\n"); }
				}
				fprintf(pFile, "		END_SOUND\n\n");
			}

			if (!m_Motion[i]->Collision.empty())
			{
				fprintf(pFile, "		SET_COLLISION\n");
				for (int j = 0; j < m_Motion[i]->Collision.size(); j++)
				{
					fprintf(pFile, "			PARTS %d\n", m_Motion[i]->Collision[j]->PartID);
					fprintf(pFile, "			POS %.2f %.2f %.2f\n", m_Motion[i]->Collision[j]->Pos.x, m_Motion[i]->Collision[j]->Pos.y, m_Motion[i]->Collision[j]->Pos.z);
					fprintf(pFile, "			SCL %.2f\n", m_Motion[i]->Collision[j]->Scl);
					fprintf(pFile, "			START %d\n", m_Motion[i]->Collision[j]->StartFrame);
					fprintf(pFile, "			END %d\n", m_Motion[i]->Collision[j]->EndFrame);
					if (j < m_Motion[i]->Collision.size() - 1) { fprintf(pFile, "\n"); }
				}
				fprintf(pFile, "		END_COLLISION\n\n");
			}
			for (int j = 0; j < m_Motion[i]->KeyFrame.size(); ++j)
			{
				fprintf(pFile, "		FRAME %d\n", m_Motion[i]->KeyFrame[j]->Frame);

				for (int k = 0; k < m_Motion[i]->KeyFrame[j]->Key.size(); ++k)
				{
					fprintf(pFile, "			POS %.2f %.2f %.2f\n", m_Motion[i]->KeyFrame[j]->Key[k]->Pos.x, m_Motion[i]->KeyFrame[j]->Key[k]->Pos.y, m_Motion[i]->KeyFrame[j]->Key[k]->Pos.z);
					fprintf(pFile, "			ROT %.2f %.2f %.2f\n", m_Motion[i]->KeyFrame[j]->Key[k]->Rot.x, m_Motion[i]->KeyFrame[j]->Key[k]->Rot.y, m_Motion[i]->KeyFrame[j]->Key[k]->Rot.z);
					fprintf(pFile, "			SCL %.2f %.2f %.2f\n", m_Motion[i]->KeyFrame[j]->Key[k]->Scl.x, m_Motion[i]->KeyFrame[j]->Key[k]->Scl.y, m_Motion[i]->KeyFrame[j]->Key[k]->Scl.z);
					if (k < m_Motion[i]->KeyFrame[j]->Key.size() - 1) { fprintf(pFile, "\n"); }
				}
			}
		}
		fprintf(pFile, "END_MOTION\n");

		// �Z�[�u�I��
		fprintf(pFile, "END_LOAD");
		fclose(pFile);				// �t�@�C����
		MessageBox(NULL, "�ۑ����܂����B", "�I��", MB_DEFBUTTON2);
	}
	else
	{
		assert(!"�����o���G���[�I");          // �G���[���b�Z�[�W
	}
}

//=======================================================================================
//   ���[�V�����𑝌�����
//=======================================================================================
void CSceneMotionPartsX::MotionAdd()
{
	m_Motion.push_back(new MOTION);
	for (int i = 0; i < m_Motion[0]->KeyFrame.size(); i++)
	{
		m_Motion.back()->KeyFrame.push_back(new KEY_FRAME);
		m_Motion.back()->KeyFrame[i]->Frame = 30;
		for (int j = 0; j < m_Part.size(); j++)
		{
			m_Motion.back()->KeyFrame[i]->Key.push_back(new KEY);
			m_Motion.back()->KeyFrame[i]->Key[j]->Pos.x = 0.0f;
			m_Motion.back()->KeyFrame[i]->Key[j]->Pos.y = 0.0f;
			m_Motion.back()->KeyFrame[i]->Key[j]->Pos.z = 0.0f;

			m_Motion.back()->KeyFrame[i]->Key[j]->Rot.x = 0.0f;
			m_Motion.back()->KeyFrame[i]->Key[j]->Rot.y = 0.0f;
			m_Motion.back()->KeyFrame[i]->Key[j]->Rot.z = 0.0f;

			m_Motion.back()->KeyFrame[i]->Key[j]->Scl.x = 0.0f;
			m_Motion.back()->KeyFrame[i]->Key[j]->Scl.y = 0.0f;
			m_Motion.back()->KeyFrame[i]->Key[j]->Scl.z = 0.0f;
		}
	}
}
void CSceneMotionPartsX::MotionSub()
{
	if (m_Motion.size() <= 1)
		return;

	m_Motion.pop_back();

	if (m_MotionID >= m_Motion.size())
		m_MotionID--;
}

//=======================================================================================
//   �L�[�t���[���𑝌�����
//=======================================================================================
void CSceneMotionPartsX::KeyFrameAdd()
{
	m_Motion[m_MotionID]->KeyFrame.push_back(new KEY_FRAME);
	m_Motion[m_MotionID]->KeyFrame.back()->Frame = 30;
	for (int i = 0; i < m_Part.size(); i++)
	{
		m_Motion[m_MotionID]->KeyFrame.back()->Key.push_back(new KEY);
		m_Motion[m_MotionID]->KeyFrame.back()->Key[i]->Pos.x = 0.0f;
		m_Motion[m_MotionID]->KeyFrame.back()->Key[i]->Pos.y = 0.0f;
		m_Motion[m_MotionID]->KeyFrame.back()->Key[i]->Pos.z = 0.0f;

		m_Motion[m_MotionID]->KeyFrame.back()->Key[i]->Rot.x = 0.0f;
		m_Motion[m_MotionID]->KeyFrame.back()->Key[i]->Rot.y = 0.0f;
		m_Motion[m_MotionID]->KeyFrame.back()->Key[i]->Rot.z = 0.0f;

		m_Motion[m_MotionID]->KeyFrame.back()->Key[i]->Scl.x = 0.0f;
		m_Motion[m_MotionID]->KeyFrame.back()->Key[i]->Scl.y = 0.0f;
		m_Motion[m_MotionID]->KeyFrame.back()->Key[i]->Scl.z = 0.0f;
	}
}
void CSceneMotionPartsX::KeyFrameSub()
{
	if (m_Motion[m_MotionID]->KeyFrame.size() <= 1)
		return;

	m_Motion[m_MotionID]->KeyFrame.pop_back();

	if (m_Key >= m_Motion[m_MotionID]->KeyFrame.size())
		m_Key--;
}

//=======================================================================================
//   �L�[�𑝌�����
//=======================================================================================
void CSceneMotionPartsX::KeyAdd()
{
	for (int i = 0; i < m_Motion.size(); i++)
	{
		for (int j = 0; j < m_Motion[i]->KeyFrame.size(); j++)
		{
			m_Motion[i]->KeyFrame[j]->Key.push_back(new KEY);
			m_Motion[i]->KeyFrame[j]->Key.back()->Pos.x = 0.0f;
			m_Motion[i]->KeyFrame[j]->Key.back()->Pos.y = 0.0f;
			m_Motion[i]->KeyFrame[j]->Key.back()->Pos.z = 0.0f;

			m_Motion[i]->KeyFrame[j]->Key.back()->Rot.x = 0.0f;
			m_Motion[i]->KeyFrame[j]->Key.back()->Rot.y = 0.0f;
			m_Motion[i]->KeyFrame[j]->Key.back()->Rot.z = 0.0f;

			m_Motion[i]->KeyFrame[j]->Key.back()->Scl.x = 0.0f;
			m_Motion[i]->KeyFrame[j]->Key.back()->Scl.y = 0.0f;
			m_Motion[i]->KeyFrame[j]->Key.back()->Scl.z = 0.0f;
		}
	}
	m_SaveData.push_back(new VECTR3);
	m_SaveData.back()->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_SaveData.back()->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_SaveData.back()->Scl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_BlendData.push_back(new VECTR3);
	m_BlendData.back()->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_BlendData.back()->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_BlendData.back()->Scl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
void CSceneMotionPartsX::KeySub()
{
	for (int i = 0; i < m_Motion.size(); i++)
	{
		for (int j = 0; j < m_Motion[i]->KeyFrame.size(); j++)
		{
			m_Motion[i]->KeyFrame[j]->Key.pop_back();
		}
	}
	m_SaveData.pop_back();
	m_BlendData.pop_back();
}

//=======================================================================================
//   �����蔻��𑝌�����
//=======================================================================================
void CSceneMotionPartsX::CollisionAdd(int MotionID)
{
	m_Motion[MotionID]->Collision.push_back(new COLLISION_DATA);
	m_Motion[MotionID]->Collision.back()->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Motion[MotionID]->Collision.back()->Scl = 1.0f;
	m_Motion[MotionID]->Collision.back()->PartID = 0;
	m_Motion[MotionID]->Collision.back()->StartFrame = 0;
	m_Motion[MotionID]->Collision.back()->EndFrame = 60;

	m_Model[MotionID]->Collision.push_back(new COLLISION_SPHERE);
	m_Model[MotionID]->Collision.back()->Pos = m_Motion[MotionID]->Collision.back()->Pos;
	m_Model[MotionID]->Collision.back()->Scl = m_Motion[MotionID]->Collision.back()->Scl;
	m_Model[MotionID]->Collision.back()->Use = false;
	m_Model[MotionID]->Collision.back()->Sphere = CMeshSphere::Create(m_Model[MotionID]->Collision.back()->Pos, m_Model[MotionID]->Collision.back()->Scl, 10, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
}
void CSceneMotionPartsX::CollisionSub(int MotionID)
{
	if (!m_Motion[MotionID]->Collision.empty())
	{
		m_Motion[MotionID]->Collision.pop_back();
		m_Model[MotionID]->Collision.pop_back();
	}
}

//=======================================================================================
//   ���ʉ��𑝌�����
//=======================================================================================
void CSceneMotionPartsX::SoundAdd()
{
	m_Motion[m_MotionID]->Sound.push_back(new SOUND);
	m_Motion[m_MotionID]->Sound.back()->Frame = -1;
	m_Motion[m_MotionID]->Sound.back()->ID = CSound::SE_SELECT000;
}
void CSceneMotionPartsX::SoundSub()
{
	if (!m_Motion[m_MotionID]->Sound.empty())
	{
		CManager::GetSound()->Stop((CSound::SOUND_LABEL)m_Motion[m_MotionID]->Sound.back()->ID);
		m_Motion[m_MotionID]->Sound.pop_back();
	}
}

//=======================================================================================
//   �f�o�b�O�\���iimgui�j
//=======================================================================================
void CSceneMotionPartsX::ImGui(void)
{
	static char FileName_Save[128] = "data/text.txt";
	int MotionIDOld = m_MotionID;

	// ���C�����j���[
	ImGui::Checkbox("Updata Key", &keyUpdata);
	ImGui::Checkbox("Updata Motion", &motionMode);
	ImGui::Checkbox("    Model  Window", &model_window);
	ImGui::Checkbox("    OffSet Window", &offset_window);
	ImGui::Checkbox("All Motion Window", &all_motion_window);
	ImGui::Checkbox(" Save Data Window", &save_data_window);

	if (offset_window)
	{
		motionMode = false;
		model_window = false;
		all_motion_window = false;
		motion_window = false;
	}
	// ���[�V�����ݒ�
	if (ImGui::CollapsingHeader("Motion Data"))
	{
		ImGui::Checkbox("    Motion Window", &motion_window);
		ImGui::Checkbox(" Collision Window", &collision_window);
		bool check = sound_window;
		ImGui::Checkbox("     Sound Window", &sound_window);
		if (check != sound_window) { CManager::GetSound()->Stop(); }
		ImGui::SliderInt("MotionID", &m_MotionID, 0, m_Motion.size() - 1);
		if (ImGui::Button("Motion+"))  MotionAdd();
		if (ImGui::Button("Motion-"))  MotionSub();
		if (m_MotionID != MotionIDOld)
		{
			m_Key = 0;
			m_Frame = 0;
		}
		ImGui::SliderInt("KeyID", &m_Key, 0, m_Motion[m_MotionID]->KeyFrame.size()-1);
		if (ImGui::Button("Key+"))  KeyFrameAdd();
		if (ImGui::Button("Key-"))  KeyFrameSub();
		ImGui::SliderInt("Frame", &m_Frame, 0, m_Motion[m_MotionID]->KeyFrame[m_Key]->Frame);
		ImGui::Text("MotionFrame %d / %d", m_MotionFrame, m_MotionFrameAll);
	}

	// �Z�[�u����
	if (ImGui::CollapsingHeader("Save"))
	{
		ImGui::InputText("FilePath", FileName_Save, sizeof(FileName_Save));
		if (ImGui::Button("SaveFile")) SaveMotion(FileName_Save);
	}

	// �t�@�C���ύX
	if (ImGui::CollapsingHeader("Change"))
	{
		ImGui::Combo("FilePath", &parts_id, imGui_parts_name.c_str(), IM_ARRAYSIZE(imGui_parts_name.c_str()));   // Combo using proper array. You can also pass a callback to retrieve array value, no need to create/copy an array just for that.
		if (ImGui::Button("ChangeFile")) ChangeFile((char*)parts_name[parts_id].c_str());
	}

	// ���f���p�[�c�E�B���h�E
	if (model_window) imGuiModel();

	// �I�t�Z�b�g�ݒ�
	if (offset_window) imGuiOffset();

	// ���[�V�����E�B���h�E
	if (all_motion_window) imGuiAllMotion();

	// �����蔻��E�B���h�E
	if (collision_window) imGuiCollision();

	// �T�E���h�E�B���h�E
	if (sound_window) imGuiSound();

	// ���݂̃��[�V�����E�B���h�E
	if (motion_window) imGuiMotion();

	// �ꎞ�ۑ��f�[�^�E�B���h�E
	if (save_data_window)
	{
		ImGui::SetNextWindowPos(ImVec2(350, 20), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Save Data Window", &model_window);

		// �o�[�c�̏��
		for (int i = 0; i < m_Part.size(); i++) {

				ImGui::Text("pos%d( %.3f, %.3f, %.3f)",i, m_SaveData[i]->Pos.x, m_SaveData[i]->Pos.y, m_SaveData[i]->Pos.z);
				ImGui::Text("rot%d( %.3f, %.3f, %.3f)",i, m_SaveData[i]->Rot.x, m_SaveData[i]->Rot.y, m_SaveData[i]->Rot.z);
				ImGui::Text("scl%d( %.3f, %.3f, %.3f)",i, m_SaveData[i]->Scl.x, m_SaveData[i]->Scl.y, m_SaveData[i]->Scl.z);
				ImGui::Text(" ");
		}
		ImGui::End();
	}
}

void CSceneMotionPartsX::imGuiModel()
{
	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Model Window", &model_window);
	char BoxName[16];
	// �I�t�Z�b�g�L�[���ۑ�
	if (ImGui::Button("SaveKey"))
	{
		for (int i = 0; i < m_Part.size(); i++) {
			m_SaveData[i]->Pos = m_Part[i]->OffSetPos;
			m_SaveData[i]->Rot = m_Part[i]->OffSetRot;
			m_SaveData[i]->Scl = m_Part[i]->OffSetScl;
		}
	}

	// �o�[�c�̏��
	ImGui::Text("Parts Num %d", m_Part.size());
	for (int i = 0; i < m_Part.size(); i++) {
		ImGui::Text("Index  %d(%s)", i, m_Part[i]->Model->Name);
		ImGui::Text("Parent %d", m_Part[i]->Parent);
		sprintf(BoxName, "pos %d", i);
		ImGui::DragFloat3(BoxName, &m_Model[i]->Pos[0], 0.01f);
		sprintf(BoxName, "rot %d", i);
		ImGui::DragFloat3(BoxName, &m_Model[i]->Rot[0], 0.01f);
		sprintf(BoxName, "scl %d", i);
		ImGui::DragFloat3(BoxName, &m_Model[i]->Scl[0], 0.01f);
	}
	ImGui::End();
}
void CSceneMotionPartsX::imGuiOffset()
{
	char BoxName[16];
	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Model Window", &offset_window);
	ImGui::InputFloat("Value", &value, 0.001f);

	// �I�t�Z�b�g�L�[���ۑ�
	if (ImGui::Button("SaveKey"))
	{
		for (int i = 0; i < m_Part.size(); i++) {
			m_SaveData[i]->Pos = m_Part[i]->OffSetPos;
			m_SaveData[i]->Rot = m_Part[i]->OffSetRot;
			m_SaveData[i]->Scl = m_Part[i]->OffSetScl;
		}
	}
	// �I�t�Z�b�g�L�[���ۑ�
	if (ImGui::Button("LoadKey"))
	{
		for (int i = 0; i < m_Part.size(); i++) {
			m_Part[i]->OffSetPos = m_SaveData[i]->Pos;
			m_Part[i]->OffSetRot = m_SaveData[i]->Rot;
			m_Part[i]->OffSetScl = m_SaveData[i]->Scl;
		}
	}

	if (ImGui::Button("Add Model"))
	{
		CreateModel();
	}
	if (ImGui::Button("Delete Model"))
	{
		if (m_Part.size() > 1)
		{
			DelModel();
		}
	}

	// �o�[�c�̏��
	ImGui::Text("Parts Num %d", m_Part.size());
	for (int i = 0; i < m_Part.size(); i++) {

		// �C���f�b�N�X
		sprintf(BoxName, "PartsID %d", i);
		if (ImGui::TreeNode(BoxName))
		{
			// �p�[�c���f���̕ύX
			sprintf(BoxName, "File %d", i);
			std::string imGuiModelName = m_Part[i]->Model->Name + imGui_model_name;
			ImGui::Combo(BoxName, &model_id, imGuiModelName.c_str(), 10);
			if (model_id != 0)
			{
				char* name = new char[model_name[model_id].size() + 1]; // �������m��
				std::strcpy(name, model_name[model_id].c_str());        // �R�s�[
				ChangeModel(m_Part[i], name);
				model_id = 0;
				delete[] name; // ���
			}

			// �e���ύX
			if (m_Part[i]->Parent < 0)
			{
				ImGui::Text("Parent %d", m_Part[i]->Parent);
			}
			else
			{
				sprintf(BoxName, "Parent %d", i);
				ImGui::InputInt(BoxName, &m_Part[i]->Parent);
				if (m_Part[i]->Parent < 0)
				{
					m_Part[i]->Parent = 0;
				}
				if (m_Part[i]->Parent > m_Part.size()-1)
				{
					m_Part[i]->Parent = m_Part.size() - 1;
				}
				m_Model[i]->pParent = m_Model[m_Part[i]->Parent];
			}

			sprintf(BoxName, "POS %d", i);
			if (ImGui::TreeNode(BoxName))
			{
				sprintf(BoxName, "posX  %d", i);
				ImGui::InputFloat(BoxName, &m_Part[i]->OffSetPos.x, value);
				sprintf(BoxName, " posX %d", i);
				ImGui::DragFloat(BoxName, &m_Part[i]->OffSetPos.x, value);

				sprintf(BoxName, "posY  %d", i);
				ImGui::InputFloat(BoxName, &m_Part[i]->OffSetPos.y, value);
				sprintf(BoxName, " posY %d", i);
				ImGui::DragFloat(BoxName, &m_Part[i]->OffSetPos.y, value);

				sprintf(BoxName, "posZ  %d", i);
				ImGui::InputFloat(BoxName, &m_Part[i]->OffSetPos.z, value);
				sprintf(BoxName, " posZ %d", i);
				ImGui::DragFloat(BoxName, &m_Part[i]->OffSetPos.z, value);
				ImGui::TreePop();
			}

			sprintf(BoxName, "ROT %d", i);
			if (ImGui::TreeNode(BoxName))
			{
				sprintf(BoxName, "rotX  %d", i);
				ImGui::InputFloat(BoxName, &m_Part[i]->OffSetRot.x, value);
				sprintf(BoxName, " rotX %d", i);
				ImGui::DragFloat(BoxName, &m_Part[i]->OffSetRot.x, value);

				sprintf(BoxName, "rotY  %d", i);
				ImGui::InputFloat(BoxName, &m_Part[i]->OffSetRot.y, value);
				sprintf(BoxName, " rotY %d", i);
				ImGui::DragFloat(BoxName, &m_Part[i]->OffSetRot.y, value);

				sprintf(BoxName, "rotZ  %d", i);
				ImGui::InputFloat(BoxName, &m_Part[i]->OffSetRot.z, value);
				sprintf(BoxName, " rotZ %d", i);
				ImGui::DragFloat(BoxName, &m_Part[i]->OffSetRot.y, value);
				ImGui::TreePop();
			}

			sprintf(BoxName, "SCL %d", i);
			if (ImGui::TreeNode(BoxName))
			{
				ImGui::InputFloat("Scl  ALL", &scl_all, value);
				ImGui::DragFloat(" Scl ALL", &scl_all, value);
				m_Part[i]->OffSetScl.x += scl_all;
				m_Part[i]->OffSetScl.y += scl_all;
				m_Part[i]->OffSetScl.z += scl_all;
				scl_all = 0.0f;
				sprintf(BoxName, "sclX  %d", i);
				ImGui::InputFloat(BoxName, &m_Part[i]->OffSetScl.x, value);
				sprintf(BoxName, " sclX %d", i);
				ImGui::DragFloat(BoxName, &m_Part[i]->OffSetScl.x, value);

				sprintf(BoxName, "sclY  %d", i);
				ImGui::InputFloat(BoxName, &m_Part[i]->OffSetScl.y, value);
				sprintf(BoxName, " sclY %d", i);
				ImGui::DragFloat(BoxName, &m_Part[i]->OffSetScl.y, value);

				sprintf(BoxName, "sclZ  %d", i);
				ImGui::InputFloat(BoxName, &m_Part[i]->OffSetScl.z, value);
				sprintf(BoxName, " sclZ %d", i);
				ImGui::DragFloat(BoxName, &m_Part[i]->OffSetScl.z, value);

				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
}
void CSceneMotionPartsX::imGuiAllMotion()
{
	char BoxName[16];
	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Motion Window", &model_window);
	ImGui::InputFloat("Value", &value, 0.001f);

	// �p�[�c���\��
	ImGui::Text("Parts Num %d", m_Part.size());

	// �L�[�t���[�����ۑ�
	if (ImGui::Button("SaveKey"))
	{
		for (int i = 0; i < m_Part.size(); i++) {
			m_SaveData[i]->Pos = m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos;
			m_SaveData[i]->Rot = m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot;
			m_SaveData[i]->Scl = m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl;
		}
	}
	// �L�[�t���[�����Ǎ�
	if (ImGui::Button("LoadKey"))
	{
		for (int i = 0; i < m_Part.size(); i++) {
			m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos = m_SaveData[i]->Pos;
			m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot = m_SaveData[i]->Rot;
			m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl = m_SaveData[i]->Scl;
		}
	}

	// �e���[�V�������
	for (int i = 0; i < m_Motion.size(); i++)
	{
		sprintf(BoxName, "MotionID %d", i);
		if (ImGui::CollapsingHeader(BoxName))
		{
			// �e�L�[�t���[�����
			for (int j = 0; j < m_Motion[i]->KeyFrame.size(); j++)
			{
				sprintf(BoxName, "KeyID %d", j);
				if (ImGui::TreeNode(BoxName))
				{
					sprintf(BoxName, "Frame %d", j);
					ImGui::DragInt(BoxName, &m_Motion[i]->KeyFrame[j]->Frame);

					// �e�p�[�c���
					for (int k = 0; k < m_Part.size(); k++) {
						ImGui::Text("Num %d(%s)", k, m_Part[k]->Model->Name);

						sprintf(BoxName, "POS %d-%d", j, k);
						if (ImGui::TreeNode(BoxName))
						{
							sprintf(BoxName, "posX  %d-%d", j, k);
							ImGui::InputFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Pos.x, value);
							sprintf(BoxName, " posX %d-%d", j, k);
							ImGui::DragFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Pos.x, value);

							sprintf(BoxName, "posY  %d-%d", j, k);
							ImGui::InputFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Pos.y, value);
							sprintf(BoxName, " posY %d-%d", j, k);
							ImGui::DragFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Pos.y, value);

							sprintf(BoxName, "posZ  %d-%d", j, k);
							ImGui::InputFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Pos.z, value);
							sprintf(BoxName, " posZ %d-%d", j, k);
							ImGui::DragFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Pos.z, value);
							ImGui::TreePop();
						}

						sprintf(BoxName, "ROT %d-%d", j, k);
						if (ImGui::TreeNode(BoxName))
						{
							sprintf(BoxName, "rotX  %d-%d", j, k);
							ImGui::InputFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Rot.x, value);
							sprintf(BoxName, " rotX %d-%d", j, k);
							ImGui::DragFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Rot.x, value);

							sprintf(BoxName, "rotY  %d-%d", j, k);
							ImGui::InputFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Rot.y, value);
							sprintf(BoxName, " rotY %d-%d", j, k);
							ImGui::DragFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Rot.y, value);

							sprintf(BoxName, "rotZ  %d-%d", j, k);
							ImGui::InputFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Rot.z, value);
							sprintf(BoxName, " rotZ %d-%d", j, k);
							ImGui::DragFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Rot.z, value);
							ImGui::TreePop();
						}

						sprintf(BoxName, "SCL %d-%d", j, k);
						if (ImGui::TreeNode(BoxName))
						{
							ImGui::InputFloat("Scl  ALL", &scl_all, value);
							ImGui::DragFloat(" Scl ALL", &scl_all, value);
							m_Motion[i]->KeyFrame[j]->Key[k]->Scl.x += scl_all;
							m_Motion[i]->KeyFrame[j]->Key[k]->Scl.y += scl_all;
							m_Motion[i]->KeyFrame[j]->Key[k]->Scl.z += scl_all;
							scl_all = 0.0f;
							sprintf(BoxName, "sclX  %d-%d", j, k);
							ImGui::InputFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Scl.x, value);
							sprintf(BoxName, " sclX %d-%d", j, k);
							ImGui::DragFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Scl.x, value);

							sprintf(BoxName, "sclY  %d-%d", j, k);
							ImGui::InputFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Scl.y, value);
							sprintf(BoxName, " sclY %d-%d", j, k);
							ImGui::DragFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Scl.y, value);

							sprintf(BoxName, "sclZ  %d-%d", j, k);
							ImGui::InputFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Scl.z, value);
							sprintf(BoxName, " sclZ %d-%d", j, k);
							ImGui::DragFloat(BoxName, &m_Motion[i]->KeyFrame[j]->Key[k]->Scl.z, value);
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
			}
		}
	}
	ImGui::End();
}
void CSceneMotionPartsX::imGuiCollision()
{
	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Collision Window", &collision_window);
	std::string BoxName;
	ImGui::Text("MotionID %d", m_MotionID);
	ImGui::InputFloat("Value", &value, 0.001f);

	if (ImGui::Button("Collision++")) CollisionAdd(m_MotionID);
	if (ImGui::Button("Collision--")) CollisionSub(m_MotionID);
	for (int j = 0; j < m_Motion[m_MotionID]->Collision.size(); j++)
	{
		BoxName = "CollisionID " + std::to_string(j);
		if (ImGui::CollapsingHeader(BoxName.c_str()))
		{
			BoxName = "PartsID " + std::to_string(j);
			ImGui::InputInt(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->PartID);
			if (m_Motion[m_MotionID]->Collision[j]->PartID < 0) m_Motion[m_MotionID]->Collision[j]->PartID = 0;
			if (m_Motion[m_MotionID]->Collision[j]->PartID >= (int)m_Part.size()) m_Motion[m_MotionID]->Collision[j]->PartID = (int)m_Part.size()-1;

			BoxName = "POS " + std::to_string(j);
			if (ImGui::TreeNode(BoxName.c_str()))
			{
				BoxName = "posX  " + std::to_string(j);
				ImGui::InputFloat(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->Pos.x, value);
				BoxName = " posX " + std::to_string(j);
				ImGui::DragFloat(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->Pos.x, value);

				BoxName = "posY  " + std::to_string(j);
				ImGui::InputFloat(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->Pos.y, value);
				BoxName = " posY " + std::to_string(j);
				ImGui::DragFloat(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->Pos.y, value);

				BoxName = "posZ  " + std::to_string(j);
				ImGui::InputFloat(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->Pos.z, value);
				BoxName = " posZ " + std::to_string(j);
				ImGui::DragFloat(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->Pos.z, value);
				ImGui::TreePop();
			}

			BoxName = "SCL " + std::to_string(j);
			if (ImGui::TreeNode(BoxName.c_str()))
			{
				BoxName = "scl  " + std::to_string(j);
				ImGui::InputFloat(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->Scl, value);
				BoxName = " scl " + std::to_string(j);
				ImGui::DragFloat(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->Scl, value);
				ImGui::TreePop();
			}

			BoxName = "START FRAME " + std::to_string(j);
			ImGui::InputInt(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->StartFrame);
			if (m_Motion[m_MotionID]->Collision[j]->StartFrame < 0) m_Motion[m_MotionID]->Collision[j]->StartFrame = 0;

			BoxName = "  END FRAME " + std::to_string(j);
			ImGui::InputInt(BoxName.c_str(), &m_Motion[m_MotionID]->Collision[j]->EndFrame);
			if (m_Motion[m_MotionID]->Collision[j]->StartFrame > m_Motion[m_MotionID]->Collision[j]->EndFrame)
				m_Motion[m_MotionID]->Collision[j]->EndFrame = m_Motion[m_MotionID]->Collision[j]->StartFrame;
		}
	}
	ImGui::End();
}
void CSceneMotionPartsX::imGuiSound()
{
	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Sound Window", &sound_window);
	std::string BoxName;
	ImGui::Text("MotionID %d", m_MotionID);

	if (ImGui::Button("Sound++")) SoundAdd();
	if (ImGui::Button("Sound--")) SoundSub();

	for (int i = 0; i < m_Motion[m_MotionID]->Sound.size(); i++)
	{
		BoxName = "SoundID " + std::to_string(i);
		if (ImGui::CollapsingHeader(BoxName.c_str()))
		{
			BoxName = "Frame " + std::to_string(i);
			ImGui::InputInt(BoxName.c_str(), &m_Motion[m_MotionID]->Sound[i]->Frame);
			if (m_Motion[m_MotionID]->Sound[i]->Frame < -1) { m_Motion[m_MotionID]->Sound[i]->Frame = -1; }
			if (m_Motion[m_MotionID]->Sound[i]->Frame >= m_MotionFrameAll) { m_Motion[m_MotionID]->Sound[i]->Frame = m_MotionFrameAll - 1; }

			int IDold = m_Motion[m_MotionID]->Sound[i]->ID;
			BoxName = "Sound " + std::to_string(i);
			ImGui::InputInt(BoxName.c_str(), &m_Motion[m_MotionID]->Sound[i]->ID);
			if (m_Motion[m_MotionID]->Sound[i]->ID < 0) { m_Motion[m_MotionID]->Sound[i]->ID = 0; }
			if (m_Motion[m_MotionID]->Sound[i]->ID >= CSound::SOUND_MAX) { m_Motion[m_MotionID]->Sound[i]->ID = CSound::SOUND_MAX - 1; }
			if (IDold != m_Motion[m_MotionID]->Sound[i]->ID)
			{
				CManager::GetSound()->Stop((CSound::SOUND_LABEL)IDold);
			}
			ImGui::Text("%d-(%s)",i, CManager::GetSound()->m_aParam[m_Motion[m_MotionID]->Sound[i]->ID].pFilename);
			BoxName = "Play Sound " + std::to_string(i);
			if (ImGui::Button(BoxName.c_str()))
			{
				CManager::GetSound()->Play((CSound::SOUND_LABEL)m_Motion[m_MotionID]->Sound[i]->ID);
			}
		}
	}
	ImGui::End();
}
void CSceneMotionPartsX::imGuiMotion()
{
	char BoxName[32];
	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Now Motion Window", &model_window);
	ImGui::InputFloat("Value", &value, 0.001f);

	// �p�[�c���\��
	ImGui::Text("Parts Num %d", m_Part.size());

	sprintf(BoxName, "Start KeyID %d", m_Key);
	if (ImGui::CollapsingHeader(BoxName))
	{
		// �L�[�t���[�����ۑ�
		if (ImGui::Button("SaveStartKey"))
		{
			for (int i = 0; i < m_Part.size(); i++) {
				m_SaveData[i]->Pos = m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos;
				m_SaveData[i]->Rot = m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot;
				m_SaveData[i]->Scl = m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl;
			}
		}
		// �L�[�t���[�����Ǎ�
		if (ImGui::Button("LoadStartKey"))
		{
			for (int i = 0; i < m_Part.size(); i++) {
				m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos = m_SaveData[i]->Pos;
				m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot = m_SaveData[i]->Rot;
				m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl = m_SaveData[i]->Scl;
			}
		}

		// �t���[�����
		sprintf(BoxName, "Frame %d", m_Key);
		ImGui::DragInt(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Frame);

		// �e�p�[�c���
		for (int i = 0; i < m_Part.size(); i++) {
			ImGui::Text("Num %d(%s)", i, m_Part[i]->Model->Name);

			sprintf(BoxName, "POS %d-%d", m_Key, i);
			if (ImGui::TreeNode(BoxName))
			{
				sprintf(BoxName, "posX  %d-%d", m_Key, i);
				ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos.x, value);
				sprintf(BoxName, " posX %d-%d", m_Key, i);
				ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos.x, value);

				sprintf(BoxName, "posY  %d-%d", m_Key, i);
				ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos.y, value);
				sprintf(BoxName, " posY %d-%d", m_Key, i);
				ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos.y, value);

				sprintf(BoxName, "posZ  %d-%d", m_Key, i);
				ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos.z, value);
				sprintf(BoxName, " posZ %d-%d", m_Key, i);
				ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Pos.z, value);
				ImGui::TreePop();
			}

			sprintf(BoxName, "ROT %d-%d", m_Key, i);
			if (ImGui::TreeNode(BoxName))
			{
				sprintf(BoxName, "rotX  %d-%d", m_Key, i);
				ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot.x, value);
				sprintf(BoxName, " rotX %d-%d", m_Key, i);
				ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot.x, value);

				sprintf(BoxName, "rotY  %d-%d", m_Key, i);
				ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot.y, value);
				sprintf(BoxName, " rotY %d-%d", m_Key, i);
				ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot.y, value);

				sprintf(BoxName, "rotZ  %d-%d", m_Key, i);
				ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot.z, value);
				sprintf(BoxName, " rotZ %d-%d", m_Key, i);
				ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Rot.z, value);
				ImGui::TreePop();
			}

			sprintf(BoxName, "SCL %d-%d", m_Key, i);
			if (ImGui::TreeNode(BoxName))
			{
				ImGui::InputFloat("Scl  ALL", &scl_all, value);
				ImGui::DragFloat(" Scl ALL", &scl_all, value);
				m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl.x += scl_all;
				m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl.y += scl_all;
				m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl.z += scl_all;
				scl_all = 0.0f;
				sprintf(BoxName, "sclX  %d-%d", m_Key, i);
				ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl.x, value);
				sprintf(BoxName, " sclX %d-%d", m_Key, i);
				ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl.x, value);

				sprintf(BoxName, "sclY  %d-%d", m_Key, i);
				ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl.y, value);
				sprintf(BoxName, " sclY %d-%d", m_Key, i);
				ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl.y, value);

				sprintf(BoxName, "sclZ  %d-%d", m_Key, i);
				ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl.z, value);
				sprintf(BoxName, " sclZ %d-%d", m_Key, i);
				ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[m_Key]->Key[i]->Scl.z, value);
				ImGui::TreePop();
			}
		}
	}

	if (m_Motion[m_MotionID]->KeyFrame[m_Key]->Frame > 0)
	{
		int NextKey = m_Key + 1;

		// ���̃L�[�t���[���̃N���A
		if (NextKey >= m_Motion[m_MotionID]->KeyFrame.size())
			NextKey = 0;

		sprintf(BoxName, "  End KeyID %d", NextKey);
		if (ImGui::CollapsingHeader(BoxName))
		{
			// �L�[�t���[�����ۑ�
			if (ImGui::Button("SaveEndKey"))
			{
				for (int i = 0; i < m_Part.size(); i++) {
					m_SaveData[i]->Pos = m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Pos;
					m_SaveData[i]->Rot = m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Rot;
					m_SaveData[i]->Scl = m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl;
				}
			}
			// �L�[�t���[�����Ǎ�
			if (ImGui::Button("LoadEndKey"))
			{
				for (int i = 0; i < m_Part.size(); i++) {
					m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Pos = m_SaveData[i]->Pos;
					m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Rot = m_SaveData[i]->Rot;
					m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl = m_SaveData[i]->Scl;
				}
			}

			// �t���[�����
			sprintf(BoxName, "Frame %d", NextKey);
			ImGui::DragInt(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Frame);

			// �e�p�[�c���
			for (int i = 0; i < m_Part.size(); i++) {
				ImGui::Text("Num %d(%s)", i, m_Part[i]->Model->Name);

				sprintf(BoxName, "POS %d-%d", NextKey, i);
				if (ImGui::TreeNode(BoxName))
				{
					sprintf(BoxName, "posX  %d-%d", NextKey, i);
					ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Pos.x, value);
					sprintf(BoxName, " posX %d-%d", NextKey, i);
					ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Pos.x, value);

					sprintf(BoxName, "posY  %d-%d", NextKey, i);
					ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Pos.y, value);
					sprintf(BoxName, " posY %d-%d", NextKey, i);
					ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Pos.y, value);

					sprintf(BoxName, "posZ  %d-%d", NextKey, i);
					ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Pos.z, value);
					sprintf(BoxName, " posZ %d-%d", NextKey, i);
					ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Pos.z, value);
					ImGui::TreePop();
				}

				sprintf(BoxName, "ROT %d-%d", NextKey, i);
				if (ImGui::TreeNode(BoxName))
				{
					sprintf(BoxName, "rotX  %d-%d", NextKey, i);
					ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Rot.x, value);
					sprintf(BoxName, " rotX %d-%d", NextKey, i);
					ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Rot.x, value);

					sprintf(BoxName, "rotY  %d-%d", NextKey, i);
					ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Rot.y, value);
					sprintf(BoxName, " rotY %d-%d", NextKey, i);
					ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Rot.y, value);

					sprintf(BoxName, "rotZ  %d-%d", NextKey, i);
					ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Rot.z, value);
					sprintf(BoxName, " rotZ %d-%d", NextKey, i);
					ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Rot.z, value);
					ImGui::TreePop();
				}

				sprintf(BoxName, "SCL %d-%d", NextKey, i);
				if (ImGui::TreeNode(BoxName))
				{
					ImGui::InputFloat("Scl  ALL", &scl_all, value);
					ImGui::DragFloat(" Scl ALL", &scl_all, value);
					m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl.x += scl_all;
					m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl.y += scl_all;
					m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl.z += scl_all;
					scl_all = 0.0f;
					sprintf(BoxName, "sclX  %d-%d", NextKey, i);
					ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl.x, value);
					sprintf(BoxName, " sclX %d-%d", NextKey, i);
					ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl.x, value);

					sprintf(BoxName, "sclY  %d-%d", NextKey, i);
					ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl.y, value);
					sprintf(BoxName, " sclY %d-%d", NextKey, i);
					ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl.y, value);

					sprintf(BoxName, "sclZ  %d-%d", NextKey, i);
					ImGui::InputFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl.z, value);
					sprintf(BoxName, " sclZ %d-%d", NextKey, i);
					ImGui::DragFloat(BoxName, &m_Motion[m_MotionID]->KeyFrame[NextKey]->Key[i]->Scl.z, value);
					ImGui::TreePop();
				}
			}
		}
	}

	ImGui::End();
}