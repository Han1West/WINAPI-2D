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

	// ������ Player�� ã�´�.
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
	
	Vec2 pPlayerPos = pPlayer->GetPos();
	Vec2 pCurPos = pMonster->GetPos();

	// ������ ������ ���� �Ѵ�.
	if (pPlayerPos.x < pCurPos.x)
		m_IsDirRight = false;
	else
		m_IsDirRight = true;

	pAnimator->Play(L"spider_jump", false);
	pRigidBody->Jump();
}

void CJumpState::FinalTick()
{
	// Jump ������ ���͸� �˾Ƴ���.
	CMonster* pMonster = GetOwner<CMonster>();
	
	CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();
	
	float DetectRange = pMonster->GetMonsterInfo().DetectRange;

	// ������ Player�� ã�´�.
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	// Player �������� �����Ѵ�.
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



