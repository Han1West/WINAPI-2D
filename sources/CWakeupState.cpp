#include "pch.h"
#include "CWakeupState.h"

#include "CMonster.h"
#include "CSkeleton.h"
#include "CAnimator.h"
#include "CStateMachine.h"
#include "CAssetMgr.h"
#include "CSound.h"
#include "CTimeMgr.h"

CWakeupState::CWakeupState()
{
}

CWakeupState::~CWakeupState()
{
}


void CWakeupState::Enter()
{
	CMonster* pMonster = GetOwner<CMonster>();
	CAnimator* pAnimator = pMonster->GetComponent<CAnimator>();
	CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();

	if (pMonster->GetName() == L"Skeleton")
	{
		pAnimator->Play(L"skeleton_wakeup", false);
		CSound* pSound = CAssetMgr::Get()->LoadSound(L"skeletonwakeup", L"sound\\skeleton_arise.wav");
		pSound->Play();
	}

}

void CWakeupState::FinalTick()
{
	// Trace 상태의 몬스터를 알아낸다.
	CMonster* pMonster = GetOwner<CMonster>();
	CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();

	// Skeleton 의 경우
	if (pMonster->GetName() == L"Skeleton")
	{
		m_AccTime += DT;
		if (0.3f < m_AccTime)
		{
			GetStateMachine()->ChangeState(L"TraceState");
			m_AccTime = 0.f;
		}
	}
}

void CWakeupState::Exit()
{
}

