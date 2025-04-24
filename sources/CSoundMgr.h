#pragma once

class CSound;

class CSoundMgr
{
	SINGLE(CSoundMgr)
private:
	LPDIRECTSOUND8	m_pSound;	// 사운드 카드 제어
	CSound*			m_pBGM;		// BGM Sound


public:
	int Init();
	LPDIRECTSOUND8 GetSoundDevice() { return m_pSound; }	
	void RegisterToBGM(CSound* _pSound);
	CSound* GetCurBGM() { return m_pBGM; }
};

