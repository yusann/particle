//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*********************************************************
// �T�E���h�N���X
//*********************************************************
class CSound
{
public:
	typedef enum
	{
		BGM_TITLE000 = 0,		// BGM�^�C�g��
		BGM_STAGE000,			// BGM�X�e�[�W01
		BGM_BOSS000,			// BGM�X�e�[�W01BOOS
		BGM_BOSS001,			// BGM�X�e�[�W01LastBOOS
		BGM_WIN000,				// BGM����
		BGM_RESULT000,			// BGM���U���g
		BGM_LOSE000,			// BGM����

		SE_SELECT000,			// SE�I����
		SE_ENTER000,			// SE���艹
		SE_CANCEL000,			// SE�L�����Z����
		SE_SHOT000,				// SE�e���ˉ�
		SE_EXPLOSION000,		// SE������
		SE_ITEM000,				// SE�A�C�e����
		SE_WARNING000,			// SE�A���[����
		SE_ATTACK000,			// SE�U����
		SE_DAMAGE000,
		SE_DASH,
		SE_JUMP,
		SE_ALARM,
		SE_NUMBER,
		SOUND_MAX,
	} SOUND_LABEL;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	void Stop(void);
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} SOUNDPARAM;

	// �e���f�ނ̃p�����[�^
	SOUNDPARAM m_aParam[SOUND_MAX] =
	{
		{ "data/BGM/title000.wav", -1 },		// BGM�^�C�g��(-1�͖������[�v�A0�̓����V���b�g�A3��3��炷)
		{ "data/BGM/stage000.wav", -1 },		// BGM�X�e�[�W01
		{ "data/BGM/boss000.wav", -1 },			// BGM�X�e�[�W01BOOS
		{ "data/BGM/LASTBOOS.wav", -1 },		// BGM�X�e�[�W01LastBOOS
		{ "data/BGM/win000.wav", 0 },			// BGM����
		{ "data/BGM/result000.wav", -1 },		// BGM���U���g
		{ "data/BGM/lose000.wav", -1 },			// BGM����

		{ "data/SE/select000.wav", 0 },			// SE�I����
		{ "data/SE/enter000.wav", 0 },			// SE���艹
		{ "data/SE/cancel000.wav", 0 },			// SE�L�����Z����
		{ "data/SE/shot001.wav", 0 },			// SE�e���ˉ�
		{ "data/SE/explosion000.wav", 0 },		// SE������
		{ "data/SE/item000.wav", 0 },			// SE�A�C�e����
		{ "data/SE/warning000.wav", 0 },		// SE�A���[����
		{ "data/SE/attack000.wav", 0 },			// SE�U����
		{ "data/SE/damage000.wav", 0 },			// SE�U����
		{ "data/SE/dash.wav", 0 },				// SE�U����
		{ "data/SE/jump.wav", 0 },				// SE�U����
		{ "data/SE/warning001.wav", 0 },		// SE�U����
		{ "data/SE/number.wav", 0 },			// SE�U����
	};

private:

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_MAX] = {};	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_MAX] = {};					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y
};

#endif
