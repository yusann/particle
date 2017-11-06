//*****************************************************************************
//   �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "sceneMesh.h"
#include "meshField.h"
#include "texture.h"

//*****************************************************************************
//   �萔��`
//*****************************************************************************
#define FIELD_WIDTH       (12.0f)                                      // ��
#define FIELD_DEPTH       (12.0f)                                      // ���s
#define FIELD_TEXU        (0.1f)                                     // �e�N�X�`����������
#define FIELD_TEXV        (0.1f)                                     // �e�N�X�`���c������
#define FIELD_HEIGHT      (10)

//=======================================================================================
//   �R���X�g���N�^
//=======================================================================================
CMeshField::CMeshField() :CSceneMesh(1)
{
}

//=======================================================================================
//   �f�X�g���N�^
//=======================================================================================
CMeshField::~CMeshField()
{
}

//==================================================================================================================================================
// �쐬����
//==================================================================================================================================================
CMeshField *CMeshField::Create(void)
{
	CMeshField *pScene3D;                            // �ϐ��錾
	pScene3D = new CMeshField;                       // ���I�m��
	pScene3D->Init();                            // ������
	return pScene3D;                             // �l��Ԃ�
}

//=======================================================================================
//   ����������
//=======================================================================================
void CMeshField::Init()
{
	LoadFile();

	// �����o�ϐ��̏�����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(FIELD_WIDTH, FIELD_DEPTH);

	// �v���C�x�[�g�ϐ��̏�����
	m_pIdxBuff = NULL;
	m_divide = D3DXVECTOR2((float)m_MaxX + 1, (float)m_MaxY);
	m_texUV = D3DXVECTOR2(FIELD_TEXU, FIELD_TEXV);
	m_nPolygonNum = (int)((m_divide.y*(m_divide.x * 2)) + ((m_divide.y - 1) * 2) - 2);
	m_nVexNum = (int)(m_divide.x * (m_divide.y + 1));

	// ���_�ݒ�
	MakeVex();
	MakeBuff();

	// �e�N�X�`���Z�b�g
	m_pTexture = *CTexture::GetTexture((int)CTexture::TEXTYPE_FIELD);

	// �^�C�v�̑��
	CSceneMesh::Init();
}

//=======================================================================================
//   �I������
//=======================================================================================
void CMeshField::Uninit()
{
	CSceneMesh::Uninit();
}

//=======================================================================================
//   �X�V����
//=======================================================================================
void CMeshField::Update()
{
}

//=======================================================================================
//   �`�揈��
//=======================================================================================
void CMeshField::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return;
	}
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	CSceneMesh::Draw();
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

//=======================================================================================
// �|���S���̒��_�ݒ�
//=======================================================================================
void CMeshField::MakeVex(void)
{
	// �ϐ��錾
	int nCntHeight;     // �n�ʃ��[�v�p
	int nCntWidth;      // �n�ʃ��[�v�p

						// �@���̌v�Z�p�ϐ�
	D3DXVECTOR3 m_vtxPos[128 + 1][128 + 1];
	D3DXVECTOR3 directionX, directionZ, normalX, normalZ, normal;

	const float g_fieldPosX = m_MaxX*FIELD_WIDTH*-0.5f;                            // X���W
	const float g_fieldPosZ = m_MaxY*FIELD_DEPTH*0.5f;                                 // Z���W

	// ���W���
	for (nCntHeight = 0; nCntHeight < (int)m_divide.y + 1; nCntHeight++)
	{
		for (nCntWidth = 0; nCntWidth < (int)m_divide.x; nCntWidth++)
		{
			m_vtxPos[nCntHeight][nCntWidth].x = g_fieldPosX + (nCntWidth * m_size.x);
			m_vtxPos[nCntHeight][nCntWidth].y = m_PosY[nCntHeight*(int)m_divide.x+nCntWidth];
			m_vtxPos[nCntHeight][nCntWidth].z = g_fieldPosZ - (nCntHeight * m_size.y);
		}
	}

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVexNum,           // �쐬���������_�o�b�t�@�̃T�C�Y�i��̒��_*���_���j
		D3DUSAGE_WRITEONLY,                                      // �������ނ������Ȃ��i�`�F�b�N���Ȃ��j
		FVF_VERTEX_3D,                                           // �ǂ�Ȓ��_�ŏ����́i0�ɂ��Ă�OK�j
		D3DPOOL_MANAGED,                                         // �������Ǘ������C���ɂ���
		&m_pVtxBuff,
		NULL);

	// ���_����ݒ�
	// ���_���i�[�p�^���o�b�t�@�̐錾
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����i0,0���L������ƑS�������b�N�j
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntHeight = 0; nCntHeight < m_divide.y + 1; nCntHeight++)
	{
		for (nCntWidth = 0; nCntWidth < m_divide.x; nCntWidth++, pVtx++)
		{
			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_fieldPosX + (nCntWidth * m_size.x),      // X���W�̐ݒ�i 3D���W�E�E��� �j
				m_PosY[nCntHeight*(int)m_divide.x + nCntWidth],                                           // Y���W�̐ݒ�
				g_fieldPosZ - (nCntHeight * m_size.y));    // Z���W�̐ݒ�

														   // �O���̒��_���s���Ȃ�
			if (nCntHeight != 0 && nCntHeight != m_divide.y &&
				nCntWidth != 0 && nCntWidth != m_divide.x - 1)
			{
				// X���̖@���v�Z
				directionX = m_vtxPos[nCntHeight][nCntWidth + 1] - m_vtxPos[nCntHeight][nCntWidth - 1];
				normalX.x = -directionX.y;
				normalX.y = directionX.x;
				normalX.z = 0.0f;

				// Z���̖@���v�Z
				directionZ = m_vtxPos[nCntHeight + 1][nCntWidth] - m_vtxPos[nCntHeight - 1][nCntWidth];
				normalZ.x = 0.0f;
				normalZ.y = directionZ.x;
				normalZ.z = -directionZ.y;

				// X����Z���̖@���v�Z
				normal = normalX + normalZ;

				// 1�Ƀm�[�}���C�Y
				D3DXVec3Normalize(&normal, &normal);
			}
			// �O���̒��_�@���ݒ�
			else
			{
				normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			}

			pVtx[0].normal = D3DXVECTOR3(normal.x, normal.y, normal.z);					// �@���̐ݒ�
			pVtx[0].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);						// �J���[�̐ݒ�i0~255�̐����l�j
			pVtx[0].tex = D3DXVECTOR2(0.0f + (nCntWidth * m_texUV.x),					// �e�N�X�`��U���W�̐ݒ�
				0.0f + (nCntHeight * m_texUV.y));									// �e�N�X�`��V���W�̐ݒ�
		}
	}

	// �����J����
	m_pVtxBuff->Unlock();
}


//=======================================================================================
// �|���S���̃o�b�t�@�ݒ�
//=======================================================================================
void CMeshField::MakeBuff(void)
{
	// �ϐ��錾
	int nCntHeight;     // �n�ʃ��[�v�p
	int nCntWidth;      // �n�ʃ��[�v�p

						// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "NULL�`�F�b�N���Ă��������I", "�G���[", MB_OK | MB_ICONASTERISK);         // �G���[���b�Z�[�W
		return;
	}

	// �C���f�b�N�X�o�b�t�@�̍쐬
	HRESULT hr;
	hr = pDevice->CreateIndexBuffer(sizeof(WORD) * (m_nPolygonNum + 2),        // ���_���iWORD��16Bid�j
		D3DUSAGE_WRITEONLY,              // �g�p�p�r�t���O�i�������ނ̂݁j
		D3DFMT_INDEX16,                  // �f�[�^�̃t�H�[�}�b�g�i32Bid�ݒ肵����16��32�ɕύX�j
		D3DPOOL_MANAGED,                 // �������̊Ǘ����@�i���C���j
		&m_pIdxBuff,              // �o�b�t�@�\�C���^�[�t�F�C�X�|�C���^�̃A�h���X
		NULL);

	// �C���f�b�N�X���i�[�p�^���o�b�t�@�̐錾
	WORD* pIdx;

	// ���b�N
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// Z���̃��[�v
	for (nCntHeight = 0; nCntHeight < m_divide.y; nCntHeight++)
	{
		// X���̃��[�v
		for (nCntWidth = 0; nCntWidth < m_divide.x; nCntWidth++, pIdx += 2)                          // ���[�v�����pIdx��2�O�i
		{
			// 1��ڈȊO��1�s�ڂ̏���
			if (nCntHeight != 0 && nCntWidth == 0) {
				pIdx[0] = (unsigned short)(m_divide.x + (nCntHeight * m_divide.x + nCntWidth));                  // �������ɃC���f�b�N�X������
				pIdx += 1;                                                                               // 1�O�i
			}
			// �d�����Ȃ����_�̏���
			pIdx[0] = (unsigned short)(m_divide.x + (nCntHeight * m_divide.x + nCntWidth));
			pIdx[1] = (unsigned short)(m_divide.x + (nCntHeight * m_divide.x + nCntWidth) - m_divide.x);

			// �ŏI��ڈȊO�̍ŏI�s�ڂ̏���
			if (nCntHeight != m_divide.y - 1 && nCntWidth == m_divide.x - 1) {
				pIdx[2] = (unsigned short)(m_divide.x + (nCntHeight * m_divide.x + nCntWidth) - m_divide.x);  // �������ɃC���f�b�N�X������
				pIdx += 1;                                                                               // 1�O�i
			}
		}
	}
	// ���b�N���
	m_pIdxBuff->Unlock();
}

float CMeshField::GetHeight(D3DXVECTOR3 pos)
{
	// ���_����ݒ�
	// ���_���i�[�p�^���o�b�t�@�̐錾
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����i0,0���L������ƑS�������b�N�j
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���W�ϊ�
	float posZ = -pos.z + (FIELD_DEPTH*m_MaxY*0.5f);
	float posX = pos.x + (FIELD_WIDTH*m_MaxX*0.5f);

	// ���W����C���f�b�N�X�ϊ�
	int indexZ = (int)(posZ / FIELD_DEPTH);
	int indexX = (int)(posX / FIELD_WIDTH);

	if (indexX < 0 || indexZ < 0)
	{
		return pos.y;
	}

	// �v�Z�p�ϐ�
	D3DXVECTOR3 normal, work;
	D3DXVECTOR3 vecF, vecT;
	D3DXVECTOR3 pos0, pos1, pos2;
	D3DXVECTOR3 posT = pos;

	// ���_���W���
	pos0 = pVtx[indexZ*(m_MaxX + 1) + indexX + 1].pos;
	pos1 = pVtx[(indexZ + 1)*(m_MaxX + 1) + indexX + 1].pos;
	pos2 = pVtx[indexZ*(m_MaxX + 1) + indexX].pos;

	// �x�N�g���v�Z
	vecF = pos1 - pos0;
	vecT = posT - pos0;

	// ���όv�Z
	D3DXVec3Cross(&vecF, &vecF, &vecT);

	// �͈͓��`�F�b�N
	if (vecF.y >= 0.0f)
	{
		vecF = pos2 - pos1;
		vecT = posT - pos1;
		D3DXVec3Cross(&vecF, &vecF, &vecT);
		if (vecF.y >= 0.0f)
		{
			vecF = pos0 - pos2;
			vecT = posT - pos2;
			D3DXVec3Cross(&vecF, &vecF, &vecT);
			if (vecF.y >= 0.0f)
			{
				// �����v�Z�i�@���j
				vecF = pos1 - pos0;
				vecT = pos2 - pos0;
				D3DXVec3Cross(&normal, &vecF, &vecT);

				// �m�[�}���C�Y�i�P�ʂP�ɂ���j
				D3DXVec3Normalize(&normal, &normal);

				// �����J����
				m_pVtxBuff->Unlock();

				// ������Ԃ�
				return pos0.y - ((pos.x - pos0.x) * normal.x + (pos.z - pos0.z) * normal.z) / normal.y;
			}
		}
	}
	pos0 = pVtx[(indexZ + 1)*(m_MaxX + 1) + indexX].pos;
	pos1 = pVtx[indexZ*(m_MaxX + 1) + indexX].pos;
	pos2 = pVtx[(indexZ + 1)*(m_MaxX + 1) + indexX + 1].pos;

	// �����J����
	m_pVtxBuff->Unlock();

	vecF = pos1 - pos0;
	vecT = posT - pos0;
	D3DXVec3Cross(&vecF, &vecF, &vecT);
	if (vecF.y >= 0.0f)
	{
		vecF = pos2 - pos1;
		vecT = posT - pos1;
		D3DXVec3Cross(&vecF, &vecF, &vecT);
		if (vecF.y >= 0.0f)
		{
			vecF = pos0 - pos2;
			vecT = posT - pos2;
			D3DXVec3Cross(&vecF, &vecF, &vecT);
			if (vecF.y >= 0.0f)
			{
				vecF = pos1 - pos0;
				vecT = pos2 - pos0;
				D3DXVec3Cross(&normal, &vecF, &vecT);
				D3DXVec3Normalize(&normal, &normal);
				return pos0.y - ((pos.x - pos0.x) * normal.x + (pos.z - pos0.z) * normal.z) / normal.y;
			}
		}
	}
	return pos.y;
}

void CMeshField::LoadFile(void)
{
	char aWork[64];
	// �����o������
	FILE *pFile;
	fopen_s(&pFile, "data/field.txt", "r+");

	if (pFile == NULL)
	{
		assert(!("���[�h����t�@�C���iLOAD_NAME�j������܂���I"));
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
			else if (strcmp(aWork, "LOAD_SIZE") == 0) {

				//  
				fscanf(pFile, "%d", &m_MaxX);
				fscanf(pFile, "%d", &m_MaxY);
				m_PosY = new float[(m_MaxY + 1)*(m_MaxX + 1)];

				// �I��
				while (1) {
					// �����ǂݍ���
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "END_SIZE") == 0) {
						break;
					}
				}
				continue;
			}
			else if (strcmp(aWork, "LOAD_FIELD") == 0) {

				for (int i = 0; i < m_MaxY + 1; i++)
				{
					for (int j = 0; j < m_MaxX + 1; j++)
					{
						// ���W
						fscanf(pFile, "%f ", &m_PosY[i*(m_MaxX + 1) + j]);
					}
				}

				// �I��
				while (1) {
					// �����ǂݍ���
					fscanf(pFile, "%s", &aWork);
					if (strcmp(aWork, "END_FIELD") == 0) {
						break;
					}
				}
				continue;
			}

			//  ���[�h�I��
			else if (strcmp(aWork, "END_LOAD") == 0) {
				break;
			}

			// �G���[
			assert(!("�Z�[�u���e���Ԉ���Ă���I�I"));
		}
		//  �t�@�C�������
		fclose(pFile);
	}

}