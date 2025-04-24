#include "pch.h"
#include "CLevel_Ending.h"

#include "CEngine.h"
#include "CAssetMgr.h"
#include "CSound.h"
#include "CKeyMgr.h"
#include "CBackground.h"
#include "CLogMgr.h"

#include "CTextUI.h"

#include "CCamera.h"
#include "CLevelMgr.h"
#include "CTimeMgr.h"

CLevel_Ending::CLevel_Ending()
	:m_GoldCall(false)
	, m_TimeCall(false)
	, m_Gold(0)
	, m_AccTime(0.f)
	, m_PlayTime(0.f)
{
}

CLevel_Ending::~CLevel_Ending()
{
}

void CLevel_Ending::Init()
{
	// BGM 지정
	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"Ending", L"sound\\EndCredits.wav");
	pBGM->SetVolume(30);
	pBGM->PlayToBGM();

	CSound* pSound = CAssetMgr::Get()->LoadSound(L"idol_get", L"sound\\idol_get.wav");
	pSound->Play();

	CLevel* pPrevLevel = CLevelMgr::Get()->GetPrevLevel();
	SetpInfo(pPrevLevel->GetpIfno());

	PlayerInfo pInfo = GetpIfno();

	m_Gold = pInfo.Gold;
	m_PlayTime = pPrevLevel->LoadPlayTime();

	CBackground* pBackGround = new CBackground;
	pBackGround->SetName(L"BackGround");
	pBackGround->SetAtlas(CAssetMgr::Get()->LoadTexture(L"Ending", L"texture\\ending.png"));
	pBackGround->SetPos(Vec2(0.f, 0.f));

	AddObject(pBackGround, LAYER_TYPE::BACKGROUND);





	Vec2 vPos = CEngine::Get()->GetResolution();
	CCamera::Get()->SetLookAt(Vec2(vPos.x/2.f, vPos.y/2.f));
}

void CLevel_Ending::Tick()
{
	CLevel::Tick();
	m_AccTime += DT;
	if (1.f < m_AccTime && !m_GoldCall)
	{
		CTextUI* pTextUI = new CTextUI;
		pTextUI->SetName(L"GoldRecord");
		pTextUI->SetPos(Vec2(620.f, 205.f));
		pTextUI->SetString(std::to_wstring(m_Gold));
		pTextUI->SetScale(30);
		AddObject(pTextUI, LAYER_TYPE::UI);
		CSound* pSound = CAssetMgr::Get()->LoadSound(L"Deposit", L"sound\\deposit.wav");
		pSound->Play();
		m_GoldCall = true;
	}
	else if (2.f < m_AccTime && !m_TimeCall)
	{
		int pMinute = m_PlayTime / 60;
		int pSecond = (int)m_PlayTime % 60;
		wstring MinString;
		wstring SecString;

		if (pMinute < 10) 
		{
			MinString = L"0" + std::to_wstring(pMinute);
		}
		else
		{
			MinString = std::to_wstring(pMinute);
		}

		if (pSecond < 10)
		{
			SecString = L"0" + std::to_wstring(pSecond);
		}
		else
		{
			SecString = std::to_wstring(pSecond);
		}
		

		wstring str = MinString + L" : " + SecString;

		CTextUI* pTextUI = new CTextUI;
		pTextUI->SetName(L"TimeRecord");
		pTextUI->SetPos(Vec2(620.f, 305.f));
		pTextUI->SetString(str);
		pTextUI->SetScale(30);
		AddObject(pTextUI, LAYER_TYPE::UI);
		CSound* pSound = CAssetMgr::Get()->LoadSound(L"Deposit", L"sound\\deposit.wav");
		pSound->Play();
		m_TimeCall = true;
	}

	if (KEY_TAP(KEY::Z))
	{
		ChangeLevel((int)LEVEL_TYPE::TITLE);
	}
}

void CLevel_Ending::Exit()
{
	// 모든 오브젝트를 삭제해둔다.
	DeleteAllObjects();
}




