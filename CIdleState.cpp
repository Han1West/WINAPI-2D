#include "pch.h"
#include "CIdleState.h"

#include "CAnimator.h"
#include "CStateMachine.h"
#include "CTimeMgr.h"

#include "CMonster.h"
#include "CBomb.h"

CIdleState::CIdleState()
	:m_AttDelay(0.f)
{
}

CIdleState::~CIdleState()
{
}

void CIdleState::Enter()
{
	CMonster* pMonster = GetOwner<CMonster>();
	CAnimator* pAnimator = pMonster->GetComponent<CAnimator>();
	CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();

	if (pMonster->GetName() == L"Victim")
	{
		if (pSm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		{
			pAnimator->Play(L"npc_idle_r", true);
		}
		else
		{
			pAnimator->Play(L"npc_idle_l", true);
		}
	}

	if (pMonster->GetName() == L"Caveman")
	{
		if (pSm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		{
			pAnimator->Play(L"caveman_idle_r", true);
		}
		else
		{
			pAnimator->Play(L"caveman_idle_l", true);
		}
	}

	if (pMonster->GetName() == L"Skeleton")
	{
		pAnimator->Play(L"skeleton_idle", false);
	}

	if (pMonster->GetName() == L"Shopkeeper")
	{
		if (pSm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		{
			pAnimator->Play(L"shopkeeper_idle_r", true);
		}
		else
		{
			pAnimator->Play(L"shopkeeper_idle_l", true);
		}
	}
}

void CIdleState::FinalTick()
{
	// ������ �νĹ��� ���� Player �� �ִ��� Ȯ���Ѵ�.
	CMonster* pMonster = GetOwner<CMonster>();
	CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();
	float DetectRange = pMonster->GetMonsterInfo().DetectRange;

	// DetecteRange ����� ������
	//DrawDebugShape(DEBUG_SHAPE::CIRCLE, PEN_TYPE::BLUE, BRUSH_TYPE::HOLLOW
	//	         , pMonster->GetRenderPos(), Vec2(DetectRange * 2.f, DetectRange * 2.f));


	// ���࿡ �νĹ��� ���� Player �� �־����� �ڽ��� ���¸� TraceState ���·� �����Ѵ�.

	
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
	Vec2 pPlayerPos = {};
	if(pPlayer)
		pPlayerPos = pPlayer->GetPos();
	Vec2 pCurPos = pMonster->GetPos();

	// CaveMan �� ���
	if (pMonster->GetName() == L"Caveman")
	{
		if (pSm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		{
			if (pCurPos.y - 20.f < pPlayerPos.y && pPlayerPos.y < pCurPos.y + 20.f)
			{
				if (pCurPos.x < pPlayerPos.x && pPlayerPos.x < pCurPos.x + DetectRange)
				{
					GetStateMachine()->ChangeState(L"TraceState");
					pSm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
				}
			}
		}
		else
		{
			if (pCurPos.y - 20.f < pPlayerPos.y && pPlayerPos.y < pCurPos.y + 20.f)
			{
				if (pCurPos.x > pPlayerPos.x && pPlayerPos.x > pCurPos.x - DetectRange)
				{
					GetStateMachine()->ChangeState(L"TraceState");
					pSm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
				}
			}
		}
	}
	

	// Skeleton �� ���
	if (pMonster->GetName() == L"Skeleton")
	{
		if (pCurPos.y - 10.f < pPlayerPos.y && pPlayerPos.y < pCurPos.y + 10.f)
		{
			if (pCurPos.x - DetectRange < pPlayerPos.x && pPlayerPos.x < pCurPos.x + DetectRange)
			{
				GetStateMachine()->ChangeState(L"WakeupState");
				if (pCurPos.x - DetectRange < pPlayerPos.x)
					pSm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
				else
					pSm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
			}
		}	
	}

	// Spider �� ���
	if (pMonster->GetName() == L"Spider")
	{
		if (pCurPos.x - 10.f < pPlayerPos.x && pPlayerPos.x < pCurPos.x + 10.f)
		{
			if (pCurPos.y < pPlayerPos.y && pPlayerPos.y < pCurPos.y + DetectRange)
			{
				GetStateMachine()->ChangeState(L"TraceState");
			}
		}
	}

	// Olmec �� ���
	if (pMonster->GetName() == L"Olmec")
	{
		Vec2 vScale = pMonster->GetScale();

		m_AttDelay += DT;
		if(2.f < m_AttDelay)
		{
			if (pCurPos.x - vScale.x - 100.f < pPlayerPos.x && pPlayerPos.x < pCurPos.x + vScale.x + 100.f
				&& pCurPos.y < pPlayerPos.y)
				GetStateMachine()->ChangeState(L"AttackState");
			else
				GetStateMachine()->ChangeState(L"TraceState");
			
			m_AttDelay = 0.f;
		}
	}
}

void CIdleState::Exit()
{

}
