#include "pch.h"
#include "CSoundMgr.h"

#include "CEngine.h"
#include "CSound.h"


CSoundMgr::CSoundMgr()
	: m_pSound(nullptr)
{

}

CSoundMgr::~CSoundMgr()
{
	// DirectSound8 객체 메모리 해제
	m_pSound->Release();
}

int CSoundMgr::Init()
{
	if (FAILED(DirectSoundCreate8(NULL, &m_pSound, NULL)))
	{
		MessageBox(NULL, L"사운드 디바이스 생성 실패", L"SYSTEM ERROR", MB_OK);
		return false;
	}

	// 사운드 협조레벨 설정
	HWND hWnd = CEngine::Get()->GetMainHwnd();
	if (FAILED(m_pSound->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE)))
	{
		MessageBox(NULL, L"사운드 매니저 초기화 실패", L"SYSTEM ERROR", MB_OK);
		return false;
	}

	return true;
}


void CSoundMgr::RegisterToBGM(CSound* _pSound)
{
	if (m_pBGM != nullptr)
		m_pBGM->Stop(true);

	m_pBGM = _pSound;
}
