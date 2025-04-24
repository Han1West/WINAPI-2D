#include "pch.h"
#include "CPlayerHitState.h"

#include "CAnimator.h"
#include "CStateMachine.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"


CPlayerHitState::CPlayerHitState()
	:m_AccTime(0.f)
{
}

CPlayerHitState::~CPlayerHitState()
{
}

void CPlayerHitState::Enter()
{
	CPlayer* pPlayer = GetOwner<CPlayer>();
	CRigidBody* pRigidBody = pPlayer->GetComponent<CRigidBody>();


	if (GetStateMachine()->GetState(PLAYER_STATE::ISRIGHT))
		pRigidBody->AddForce(Vec2(-1000000000000000.f, -1000000000000000.f));
	else
		pRigidBody->AddForce(Vec2(1000000000000000.f, -1000000000000000.f));
}

void CPlayerHitState::FinalTick()
{
	m_AccTime += DT;

	if (1.5f < m_AccTime)
	{
		m_AccTime = 0.f;
		GetStateMachine()->ChangeState(L"PlayerIdleState");
	}
}

void CPlayerHitState::Exit()
{
}




