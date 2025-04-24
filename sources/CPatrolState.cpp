#include "pch.h"
#include "CPatrolState.h"

#include "CAnimator.h"
#include "CStateMachine.h"

#include "CMonster.h"
#include "CSnake.h"
#include "CBigSnake.h"

#include "CTimeMgr.h"

CPatrolState::CPatrolState()
{
}

CPatrolState::~CPatrolState()
{
}

void CPatrolState::Enter()
{
	CMonster* pMonster = GetOwner<CMonster>();

	CAnimator* pAnimator = pMonster->GetComponent<CAnimator>();
	CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();

	if (pMonster->GetName() == L"Victim")
	{
		if (pSm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		{
			pAnimator->Play(L"npc_move_r", true);
		}
		else
		{
			pAnimator->Play(L"npc_move_l", true);
		}
	}

}

void CPatrolState::FinalTick()
{
	CMonster* pMonster = GetOwner<CMonster>();
	CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();
	Vec2 vPos = pMonster->GetPos();

	if (pSm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		vPos.x += 200.f * DT;
	else
		vPos.x -= 200.f * DT;




	pMonster->SetPos(vPos);
}

void CPatrolState::Exit()
{
}

