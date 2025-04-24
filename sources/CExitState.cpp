#include "pch.h"
#include "CExitState.h"

#include "CMonster.h"
#include "CAnimator.h"
#include "CStateMachine.h"
#include "CEffect.h"
#include "CAnimator.h"

#include "CTimeMgr.h"

CExitState::CExitState()
	: m_AccTime(0.f)
{
}

CExitState::~CExitState()
{
}

void CExitState::Enter()
{
	CMonster* pMonster = GetOwner<CMonster>();
	CAnimator* pAnimator = pMonster->GetComponent<CAnimator>();

	Vec2 vPos = pMonster->GetPos();

	if (pMonster->GetName() == L"Victim")
	{
		pAnimator->Play(L"npc_enter", false);
	}
}

void CExitState::FinalTick()
{
	CMonster* pMonster = GetOwner<CMonster>();
	m_AccTime += DT;

	if (1.f < m_AccTime)
	{
		DeleteObject(pMonster);
		m_AccTime;
	}


}

void CExitState::Exit()
{
}



