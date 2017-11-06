//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*********************************************************
// サウンドクラス
//*********************************************************
class CSound
{
public:
	typedef enum
	{
		BGM_TITLE000 = 0,		// BGMタイトル
		BGM_STAGE000,			// BGMステージ01
		BGM_BOSS000,			// BGMステージ01BOOS
		BGM_BOSS001,			// BGMステージ01LastBOOS
		BGM_WIN000,				// BGM勝利
		BGM_RESULT000,			// BGMリザルト
		BGM_LOSE000,			// BGM負け

		SE_SELECT000,			// SE選択音
		SE_ENTER000,			// SE決定音
		SE_CANCEL000,			// SEキャンセル音
		SE_SHOT000,				// SE弾発射音
		SE_EXPLOSION000,		// SE爆発音
		SE_ITEM000,				// SEアイテム音
		SE_WARNING000,			// SEアラーム音
		SE_ATTACK000,			// SE攻撃音
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
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;

	// 各音素材のパラメータ
	SOUNDPARAM m_aParam[SOUND_MAX] =
	{
		{ "data/BGM/title000.wav", -1 },		// BGMタイトル(-1は無限ループ、0はワンショット、3は3回鳴らす)
		{ "data/BGM/stage000.wav", -1 },		// BGMステージ01
		{ "data/BGM/boss000.wav", -1 },			// BGMステージ01BOOS
		{ "data/BGM/LASTBOOS.wav", -1 },		// BGMステージ01LastBOOS
		{ "data/BGM/win000.wav", 0 },			// BGM勝利
		{ "data/BGM/result000.wav", -1 },		// BGMリザルト
		{ "data/BGM/lose000.wav", -1 },			// BGM負け

		{ "data/SE/select000.wav", 0 },			// SE選択音
		{ "data/SE/enter000.wav", 0 },			// SE決定音
		{ "data/SE/cancel000.wav", 0 },			// SEキャンセル音
		{ "data/SE/shot001.wav", 0 },			// SE弾発射音
		{ "data/SE/explosion000.wav", 0 },		// SE爆発音
		{ "data/SE/item000.wav", 0 },			// SEアイテム音
		{ "data/SE/warning000.wav", 0 },		// SEアラーム音
		{ "data/SE/attack000.wav", 0 },			// SE攻撃音
		{ "data/SE/damage000.wav", 0 },			// SE攻撃音
		{ "data/SE/dash.wav", 0 },				// SE攻撃音
		{ "data/SE/jump.wav", 0 },				// SE攻撃音
		{ "data/SE/warning001.wav", 0 },		// SE攻撃音
		{ "data/SE/number.wav", 0 },			// SE攻撃音
	};

private:

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_MAX] = {};	// ソースボイス
	BYTE *m_apDataAudio[SOUND_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_MAX] = {};					// オーディオデータサイズ
};

#endif
