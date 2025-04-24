#include "pch.h"
#include "CJumpState.h"

#include "CAnimator.h"
#include "CStateMachine.h"
#include "CRigidBody.h"
#include "CMonster.h"


CJumpState::CJumpState()
{
}

CJumpState::~CJumpState()
{
}

void CJumpState::Enter()
{
	CMonster* pMonster = GetOwner<CMonster>();
	CRigidBody* pRigidBody = pMonster->GetComponent<CRigidBody>();
	CAnimator* pAnimator = pMonster->GetComponent<CAnimator>();

	// 추적할 Player를 찾는다.
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
	
	Vec2 pPlayerPos = pPlayer->GetPos();
	Vec2 pCurPos = pMonster->GetPos();

	// 추적할 방향을 결정 한다.
	if (pPlayerPos.x < pCurPos.x)
		m_IsDirRight = false;
	else
		m_IsDirRight = true;

	pAnimator->Play(L"spider_jump", false);
	pRigidBody->Jump();
}

void CJumpState::FinalTick()
{
	// Jump 상태의 몬스터를 알아낸다.
	CMonster* pMonster = GetOwner<CMonster>();
	
	CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();
	
	float DetectRange = pMonster->GetMonsterInfo().DetectRange;

	// 추적할 Player를 찾는다.
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	// Player 방향으로 점프한다.
	Vec2 pCurPos = pMonster->GetPos();

	if(true == m_IsDirRight)
		pCurPos.x += 1.f * pMonster->GetMonsterInfo().Speed * DT;
	else
		pCurPos.x += -1.f * pMonster->GetMonsterInfo().Speed * DT;

	pMonster->SetPos(pCurPos);
}

void CJumpState::Exit()
{
}



