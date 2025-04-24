#include "pch.h"
#include "CPlayerJumpState.h"

#include "CAnimator.h"
#include "CStateMachine.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"

CPlayerJumpState::CPlayerJumpState()
{
}

CPlayerJumpState::~CPlayerJumpState()
{
}

void CPlayerJumpState::Enter()
{
	//CPlayer* pPlayer = GetOwner<CPlayer>();
	//CStateMachine* pSm = pPlayer->GetComponent<CStateMachine>();

	//pSm->SetState(PLAYER_STATE::ISJUMPING, true);
}

void CPlayerJumpState::FinalTick()
{
}

void CPlayerJumpState::Exit()
{
	//CPlayer* pPlayer = GetOwner<CPlayer>();
	//CStateMachine* pSm = pPlayer->GetComponent<CStateMachine>();
	//CAnimator* pAnimator = pPlayer->GetComponent<CAnimator>();

	//pSm->SetState(PLAYER_STATE::ISJUMPING, false);
	//if (GetStateMachine()->GetState(PLAYER_STATE::ISRIGHT))
	//	pAnimator->Play(L"player_idle_r", false);
	//else
	//	pAnimator->Play(L"player_idle_r", false);
}




