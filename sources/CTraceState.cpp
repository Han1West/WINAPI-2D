#include "pch.h"
#include "CTraceState.h"

#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CSound.h"

#include "CShopkeeper.h"
#include "CShotgun.h"
#include "CPassiveItem.h"
#include "COlmec.h"
#include "CMonster.h"
#include "CSkeleton.h"
#include "CAnimator.h"
#include "CStateMachine.h"
#include "CRigidBody.h"


CTraceState::CTraceState()
	:m_AccTime(0.f)
{
}

CTraceState::~CTraceState()
{
}

void CTraceState::Enter()
{
	CMonster* pMonster = GetOwner<CMonster>();
	CAnimator* pAnimator = pMonster->GetComponent<CAnimator>();
	CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();
	CRigidBody* m_RigidBody = pMonster->GetComponent<CRigidBody>();

	wstring pName = pMonster->GetName();

	if (pMonster->GetName() == L"Shopkeeper")
	{
		if (pSm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		{
			pAnimator->Play(L"shopkeeper_move_r", true);
		}
		else
		{
			pAnimator->Play(L"shopkeeper_move_l", true);
		}
	}

	if (pMonster->GetName() == L"Caveman")
	{
		if (pSm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		{
			pAnimator->Play(L"caveman_move_r", true);
		}
		else
		{
			pAnimator->Play(L"caveman_move_l", true);
		}
		CSound* pSound = CAssetMgr::Get()->LoadSound(L"grunt03", L"sound\\grunt03.wav");
		pSound->Play();
	}

	if (pMonster->GetName() == L"Skeleton")
	{
		CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

		Vec2 pPlayerPos = pPlayer->GetPos();
		Vec2 pCurPos = pMonster->GetPos();

		if (pCurPos.x < pPlayerPos.x)
		{
			pSm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
			pAnimator->Play(L"skeleton_move_r", true);
		}
		else if (pPlayerPos.x < pCurPos.x)
		{
			pSm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
			pAnimator->Play(L"skeleton_move_r", false);
		}
	}

	if (pMonster->GetName() == L"Spider")
	{
		pAnimator->Play(L"spider_drop", false);
		m_RigidBody->UseGravity(true);
	}

	// 추적할 Player를 찾는다.
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	Vec2 pPlayerPos = pPlayer->GetPos();
	Vec2 pCurPos = pMonster->GetPos();
	m_AccTime = 0.f;

	// 추적할 방향을 결정 한다.
	if (pPlayerPos.x < pCurPos.x)
		m_IsDirRight = false;
	else
		m_IsDirRight = true;
}

void CTraceState::FinalTick()
{
	// Trace 상태의 몬스터를 알아낸다.
	CMonster* pMonster = GetOwner<CMonster>();
	CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();
	float DetectRange = pMonster->GetMonsterInfo().DetectRange;
	float AttackRange = pMonster->GetMonsterInfo().AttackRange;

	// 추적할 Player를 찾는다.
	//CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	//Vec2 vDir = pPlayer->GetPos() - pMonster->GetPos();
	//vDir.Normalize();
	//vDir.y = 0.f;



	// 만약 Player 가 탐지범위 이상으로 멀어지면 
	// or 추적을 시작한 이후로 일정시간이 지난 경우
	// or 초기위치에서 일정거리 이상 추격한경우

	// 추적할 Player를 찾는다.
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	Vec2 pPlayerPos = pPlayer->GetPos();
	Vec2 pCurPos = pMonster->GetPos();


	// CaveMan 의 경우
	if (pMonster->GetName() == L"Caveman")
	{
		if (pSm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		{
			Vec2 vPos = pMonster->GetPos() + Vec2(1.f, 0.f) * pMonster->GetMonsterInfo().Speed * DT;
			pMonster->SetPos(vPos);
			if (pCurPos.y - 5.f > pPlayerPos.y || pPlayerPos.y > pCurPos.y + 5.f)
			{
				if (pCurPos.x > pPlayerPos.x || pPlayerPos.x > pCurPos.x + DetectRange)
				{
					m_AccTime += DT;

					if (1.f < m_AccTime)
					{
						GetStateMachine()->ChangeState(L"IdleState");
						pSm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
						m_AccTime = 0.f;
					}
				}
			}
		}
		else
		{
			Vec2 vPos = pMonster->GetPos() + Vec2(-1.f, 0.f) * pMonster->GetMonsterInfo().Speed * DT;
			pMonster->SetPos(vPos);
			if (pCurPos.y - 5.f > pPlayerPos.y || pPlayerPos.y > pCurPos.y + 5.f)
			{
				if (pCurPos.x < pPlayerPos.x || pPlayerPos.x < pCurPos.x - DetectRange)
				{
					m_AccTime += DT;

					if (1.f < m_AccTime)
					{
						GetStateMachine()->ChangeState(L"IdleState");
						pSm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
						m_AccTime = 0.f;
					}
				}
			}
		}
	}

	// Skeleton 의 경우
	if (pMonster->GetName() == L"Skeleton")
	{
		m_AccTime += DT;
		if (0.3f < m_AccTime)
		{
			if (pSm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
			{
				Vec2 vPos = pMonster->GetPos() + Vec2(1.f, 0.f) * pMonster->GetMonsterInfo().Speed * DT;
				pMonster->SetPos(vPos);
				if (pCurPos.y - 5.f > pPlayerPos.y || pPlayerPos.y > pCurPos.y + 5.f)
				{
					if (pCurPos.x > pPlayerPos.x || pPlayerPos.x > pCurPos.x + AttackRange)
					{
						m_AccTime += DT;

						if (5.f < m_AccTime)
						{
							GetStateMachine()->ChangeState(L"IdleState");
							m_AccTime = 0.f;
						}					
					}
				}
			}
			else
			{
				Vec2 vPos = pMonster->GetPos() + Vec2(-1.f, 0.f) * pMonster->GetMonsterInfo().Speed * DT;
				pMonster->SetPos(vPos);
				if (pCurPos.y - 5.f > pPlayerPos.y || pPlayerPos.y > pCurPos.y + 5.f)
				{
					if (pCurPos.x < pPlayerPos.x || pPlayerPos.x < pCurPos.x - AttackRange)
					{
						m_AccTime += DT;

						if (8.3f < m_AccTime)
						{
							GetStateMachine()->ChangeState(L"IdleState");
							m_AccTime = 0.f;
						}					
					}
				}
			}
		}
	}

	// Spider 의 경우
	if (pMonster->GetName() == L"Spider")
	{
		m_AccTime += DT;
		if (1.f < m_AccTime)
		{
			GetStateMachine()->ChangeState(L"JumpState");
		}
	}

	// Olmec 의 경우
	if (pMonster->GetName() == L"Olmec")
	{
		COlmec* pOlmec = dynamic_cast<COlmec*>(pMonster);
		Vec2 vDir = Vec2(1.f, 0.f);
		if (true == m_IsDirRight)
			vDir = Vec2(1.f, 0.f);
		else
			vDir = Vec2(-1.f, 0.f);
		
		m_AccTime += DT;
		if(m_AccTime < 0.7f)
		{
			pCurPos.y -= 300.f * DT;
			pOlmec->SetPos(pCurPos);
		}
		else if(0.7f < m_AccTime && m_AccTime < 1.4f)
		{
			if (pOlmec->IsCanMove())
			{
				pCurPos.y -= 100.f * DT;
				pCurPos.x += 400.f * vDir.x * DT;
				pOlmec->SetPos(pCurPos);
			}
		}
		else if(1.4f < m_AccTime)
		{
			if (pOlmec->IsCanMove())
			{
				pCurPos.y += 500.f * DT;
				pCurPos.x += 100.f * vDir.x * DT;
				pOlmec->SetPos(pCurPos);
			}
		}
	}
}

void CTraceState::Exit()
{
	m_AccTime = 0.f;
}